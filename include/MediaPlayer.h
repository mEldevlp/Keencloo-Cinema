#pragma once

#include "BaseDesignWindow.h"
#include "VideoPlayer.h"
#include "ExploreFilesList.h"

class MediaPlayer : public BaseDesignWindow
{
	Q_OBJECT

public:
	explicit MediaPlayer(QWidget* parent = nullptr);
	~MediaPlayer();

private slots:
	void on_exploreFilesButton_click();

protected:
	VideoPlayer* videoPlayer = nullptr;
	QPushButton* exploreFilesButton;
};