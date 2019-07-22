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
//	void clicked(QMouseEvent *ev);  //̧��
//	void pressed(QMouseEvent *ev);  //����
//	void doubled(QMouseEvent *ev);  //˫��
//	void moved(QMouseEvent *ev);    //�϶�
//	//void sig_GetOneFrame(QImage img);

protected:

	void mouseReleaseEvent(QMouseEvent *ev);  //̧��
	void mousePressEvent(QMouseEvent *ev);    //����
	void mouseDoubleClickEvent(QMouseEvent *ev);  //˫��
	void mouseMoveEvent(QMouseEvent *ev);     //�϶�
	void paintEvent(QPaintEvent *ev);
	void keyPressEvent(QKeyEvent *ev);
public:

	void setOffset(QPoint offset);
	void calPar(int cur_shape_no);//ͳ�������С��
	void calRealCor(QPoint pt, int &rx,int &ry);//�������λ����ԭʼͼ���е�ʵ������
	void draw_shape(int shape_no);
	int getCurImgIndex(); //��ȡ��ǰͼ���±�
	QPoint offset;              //һ�ε�ͼƬƫ��ֵ
	QPoint Alloffset;           //��ƫ��
	int m_flag_press;
	int m_shapeType;
	int m_action;//������ʶ��0-�����ƶ���1-�㣻2-���Σ�3-Բ��4-��Բ�� 5-ɾ��
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
