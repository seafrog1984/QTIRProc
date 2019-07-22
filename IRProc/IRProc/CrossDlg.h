#pragma once

#include <QWidget>
#include "ui_CrossDlg.h"

class CrossDlg : public QWidget
{
	Q_OBJECT

public:
	CrossDlg(QWidget *parent = Q_NULLPTR);
	~CrossDlg();


	void exec();
	QEventLoop *m_eventLoop;

private:
	Ui::CrossDlg ui;

public slots:

	void updateHist();
	void btn_red();
	void btn_blue();
	void btn_green();
	void btn_yellow();
	void btn_clear();

};
