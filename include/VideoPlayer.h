#pragma once
#include "common.h"

class VideoPlayerUI
{
public:
	QGroupBox* videoPlayer = nullptr;
	QGroupBox* videoBar = nullptr;
	QVBoxLayout* mainLayout = nullptr;
	QSlider* sliderDurationVideo = nullptr;
	QPushButton* stopPlayButton = nullptr;
	QPushButton* volumeButton = nullptr;
	QSlider* volumeSlider = nullptr;
	QLabel* currentTimeVideo = nullptr;
	QLabel* separator = nullptr;
	QLabel* totalTimeVideo = nullptr;
	QSpacerItem* horizontalSpacer = nullptr;
	QPushButton* fullscreenButton = nullptr;
	QVBoxLayout* videoPlayerLayout = nullptr;
	QVBoxLayout* videoBarLayout = nullptr;
	QGridLayout* buttonsLayout = nullptr;

	QMediaPlayer* player = nullptr;
	QAudioOutput* audio = nullptr;
	QVideoWidget* video = nullptr;

public:
	void setup_ui(QWidget* parent = nullptr);
	virtual ~VideoPlayerUI();
};

class VideoPlayer : public QWidget
{
	Q_OBJECT

	friend class MediaPlayer;

public:
	explicit VideoPlayer(QWidget* parent = nullptr);
	~VideoPlayer();

private slots:
	void duration_changed(int64_t duration);
	void position_changed(int64_t duration);
	void on_volumeButton_clicked();
	void on_stopPlayButton_clicked();
	void on_fullscreenButton_clicked();
	void on_sliderDurationVideo_valueChanged(int value);
	void on_volumeSlider_valueChanged(int value);

protected:
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;

	VideoPlayerUI* ui;
	int64_t duration = 0ll;
	bool is_paused = false;
	bool is_muted = false;
	int64_t last_position = -1;
	QTimer* hideBarTimer = nullptr;
protected:
	void update_duration(int64_t duration);
};