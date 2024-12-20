#include "ExploreFilesList.h"

ExploreFilesList::ExploreFilesList(QWidget* parent, const std::string& folder)
	: BaseDesignWindow(parent)
{
	setMinimumSize(SERVE_RES);

	mediaPlayer = qobject_cast<MediaPlayer*>(parent);

	ui->hideButton->hide();
	listView = new QListView(this);
	ui->mainLayout->addWidget(listView);
	filesList = new QListFiles(this);

	listView->setModel(filesList);
	listView->setItemDelegate(new QFilesItemDelegate(listView));
	listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	listView->setFocusPolicy(Qt::NoFocus);

	QDir dir(folder.empty() ? APP_DIR + "/videos" : folder.c_str());

	QStringList files = dir.entryList({ "*.avi", "*.mp4", "*.mov", "*.mkv", "*.wmv", "*.webm" }, QDir::Files);
	
	for (auto filename = files.begin(); filename != files.end(); ++filename)
	{
		MetaDataVideo metadata = extract_video_metadata(dir.absoluteFilePath(*filename).toStdString().c_str());
		QFile file(metadata.file_path);
		file.open(QIODevice::ReadOnly);
		QCryptographicHash hash_md5(QCryptographicHash::Md5);

		while (!file.atEnd())
		{
			hash_md5.addData(file.read(10 * 1024)); // Read per 10 kb
		}
		file.close();

		VideoFile video;

		video.hashVideo = hash_md5.result().toHex();
		video.deepView = this->getDeepViewFromJson(video.hashVideo);

		video.data_img = metadata.frame_data.data;
		video.width = metadata.frame_data.resolution.width;
		video.height = metadata.frame_data.resolution.height;

		video.filePath = metadata.file_path;
		video.title = QString::fromStdString(metadata.file_name);
		video.resolution = QString::number(metadata.frame_data.resolution.width) + "x" + QString::number(metadata.frame_data.resolution.height);

		long long duration = metadata.duration / 1000000ll;
		QTime total_time((duration / 3600ll) % 60, (duration / 60ll) % 60ll, duration % 60ll, (duration * 1000) % 1000);
		video.duration = total_time.toString((duration > 3600ll) ? "hh:mm:ss" : "mm:ss");

		video.videoBitRate = video.convertToByte(metadata.video_bit_rate, true) + "ps"; // video bitrate 
		video.audioBitRate = video.convertToByte(metadata.audio_bit_rate, true) + "ps";
		video.fileSize = video.convertToByte(metadata.file_size);
		video.fps = QString::number(static_cast<int>(metadata.fps));

		filesList->addVideoFile(video);
	}

	connect(listView, &QListView::doubleClicked, this, &ExploreFilesList::doubleClicked);
}

ExploreFilesList::~ExploreFilesList()
{
	delete listView;
	delete filesList;
}

QString ExploreFilesList::getDeepViewFromJson(const QString& hash)
{
	QFile videos(APP_DIR + "/videos.json");
	videos.open(QIODevice::ReadWrite | QIODevice::Text);

	std::string json_str = videos.readAll().toStdString();
	rapidjson::Document videos_json;
	
	if (json_str.empty())
	{
		videos_json.SetObject();
		videos_json.AddMember("videos", rapidjson::Value(rapidjson::kArrayType), videos_json.GetAllocator());
	}
	else
	{
		videos_json.Parse(json_str.c_str());

		for (auto& video : videos_json["videos"].GetArray())
		{
			if (video["hash_md5"].GetString() == hash.toStdString())
			{
				videos.close();
				return QString::number(video["viewed_duration"].GetInt());
			}
		}
	}

	auto& allocator = videos_json.GetAllocator();

	rapidjson::Value videoObject(rapidjson::kObjectType);
	videoObject.AddMember("hash_md5", rapidjson::Value(hash.toStdString().c_str(), allocator), allocator);
	videoObject.AddMember("viewed_duration", 0, allocator);
	videos_json["videos"].PushBack(videoObject, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	videos_json.Accept(writer);

	videos.resize(0);
	videos.write(buffer.GetString(), static_cast<qint64>(buffer.GetSize()));
	videos.close();
	return QString("0");
}

void ExploreFilesList::doubleClicked(const QModelIndex& index)
{
	if (index.isValid())
	{
		VideoFile video_data = index.data(Qt::DisplayRole).value<VideoFile>();
		mediaPlayer->openVideo(video_data.filePath, video_data.hashVideo, video_data.deepView.toInt()); // !!
		
		this->close();
	}
}

QListFiles::QListFiles(QObject* parent)
	: QAbstractListModel(parent)
{}

int QListFiles::rowCount(const QModelIndex& parent) const
{
	return videoFiles.empty() ? 1 : static_cast<int>(videoFiles.size());
}

QVariant QListFiles::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= (videoFiles.empty() ? 1 : videoFiles.size()))
	{
		return QVariant();
	}

	switch (role)
	{
	case Qt::DisplayRole:
		return QVariant::fromValue(videoFiles.empty() && index.row() == 0 ? VideoFile() : videoFiles[index.row()]);
	default:
		return QVariant();
	}
}

bool QListFiles::setData(const QModelIndex& index, const QVariant& value, int role)
{
	return true;
}

void QListFiles::addVideoFile(const VideoFile& video)
{
	videoFiles.push_back(video);
}

Qt::ItemFlags QListFiles::flags(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return Qt::NoItemFlags;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QFilesItemDelegate::QFilesItemDelegate(QObject* parent)
	: QStyledItemDelegate(parent)
{}
 
QWidget* QFilesItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return nullptr;
}

void QFilesItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	painter->save();

	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	VideoFile video = index.data(Qt::DisplayRole).value<VideoFile>();
	
	// Check fiction value if vector is empty
	if (video.isEmpty())
	{
		painter->setFont(QFont("Arial", 14, QFont::Light));
		painter->drawText(opt.rect, Qt::AlignHCenter | Qt::AlignVCenter,
			QString("In chosen folder has not a video :("));
		painter->restore();
		return;
	}

	// ---------

	if (option.state & QStyle::State_MouseOver)
	{
		QRect highlightRect = opt.rect.adjusted(1, 2, 0, -2);
		painter->fillRect(highlightRect, Qt::darkCyan);
	}

	QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

	// Set padding for rect
	int verticalPadding = 10; // up/down padding
	QRect adjustedRect = option.rect.adjusted(9, verticalPadding, -9, -verticalPadding);

	// Calc rect size for img (full height item)
	int padding = 5;
	int imageSize = adjustedRect.height();
	QRect imageRect(adjustedRect.left(), adjustedRect.top(), imageSize, imageSize);

	// Rect for texts
	int textAreaWidth = adjustedRect.width() - imageSize - padding;
	int textAreaLeft = imageRect.right() + padding;
	int lineHeight = imageRect.height() / 3;

	QRect topTextRect(textAreaLeft, imageRect.top() - padding, textAreaWidth, lineHeight);
	QRect middleTextRect(textAreaLeft, imageRect.top() + lineHeight, textAreaWidth, lineHeight);
	QRect bottomTextRect(textAreaLeft, imageRect.top() + 2 * lineHeight + padding, textAreaWidth, lineHeight);

	// Draw preview
	painter->drawImage(imageRect, QImage(video.data_img.data(),
		video.width,
		video.height,
		video.width * 3,
		QImage::Format_RGB888));

	// Fonts for title and other
	QFont fontTitle("Arial", 13, QFont::Bold);
	QFont fontNormal("Arial", 10, QFont::Normal);
	
	// Slice QString until length in pixels is less than MAX_LENGTH_TITLE_PX (430)
	QString truncatedTitle = video.title;
	qsizetype len = video.title.size();
	QFontMetrics fm(fontTitle);
	while (fm.horizontalAdvance(truncatedTitle) >= MAX_LENGTH_TITLE_PX)
	{
		--len;
		truncatedTitle = video.title.left(len);
	}

	QString topTextLeft = len == video.title.size() ? truncatedTitle : truncatedTitle + "...";
	QString topTextRight = video.duration;

	QString middleTextLeft = video.resolution;
	QString middleTextRight = video.videoBitRate;

	QString bottomTextLeft = video.fps + " FPS";
	QString bottomTextRight = video.fileSize;

	// Draw up line (title left, duration right)
	painter->setFont(fontTitle);
	painter->drawText(topTextRect, Qt::AlignLeft | Qt::AlignVCenter, topTextLeft);

	painter->setFont(fontNormal);
	painter->drawText(topTextRect, Qt::AlignRight | Qt::AlignVCenter, topTextRight);

	// Draw middle line
	painter->drawText(middleTextRect, Qt::AlignLeft | Qt::AlignVCenter, middleTextLeft);
	painter->drawText(middleTextRect, Qt::AlignRight | Qt::AlignVCenter, middleTextRight);

	// Draw bottom line
	painter->drawText(bottomTextRect, Qt::AlignLeft | Qt::AlignVCenter, bottomTextLeft);
	painter->drawText(bottomTextRect, Qt::AlignRight | Qt::AlignVCenter, bottomTextRight);

	painter->restore();
}

QSize QFilesItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(), 100);
}

QString VideoFile::convertToByte(uint64_t bit, bool isDecimal)
{
	int step = 0;
	uint64_t unit = 1;
	uint64_t divider = isDecimal ? 1000ull : 1024ull;
	QString unit_prefix[] = {" B", " KB", " MB", " GB" };

	while (bit >= unit * divider && step < 3)
	{
		unit *= divider;
		++step;
	}

	if (isDecimal)
	{
		return QString::number((bit / unit) + 1) + unit_prefix[step];
	}
	else
	{
		double size = static_cast<double>(bit) / unit;
		uint64_t int_part = static_cast<uint64_t>(size);
		uint64_t frac_part = static_cast<uint64_t>((size - int_part) * 100);

		return QString::number(int_part) + "." + QString::number(frac_part) + unit_prefix[step];
	}
}

bool VideoFile::isEmpty() const
{
	return this->filePath.isEmpty();
}

