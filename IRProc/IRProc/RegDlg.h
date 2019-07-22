#pragma once

#include <QWidget>
#include "ui_RegDlg.h"
#include "./incl/client.h"
#include <QEventLoop>

class RegDlg : public QWidget
{
	Q_OBJECT

public:
	RegDlg(QWidget *parent = Q_NULLPTR);
	~RegDlg();
	void exec();

	void closeEvent(QCloseEvent *event);

	QEventLoop *m_eventLoop;

public:
	Ui::RegDlg ui;

	public slots:
	void btn_reg();
	void setGender(int);

};
