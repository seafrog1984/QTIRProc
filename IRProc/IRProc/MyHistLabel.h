#ifndef MYHISTLABEL_H
#define MYHISTLABEL_H
#include<QLabel>
#include<QString>
#include<QWidget>
#include<QMainWindow>
#include<QLine>
#include"QMessageBox"


class MyHistLabel :public QLabel
{
	Q_OBJECT
public:
	MyHistLabel(QWidget *parent = 0);
	~MyHistLabel(){}

signals:

	void calHist();


protected:

	void mouseReleaseEvent(QMouseEvent *ev);  //抬起
	void mousePressEvent(QMouseEvent *ev);    //按下
	void mouseDoubleClickEvent(QMouseEvent *ev);  //双击
	void mouseMoveEvent(QMouseEvent *ev);     //拖动
	void paintEvent(QPaintEvent *ev);
	void keyPressEvent(QKeyEvent *ev);

	QPoint p1, p2;

};

#endif // MyHistLabel_H
