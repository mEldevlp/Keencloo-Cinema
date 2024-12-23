#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget* parent)
{
	setMinimumSize(500, 340);

	ui->hideButton->hide();

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    QPushButton* chooseFolderButton = new QPushButton();
    chooseFolderButton->setIcon(QIcon(RSC_DIR + "folder.ico"));
    chooseFolderButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QFile settings(APP_DIR + "/settings.json");
    QString pathToFolder;

    if (settings.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        std::string json_str = settings.readAll().toStdString();
        rapidjson::Document settings_json;

        if (json_str.empty())
        {
            pathToFolder = APP_DIR + "/videos";
        }
        else
        {
            settings_json.Parse(json_str.c_str());
            pathToFolder = settings_json["dir"].GetString();
        }
    }
    settings.close();

    pathToFolderLabel = new QLabel(pathToFolder);

    pathToFolderLabel->setWordWrap(true);
    pathToFolderLabel->setFixedWidth(470);

    buttonLayout->addWidget(chooseFolderButton);
    buttonLayout->addWidget(pathToFolderLabel);

    QFrame* line = new QFrame;
    line->setStyleSheet("QFrame { background-color: rgb(192, 192, 192); }");
    line->setFixedHeight(2);

    QTextBrowser* aboutTextBrowser = new QTextBrowser;
    aboutTextBrowser->setHtml(QString(R"(
    <style>
    *{font-size: 15px;}        
    a { color: #002137; }
    </style>
    <p><b>Version:</b> 1.0.0</p>
    <p>Simplify the lives of movie fans.</p>
    <p><b>Author:</b> meldevlp (Daniel Kondrashov)</p>
    <p><b>License:</b> MIT License - <a href='https://github.com/mEldevlp/Keencloo-Cinema/blob/master/LICENSE'>details</a></p>
    <p><img src='%1' width='32' style='vertical-align: middle;' /> This application uses <a href='https://www.qt.io'>Qt</a>.</p>
    <p><img src='%2' width='32' style='vertical-align: middle;' /> <b>Source Code:</b> available on <a href='https://github.com/mEldevlp/Keencloo-Cinema/'>GitHub</a>.</p>
)").arg(RSC_DIR + "qt_logo.png").arg(RSC_DIR + "github_logo"));

    aboutTextBrowser->setOpenExternalLinks(true);
    aboutTextBrowser->setStyleSheet("background: transparent; border: none;");

    ui->mainLayout->addLayout(buttonLayout);
    ui->mainLayout->addWidget(line);
    ui->mainLayout->addWidget(aboutTextBrowser);

    ui->mainLayout->setSpacing(12);
    ui->mainLayout->setContentsMargins(7, 7, 7, 0);

    this->setLayout(ui->mainLayout);
    
    connect(chooseFolderButton, &QPushButton::clicked, this, &SettingsWindow::on_chooseFolderButton_click);
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::on_chooseFolderButton_click()
{
    QFile settings(APP_DIR + "/settings.json");

    QString settings_dir = QFileDialog::getExistingDirectory(this, "Choose folder", APP_DIR,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    rapidjson::Document settings_json;
    QString pathToFolder;

    if (settings.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        std::string json_str = settings.readAll().toStdString();
        
        if (json_str.empty())
        {
            settings_json.SetObject();
            auto& alloc = settings_json.GetAllocator();
            std::string pathStdString = settings_dir.toStdString(); // fix dagling
            const char* path = pathStdString.c_str();
            pathToFolder = path;

            settings_json.AddMember("dir", rapidjson::Value(path, alloc), alloc);
        }
        else
        {
            settings_json.Parse(json_str.c_str());
            pathToFolder = settings_dir.toStdString().c_str();
            settings_json["dir"] = rapidjson::Value(settings_dir.toStdString().c_str(), settings_json.GetAllocator());
        }
    }
    

    // Serialize JSON in string buffer
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    settings_json.Accept(writer);

    settings.resize(0); // clear the file

    QTextStream out(&settings);
    out << buffer.GetString();
    settings.close();

    pathToFolderLabel->setText(pathToFolder);
}
