/********************************************************************************
** Form generated from reading UI file 'RegDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGDLG_H
#define UI_REGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegDlg
{
public:
    QLabel *label_5;
    QPushButton *btn_reg;
    QLabel *label_2;
    QLabel *label_6;
    QLineEdit *lineEdit_card;
    QLineEdit *lineEdit_name;
    QLabel *label;
    QLineEdit *lineEdit_age;
    QLineEdit *lineEdit_5;
    QComboBox *comboBox;
    QLabel *label_4;
    QLineEdit *lineEdit_ID;
    QLabel *label_3;

    void setupUi(QWidget *RegDlg)
    {
        if (RegDlg->objectName().isEmpty())
            RegDlg->setObjectName(QStringLiteral("RegDlg"));
        RegDlg->resize(400, 300);
        RegDlg->setStyleSheet(QLatin1String("QWidget{color: rgb(255, 255, 255);\n"
"background-color: rgb(15, 21, 42);\n"
"}\n"
"QPushButton{\n"
"background-color: rgb(0, 0, 0);\n"
"}"));
        label_5 = new QLabel(RegDlg);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 140, 54, 12));
        btn_reg = new QPushButton(RegDlg);
        btn_reg->setObjectName(QStringLiteral("btn_reg"));
        btn_reg->setGeometry(QRect(180, 240, 75, 23));
        label_2 = new QLabel(RegDlg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 50, 54, 12));
        label_6 = new QLabel(RegDlg);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(30, 170, 54, 12));
        lineEdit_card = new QLineEdit(RegDlg);
        lineEdit_card->setObjectName(QStringLiteral("lineEdit_card"));
        lineEdit_card->setGeometry(QRect(100, 140, 251, 20));
        lineEdit_name = new QLineEdit(RegDlg);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));
        lineEdit_name->setGeometry(QRect(100, 50, 251, 20));
        label = new QLabel(RegDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 20, 54, 12));
        lineEdit_age = new QLineEdit(RegDlg);
        lineEdit_age->setObjectName(QStringLiteral("lineEdit_age"));
        lineEdit_age->setGeometry(QRect(100, 110, 251, 20));
        lineEdit_5 = new QLineEdit(RegDlg);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(100, 170, 251, 20));
        comboBox = new QComboBox(RegDlg);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(100, 80, 69, 22));
        label_4 = new QLabel(RegDlg);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 110, 54, 12));
        lineEdit_ID = new QLineEdit(RegDlg);
        lineEdit_ID->setObjectName(QStringLiteral("lineEdit_ID"));
        lineEdit_ID->setGeometry(QRect(100, 20, 251, 20));
        label_3 = new QLabel(RegDlg);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 80, 54, 12));

        retranslateUi(RegDlg);

        QMetaObject::connectSlotsByName(RegDlg);
    } // setupUi

    void retranslateUi(QWidget *RegDlg)
    {
        RegDlg->setWindowTitle(QApplication::translate("RegDlg", "\344\277\256\346\224\271\344\277\241\346\201\257", 0));
        label_5->setText(QApplication::translate("RegDlg", "\345\215\241\345\217\267\357\274\232", 0));
        btn_reg->setText(QApplication::translate("RegDlg", "\347\231\273\350\256\260", 0));
        label_2->setText(QApplication::translate("RegDlg", "\345\247\223\345\220\215\357\274\232", 0));
        label_6->setText(QApplication::translate("RegDlg", "\346\211\253\346\217\217\345\217\267\357\274\232", 0));
        label->setText(QApplication::translate("RegDlg", "\350\272\253\344\273\275\350\257\201\345\217\267\357\274\232", 0));
        comboBox->setCurrentText(QString());
        label_4->setText(QApplication::translate("RegDlg", "\345\271\264\351\276\204\357\274\232", 0));
        label_3->setText(QApplication::translate("RegDlg", "\346\200\247\345\210\253\357\274\232", 0));
    } // retranslateUi

};

namespace Ui {
    class RegDlg: public Ui_RegDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGDLG_H
