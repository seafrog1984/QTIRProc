#include "ThumLabel.h"

#include<QMouseEvent>
#include <QKeyEvent>

#define IMGE_TOTAL_NUM 12 //��ʾ��ͼ������

extern int g_cur_img;//��ǰͼ���±�
extern int g_picNum;//ͼ����
extern int g_img_show_flag[IMGE_TOTAL_NUM];//ͼ����ʾ��־
extern int g_flagShowBigImg;
extern int g_bigNum;//��ͼ�����������Ϊ3
extern int g_bigIndex[3];//�洢��ͼ��ԭʼ�±�
extern int g_curBig;//��ǰ��ͼ���±�

extern int g_big_show[3];

ThumLabel::ThumLabel(QWidget* parent)
:QLabel(parent)
{
}

ThumLabel::~ThumLabel()
{
}

int ThumLabel::getCurImgIndex()
{
	cur_img = objectName().toInt()-30;

	return cur_img;
}

void ThumLabel::mousePressEvent(QMouseEvent *ev)
{
	g_cur_img = getCurImgIndex();
	for (int i = 0; i < g_picNum; i++)
		{
		ThumLabel *p = parent()->findChild<ThumLabel*>(QString::number(i+30));
		if (p!=NULL)
			p->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 255, 255);");
		}
	setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(255, 0, 0);");

	if (g_flagShowBigImg)
	{
		g_curBig = (g_curBig + 1) % 3;
		g_bigIndex[g_curBig] = g_cur_img;

		if (g_bigNum < 3) g_bigNum++;
		g_big_show[g_bigNum - 1] = 1;

		g_flagShowBigImg = 1;
		g_img_show_flag[g_cur_img] = 1;
		emit thumClicked();
	}
	else
	{
		if (g_img_show_flag[g_cur_img] == 0)
		{
			g_img_show_flag[g_cur_img] = 1;
			emit thumClicked();
		}

	}

}