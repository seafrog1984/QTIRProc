#include "CamParDlg.h"
#include <fstream>
#include <iostream> 
#include <QMessageBox>


using namespace std;

extern float inputEmiss;		//���������
extern float inputReflect ;		//���뷴���¶�
extern float inputDis;			//�������


CamParDlg::CamParDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.lineEdit_emiss->setText(QString::number(inputEmiss));
	ui.lineEdit_reflect->setText(QString::number(inputReflect));
	ui.lineEdit_dis->setText(QString::number(inputDis));


	connect(ui.btn_set_Par, SIGNAL(clicked()), this, SLOT(btn_setPar()));

}

CamParDlg::~CamParDlg()
{
}

void CamParDlg::btn_setPar()
{

	inputEmiss = ui.lineEdit_emiss->text().toFloat();
	inputReflect = ui.lineEdit_reflect->text().toFloat();
	inputDis = ui.lineEdit_dis->text().toFloat();

	ofstream fout("campar.ini");

	fout << inputEmiss << ' ' << inputReflect << ' ' << inputDis ;

	fout.close();

	QMessageBox::information(NULL, "Title", QString::fromLocal8Bit("�޸ĳɹ�"));


}