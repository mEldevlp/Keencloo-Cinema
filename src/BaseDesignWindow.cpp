#include "BaseDesignWindow.h"

void BaseUI::setup_ui(QWidget* parent)
{
    centralWidget = new QWidget(parent);
    iconLabel = new QLabel(centralWidget); // logo
    
    QPixmap iconPixmap(QString(APP_DIR + "/rsc/logo_full.png"));
     
    iconLabel->setPixmap(iconPixmap.scaled(120, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    closeButton = new QPushButton(centralWidget);
    closeButton->setIcon(QIcon(APP_DIR + "/rsc/close_window.ico"));
    closeButton->setObjectName("closeButton");
    closeButton->setFocusPolicy(Qt::NoFocus);

    hideButton = new QPushButton(centralWidget);
    hideButton->setIcon(QIcon(APP_DIR + "/rsc/minimize_window.ico"));
    hideButton->setObjectName("hideButton");
    hideButton->setFocusPolicy(Qt::NoFocus);

    topLayout = new QHBoxLayout();
    topLayout->addWidget(iconLabel);
    topLayout->addStretch();
    topLayout->addWidget(hideButton);
    topLayout->addWidget(closeButton);
    topLayout->setContentsMargins(10, 10, 10, 0);
    topLayout->setSpacing(10);
     
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

    setMinimumSize(DEFAULT_RES);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(APP_DIR + "/rsc/logo_minimal.png"));

    setCentralWidget(ui->centralWidget);

    connect(ui->closeButton, &QPushButton::clicked, this, [this]() {
        this->close();
        this->deleteLater();
        });
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
    // Check that lmb is pressed and cursor is at header of window
    if (event->button() == Qt::LeftButton && event->position().y() <= this->ui->topLayout->geometry().height())
    {
        this->clickPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        this->isMoving = true;
        event->accept();
    }
}

void BaseDesignWindow::mouseMoveEvent(QMouseEvent* event)
{
    // Check move action is active
    if (this->isMoving && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPosition().toPoint() - this->clickPosition);
        event->accept();
    }
}

void BaseDesignWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->isMoving = false;
        event->accept();
    }
}
