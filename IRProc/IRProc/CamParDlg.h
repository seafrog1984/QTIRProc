#pragma once

#include <QWidget>
#include "ui_CamParDlg.h"

class CamParDlg : public QWidget
{
	Q_OBJECT

public:
	CamParDlg(QWidget *parent = Q_NULLPTR);
	~CamParDlg();

private:
	Ui::CamParDlg ui;

	public slots:
	void btn_setPar();
};
