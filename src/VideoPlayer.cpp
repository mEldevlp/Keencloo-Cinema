#include "VideoPlayer.h"

void VideoPlayerUI::setup_ui(QWidget* parent)
{
	mainLayout = new QVBoxLayout(parent);

	player = new QMediaPlayer();
	audio = new QAudioOutput();
	video = new QVideoWidget();
	
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
	totalTimeVideo = new QLabel("00:00");
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
	buttonsLayout->addWidget(new QLabel("/"),	0, item++);
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

	ui->volumeButton->installEventFilter(this);
	ui->stopPlayButton->installEventFilter(this);
	ui->fullscreenButton->installEventFilter(this);
	ui->sliderDurationVideo->installEventFilter(this);
	ui->video->installEventFilter(this);

	ui->videoBar->setAttribute(Qt::WA_TranslucentBackground); // Поддержка прозрачного фона
	ui->videoBar->setAttribute(Qt::WA_ShowWithoutActivating); // Не перехватывать фокус

	connect(hideBarTimer, &QTimer::timeout, [&]() {
		if (ui->video->isFullScreen()) ui->videoBar->hide();
	});

	connect(ui->player, &QMediaPlayer::durationChanged, this, &VideoPlayer::duration_changed);
	connect(ui->player, &QMediaPlayer::positionChanged, this, &VideoPlayer::position_changed);
}

VideoPlayer::~VideoPlayer()
{
	delete ui;
}

void VideoPlayer::on_stopPlayButton_clicked()
{
	if (!ui->player->source().isEmpty())
	{
		if (this->is_paused)
		{
			ui->player->play();
			ui->stopPlayButton->setIcon(QIcon(RSC_DIR + "play.ico"));
		}
		else
		{
			ui->player->pause();
			ui->stopPlayButton->setIcon(QIcon(RSC_DIR + "pause.ico"));
		}

		this->is_paused = !this->is_paused;
	}
}

void VideoPlayer::on_fullscreenButton_clicked()
{
	if (!ui->player->source().isEmpty())
	{
		if (!ui->video->isFullScreen())
		{
			ui->video->setFullScreen(true);
			ui->videoBar->setStyleSheet("#videoBar { background-color: rgba(0, 0, 0, 140); border: none;}");

			// Перенос videoBar поверх QVideoWidget
			ui->videoBar->setParent(nullptr); // Отсоединяем от родителя
			ui->videoBar->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // Убираем рамку и делаем всегда сверху

			// Показ videoBar поверх видео
			QRect videoGeometry = ui->video->geometry();
			ui->videoBar->setGeometry(videoGeometry.x(),
				videoGeometry.y() + videoGeometry.height() - ui->videoBar->height(),
				videoGeometry.width(),
				ui->videoBar->height());
			ui->videoBar->show();
		}
		else
		{
			ui->video->setFullScreen(false);

			// Вернуть videoBar в основной макет
			ui->videoBar->setParent(this);
			ui->videoBar->setStyleSheet("#videoBar { background-color: rgba(0, 0, 0);}");
			ui->video->setGeometry(0, 0, ui->videoPlayer->width(), ui->videoPlayer->height() - 90);
			ui->mainLayout->addWidget(ui->videoBar);
		}
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
		QRect videoGeometry = ui->video->geometry();
		ui->videoBar->setGeometry(videoGeometry.x(),
			videoGeometry.y() + videoGeometry.height() - ui->videoBar->height(),
			videoGeometry.width(),
			ui->videoBar->height());
	}
	QWidget::resizeEvent(event);
}

bool VideoPlayer::eventFilter(QObject* watched, QEvent* event)
{
	if (watched->isWidgetType() && qobject_cast<QPushButton*>(watched))
	{
		if (event->type() == QEvent::Enter)
		{
			qobject_cast<QWidget*>(watched)->setCursor(Qt::PointingHandCursor);
			return true;
		}
		else if (event->type() == QEvent::Leave)
		{
			qobject_cast<QWidget*>(watched)->unsetCursor();
			return true;
		}
	}

	return QWidget::eventFilter(watched, event);
}

void VideoPlayer::mouseMoveEvent(QMouseEvent* event)
{
	if (ui->video->isFullScreen())
	{
		ui->videoBar->show();
		hideBarTimer->start(2000);
	}

	QWidget::mouseMoveEvent(event);
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
		ui->volumeButton->setIcon(QIcon(RSC_DIR + "volume" + (this->is_muted ? "_up.ico" : "_off.ico")));

		ui->audio->setMuted(this->is_muted = !this->is_muted);
	}
}
