#include "Keencloo.h"

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));

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

    a.setPalette(palette);

    //ExploreFilesList w;
    MediaPlayer w;
    w.show();
    return a.exec();
}

