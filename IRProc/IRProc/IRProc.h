#pragma once
#include <QtWidgets/QMainWindow>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include "ui_IRProc.h"
#include "string.h"
#include "SetAuthDlg.h"
#include "SettingDlg.h"
#include "RegDlg.h"
#include "CrossDlg.h"
#include "ThumLabel.h"
#include "CamParDlg.h"
#include <QProgressDialog>

struct shape{
	bool del_flag;
	int shape_type; //1.点，2.矩形 3. 圆 4.椭圆
	int lt_x, lt_y;//左上角坐标
	int rb_x, rb_y;//右下角坐标
	double t_max;
	double t_min;
	double t_aver;
	double t_msd;
	std::string comment;//注释
};
typedef struct shape Shape;

using namespace cv;

class IRProc : public QMainWindow
{
	Q_OBJECT

public:
	IRProc(QWidget *parent = Q_NULLPTR);

	void conDataBase();//连接数据库
	void updateData();
	void addData(int index, QString cardID, QString scanID, QString RegTime);
	void keyUpdate();

	//virtual void keyPressEvent(QKeyEvent *event);
//	bool eventFilter(QObject *, QEvent *);

private:
	Ui::IRProcClass ui;

	void imgProc();//图像处理
	void imgMerge();//图像融合
	void dataManage();//数据管理
	void sysSetting();//系统设置
	virtual void resizeEvent(QResizeEvent * event);


public slots:

	void userAreaFull();//系统设置-用户区最大化
	void toolBarExpand();//系统设置-展开所有工具栏
	void sysSettingOp();//系统设置-权限设置
	void customize();//系统设置-定制
	void btn_sysPar();//系统参数设置
	void btn_show_func();//显示功能区
	void btn_camPar();//温度修正参数设置

	void imgChange();//图像右上角按钮
	void btnAnalyze();//打开图像，转到分析页面
	void updateImage();//更新图像
	void colorTypeChange();
	void changeWinWidth();//修改窗宽
	void setFilter(int);//设置滤波方式
	void addPoint();//画点
	void addRect();//画矩形
	void addCircle();//画圆形
	void addEllipse();//画椭圆
	void btn_closeAll();//关闭所有
	void btn_closeOther();//关闭其他
	void sliderBot();//断层
	void btn_hist();//直方图
	void btn_cross();//交叉线分析
	void btn_sel();//选择
	void btn_zoom();//放大缩小
	void showTemper();//显示温度
	void saveMeasure();//保存测量
	void setUpAll();//同步断层
	void tagSel();//选择标签
	void tagDel();//删除标签
	void keyUp();//键盘断层up
	void keyDown();//键盘断层down
	void keyCtrlUp();//键盘断层ctrl+up
	void keyCtrlDown();//键盘断层ctrl+down
	void keyShiftUp();//键盘断层shift+up
	void keyShiftDown();//键盘断层shift+down
	void nameChanged(int);//缩略图切换人名
	void scanChanged(QListWidgetItem*);//缩略图切换扫描号
	

	void btnMerDef();//自定义融合图
	void sliderMerRatio();//融合度调整
	void sliderMerRatio2();//融合度调整
	void btnMerWid();//融合图变宽
	void btnMerNar();//融合图变窄
	void btnMerHigher();//融合图变高
	void btnMerLower();//融合图变矮
	void btnMerLeft();//融合图左移
	void btnMerRight();//融合图右移
	void btnMerUp();//融合图上移
	void btnMerDown();//融合图下移
	void changeMerGender();//设置性别
	void changeMerPose();//设置体位
	void changeMerType();//设置类型

	
	void btn_del();
	void btn_change();
	void btn_pre();
	void btn_next();
	void btn_start();
	void btn_end();
	void btn_dateSel();
	void btn_nameSel();
	void btn_showAll();
	void radio_week();
	void radio_month();
	void radio_year();
	void dataOut();//数据导出
	void dataIn();//数据导入

	void calData();//更新数据区
	void upInfo();//更新图像信息
	void time_update();//更新时间
	void thumClicked();//点击缩略图

public:
	void changeLabel(int totalNum,int imagePerRow);//调整显示窗口数
	void showImage(int totalNum);//显示图像

	void wheelEvent(QWheelEvent*event); //这个是鼠标上下滚轮的时候会调用

	double calTR(Mat &img);//计算图像均方差

	void showThum();//显示缩略图

	void updateMer();//更新融合图

	void resetShape(int cur_img);//重置选框

	void updateTag();//更新选框信息

	int m_mx, m_my;

	int m_user_index;
	QString m_user[2];
	QString m_scan[2];

	QProgressDialog *progressDialog;

	SettingDlg *dlg;
	SetAuthDlg *adlg;
	RegDlg *rdlg;
	CrossDlg *cdlg;
	CamParDlg *camdlg;

	QString m_msg;

	QLabel *currentTimeLabel;//显示系统时间
};
