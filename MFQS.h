#pragma once

#include <iostream>
#include <queue>
#include <vector>
#include <string>

struct Progm{
	std::string name="";
	unsigned arrivetime = 0;
	unsigned needtime = 0;
	unsigned leftime = 0;
};

struct Qu {
	std::queue<Progm> qu;
	unsigned timeslice = 1;
	unsigned priority = 1;
};

class Progmlist {
public:
	void setsize();//设置进程队列长度
	void init();//初始化进程队列中进程信息
	void print();//打印当前队列信息
	std::vector<Progm> prolist;
	unsigned Size = 0;
};

class MFQS {
public:
	MFQS() {//初始化
		for (size_t i = 0; i < Size; i++)
		{
			MQ[i].priority = i + 1;
			MQ[i].timeslice = (i + 1) * 2;
		}
	}
	unsigned size();//返回反馈队列级数
	void printime();//打印系统当前时间
	void getprgmlist(Progmlist prgmlist);//获取进程队列
	void start();//将第一个进程入队，更新time,ptr
	void run();//执行主体
	int check();//检查当前队列中状态，确定应该运行的程序
	bool haveNewArrive(int qn);//是否有新到达程序
	void change1(int qn);//进程调度1
	void change2(int qn);//进程调度2
	void handle1(unsigned arrivetime, int qn);//事件处理1
	void handle2(int qn);//事件处理2
	void handle3(int qn);//事件处理3
	void refresh(Progm pgm);//更新itfce中信息
	//bool finished();//判断进程队列是否已经完全执行完毕

private:
	Qu MQ[4];//多级队列数组
	unsigned Size = 4;//队列大小
	unsigned time = 0;//系统时间
	Progmlist itfce;//捕获进程队列的接口
	unsigned ptr = 0;//指向当前即将要入队的进程
};