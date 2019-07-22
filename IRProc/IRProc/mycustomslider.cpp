#include "mycustomslider.h"
#include <QPalette>

MyCustomSlider::MyCustomSlider(QWidget *parent) :QSlider(parent)
{
	m_displayLabel = new QLabel(this);
	m_displayLabel->setFixedSize(QSize(25, 10));
	//设置游标背景为白色
	m_displayLabel->setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Background, Qt::white);
	m_displayLabel->setPalette(palette);

	m_displayLabel->setAlignment(Qt::AlignCenter);

	m_displayLabel->setVisible(false);
	m_displayLabel->move(0, 0);

	type = 0;
}

MyCustomSlider::~MyCustomSlider()
{

}

void MyCustomSlider::mousePressEvent(QMouseEvent *event)
{
	if (!m_displayLabel->isVisible())
	{
		m_displayLabel->setVisible(true);
		m_displayLabel->setText(QString::number(this->value()));
	}
	QSlider::mousePressEvent(event);
}

void MyCustomSlider::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_displayLabel->isVisible())
	{
		m_displayLabel->setVisible(false);
	}
	QSlider::mouseReleaseEvent(event);
}

void MyCustomSlider::mouseMoveEvent(QMouseEvent *event)
{
	if (type==0)
		m_displayLabel->setText(QString::number(this->value()));
	else
		m_displayLabel->setText(QString::number(this->value()*1.0/10));
	m_displayLabel->move((this->width() - m_displayLabel->width())*this->value() / (this->maximum() - this->minimum()), 0);

	QSlider::mouseMoveEvent(event);
}
