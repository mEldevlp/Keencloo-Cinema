#include "MediaPlayer.h"

MediaPlayer::MediaPlayer(QWidget* parent)
	: BaseDesignWindow(parent)
{
	videoPlayer = new VideoPlayer(this);
    
	exploreFilesButton = new QPushButton();
    exploreFilesButton->setIcon(QIcon(APP_DIR + "/rsc/folder_films.ico"));
    exploreFilesButton->setObjectName("exploreFilesButton");

    settingsButton = new QPushButton();
    settingsButton->setIcon(QIcon(APP_DIR + "/rsc/settings.ico"));
    settingsButton->setObjectName("settingsButton");

	ui->mainLayout->addWidget(videoPlayer);
	ui->mainLayout->addWidget(settingsButton);
	ui->topLayout->insertWidget(0, exploreFilesButton);
	ui->topLayout->insertWidget(1, settingsButton);
    ui->topLayout->insertStretch(2);

    connect(exploreFilesButton, &QPushButton::clicked, this, &MediaPlayer::on_exploreFilesButton_click);
}

MediaPlayer::~MediaPlayer()
{
	delete videoPlayer;
}

void MediaPlayer::on_exploreFilesButton_click()
{
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    bool isWindowOpen = false;
    for (QWidget* widget : topLevelWidgets)
        if (widget->objectName() == "ExploreFilesList" && widget->isVisible())
            isWindowOpen = true;

    if (!isWindowOpen)
    {
        // Get center parent
        QPoint center = this->geometry().center();

        exploreFilesList = new ExploreFilesList(this);
        exploreFilesList->setObjectName("ExploreFilesList");
        
        exploreFilesList->show();

        // Move new window to center of parent
        exploreFilesList->move(center - QPoint(exploreFilesList->width() / 2, exploreFilesList->height() / 2));
    }
    else
    {
        exploreFilesList->raise();
        exploreFilesList->activateWindow();
    }
}