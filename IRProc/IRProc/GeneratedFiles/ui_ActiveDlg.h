/********************************************************************************
** Form generated from reading UI file 'ActiveDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACTIVEDLG_H
#define UI_ACTIVEDLG_H

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

class Ui_ActiveDlg
{
public:
    QLabel *label_2;
    QLineEdit *lineEdit_Mac;
    QLabel *label;
    QLineEdit *lineEdit_SN;
    QPushButton *pushButton_Active;
    QPushButton *pushButton_Cancel;

    void setupUi(QWidget *ActiveDlg)
    {
        if (ActiveDlg->objectName().isEmpty())
            ActiveDlg->setObjectName(QStringLiteral("ActiveDlg"));
        ActiveDlg->resize(400, 300);
        ActiveDlg->setStyleSheet(QLatin1String("QWidget{color: rgb(255, 255, 255);\n"
"background-color: rgb(15, 21, 42);\n"
"}\n"
"QPushButton{\n"
"background-color: rgb(0, 0, 0);\n"
"}"));
        label_2 = new QLabel(ActiveDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 155, 54, 12));
        lineEdit_Mac = new QLineEdit(ActiveDlg);
        lineEdit_Mac->setObjectName(QStringLiteral("lineEdit_Mac"));
        lineEdit_Mac->setGeometry(QRect(110, 90, 220, 20));
        label = new QLabel(ActiveDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 95, 54, 12));
        lineEdit_SN = new QLineEdit(ActiveDlg);
        lineEdit_SN->setObjectName(QStringLiteral("lineEdit_SN"));
        lineEdit_SN->setGeometry(QRect(110, 150, 220, 20));
        pushButton_Active = new QPushButton(ActiveDlg);
        pushButton_Active->setObjectName(QStringLiteral("pushButton_Active"));
        pushButton_Active->setGeometry(QRect(170, 220, 75, 23));
        pushButton_Cancel = new QPushButton(ActiveDlg);
        pushButton_Cancel->setObjectName(QStringLiteral("pushButton_Cancel"));
        pushButton_Cancel->setGeometry(QRect(260, 220, 75, 23));

        retranslateUi(ActiveDlg);

        QMetaObject::connectSlotsByName(ActiveDlg);
    } // setupUi

    void retranslateUi(QWidget *ActiveDlg)
    {
        ActiveDlg->setWindowTitle(QApplication::translate("ActiveDlg", "\350\275\257\344\273\266\346\277\200\346\264\273", 0));
        label_2->setText(QApplication::translate("ActiveDlg", "\346\277\200\346\264\273\347\240\201\357\274\232", 0));
        label->setText(QApplication::translate("ActiveDlg", "\346\234\272\345\231\250\347\240\201\357\274\232", 0));
        pushButton_Active->setText(QApplication::translate("ActiveDlg", "\346\277\200\346\264\273", 0));
        pushButton_Cancel->setText(QApplication::translate("ActiveDlg", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class ActiveDlg: public Ui_ActiveDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACTIVEDLG_H
