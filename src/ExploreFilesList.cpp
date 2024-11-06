#include "ExploreFilesList.h"

ExploreFilesList::ExploreFilesList(QWidget* parent)
	: BaseDesignWindow(parent)
{
	setMinimumSize(SERVE_RES);
	listView = new QListView(this);
	ui->mainLayout->addWidget(listView);
	filesList = new QListFiles(this);

	listView->setModel(filesList);
	listView->setItemDelegate(new QFilesItemDelegate(listView));

	QDir dir(QCoreApplication::applicationDirPath() + "/videos");
	QStringList files = dir.entryList(QDir::Files);

	for (const QString& fileName : files)
	{
		filesList->addVideoFile(dir.absoluteFilePath(fileName).toStdString());
	}

#pragma region("qss style")
	listView->setStyleSheet(
		"QListView {"
		"	background: transparent;"
		"}"

		"QScrollBar:vertical {"
		"    border: none;"
		"    background-color: #c0c0c0;"
		"    width: 15px;"
		"    margin: 0px 0px 5px 5px;"
		"    border-radius: 5px;"
		"}"

		"QScrollBar::handle:vertical {"
		"    background-color: #27c4c3;"
		"    min-height: 10px;"
		"    max-height: 10px;"
		"    border-radius: 5px;"
		"}"

		"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
		"    background: none;"
		"}"

		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
		"    background: none;"
		"}"
	);
#pragma endregion
}

ExploreFilesList::~ExploreFilesList()
{}

QListFiles::QListFiles(QObject* parent)
	: QAbstractListModel(parent)
{}

int QListFiles::rowCount(const QModelIndex& parent) const
{
	return static_cast<int>(videoFiles.size());
}

QVariant QListFiles::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= videoFiles.size())
		return QVariant();

	switch (role)
	{
		case Qt::DisplayRole:
		{
			return QVariant::fromValue(videoFiles[index.row()]);
		}
		default:
		{
			return QVariant();
		}
	}
}

bool QListFiles::setData(const QModelIndex& index, const QVariant& value, int role)
{
	return true;
}

void QListFiles::addVideoFile(const std::string& video)
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

	std::string path = index.data(Qt::DisplayRole).value<std::string>();

	MetaDataVideo metadata = extract_video_metadata(path.c_str());

	VideoFile video;

	// TODO(make a preview icon)
	video.preview = QImage(metadata.frame_data.data.data(),
		metadata.frame_data.resolution.width,
		metadata.frame_data.resolution.height,
		metadata.frame_data.resolution.width * metadata.frame_data.channels,
		QImage::Format_RGB888);

	video.filePath = metadata.file_path;
	video.title = QString::fromStdString(metadata.file_name);
	video.resolution = QString::number(metadata.frame_data.resolution.width) + "x" + QString::number(metadata.frame_data.resolution.height);

	long long duration = metadata.duration / 1000000ll;
	QTime total_time((duration / 3600ll) % 60, (duration / 60ll) % 60ll, duration % 60ll, (duration * 1000) % 1000);
	video.duration = total_time.toString((duration > 3600ll) ? "hh:mm:ss" : "mm:ss");

	static auto convert_bits = [](long long bit) -> QString {
		long long divider = 1;
		QString vel = "bits";

		if (bit >= 1000000000ll)
		{
			divider = 1000000000ll;
			vel = " GB";
		}
		else if (bit >= 1000000ll)
		{
			divider = 1000000ll;
			vel = " MB";
		}
		else if (bit >= 1000ll)
		{
			divider = 1000ll;
			vel = " KB";
		}

		return QString::number(bit / divider) + vel;
		};

	video.videoBitRate = convert_bits(metadata.video_bit_rate + 1) + "ps"; // video bitrate 
	video.audioBitRate = convert_bits(metadata.audio_bit_rate + 1) + "ps";

	// TODO make div mb gb kb
	video.fileSize = convert_bits(metadata.file_size);
	video.fps = QString::number(static_cast<int>(metadata.fps));

	// ---------

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

	QRect topTextRect(textAreaLeft, imageRect.top(), textAreaWidth, lineHeight);
	QRect middleTextRect(textAreaLeft, imageRect.top() + lineHeight, textAreaWidth, lineHeight);
	QRect bottomTextRect(textAreaLeft, imageRect.top() + 2 * lineHeight, textAreaWidth, lineHeight);

	// Draw preview
	painter->drawImage(imageRect, video.preview);

	// Format text and align elements
	QString topTextLeft = video.title;
	QString topTextRight = video.duration;

	QString middleTextLeft = video.resolution;
	QString middleTextRight = video.audioBitRate;

	QString bottomTextLeft = video.fps + " FPS";
	QString bottomTextRight = video.fileSize;

	// Draw up line (title left, duration right)
	painter->drawText(topTextRect, Qt::AlignLeft | Qt::AlignVCenter, topTextLeft);
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
