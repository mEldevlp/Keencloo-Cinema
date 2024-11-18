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
	buttonsLayout = new QGridLayout();
	stopPlayButton = new QPushButton();
	stopPlayButton->setObjectName("stopPlayButton");
	stopPlayButton->setIcon(QIcon(APP_DIR + "/rsc/play.ico"));

	volumeButton = new QPushButton();
	volumeButton->setObjectName("volumeButton");
	volumeButton->setIcon(QIcon(APP_DIR + "/rsc/volume_up.ico"));

	currentTimeVideo = new QLabel("00:00");
	totalTimeVideo = new QLabel("00:00");
	horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	fullscreenButton = new QPushButton();
	fullscreenButton->setObjectName("fullscreenButton");
	fullscreenButton->setIcon(QIcon(APP_DIR + "/rsc/fullscreen1.ico"));

	buttonsLayout->addWidget(stopPlayButton, 0, 0);
	buttonsLayout->addWidget(volumeButton, 0, 1);
	buttonsLayout->addWidget(currentTimeVideo, 0, 2);
	buttonsLayout->addWidget(new QLabel("/"), 0, 3);
	buttonsLayout->addWidget(totalTimeVideo, 0, 4);
	buttonsLayout->addItem(horizontalSpacer, 0, 5);
	buttonsLayout->addWidget(fullscreenButton, 0, 6);

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

	connect(ui->volumeButton, &QPushButton::clicked, this, &VideoPlayer::on_volumeButton_clicked);
	connect(ui->stopPlayButton, &QPushButton::clicked, this, &VideoPlayer::on_stopPlayButton_clicked);
	connect(ui->fullscreenButton, &QPushButton::clicked, this, &VideoPlayer::on_fullscreenButton_clicked);
	connect(ui->sliderDurationVideo, &QSlider::valueChanged, this, &VideoPlayer::on_sliderDurationVideo_valueChanged);

	ui->volumeButton->installEventFilter(this);
	ui->stopPlayButton->installEventFilter(this);
	ui->fullscreenButton->installEventFilter(this);
	ui->sliderDurationVideo->installEventFilter(this);
	
	connect(ui->player, &QMediaPlayer::durationChanged, this, &VideoPlayer::duration_changed);
	connect(ui->player, &QMediaPlayer::positionChanged, this, &VideoPlayer::position_changed);
}

VideoPlayer::~VideoPlayer()
{
	delete ui;
}

void VideoPlayer::on_stopPlayButton_clicked(bool check)
{
	if (this->is_paused)
	{
		ui->player->play();
		ui->stopPlayButton->setIcon(QIcon(APP_DIR + "/rsc/play.ico"));
	}
	else
	{
		ui->player->pause();
		ui->stopPlayButton->setIcon(QIcon(APP_DIR + "/rsc/pause.ico"));
	}

	this->is_paused = !this->is_paused;
}

void VideoPlayer::on_fullscreenButton_clicked()
{
	// for tests
	QString FileName = QFileDialog::getOpenFileName(this, tr("Select Video File"), "", tr("MP4 Files (*.mp4)"));

	if (FileName.isEmpty()) return;

	ui->video->setGeometry(0, 0, ui->videoPlayer->width(), ui->videoPlayer->height() - 90);
	ui->video->setParent(ui->videoPlayer);

	ui->player->setSource(QUrl(FileName));

	ui->sliderDurationVideo->setRange(0, ui->player->duration() / 100);

	ui->video->setVisible(true);
	ui->video->show();

	ui->player->play();
}

void VideoPlayer::on_sliderDurationVideo_valueChanged(int value)
{
	ui->sliderDurationVideo->setValue(value);
	ui->player->setPosition(static_cast<qint64>(ui->sliderDurationVideo->value()) * 1000);
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

void VideoPlayer::duration_changed(int64_t duration)
{
	this->duration = duration / 1000;
	ui->sliderDurationVideo->setMaximum(this->duration);
}

void VideoPlayer::position_changed(int64_t duration)
{
	int actual_duration = duration / 1000;

	if (!ui->sliderDurationVideo->isSliderDown())
	{
		ui->sliderDurationVideo->setValue(actual_duration);
	}

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
	if (!this->is_muted)
	{
		this->is_muted = true;
		// change icon muted
	}
	else
	{
		this->is_muted = false;
		// change icon unmute
	}

	ui->audio->setMuted(this->is_muted);
}
