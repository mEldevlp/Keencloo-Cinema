#pragma once

#include "BaseDesignWindow.h"

class SettingsWindow : public BaseDesignWindow
{
	Q_OBJECT

public:
	explicit SettingsWindow(QWidget* parent = nullptr);
	~SettingsWindow();


protected:
	QLabel* pathToFolderLabel;

private slots:
	void on_chooseFolderButton_click();
};