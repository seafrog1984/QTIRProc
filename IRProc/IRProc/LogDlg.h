#pragma once

#include <QWidget>
#include "ui_LogDlg.h"

class LogDlg : public QWidget
{
	Q_OBJECT

public:
	LogDlg(QWidget *parent = Q_NULLPTR);
	~LogDlg();

private:
	Ui::LogDlg ui;


	private slots:
	void log();
};
