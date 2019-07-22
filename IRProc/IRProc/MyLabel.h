#ifndef MYLABEL_H
#define MYLABEL_H
#include<QLabel>
#include<QString>
#include<QWidget>
#include<QMainWindow>
#include<QLine>
#include"QMessageBox"
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>

class MyLabel :public QLabel
{
	Q_OBJECT
public:
	MyLabel( QWidget *parent = 0);
	~MyLabel(){}

	void setShapeType(int shapeType);
signals:

	void calData();
//	void clicked(QMouseEvent *ev);  //抬起
//	void pressed(QMouseEvent *ev);  //按下
//	void doubled(QMouseEvent *ev);  //双击
//	void moved(QMouseEvent *ev);    //拖动
//	//void sig_GetOneFrame(QImage img);

protected:

	void mouseReleaseEvent(QMouseEvent *ev);  //抬起
	void mousePressEvent(QMouseEvent *ev);    //按下
	void mouseDoubleClickEvent(QMouseEvent *ev);  //双击
	void mouseMoveEvent(QMouseEvent *ev);     //拖动
	void paintEvent(QPaintEvent *ev);
	void keyPressEvent(QKeyEvent *ev);
public:

	void setOffset(QPoint offset);
	void calPar(int cur_shape_no);//统计最大最小等
	void calRealCor(QPoint pt, int &rx,int &ry);//计算鼠标位置在原始图像中的实际坐标
	void draw_shape(int shape_no);
	int getCurImgIndex(); //获取当前图像下标
	QPoint offset;              //一次的图片偏移值
	QPoint Alloffset;           //总偏移
	int m_flag_press;
	int m_shapeType;
	int m_action;//动作标识：0-缩放移动；1-点；2-矩形；3-圆；4-椭圆； 5-删除
	QPoint p1, p2;
	QVector <QLine> lines;
	//private slots :
	//void  slotGetOneFrame(QImage img);

	QString m_name;
	QString m_age;
	QString m_gender;
	QString m_ID;
	QString m_scanID;
	QString m_cardID;

	QImage wb_Image;
	int cur_img;
	double m_temper;
};

#endif // MYLABEL_H
