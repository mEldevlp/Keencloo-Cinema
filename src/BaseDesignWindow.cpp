#include "BaseDesignWindow.h"

void BaseUI::setup_ui(QWidget* parent)
{
    centralWidget = new QWidget(parent);

    iconLabel = new QLabel(centralWidget); // logo
    
    QPixmap iconPixmap(QString(QCoreApplication::applicationDirPath() + "/rsc/logo_full.png"));

    iconLabel->setPixmap(iconPixmap.scaled(120, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    closeButton = new QPushButton(centralWidget);
    //fullscreenButton = new QPushButton(centralWidget);
    hideButton = new QPushButton(centralWidget);
    
    topLayout = new QHBoxLayout();
    //topLayout->addStretch();
    topLayout->addWidget(iconLabel);
    topLayout->addStretch();
    topLayout->addWidget(hideButton);
    //topLayout->addWidget(fullscreenButton);
    topLayout->addWidget(closeButton);
    topLayout->setContentsMargins(10, 10, 10, 0);
    topLayout->setSpacing(10);

    centralWidget->setStyleSheet(
        "QPushButton {"
        "  border-radius: 5px;"
        "  background-color: #27c4c3;"
        "  padding: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: grey;"        
        "}"
    );

    closeButton->setStyleSheet(
        "QPushButton {"
        "  border-top-right-radius: 5px;"
        "  background-color: #ff2e2e;"
        "  padding: 5px;"          
        "}"
        "QPushButton:hover {"
        "  background-color: #990012;"
        "}"
    );

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(topLayout);

    centralWidget->setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
}

BaseUI::~BaseUI()
{
    delete centralWidget;
}

BaseDesignWindow::BaseDesignWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new BaseUI)
{
    ui->setup_ui(this);

    setCentralWidget(ui->centralWidget);
    setMinimumSize(DEFAULT_RES);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/rsc/logo_minimal.png"));

    connect(ui->closeButton, &QPushButton::clicked, this, &BaseDesignWindow::close);
    connect(ui->fullscreenButton, &QPushButton::clicked, this, &BaseDesignWindow::showMaximized);
    connect(ui->hideButton, &QPushButton::clicked, this, &BaseDesignWindow::showMinimized);
}

BaseDesignWindow::~BaseDesignWindow()
{
    delete ui;
}

void BaseDesignWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor windowColor = QColor(255, 255, 255);
    QColor borderColor = QColor(192, 192, 192);
   
    // Defining a path with rounded corners
    QPainterPath path;
    path.addRoundedRect(rect().adjusted(1, 1, -1, -1), 12, 12); 

    // fill window
    painter.fillPath(path, windowColor);

    QPen pen(borderColor, 2);
    painter.setPen(pen);

    painter.drawPath(path);
}

void BaseDesignWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        clickPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void BaseDesignWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPosition().toPoint() - clickPosition);
        event->accept();
    }
}