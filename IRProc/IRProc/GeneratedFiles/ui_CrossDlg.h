/********************************************************************************
** Form generated from reading UI file 'CrossDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CROSSDLG_H
#define UI_CROSSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "myhistlabel.h"

QT_BEGIN_NAMESPACE

class Ui_CrossDlg
{
public:
    QLabel *label_2;
    QLabel *label_3;
    MyHistLabel *label;
    QToolButton *btn_red;
    QToolButton *btn_blue;
    QToolButton *btn_green;
    QToolButton *btn_yellow;
    QPushButton *btn_clear;

    void setupUi(QWidget *CrossDlg)
    {
        if (CrossDlg->objectName().isEmpty())
            CrossDlg->setObjectName(QStringLiteral("CrossDlg"));
        CrossDlg->resize(638, 710);
        CrossDlg->setStyleSheet(QLatin1String("QWidget{color: rgb(255, 255, 255);\n"
"background-color: rgb(15, 21, 42);\n"
"}\n"
"QPushButton{\n"
"background-color: rgb(0, 0, 0);\n"
"}"));
        label_2 = new QLabel(CrossDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(320, 10, 256, 320));
        label_2->setMinimumSize(QSize(256, 320));
        label_2->setMaximumSize(QSize(256, 320));
        label_2->setStyleSheet(QLatin1String("border-color: rgb(255, 85, 0);\n"
"border-width: 2px;"));
        label_2->setMargin(2);
        label_3 = new QLabel(CrossDlg);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 410, 240, 256));
        label_3->setMinimumSize(QSize(240, 256));
        label_3->setMaximumSize(QSize(240, 256));
        label = new MyHistLabel(CrossDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 240, 320));
        btn_red = new QToolButton(CrossDlg);
        btn_red->setObjectName(QStringLiteral("btn_red"));
        btn_red->setGeometry(QRect(350, 430, 32, 32));
        btn_red->setStyleSheet(QStringLiteral("background-color: rgb(255, 0, 0);"));
        btn_blue = new QToolButton(CrossDlg);
        btn_blue->setObjectName(QStringLiteral("btn_blue"));
        btn_blue->setGeometry(QRect(400, 430, 32, 32));
        btn_blue->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 255);"));
        btn_green = new QToolButton(CrossDlg);
        btn_green->setObjectName(QStringLiteral("btn_green"));
        btn_green->setGeometry(QRect(450, 430, 32, 32));
        btn_green->setStyleSheet(QStringLiteral("background-color: rgb(0, 255, 0);"));
        btn_yellow = new QToolButton(CrossDlg);
        btn_yellow->setObjectName(QStringLiteral("btn_yellow"));
        btn_yellow->setGeometry(QRect(500, 430, 32, 32));
        btn_yellow->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 0);"));
        btn_clear = new QPushButton(CrossDlg);
        btn_clear->setObjectName(QStringLiteral("btn_clear"));
        btn_clear->setGeometry(QRect(400, 530, 75, 23));

        retranslateUi(CrossDlg);

        QMetaObject::connectSlotsByName(CrossDlg);
    } // setupUi

    void retranslateUi(QWidget *CrossDlg)
    {
        CrossDlg->setWindowTitle(QApplication::translate("CrossDlg", "CrossDlg", 0));
        label_2->setText(QString());
        label_3->setText(QString());
        label->setText(QString());
        btn_red->setText(QString());
        btn_blue->setText(QString());
        btn_green->setText(QString());
        btn_yellow->setText(QString());
        btn_clear->setText(QApplication::translate("CrossDlg", "\346\270\205\351\231\244", 0));
    } // retranslateUi

};

namespace Ui {
    class CrossDlg: public Ui_CrossDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CROSSDLG_H
