#include "VideoPlayer.h"

void VideoPlayerUI::setup_ui(QWidget* parent)
{
	mainLayout = new QVBoxLayout(parent);

	player = new QMediaPlayer();
	audio = new QAudioOutput();
	video = new QVideoWidget();
	video->setMouseTracking(true);
	
	player->setAudioOutput(audio);
	player->setVideoOutput(video);

	videoPlayer = new QGroupBox();
	videoPlayer->setObjectName("videoPlayer");

	videoBar = new QGroupBox();
	videoBar->setObjectName("videoBar");

	videoPlayerLayout = new QVBoxLayout(videoPlayer);

	videoPlayerLayout->addStretch();
	videoPlayerLayout->addWidget(videoBar);
	
	videoBarLayout = new QVBoxLayout(videoBar);

	sliderDurationVideo = new QSlider(Qt::Orientation::Horizontal);
	sliderDurationVideo->setFocusPolicy(Qt::NoFocus);

	buttonsLayout = new QGridLayout();
	stopPlayButton = new QPushButton();
	stopPlayButton->setObjectName("stopPlayButton");
	stopPlayButton->setIcon(QIcon(RSC_DIR + "play.ico"));
	stopPlayButton->setFocusPolicy(Qt::NoFocus);

	volumeButton = new QPushButton();
	volumeButton->setObjectName("volumeButton");
	volumeButton->setIcon(QIcon(RSC_DIR + "volume_up.ico"));
	volumeButton->setFocusPolicy(Qt::NoFocus);

	volumeSlider = new QSlider(Qt::Orientation::Horizontal);
	volumeSlider->setMaximumWidth(100);
	volumeSlider->setMaximum(0);
	volumeSlider->setMaximum(100);
	volumeSlider->setValue(60);
	volumeSlider->setFocusPolicy(Qt::NoFocus);

	currentTimeVideo = new QLabel("00:00");
	currentTimeVideo->setObjectName("label_videobar");
	separator = new QLabel("/");
	separator->setObjectName("label_videobar");
	totalTimeVideo = new QLabel("00:00");
	totalTimeVideo->setObjectName("label_videobar");

	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	fullscreenButton = new QPushButton();
	fullscreenButton->setObjectName("fullscreenButton");
	fullscreenButton->setIcon(QIcon(RSC_DIR + "fullscreen.ico"));
	fullscreenButton->setFocusPolicy(Qt::NoFocus);

	int item = 0;
	buttonsLayout->addWidget(stopPlayButton,	0, item++);
	buttonsLayout->addWidget(volumeButton,		0, item++);
	buttonsLayout->addWidget(volumeSlider,		0, item++);
	buttonsLayout->addWidget(currentTimeVideo,	0, item++);
	buttonsLayout->addWidget(separator,			0, item++);
	buttonsLayout->addWidget(totalTimeVideo,	0, item++);
	buttonsLayout->addItem(horizontalSpacer,	0, item++);
	buttonsLayout->addWidget(fullscreenButton,	0, item);

	videoBarLayout->addWidget(sliderDurationVideo);
	videoBarLayout->addLayout(buttonsLayout);

	mainLayout->addWidget(videoPlayer);
}

VideoPlayerUI::~VideoPlayerUI()
{
	delete mainLayout;
	delete player;
}

VideoPlayer::VideoPlayer(QWidget* parent)
	: QWidget(parent),
	ui(new VideoPlayerUI)
{
	ui->setup_ui(this);
	ui->audio->setMuted(this->is_muted);
	hideBarTimer = new QTimer(this);

	connect(ui->volumeButton, &QPushButton::clicked, this, &VideoPlayer::on_volumeButton_clicked);
	connect(ui->stopPlayButton, &QPushButton::clicked, this, &VideoPlayer::on_stopPlayButton_clicked);
	connect(ui->fullscreenButton, &QPushButton::clicked, this, &VideoPlayer::on_fullscreenButton_clicked);
	connect(ui->sliderDurationVideo, &QSlider::valueChanged, this, &VideoPlayer::on_sliderDurationVideo_valueChanged);
	connect(ui->volumeSlider, &QSlider::valueChanged, this, &VideoPlayer::on_volumeSlider_valueChanged);

	//ui->volumeButton->installEventFilter(this);
	//ui->stopPlayButton->installEventFilter(this);
	//ui->fullscreenButton->installEventFilter(this);
	ui->video->installEventFilter(this);
	ui->video->setAttribute(Qt::WA_Hover);

	ui->videoBar->setAttribute(Qt::WA_TranslucentBackground);
	ui->videoBar->setAttribute(Qt::WA_ShowWithoutActivating);
	
	connect(ui->player, &QMediaPlayer::durationChanged, this, &VideoPlayer::duration_changed);
	connect(ui->player, &QMediaPlayer::positionChanged, this, &VideoPlayer::position_changed);
	connect(hideBarTimer, &QTimer::timeout, [this]() {
		ui->videoBar->hide();
		QApplication::setOverrideCursor(Qt::BlankCursor);
	});
}

VideoPlayer::~VideoPlayer()
{
	delete ui;
}

void VideoPlayer::on_stopPlayButton_clicked()
{
	if (!ui->player->source().isEmpty())
	{
		QString path = ui->video->isFullScreen() ? RSC_DIR + "fullscreen/" : RSC_DIR;

		if (this->is_paused)
		{
			ui->player->play();
			ui->stopPlayButton->setIcon(QIcon(path + "play.ico"));
		}
		else
		{
			ui->player->pause();
			ui->stopPlayButton->setIcon(QIcon(path + "pause.ico"));
		}

		this->is_paused = !this->is_paused;
	}
}

void VideoPlayer::on_fullscreenButton_clicked()
{
	if (!ui->player->source().isEmpty())
	{
		bool isFullScreen = ui->video->isFullScreen();
		QString path = !isFullScreen ? RSC_DIR + "fullscreen/" : RSC_DIR;

		if (!isFullScreen)
		{
			ui->fullscreenButton->setIcon(QIcon(path + "screenminimaze.ico"));
			ui->video->setFullScreen(true);

			// Gradient bg
			ui->videoBar->setStyleSheet("#videoBar { background-color: qlineargradient(spread : pad, x1 : 0,"
				"y1 : 1, x2 : 0, y2 : 0, stop : 0 rgba(0, 0, 0, 255), stop : 1 rgba(0, 0, 0, 0));"
				"border: none; } #label_videobar{color: white; font-size: 16px;}");
			
			// Move videBar on QVideoWidget
			ui->videoBar->setParent(nullptr); // remove parent for separate window
			ui->videoBar->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

			// Shows videbar on video
			QRect videoGeometry = ui->video->geometry();
			ui->videoBar->setGeometry(videoGeometry.x(),
				videoGeometry.y() + videoGeometry.height() - ui->videoBar->height(),
				videoGeometry.width(),
				ui->videoBar->height()); 

			ui->videoBar->show();
			ui->video->setFocus();
		}
		else
		{
			hideBarTimer->stop();
			QApplication::setOverrideCursor(Qt::ArrowCursor);
			
			ui->fullscreenButton->setIcon(QIcon(path + "fullscreen.ico"));
			ui->video->setFullScreen(false);

			// restore videoBar in main
			ui->videoBar->show();
			ui->videoBar->setParent(this);
			ui->videoBar->setStyleSheet("#videoBar { background-color: rgba(0, 0, 0); border: 2px solid black; } #label_videobar {color: black; font-size: 16px;}");
			ui->video->setGeometry(0, 0, ui->videoPlayer->width(), ui->videoPlayer->height() - ui->videoBar->height());
			ui->mainLayout->addWidget(ui->videoBar);
		}

		// Change icons
		ui->stopPlayButton->setIcon(QIcon(path + (!this->is_paused ? "play.ico" : "pause.ico")));
		ui->volumeButton->setIcon(QIcon(path + "volume" + (!this->is_muted ? "_up.ico" : "_off.ico")));
	}
}

void VideoPlayer::on_sliderDurationVideo_valueChanged(int value)
{
	ui->sliderDurationVideo->setValue(value);
	ui->player->setPosition(static_cast<qint64>(ui->sliderDurationVideo->value()) * 1'000);
}

void VideoPlayer::on_volumeSlider_valueChanged(int value)
{
	ui->volumeSlider->setValue(value);
	ui->audio->setVolume(value / 100.f);
}

void VideoPlayer::resizeEvent(QResizeEvent* event)
{
	if (ui->video->isFullScreen())
	{
		/*
		QRect videoGeometry = ui->video->geometry();
		ui->videoBar->setGeometry(videoGeometry.x(),
			videoGeometry.y() + videoGeometry.height() - ui->videoBar->height(),
			videoGeometry.width(),
			ui->videoBar->height());*/
	}
	QWidget::resizeEvent(event);
}

bool VideoPlayer::eventFilter(QObject* watched, QEvent* event)
{
	/* When out of fullscreen this code doesnt work */
	//if (watched->isWidgetType() && qobject_cast<QPushButton*>(watched))
	//{
	//if (event->type() == QEvent::Enter)
	//	{
	//		qobject_cast<QWidget*>(watched)->setCursor(Qt::PointingHandCursor);
	//		return true;
	//	}
	//	else if (event->type() == QEvent::Leave)
	//	{
	//		qobject_cast<QWidget*>(watched)->unsetCursor();
	//		return true;
	//	}
	//}
	
	if (watched->isWidgetType() && qobject_cast<QVideoWidget*>(watched))
	{
		if (event->type() == QEvent::HoverMove && ui->video->isFullScreen())
		{
			if (ui->videoBar->isVisible())
			{
				hideBarTimer->stop();
				hideBarTimer->start(3000);
			}
			else
			{
				QApplication::setOverrideCursor(Qt::ArrowCursor);
				ui->videoBar->show();
			}
		}
	}

	return QWidget::eventFilter(watched, event);
}

void VideoPlayer::duration_changed(int64_t duration)
{
	this->duration = duration / 1000;
	ui->sliderDurationVideo->setMaximum(this->duration);
}

void VideoPlayer::position_changed(int64_t duration)
{
	int actual_duration = duration / 1000;

	// fix audio lag
	ui->sliderDurationVideo->blockSignals(true); // brilliant
	ui->sliderDurationVideo->setValue(actual_duration);
	ui->sliderDurationVideo->blockSignals(false);

	this->update_duration(actual_duration);
}

void VideoPlayer::update_duration(int64_t duration)
{
	if (duration || this->duration)
	{
		QTime current_time((duration / 3600) % 60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000);
		QTime total_time((this->duration / 3600) % 60, (this->duration / 60) % 60, this->duration % 60, (this->duration * 1000) % 1000);
		QString format = (this->duration > 3600) ? "hh:mm:ss" : "mm:ss";

		ui->currentTimeVideo->setText(current_time.toString(format));
		ui->totalTimeVideo->setText(total_time.toString(format));
	}
}

void VideoPlayer::on_volumeButton_clicked()
{
	if (!ui->player->source().isEmpty())
	{
		QString path = ui->video->isFullScreen() ? RSC_DIR + "fullscreen/" : RSC_DIR;
		ui->volumeButton->setIcon(QIcon(path + "volume" + (this->is_muted ? "_up.ico" : "_off.ico")));
		ui->audio->setMuted(this->is_muted = !this->is_muted);
	}
}
