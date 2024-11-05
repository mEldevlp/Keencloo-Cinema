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

	// TODO make div mb gb kb
	video.fileSize = QString::number(metadata.file_size / (1024 * 1024));
	video.fps = QString::number(static_cast<int>(metadata.fps));

	// ---------

	// Установим отступы для rect
	int verticalPadding = 10;  // Отступ сверху и снизу
	QRect adjustedRect = option.rect.adjusted(9, verticalPadding, -9, -verticalPadding);

	// Определяем квадратный размер для изображения (равен всей высоте)
	int padding = 5;
	int imageSize = adjustedRect.height();  // Квадрат по высоте adjustedRect
	QRect imageRect(adjustedRect.left(), adjustedRect.top(), imageSize, imageSize);

	// Прямоугольники для текста
	int textAreaWidth = adjustedRect.width() - imageSize - padding;
	int textAreaLeft = imageRect.right() + padding;
	int lineHeight = imageRect.height() / 3;

	QRect topTextRect(textAreaLeft, imageRect.top(), textAreaWidth, lineHeight);
	QRect middleTextRect(textAreaLeft, imageRect.top() + lineHeight, textAreaWidth, lineHeight);
	QRect bottomTextRect(textAreaLeft, imageRect.top() + 2 * lineHeight, textAreaWidth, lineHeight);

	// Рисуем изображение и текст
	painter->drawImage(imageRect, video.preview);

	// Форматируем текст и выравниваем правые элементы
	QString topTextLeft = video.title;
	QString topTextRight = video.duration;

	QString middleTextLeft = video.resolution;
	QString middleTextRight = video.audioBitRate;

	QString bottomTextLeft = video.fps + " FPS";
	QString bottomTextRight = video.fileSize + " MB";

	// Рисуем верхнюю строку (title слева, duration справа)
	painter->drawText(topTextRect, Qt::AlignLeft | Qt::AlignVCenter, topTextLeft);
	painter->drawText(topTextRect, Qt::AlignRight | Qt::AlignVCenter, topTextRight);

	// Рисуем среднюю строку (resolution слева, bitrate справа)
	painter->drawText(middleTextRect, Qt::AlignLeft | Qt::AlignVCenter, middleTextLeft);
	painter->drawText(middleTextRect, Qt::AlignRight | Qt::AlignVCenter, middleTextRight);

	// Рисуем нижнюю строку (fps слева, filesize справа)
	painter->drawText(bottomTextRect, Qt::AlignLeft | Qt::AlignVCenter, bottomTextLeft);
	painter->drawText(bottomTextRect, Qt::AlignRight | Qt::AlignVCenter, bottomTextRight);

	painter->restore();
}

QSize QFilesItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(option.rect.width(), 100);
}
