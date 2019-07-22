#include "SetAuthDlg.h"
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QModelIndex>


extern QString g_port;
extern QString g_ip;
extern QString g_uport;
extern QString g_user;
extern QString g_passwd;

extern client_t m_cli;
extern int iTestFlag;


SetAuthDlg::SetAuthDlg(QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.btn_get_user, SIGNAL(clicked()), this, SLOT(btn_getUser()));
	connect(ui.radioButton_add, SIGNAL(clicked()), this, SLOT(addUser()));
	connect(ui.radioButton_del, SIGNAL(clicked()), this, SLOT(delUser()));
	connect(ui.radioButton_mod, SIGNAL(clicked()), this, SLOT(changeUser()));
	connect(ui.btn_process, SIGNAL(clicked()), this, SLOT(btn_process()));
	connect(ui.listView, SIGNAL(clicked(QModelIndex)), this, SLOT(selUser(QModelIndex)));

	m_level = 0;
	m_opt_level = 0;
	m_opt = 0;

	if (m_cli.init(g_ip.toStdString(), g_uport.toInt()))
	{
		m_msg = QString::fromLocal8Bit("连接成功");

	}
	else
	{
		m_msg = QString::fromLocal8Bit("连接失败\n请确认IP或端口号");
	}


}

SetAuthDlg::~SetAuthDlg()
{
}


void SetAuthDlg::set_permissions()
{
	if (!m_cli.get_users(g_user.toStdString(), g_passwd.toStdString(), m_userinfo, m_level))
	{
		m_msg = QString::fromLocal8Bit("获取用户列表失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		m_cli.close();
	}


	if (ui.listView->model()->rowCount() == 0)
	{
		return;
	}

	if (m_userinfo.end() != m_userinfo.find(m_list.toStdString()))
	{
		//MessageBox(m_userinfo[m_list.GetBuffer()].c_str());
		int permissions = atoi(m_userinfo[m_list.toStdString()].c_str());
		if (permissions & 0x01)
		{
			ui.checkBox_scan->setChecked(true);
		}
		else
		{
			ui.checkBox_scan->setChecked(false);
		}
		if (permissions & 0x02)
		{
			ui.checkBox_an->setChecked(true);
		}
		else
		{
			ui.checkBox_an->setChecked(false);
		}
		if (permissions & 0x04)
		{
			ui.checkBox_adm->setChecked(true);
			ui.radioButton_adm->setChecked(true);
		}
		else
		{
			ui.checkBox_adm->setChecked(false);
			ui.radioButton_nor->setChecked(true);
		}
	}
	else
	{
		m_msg = QString::fromLocal8Bit("not find permissions for ") + m_list;
		QMessageBox::information(NULL, "Title", m_msg);
	}
}


int SetAuthDlg::get_permissions()
{
	int permissions = 0;
	int state = ui.checkBox_scan->isChecked();
	if (1 == state)
	{
		permissions |= 1;
	}

	state = ui.checkBox_an->isChecked();
	if (1 == state)
	{
		permissions |= (1 << 1);

	}
	state = ui.checkBox_adm->isChecked();
	if (1 == state)
	{
		permissions |= (1 << 2);
	}


	return permissions;
}



void SetAuthDlg::selUser(QModelIndex index)
{
	m_list = ui.listView->model()->data(index).toString();
	//MessageBox(m_list);
	if (0 != m_opt)
	{
		ui.lineEdit_user->setText(m_list);
		if (2 == m_opt)
		{
			set_permissions();
			ui.lineEdit_pw->setText("");
		}
	}

}



void SetAuthDlg::addUser()
{
	m_opt = 0;
	ui.groupBox->setTitle(QString::fromLocal8Bit("新增用户"));
	ui.btn_process->setText(QString::fromLocal8Bit("新增"));

	ui.lineEdit_user->setDisabled(false);
	ui.lineEdit_pw->setDisabled(false);
}

void SetAuthDlg::delUser()
{
	m_opt = 1;
	ui.groupBox->setTitle(QString::fromLocal8Bit("删除用户"));
	ui.btn_process->setText(QString::fromLocal8Bit("删除"));
	ui.lineEdit_user->setDisabled(true);
	ui.lineEdit_pw->setDisabled(true);

}

void SetAuthDlg::changeUser()
{
	m_opt = 2;
	ui.groupBox->setTitle(QString::fromLocal8Bit("修改用户"));
	ui.btn_process->setText(QString::fromLocal8Bit("修改"));
	ui.lineEdit_pw->setDisabled(false);
	ui.lineEdit_user->setDisabled(true);
}

void SetAuthDlg::btn_process()
{

	if (0 == m_opt)
	{
		QString sUser;
		sUser = ui.lineEdit_user->text();
		if (sUser == "")
		{
			m_msg = QString::fromLocal8Bit("用户名不能为空");
			QMessageBox::information(NULL, "Title", m_msg);
			return;
		}
		QString sPasswd;
		sPasswd = ui.lineEdit_pw->text();
		if (sPasswd == "")
		{
			m_msg = QString::fromLocal8Bit("密码不能为空");
			QMessageBox::information(NULL, "Title", m_msg);
			return;
		}

		int permissions = get_permissions();
		if (m_cli.new_user(sUser.toStdString(), sPasswd.toStdString(), m_opt_level, permissions, g_user.toStdString(), g_passwd.toStdString()))
		{
			QStandardItem *item = new QStandardItem(sUser);
			QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui.listView->model());
			model->appendRow(item);

			m_msg = QString::fromLocal8Bit("新增用户成功");
		}
		else
		{
			m_msg = QString::fromLocal8Bit("新增用户失败\n");
			m_msg.append(m_cli.get_msg().c_str());
		}
		QMessageBox::information(NULL, "Title", m_msg);
	}
	else if (1 == m_opt)
	{
		QString sUser;
		sUser = ui.lineEdit_user->text();
		if (sUser == "")
		{
			m_msg = QString::fromLocal8Bit("请选择要删除的用户");
			return;
		}

		if (m_cli.del_user(sUser.toStdString(), g_user.toStdString(), g_passwd.toStdString()))
		{
			QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui.listView->model());
			model->removeRow(ui.listView->currentIndex().row());
			m_msg = QString::fromLocal8Bit("删除用户成功");
		}
		else
		{
			m_msg = QString::fromLocal8Bit("删除用户失败\n");
			m_msg.append(m_cli.get_msg().c_str());
		}
		QMessageBox::information(NULL, "Title", m_msg);

	}
	else
	{
		QString sUser;
		sUser = ui.lineEdit_user->text();
		if (sUser == "")
		{
			m_msg = QString::fromLocal8Bit("请选择要修改的用户");
			return;
		}

		int permissions = get_permissions();

		QString sPasswd;
		sPasswd = ui.lineEdit_pw->text();
		if (sPasswd == "")
		{
			if (atoi(m_userinfo[m_list.toStdString()].c_str()) == permissions)
			{
				m_msg = QString::fromLocal8Bit("未修改任何信息");
				QMessageBox::information(NULL, "Title", m_msg);
				return;
			}
			sPasswd = "*";
		}

		if (m_cli.update_user(sUser.toStdString(), sPasswd.toStdString(), permissions, g_user.toStdString(), g_passwd.toStdString()))
		{
			m_msg = QString::fromLocal8Bit("修改用户成功");
		}
		else
		{
			m_msg = QString::fromLocal8Bit("修改用户失败\n");
			m_msg.append(m_cli.get_msg().c_str());
		}
		QMessageBox::information(NULL, "Title", m_msg);
	}

}



void SetAuthDlg::btn_getUser()
{
	QStandardItemModel *ItemModel = new QStandardItemModel(this);

	QStringList strList;



	if (!m_cli.get_users(g_user.toStdString(), g_passwd.toStdString(), m_userinfo, m_level))
	{
		m_msg = QString::fromLocal8Bit("获取用户列表失败\n");
		m_msg.append(m_cli.get_msg().c_str());
		m_cli.close();

	}
	else
	{
		m_msg = QString::fromLocal8Bit("获取用户列表成功\n");

		std::map<std::string, std::string>::iterator itor = m_userinfo.begin();
		for (; itor != m_userinfo.end(); ++itor)
		{
			//MessageBox((itor->first+"="+itor->second).c_str());
			strList.append(QString::fromStdString(itor->first.c_str()));
		}
	}

	int nCount = strList.size();
	for (int i = 0; i < nCount; i++)
	{
		QString string = static_cast<QString>(strList.at(i));
		QStandardItem *item = new QStandardItem(string);
		ItemModel->appendRow(item);
	}
	ui.listView->setModel(ItemModel);


	QMessageBox::information(NULL, "Title", m_msg);
}