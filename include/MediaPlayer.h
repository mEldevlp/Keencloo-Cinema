#pragma once

#include "BaseDesignWindow.h"
#include "VideoPlayer.h"
#include "ExploreFilesList.h"

class MediaPlayer : public BaseDesignWindow
{
	Q_OBJECT

friend class ExploreFilesList;
friend class VideoPlayer;

public:
	explicit MediaPlayer(QWidget* parent = nullptr);
	~MediaPlayer();
	void openVideo(QString FileName) const;

private slots:
	void on_exploreFilesButton_click();

protected:
	ExploreFilesList* exploreFilesList = nullptr;
	
public:
	VideoPlayer* videoPlayer = nullptr;
	QPushButton* exploreFilesButton = nullptr;
	QPushButton* settingsButton = nullptr;
};