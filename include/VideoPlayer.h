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

public:
	explicit VideoPlayer(QWidget* parent = nullptr);
	~VideoPlayer();

private slots:
	void duration_changed(int64_t duration);
	void position_changed(int64_t duration);
	void on_volumeButton_clicked();
	void on_stopPlayButton_clicked(bool check);
	void on_fullscreenButton_clicked();
	void on_sliderDurationVideo_valueChanged(int value);

protected:
	VideoPlayerUI* ui;
	int64_t duration;
	bool is_paused = false;
	bool is_muted = true;

protected:
	void update_duration(int64_t duration);
};