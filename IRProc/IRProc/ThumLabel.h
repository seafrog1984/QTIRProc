#ifndef ThumLabel_H
#define ThumLabel_H
#include<QLabel>
#include<QString>
#include<QWidget>
#include<QMainWindow>
#include<QLine>
#include"QMessageBox"

#include <QLabel>

class ThumLabel : public QLabel
{
	Q_OBJECT

public:
	ThumLabel(QWidget *parent = 0);
	~ThumLabel();

	void mousePressEvent(QMouseEvent *ev);    //按下
	int getCurImgIndex(); //获取当前图像下标
	int cur_img;

signals:

	void thumClicked();

};
#endif // MYLABEL_H