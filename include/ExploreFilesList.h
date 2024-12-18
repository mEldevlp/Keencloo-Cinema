#pragma once

#include "BaseDesignWindow.h"
#include "ExtractVideoMetaData.h"
#include "MediaPlayer.h"

class QListFiles;

struct VideoFile
{
	QImage preview;
	QString filePath;
	QString title;
	QString resolution;
	QString duration;
	QString audioBitRate;
	QString videoBitRate;
	QString fileSize;
	QString fps;

	QString convertToByte(uint64_t bit, bool isDecimal = false);
};

class ExploreFilesList : public BaseDesignWindow
{
	Q_OBJECT

public:
	explicit ExploreFilesList(QWidget* parent = nullptr, const std::string& folder = nullptr);
	~ExploreFilesList();

private slots:
	void doubleClicked(const QModelIndex& index);

protected:
	QListView* listView;
	QListFiles* filesList;
	MediaPlayer* mediaPlayer;
};

class QFilesItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit QFilesItemDelegate(QObject* parent = nullptr);

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

class QListFiles : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit QListFiles(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	void addVideoFile(const std::string& video);
	Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
	std::vector<std::string> videoFiles;
};