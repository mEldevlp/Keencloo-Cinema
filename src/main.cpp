#include "Keencloo.h"

int main(int argc, char** argv)
{
    QApplication keencloo_app(argc, argv);
    keencloo_app.setStyle(QStyleFactory::create("Fusion"));

    QFile style(APP_DIR + "/styles/style.qss");

    if (style.open(QFile::ReadOnly))
    {
        keencloo_app.setStyleSheet(static_cast<QString>(style.readAll()));
        style.close();
    }

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

    //ExploreFilesList w;
    MediaPlayer keencloo_main;
    keencloo_main.show();

    return keencloo_app.exec();
}