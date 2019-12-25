/********************************************************************************
** Form generated from reading UI file 'CamParDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMPARDLG_H
#define UI_CAMPARDLG_H

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

class Ui_CamParDlg
{
public:
    QLabel *label_2;
    QLineEdit *lineEdit_dis;
    QPushButton *btn_set_Par;
    QLabel *label_4;
    QLineEdit *lineEdit_reflect;
    QLineEdit *lineEdit_emiss;
    QLabel *label_3;

    void setupUi(QWidget *CamParDlg)
    {
        if (CamParDlg->objectName().isEmpty())
            CamParDlg->setObjectName(QStringLiteral("CamParDlg"));
        CamParDlg->resize(400, 259);
        CamParDlg->setStyleSheet(QLatin1String("QWidget{color: rgb(255, 255, 255);\n"
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
        label_2 = new QLabel(CamParDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(60, 40, 101, 20));
        lineEdit_dis = new QLineEdit(CamParDlg);
        lineEdit_dis->setObjectName(QStringLiteral("lineEdit_dis"));
        lineEdit_dis->setGeometry(QRect(190, 130, 113, 20));
        btn_set_Par = new QPushButton(CamParDlg);
        btn_set_Par->setObjectName(QStringLiteral("btn_set_Par"));
        btn_set_Par->setGeometry(QRect(160, 200, 75, 23));
        label_4 = new QLabel(CamParDlg);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(60, 130, 111, 20));
        lineEdit_reflect = new QLineEdit(CamParDlg);
        lineEdit_reflect->setObjectName(QStringLiteral("lineEdit_reflect"));
        lineEdit_reflect->setGeometry(QRect(190, 80, 113, 20));
        lineEdit_emiss = new QLineEdit(CamParDlg);
        lineEdit_emiss->setObjectName(QStringLiteral("lineEdit_emiss"));
        lineEdit_emiss->setGeometry(QRect(190, 40, 113, 20));
        label_3 = new QLabel(CamParDlg);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 80, 111, 20));

        retranslateUi(CamParDlg);

        QMetaObject::connectSlotsByName(CamParDlg);
    } // setupUi

    void retranslateUi(QWidget *CamParDlg)
    {
        CamParDlg->setWindowTitle(QApplication::translate("CamParDlg", "\346\270\251\345\272\246\344\277\256\346\255\243", 0));
        label_2->setText(QApplication::translate("CamParDlg", "\350\276\220\345\260\204\347\216\207\357\274\2100.01-1\357\274\211", 0));
        btn_set_Par->setText(QApplication::translate("CamParDlg", "\344\277\256\346\224\271", 0));
        label_4->setText(QApplication::translate("CamParDlg", "\350\267\235\347\246\273\357\274\2100.3-5000\357\274\211", 0));
        label_3->setText(QApplication::translate("CamParDlg", "\345\217\215\345\260\204\346\270\251\345\272\246\357\274\210-40-200\357\274\211", 0));
    } // retranslateUi

};

namespace Ui {
    class CamParDlg: public Ui_CamParDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMPARDLG_H
