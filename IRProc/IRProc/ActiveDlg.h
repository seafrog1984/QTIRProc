#pragma once

#include <QWidget>
#include "ui_ActiveDlg.h"
#include "checksn.h"

class ActiveDlg : public QWidget
{
	Q_OBJECT

public:
	ActiveDlg(QWidget *parent = Q_NULLPTR);
	~ActiveDlg();


	QString m_mac, m_sn;

private:
	Ui::ActiveDlg ui;


	protected slots:

	void btn_active();
	void btn_cancel();

};
