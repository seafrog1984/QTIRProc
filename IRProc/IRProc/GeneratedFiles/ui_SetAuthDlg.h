/********************************************************************************
** Form generated from reading UI file 'SetAuthDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETAUTHDLG_H
#define UI_SETAUTHDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetAuthDlg
{
public:
    QLabel *label;
    QRadioButton *radioButton_mod;
    QGroupBox *groupBox;
    QRadioButton *radioButton_nor;
    QRadioButton *radioButton_adm;
    QLabel *label_2;
    QLineEdit *lineEdit_user;
    QLabel *label_3;
    QLineEdit *lineEdit_pw;
    QCheckBox *checkBox_scan;
    QCheckBox *checkBox_an;
    QCheckBox *checkBox_adm;
    QPushButton *btn_process;
    QPushButton *btn_get_user;
    QListView *listView;
    QRadioButton *radioButton_add;
    QRadioButton *radioButton_del;

    void setupUi(QWidget *SetAuthDlg)
    {
        if (SetAuthDlg->objectName().isEmpty())
            SetAuthDlg->setObjectName(QStringLiteral("SetAuthDlg"));
        SetAuthDlg->resize(617, 350);
        SetAuthDlg->setStyleSheet(QLatin1String("QWidget{color: rgb(255, 255, 255);\n"
"background-color: rgb(15, 21, 42);\n"
"}\n"
"QLineEdit{\n"
"color: rgb(0,0 ,0 );\n"
"background-color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton{\n"
"background-color: rgb(18, 59, 101);\n"
"border:0px;\n"
"}"));
        label = new QLabel(SetAuthDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 30, 54, 12));
        radioButton_mod = new QRadioButton(SetAuthDlg);
        radioButton_mod->setObjectName(QStringLiteral("radioButton_mod"));
        radioButton_mod->setGeometry(QRect(440, 280, 89, 16));
        groupBox = new QGroupBox(SetAuthDlg);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(180, 20, 371, 231));
        radioButton_nor = new QRadioButton(groupBox);
        radioButton_nor->setObjectName(QStringLiteral("radioButton_nor"));
        radioButton_nor->setGeometry(QRect(50, 30, 89, 16));
        radioButton_adm = new QRadioButton(groupBox);
        radioButton_adm->setObjectName(QStringLiteral("radioButton_adm"));
        radioButton_adm->setGeometry(QRect(200, 30, 89, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 70, 54, 12));
        lineEdit_user = new QLineEdit(groupBox);
        lineEdit_user->setObjectName(QStringLiteral("lineEdit_user"));
        lineEdit_user->setGeometry(QRect(120, 70, 113, 20));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 110, 54, 12));
        lineEdit_pw = new QLineEdit(groupBox);
        lineEdit_pw->setObjectName(QStringLiteral("lineEdit_pw"));
        lineEdit_pw->setGeometry(QRect(120, 110, 113, 20));
        checkBox_scan = new QCheckBox(groupBox);
        checkBox_scan->setObjectName(QStringLiteral("checkBox_scan"));
        checkBox_scan->setGeometry(QRect(30, 170, 71, 16));
        checkBox_an = new QCheckBox(groupBox);
        checkBox_an->setObjectName(QStringLiteral("checkBox_an"));
        checkBox_an->setGeometry(QRect(140, 170, 71, 16));
        checkBox_adm = new QCheckBox(groupBox);
        checkBox_adm->setObjectName(QStringLiteral("checkBox_adm"));
        checkBox_adm->setGeometry(QRect(250, 170, 71, 16));
        btn_process = new QPushButton(groupBox);
        btn_process->setObjectName(QStringLiteral("btn_process"));
        btn_process->setGeometry(QRect(140, 200, 75, 23));
        btn_get_user = new QPushButton(SetAuthDlg);
        btn_get_user->setObjectName(QStringLiteral("btn_get_user"));
        btn_get_user->setGeometry(QRect(60, 270, 75, 23));
        listView = new QListView(SetAuthDlg);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(50, 60, 101, 192));
        radioButton_add = new QRadioButton(SetAuthDlg);
        radioButton_add->setObjectName(QStringLiteral("radioButton_add"));
        radioButton_add->setGeometry(QRect(200, 280, 89, 16));
        radioButton_add->setChecked(true);
        radioButton_del = new QRadioButton(SetAuthDlg);
        radioButton_del->setObjectName(QStringLiteral("radioButton_del"));
        radioButton_del->setGeometry(QRect(320, 280, 89, 16));

        retranslateUi(SetAuthDlg);

        QMetaObject::connectSlotsByName(SetAuthDlg);
    } // setupUi

    void retranslateUi(QWidget *SetAuthDlg)
    {
        SetAuthDlg->setWindowTitle(QApplication::translate("SetAuthDlg", "\346\235\203\351\231\220\350\256\276\347\275\256", 0));
        label->setText(QApplication::translate("SetAuthDlg", "\347\224\250\346\210\267\345\210\227\350\241\250\357\274\232", 0));
        radioButton_mod->setText(QApplication::translate("SetAuthDlg", "\344\277\256\346\224\271", 0));
        groupBox->setTitle(QApplication::translate("SetAuthDlg", "\346\226\260\345\242\236\347\224\250\346\210\267", 0));
        radioButton_nor->setText(QApplication::translate("SetAuthDlg", "\346\231\256\351\200\232\347\224\250\346\210\267", 0));
        radioButton_adm->setText(QApplication::translate("SetAuthDlg", "\347\256\241\347\220\206\345\221\230", 0));
        label_2->setText(QApplication::translate("SetAuthDlg", "\347\224\250\346\210\267\345\220\215\357\274\232", 0));
        label_3->setText(QApplication::translate("SetAuthDlg", "\345\257\206\347\240\201\357\274\232", 0));
        checkBox_scan->setText(QApplication::translate("SetAuthDlg", "\345\233\276\345\203\217\346\211\253\346\217\217", 0));
        checkBox_an->setText(QApplication::translate("SetAuthDlg", "\345\233\276\345\203\217\345\210\206\346\236\220", 0));
        checkBox_adm->setText(QApplication::translate("SetAuthDlg", "\347\256\241\347\220\206\345\221\230", 0));
        btn_process->setText(QApplication::translate("SetAuthDlg", "\346\226\260\345\242\236", 0));
        btn_get_user->setText(QApplication::translate("SetAuthDlg", "\350\216\267\345\217\226", 0));
        radioButton_add->setText(QApplication::translate("SetAuthDlg", "\346\226\260\345\242\236", 0));
        radioButton_del->setText(QApplication::translate("SetAuthDlg", "\345\210\240\351\231\244", 0));
    } // retranslateUi

};

namespace Ui {
    class SetAuthDlg: public Ui_SetAuthDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETAUTHDLG_H
