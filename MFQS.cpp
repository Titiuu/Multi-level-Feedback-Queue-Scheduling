#include "MFQS.h"
#include <assert.h>
using namespace std;

void Progmlist::setsize()
{
	cout << "请输入进程数:";
	cin >> Size;
	cout << endl;
}

void Progmlist::init()
{
	for (size_t i = 0; i < Size; i++)
	{
		Progm pro;
		cout << "第" << i + 1 << "个进程的名称、到达时间、执行时间是:";
		cin >> pro.name;
		cin >> pro.arrivetime;
		cin >> pro.needtime;
		pro.leftime = pro.needtime;//初始化剩余时间为所需时间
		prolist.push_back(pro);//保存
	}
}

void Progmlist::print()
{
	cout << endl;
	cout << "进程	 到达时间	 所需时间	已运行时间	 剩余时间" << endl;
	for (size_t i = 0; i < prolist.size(); i++)
	{
		cout << " " << prolist[i].name << "\t\t" << prolist[i].arrivetime << "\t\t" << prolist[i].needtime;
		cout << "\t\t" << prolist[i].needtime - prolist[i].leftime << "\t\t" << prolist[i].leftime << endl;
	}
}

void MFQS::refresh(Progm pgm)
{//更新信息
	for (size_t i = 0; i < itfce.Size; i++)
	{
		if (itfce.prolist[i].name == pgm.name)
			itfce.prolist[i].leftime = pgm.leftime;
	}
}

//bool MFQS::finished()
//{
//	for (size_t i = 0; i < itfce.Size; i++)
//		if (itfce.prolist[i].leftime != 0) return false;
//	return true;
//}

unsigned MFQS::size()
{//返回级数
	return Size;
}

void MFQS::printime()
{
	cout << endl;
	cout << "当前系统时间为: " << time;
}

void MFQS::getprgmlist(Progmlist progmlist)
{//类传递
	itfce = progmlist;
}

void MFQS::start()
{//将第一个进程入队,更新time,ptr
	MQ[0].qu.push(itfce.prolist[ptr]);
	time = itfce.prolist[ptr].arrivetime;
	ptr++;
}

int MFQS::check()
{//检查当前队列中状态，确定应该运行的程序(队列序号),若当前队列为空，应返回-1以return
	if (MQ[0].qu.empty()) {
		if (MQ[1].qu.empty()) {
			if (MQ[2].qu.empty()) {
				if (MQ[3].qu.empty()) {
					return -1;
				}
				else return 3;
			}
			else return 2;
		}
		else return 1;
	}
	else return 0;
}

bool MFQS::haveNewArrive(int n)
{//运行过程中确定是否有新加入的进程
	if (itfce.prolist[ptr].arrivetime < time + MQ[n].timeslice)
		return true;//当前程序到达时间<系统时间加上时间片，即时间片内存在到达
	return false;
}

void MFQS::change1(int n)
{//该进程置当前队尾（先出后入）
	Progm temp = MQ[n].qu.front();
	MQ[n].qu.pop();
	MQ[n].qu.push(temp);
}

void MFQS::change2(int n)
{//该进程出队入下一级队列
	assert(n < 3);
	MQ[n + 1].qu.push(MQ[n].qu.front());
	MQ[n].qu.pop();
}

void MFQS::handle1(unsigned arrivetime, int n)
{//判断当前程序优先级是否为1
	if (MQ[n].priority != 1)
	{//若不是则保存当前运行状态并change1, 更新time(arrivetime!=time)
		if (arrivetime == time) return;
		if (arrivetime - time <= MQ[n].qu.front().leftime)
		{//相差小于剩余时间表示可以减去
			MQ[n].qu.front().leftime -= arrivetime - time;//剩余时间减少
			refresh(MQ[n].qu.front());//更新到programlist中去
			change1(n);
			time = arrivetime;
			printime();
			itfce.print();
		}
		else
		{//handle2
			time += MQ[n].qu.front().leftime;
			MQ[n].qu.front().leftime = 0;//剩余时间为0
			refresh(MQ[n].qu.front());//更新到programlist中去
			MQ[n].qu.pop();
			printime();
			itfce.print();
		}
	}
	else
	{//若是则等待时间片结束后保存当前运行状态并change1, 更新time
		if (MQ[n].timeslice <= MQ[n].qu.front().leftime)
		{//时间片小于剩余时间表示可以减去
			MQ[n].qu.front().leftime -= MQ[n].timeslice;//剩余时间减少
			refresh(MQ[n].qu.front());//更新到programlist中去
			change2(n);
			time += MQ[n].timeslice;
			printime();
			itfce.print();
		}
		else
		{//handle2
			time += MQ[n].qu.front().leftime;
			MQ[n].qu.front().leftime = 0;//剩余时间为0
			refresh(MQ[n].qu.front());//更新到programlist中去
			MQ[n].qu.pop();
			printime();
			itfce.print();
		}
	}
}

void MFQS::handle2(int n)
{//保存当前运行状态并出队,更新time
	time += MQ[n].qu.front().leftime;
	MQ[n].qu.front().leftime = 0;//剩余时间为0
	refresh(MQ[n].qu.front());//更新到programlist中去
	MQ[n].qu.pop();
	printime();
	itfce.print();
}

void MFQS::handle3(int n)
{//判断当前队列优先级是否为4(3号队列),并更新信息
	if (n == 3)
	{//保存当前运行状态并change1，更新time
		MQ[n].qu.front().leftime -= MQ[n].timeslice;//剩余时间减少
		refresh(MQ[n].qu.front());//更新到programlist中去
		change1(n);
		time += MQ[n].timeslice;
		printime();
		itfce.print();
	}
	else
	{//保存当前运行状态并change2，更新time
		MQ[n].qu.front().leftime -= MQ[n].timeslice;//剩余时间减少
		refresh(MQ[n].qu.front());//更新到programlist中去
		change2(n);
		time += MQ[n].timeslice;
		printime();
		itfce.print();
	}
}

void MFQS::run()
{//执行主体
	int qn = check();
	if (qn == -1)
	{//队列为空
		if (ptr < itfce.Size)
		{//队列为空但是没有处理完则重新入队
			start();
			run();
			qn = check();//从这个run()出来要更新qn
		}
		else return;//队列为空且进程队列全部执行完
	}
	assert(qn >= 0);
	if (ptr < itfce.Size)
	{
		if (haveNewArrive(qn))
		{//有：将其入队,handle1,print,重新执行run
			MQ[0].qu.push(itfce.prolist[ptr]);
			handle1(itfce.prolist[ptr].arrivetime, qn);
			//itfce.print();
			ptr++;
			run();
		}
		else
		{//无：判断时间片用完前是否当前程序剩余时间为0
			if (MQ[qn].timeslice > MQ[qn].qu.front().leftime)//时间片大于剩余时间表示可以运行完
			{//handle2,重新执行run
				handle2(qn);
				//itfce.print();
				run();
			}
			else
			{//否则handle3,重新执行run
				handle3(qn);
				//itfce.print();
				run();
			}
		}
	}
	else if (ptr == itfce.Size)
	{
		if (MQ[qn].timeslice > MQ[qn].qu.front().leftime)//时间片大于剩余时间表示可以运行完
		{//handle2
			handle2(qn);
			//itfce.print();
			run();
		}
		else
		{//否则handle3,重新执行run
			handle3(qn);
			//itfce.print();
			run();
		}
	}
}


