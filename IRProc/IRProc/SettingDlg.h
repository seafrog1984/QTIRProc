#pragma once

#include <QWidget>
#include "ui_SettingDlg.h"

class SettingDlg : public QWidget
{
	Q_OBJECT

public:
	SettingDlg(QWidget *parent = Q_NULLPTR);
	~SettingDlg();

private:
	Ui::SettingDlg ui;

	public slots:
	void btn_setPar();

};
