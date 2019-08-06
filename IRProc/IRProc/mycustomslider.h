#ifndef MYCUSTOMSLIDER_H
#define MYCUSTOMSLIDER_H
#include <QSlider>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>

class MyCustomSlider : public QSlider
{
public:
	MyCustomSlider(QWidget *parent = 0);
	~MyCustomSlider();
	int type;

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent *ev);

public:
	//QLabel*	m_displayLabel;
	//QLabel*	m_displayLabel2;
};

#endif // MYCUSTOMSLIDER_H
