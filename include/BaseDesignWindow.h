#pragma once

#include "common.h"

class BaseUI
{
public:
	QWidget* centralWidget = nullptr;
	QLabel* iconLabel = nullptr;
	QPushButton* closeButton = nullptr;
	QPushButton* fullscreenButton = nullptr;
	QPushButton* hideButton = nullptr;
	QHBoxLayout* topLayout = nullptr;
	QVBoxLayout* mainLayout = nullptr;

public:
	void setup_ui(QWidget* parent = nullptr);
    virtual ~BaseUI();
};

class BaseDesignWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit BaseDesignWindow(QWidget* parent = nullptr);
	virtual ~BaseDesignWindow();

protected:
	BaseUI* ui = nullptr;
	QPoint clickPosition;

	void paintEvent(QPaintEvent* event) override;

	// move window
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

};