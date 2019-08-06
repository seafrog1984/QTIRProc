#include "mycustomslider.h"
#include <QPalette>

extern int g_win_width[12];//窗宽
extern int g_cur_img;

MyCustomSlider::MyCustomSlider(QWidget *parent) :QSlider(parent)
{


//	m_displayLabel = new QLabel(this);
//	m_displayLabel->setFixedSize(QSize(40, 10));
//	m_displayLabel->setStyleSheet("background-color: rgb(18, 59, 101);");
//	//设置游标背景为白色
////	m_displayLabel->setAutoFillBackground(true);
//	QPalette palette;
//	palette.setColor(QPalette::Background, Qt::white);
//	m_displayLabel->setPalette(palette);
//
//	m_displayLabel->setAlignment(Qt::AlignCenter);
//
//	QFont ft;
//	ft.setPointSize(6);
//
//	m_displayLabel->setFont(ft);
//	m_displayLabel->setVisible(true);
//	m_displayLabel->move(0, 0);
//
//	m_displayLabel2 = new QLabel(this);
//	m_displayLabel2->setFixedSize(QSize(40, 10));
//	m_displayLabel2->setStyleSheet("background-color: rgb(18, 59, 101);");
//	//设置游标背景为白色
//	//	m_displayLabel->setAutoFillBackground(true);
//	m_displayLabel2->setPalette(palette);
//	m_displayLabel2->setFont(ft);
//	m_displayLabel2->setAlignment(Qt::AlignCenter);
//
//	m_displayLabel2->setVisible(false);

	g_win_width[g_cur_img] = 12;
	type = 0;
}

MyCustomSlider::~MyCustomSlider()
{

}

void MyCustomSlider::mousePressEvent(QMouseEvent *event)
{
	//if (!m_displayLabel->isVisible())
	//{
	//	m_displayLabel->setVisible(true);
	//	m_displayLabel->setText(QString::number(this->value()));
	//}
	QSlider::mousePressEvent(event);
}

void MyCustomSlider::mouseReleaseEvent(QMouseEvent *event)
{
	//if (m_displayLabel->isVisible())
	//{
	//	m_displayLabel->setVisible(false);
	//}
	QSlider::mouseReleaseEvent(event);
}

void MyCustomSlider::mouseMoveEvent(QMouseEvent *event)
{
	//if (type)
	//{
	//	m_displayLabel->setText(QString::number(this->value()*1.0 / 100));
	//	m_displayLabel->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum())-15, 0);
	//	m_displayLabel2->setVisible(true);
	//	
	//	m_displayLabel2->setText(QString::number(this->value()*1.0 / 100+g_win_width[g_cur_img]));

	//	m_displayLabel2->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum())+15, 0);
	//}
	//else
	//{
	//	m_displayLabel->setText(QString::number(this->value()));

	//	m_displayLabel->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum()), 0);

	//}

	QSlider::mouseMoveEvent(event);
}

void MyCustomSlider::keyPressEvent(QKeyEvent *event)
{
	/*QWidget::keyPressEvent(event);
	if (type == 1)
	{
		if (event->key() == Qt::Key_Up)
		{
			this->setValue(this->value() + 1);

		}
		else if (event->key() == Qt::Key_Down)
		{
			this->setValue(this->value() - 1);
		}
		m_displayLabel->setText(QString::number(this->value()*1.0 / 100));

		m_displayLabel->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum())-15, 0);

		m_displayLabel2->setVisible(true);

		m_displayLabel2->setText(QString::number(this->value()*1.0 / 100 + g_win_width[g_cur_img]));

		m_displayLabel2->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum()) + 15, 0);
	}*/
}

void MyCustomSlider::paintEvent(QPaintEvent *ev)
{
	QSlider::paintEvent(ev);
	/*if (type == 1)
	{
		m_displayLabel->setText(QString::number(this->value()*1.0 / 100));

		m_displayLabel->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum()) - 15, 0);

		m_displayLabel2->setVisible(true);

		m_displayLabel2->setText(QString::number(this->value()*1.0 / 100 + g_win_width[g_cur_img]));

		m_displayLabel2->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum()) + 15, 0);
	}*/

}