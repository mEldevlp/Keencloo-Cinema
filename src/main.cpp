#include "Keencloo.h"

const std::vector<QString> g_require_res = {
    "close_window.ico",         "folder_films.ico",
    "folder_films_hover.ico",   "fullscreen.ico",
    "logo_full.png",            "logo_minimal.png",
    "minimize_window.ico",      "pause.ico",
    "play.ico",                 "settings.ico",
    "settings_hover.ico",       "volume_down.ico",
    "volume_off.ico",           "volume_up.ico"
};

int main(int argc, char** argv)
{
    QApplication keencloo_app(argc, argv);

    keencloo_app.setStyle(QStyleFactory::create("Fusion"));

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::AlternateBase, Qt::lightGray);
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, Qt::lightGray);
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, Qt::blue);
    palette.setColor(QPalette::HighlightedText, Qt::white);

    keencloo_app.setPalette(palette);

    for (const auto& res : g_require_res)
    {
        QString path = APP_DIR + "/rsc/" + res;

        if (!QFile::exists(path))
        {
            QMessageBox::critical(nullptr, "Missing Files", "Missing file:\n" + path);
            return 1;
        }
    }
    
    QString style_path = APP_DIR + "/styles/style.qss";

    if (!QFile::exists(style_path))
    {
        QMessageBox::critical(nullptr, "Missing Files", "Missing file:\n" + style_path);
        return 1;
    }

    QFile style(style_path);
   
    if (style.open(QFile::ReadOnly))
    {
        keencloo_app.setStyleSheet(static_cast<QString>(style.readAll()));
        style.close();
    }
    
    MediaPlayer keencloo_main;
    keencloo_main.show();
   
    return keencloo_app.exec();
}
