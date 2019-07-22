#pragma once

#include <QWidget>
#include "ui_SetAuthDlg.h"
#include "./incl/client.h"


class SetAuthDlg : public QWidget
{
	Q_OBJECT

public:
	SetAuthDlg(QWidget *parent = Q_NULLPTR);
	~SetAuthDlg();

	int get_permissions();
	void set_permissions();

	std::map<std::string, std::string> m_userinfo;
	int m_level;
	int m_opt_level;
	QString m_msg;
	QString m_list;

	int m_opt;

private:
	Ui::SetAuthDlg ui;

	public slots:
	void btn_getUser();
	void addUser();
	void delUser();
	void changeUser();
	void btn_process();

	void selUser(QModelIndex);


};
