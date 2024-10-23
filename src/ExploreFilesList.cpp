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

	std::vector<QString> file_paths;
	QDir dir(QCoreApplication::applicationDirPath() + "/videos");
	QStringList files = dir.entryList(QDir::Files);

	for (const QString& fileName : files)
	{
		file_paths.push_back(dir.absoluteFilePath(fileName));
	}

	for (auto itpath = file_paths.begin(); itpath != file_paths.end(); ++itpath)
	{
		MetaDataVideo metadata = extract_video_metadata(itpath->toStdString().c_str());

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
		QTime total_time((duration / 3600) % 60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000);
		video.duration = total_time.toString((duration > 3600) ? "hh:mm:ss" : "mm:ss");

		static auto cut_bit_rate = [](long long bit) -> QString {
			long long divider = 1;
			QString vel = "bps";

			if (bit >= 1000000000ll)
			{
				divider = 1000000000ll;
				vel = " g" + vel;
			}
			else if (bit >= 1000000ll)
			{
				divider = 1000000ll;
				vel = " m" + vel;
			}
			else if (bit >= 1000ll)
			{
				divider = 1000ll;
				vel = " k" + vel;
			}

			return QString::number(bit / divider) + vel;
		};

		video.videoBitRate = cut_bit_rate(metadata.video_bit_rate + 1); // video bitrate 
		video.audioBitRate = cut_bit_rate(metadata.audio_bit_rate + 1);

		video.fileSize = QString::number(metadata.file_size / (1024 * 1024));
		video.fps = QString::number(static_cast<int>(metadata.fps));

		filesList->addVideoFile(video);
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
	return videoFiles.count();
}

QVariant QListFiles::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || index.row() >= videoFiles.count())
		return QVariant();

	switch (role)
	{
		case Qt::DisplayRole:
		{
			const VideoFile& video = videoFiles.at(index.row());
			return QVariant::fromValue(video);
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

void QListFiles::addVideoFile(const VideoFile& video)
{
	videoFiles.append(video);
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

	VideoFile item = index.data(Qt::DisplayRole).value<VideoFile>();

	QRect rect1 = option.rect.adjusted(5, 5, -5, -option.rect.height() / 2);
	QRect rect2 = rect1;
	rect2.moveTop(rect2.top() + rect1.height() / 2);

	QRect rect3 = rect1.adjusted(option.rect.width() / 2, 0, 0, 0);
	QRect rect4 = rect2.adjusted(option.rect.width() / 2, 0, 0, 0);

	painter->drawText(rect1, Qt::AlignLeft, item.title);
	painter->drawText(rect2, Qt::AlignLeft, item.resolution);
	painter->drawText(rect3, Qt::AlignRight, item.duration);
	painter->drawText(rect4, Qt::AlignRight, item.audioBitRate);

	painter->restore();
}

QSize QFilesItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(), 100);
}
