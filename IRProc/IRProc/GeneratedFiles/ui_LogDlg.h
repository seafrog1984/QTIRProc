/********************************************************************************
** Form generated from reading UI file 'LogDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGDLG_H
#define UI_LOGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogDlg
{
public:
    QLineEdit *lineEdit_user;
    QLabel *label;
    QLineEdit *lineEdit_pw;
    QPushButton *btn_log;
    QLabel *label_2;

    void setupUi(QWidget *LogDlg)
    {
        if (LogDlg->objectName().isEmpty())
            LogDlg->setObjectName(QStringLiteral("LogDlg"));
        LogDlg->resize(400, 300);
        LogDlg->setStyleSheet(QLatin1String("QWidget{color: rgb(255, 255, 255);\n"
"background-color: rgb(15, 21, 42);\n"
"}\n"
"QPushButton{\n"
"background-color: rgb(0, 0, 0);\n"
"}"));
        lineEdit_user = new QLineEdit(LogDlg);
        lineEdit_user->setObjectName(QStringLiteral("lineEdit_user"));
        lineEdit_user->setGeometry(QRect(150, 60, 113, 20));
        label = new QLabel(LogDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(90, 60, 54, 12));
        lineEdit_pw = new QLineEdit(LogDlg);
        lineEdit_pw->setObjectName(QStringLiteral("lineEdit_pw"));
        lineEdit_pw->setGeometry(QRect(150, 90, 113, 20));
        btn_log = new QPushButton(LogDlg);
        btn_log->setObjectName(QStringLiteral("btn_log"));
        btn_log->setGeometry(QRect(170, 170, 75, 23));
        btn_log->setStyleSheet(QLatin1String("border-width:2px;\n"
"border-color: rgb(255, 255, 255);"));
        label_2 = new QLabel(LogDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(90, 90, 54, 12));

        retranslateUi(LogDlg);

        QMetaObject::connectSlotsByName(LogDlg);
    } // setupUi

    void retranslateUi(QWidget *LogDlg)
    {
        LogDlg->setWindowTitle(QApplication::translate("LogDlg", "\347\224\250\346\210\267\347\231\273\345\275\225", 0));
        label->setText(QApplication::translate("LogDlg", "\347\224\250\346\210\267\345\220\215\357\274\232", 0));
        btn_log->setText(QApplication::translate("LogDlg", "\347\231\273\345\275\225", 0));
        label_2->setText(QApplication::translate("LogDlg", "\345\257\206\347\240\201\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class LogDlg: public Ui_LogDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGDLG_H
