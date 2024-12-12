#include "MediaPlayer.h"

MediaPlayer::MediaPlayer(QWidget* parent)
	: BaseDesignWindow(parent)
{
    this->setFocus();
    this->setFocusPolicy(Qt::NoFocus);
    
	videoPlayer = new VideoPlayer(this);

	exploreFilesButton = new QPushButton();
    exploreFilesButton->setIcon(QIcon(RSC_DIR + "folder_films.ico"));
    exploreFilesButton->setObjectName("exploreFilesButton");
    exploreFilesButton->setFocusPolicy(Qt::NoFocus);
    
    settingsButton = new QPushButton();
    settingsButton->setIcon(QIcon(RSC_DIR + "settings.ico"));
    settingsButton->setObjectName("settingsButton");
    settingsButton->setFocusPolicy(Qt::NoFocus);
    
	ui->mainLayout->addWidget(videoPlayer);
	ui->mainLayout->addWidget(settingsButton);
	ui->topLayout->insertWidget(0, exploreFilesButton);
	ui->topLayout->insertWidget(1, settingsButton);
    ui->topLayout->insertStretch(2);

    //videoPlayer->ui->videoBar;
    QShortcut* fullscreen = new QShortcut(QKeySequence(Qt::Key_Escape), videoPlayer->ui->video);
    connect(fullscreen, &QShortcut::activated, videoPlayer->ui->video, [&]()
    {
        if (videoPlayer->ui->video->isFullScreen())
        {
            videoPlayer->on_fullscreenButton_clicked();
        }
    });

    connect(exploreFilesButton, &QPushButton::clicked, this, &MediaPlayer::on_exploreFilesButton_click);
}

MediaPlayer::~MediaPlayer()
{
	delete videoPlayer;
}

void MediaPlayer::openVideo(QString FileName) const
{
    auto ui = videoPlayer->ui;

    ui->video->setGeometry(0, 0, ui->videoPlayer->width(), ui->videoPlayer->height() - 90);
    ui->video->setParent(ui->videoPlayer);

    ui->player->setSource(QUrl(FileName));
    ui->sliderDurationVideo->setRange(0, ui->player->duration() / 100);

    ui->video->setVisible(true);
    ui->video->show();
    ui->player->play();
}

void MediaPlayer::on_exploreFilesButton_click()
{
    QFile settings(APP_DIR + "/settings.json");
    std::string videos_folder_path = APP_DIR.toStdString() + "/videos"; // by default
    
    if (settings.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        std::string json_str = settings.readAll().toStdString();
        rapidjson::Document settings_json;
        settings_json.Parse(json_str.c_str());

        if (json_str.empty())
        {
            QString settings_dir = QFileDialog::getExistingDirectory(this, "Choose folder", APP_DIR,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

            rapidjson::Document settings_json;
            settings_json.SetObject();
            auto& alloc = settings_json.GetAllocator();
            std::string pathStdString = settings_dir.toStdString(); // fix dagling
            const char* path = pathStdString.c_str();

            settings_json.AddMember("dir", rapidjson::Value(path, alloc), alloc);

            // Serialize JSON in string buffer
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            settings_json.Accept(writer);

            settings.resize(0); // clear the file

            QTextStream out(&settings);
            out << buffer.GetString();

            videos_folder_path = path;
        }
        else
        {
            settings_json.Parse(json_str.c_str());
            std::string check_val = settings_json["dir"].GetString();

            if (settings_json.HasMember("dir") && settings_json["dir"].IsString())
            {
                videos_folder_path = check_val;
            }
        }

        settings.close();
    }
    else
    {
        // cannot open the file
    }

    // ---

    const auto topLevelWidgets = QApplication::topLevelWidgets();
    bool isWindowOpen = false;
    for (QWidget* widget : topLevelWidgets)
        if (widget->objectName() == "ExploreFilesList" && widget->isVisible())
            isWindowOpen = true;

    // check the pointer (refactor) exploreFilesList
    if (!isWindowOpen)
    {
        // Get center parent
        QPoint center = this->geometry().center();

        exploreFilesList = new ExploreFilesList(this, videos_folder_path);
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