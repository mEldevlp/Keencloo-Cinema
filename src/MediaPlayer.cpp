#include "MediaPlayer.h"

MediaPlayer::MediaPlayer(QWidget* parent)
	: BaseDesignWindow(parent)
{
	videoPlayer = new VideoPlayer(this);
    
	exploreFilesButton = new QPushButton();
    exploreFilesButton->setIcon(QIcon(APP_DIR + "/rsc/folder_films.ico"));
    exploreFilesButton->setStyleSheet(
        "QPushButton {"
        "  padding: 0px;"
        "  background-color: none;"
        "  icon-size: 28px 28px;"
        "}"
        "QPushButton:hover {"
        "  icon: url(./rsc/folder_films_hover.ico);"
        "  icon-size: 28px 28px;"
        "}"
    );

    settingsButton = new QPushButton();
    settingsButton->setIcon(QIcon(APP_DIR + "/rsc/settings.ico"));
    settingsButton->setStyleSheet(
        "QPushButton {"
        "  padding: 0px;"
        "  background-color: none;"
        "  icon-size: 28px 28px;"
        "}"
        "QPushButton:hover {"
        "  icon: url(./rsc/settings_hover.ico);"
        "  icon-size: 28px 28px;"
        "}"
    );

    // Создаем анимацию для прозрачности

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

        this->exploreFilesList = new ExploreFilesList();
        this->exploreFilesList->setObjectName("ExploreFilesList");
        
        this->exploreFilesList->show();

        // Move new window to center of parent
        this->exploreFilesList->move(center - QPoint(exploreFilesList->width() / 2, exploreFilesList->height() / 2));
    }
    else
    {
        this->exploreFilesList->raise();
        this->exploreFilesList->activateWindow();
    }
}