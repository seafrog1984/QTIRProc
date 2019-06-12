#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_IRProc.h"

class IRProc : public QMainWindow
{
	Q_OBJECT

public:
	IRProc(QWidget *parent = Q_NULLPTR);

private:
	Ui::IRProcClass ui;

	void imgProc();//图像处理
	void imgMerge();//图像融合
	void dataManage();//数据管理
	void sysSetting();//系统设置

public slots:

	void userAreaFull();//系统设置-用户区最大化
	void toolBarExpand();//系统设置-展开所有工具栏
	void sysSettingOp();//系统设置-权限设置
	void customize();//系统设置-定制

	void imgChange();//图像右上角按钮
	void btnAnalyze();//打开图像，转到分析页面
	void updateImage();//更新图像
	void colorTypeChange();
	void changeWinWidth();
	void setStep();//设置断层步长
	void setFilter(int);//设置滤波方式



public:
	void changeLabel(int totalNum,int imagePerRow);//调整显示窗口数
	void showImage(int totalNum);//显示图像

	void wheelEvent(QWheelEvent*event); //这个是鼠标上下滚轮的时候会调用

	void mousePressEvent(QMouseEvent *event);//鼠标点击事件，实现放大缩小

};
