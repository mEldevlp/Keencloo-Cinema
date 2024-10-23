#include "MediaPlayer.h"

MediaPlayer::MediaPlayer(QWidget* parent)
	: BaseDesignWindow(parent)
{
	videoPlayer = new VideoPlayer(this);
    
	exploreFilesButton = new QPushButton();

	ui->mainLayout->addWidget(videoPlayer);
	ui->topLayout->insertWidget(0, exploreFilesButton);
    ui->topLayout->insertStretch(1);

	exploreFilesButton->setStyleSheet(
        "QPushButton {"
        "  background-color: orange;"
        "  border-radius: 5px;"
        "  padding: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: grey;"
        "}"
    );

    connect(exploreFilesButton, &QPushButton::clicked, this, &MediaPlayer::on_exploreFilesButton_click);
}

MediaPlayer::~MediaPlayer()
{
	delete videoPlayer;
}

void MediaPlayer::on_exploreFilesButton_click()
{
    ExploreFilesList* exploreFilesList = new ExploreFilesList();
    exploreFilesList->show();
}