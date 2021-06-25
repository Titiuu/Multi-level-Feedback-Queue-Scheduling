#include "MFQS.h"
#include <assert.h>
using namespace std;

void Progmlist::setsize()
{
	cout << "�����������:";
	cin >> Size;
	cout << endl;
}

void Progmlist::init()
{
	for (size_t i = 0; i < Size; i++)
	{
		Progm pro;
		cout << "��" << i + 1 << "�����̵����ơ�����ʱ�䡢ִ��ʱ����:";
		cin >> pro.name;
		cin >> pro.arrivetime;
		cin >> pro.needtime;
		pro.leftime = pro.needtime;//��ʼ��ʣ��ʱ��Ϊ����ʱ��
		prolist.push_back(pro);//����
	}
}

void Progmlist::print()
{
	cout << endl;
	cout << "����	 ����ʱ��	 ����ʱ��	������ʱ��	 ʣ��ʱ��" << endl;
	for (size_t i = 0; i < prolist.size(); i++)
	{
		cout << " " << prolist[i].name << "\t\t" << prolist[i].arrivetime << "\t\t" << prolist[i].needtime;
		cout << "\t\t" << prolist[i].needtime - prolist[i].leftime << "\t\t" << prolist[i].leftime << endl;
	}
}

void MFQS::refresh(Progm pgm)
{//������Ϣ
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
{//���ؼ���
	return Size;
}

void MFQS::printime()
{
	cout << endl;
	cout << "��ǰϵͳʱ��Ϊ: " << time;
}

void MFQS::getprgmlist(Progmlist progmlist)
{//�ഫ��
	itfce = progmlist;
}

void MFQS::start()
{//����һ���������,����time,ptr
	MQ[0].qu.push(itfce.prolist[ptr]);
	time = itfce.prolist[ptr].arrivetime;
	ptr++;
}

int MFQS::check()
{//��鵱ǰ������״̬��ȷ��Ӧ�����еĳ���(�������),����ǰ����Ϊ�գ�Ӧ����-1��return
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
{//���й�����ȷ���Ƿ����¼���Ľ���
	if (itfce.prolist[ptr].arrivetime < time + MQ[n].timeslice)
		return true;//��ǰ���򵽴�ʱ��<ϵͳʱ�����ʱ��Ƭ����ʱ��Ƭ�ڴ��ڵ���
	return false;
}

void MFQS::change1(int n)
{//�ý����õ�ǰ��β���ȳ����룩
	Progm temp = MQ[n].qu.front();
	MQ[n].qu.pop();
	MQ[n].qu.push(temp);
}

void MFQS::change2(int n)
{//�ý��̳�������һ������
	assert(n < 3);
	MQ[n + 1].qu.push(MQ[n].qu.front());
	MQ[n].qu.pop();
}

void MFQS::handle1(unsigned arrivetime, int n)
{//�жϵ�ǰ�������ȼ��Ƿ�Ϊ1
	if (MQ[n].priority != 1)
	{//�������򱣴浱ǰ����״̬��change1, ����time(arrivetime!=time)
		if (arrivetime == time) return;
		if (arrivetime - time <= MQ[n].qu.front().leftime)
		{//���С��ʣ��ʱ���ʾ���Լ�ȥ
			MQ[n].qu.front().leftime -= arrivetime - time;//ʣ��ʱ�����
			refresh(MQ[n].qu.front());//���µ�programlist��ȥ
			change1(n);
			time = arrivetime;
			printime();
			itfce.print();
		}
		else
		{//handle2
			time += MQ[n].qu.front().leftime;
			MQ[n].qu.front().leftime = 0;//ʣ��ʱ��Ϊ0
			refresh(MQ[n].qu.front());//���µ�programlist��ȥ
			MQ[n].qu.pop();
			printime();
			itfce.print();
		}
	}
	else
	{//������ȴ�ʱ��Ƭ�����󱣴浱ǰ����״̬��change1, ����time
		if (MQ[n].timeslice <= MQ[n].qu.front().leftime)
		{//ʱ��ƬС��ʣ��ʱ���ʾ���Լ�ȥ
			MQ[n].qu.front().leftime -= MQ[n].timeslice;//ʣ��ʱ�����
			refresh(MQ[n].qu.front());//���µ�programlist��ȥ
			change2(n);
			time += MQ[n].timeslice;
			printime();
			itfce.print();
		}
		else
		{//handle2
			time += MQ[n].qu.front().leftime;
			MQ[n].qu.front().leftime = 0;//ʣ��ʱ��Ϊ0
			refresh(MQ[n].qu.front());//���µ�programlist��ȥ
			MQ[n].qu.pop();
			printime();
			itfce.print();
		}
	}
}

void MFQS::handle2(int n)
{//���浱ǰ����״̬������,����time
	time += MQ[n].qu.front().leftime;
	MQ[n].qu.front().leftime = 0;//ʣ��ʱ��Ϊ0
	refresh(MQ[n].qu.front());//���µ�programlist��ȥ
	MQ[n].qu.pop();
	printime();
	itfce.print();
}

void MFQS::handle3(int n)
{//�жϵ�ǰ�������ȼ��Ƿ�Ϊ4(3�Ŷ���),��������Ϣ
	if (n == 3)
	{//���浱ǰ����״̬��change1������time
		MQ[n].qu.front().leftime -= MQ[n].timeslice;//ʣ��ʱ�����
		refresh(MQ[n].qu.front());//���µ�programlist��ȥ
		change1(n);
		time += MQ[n].timeslice;
		printime();
		itfce.print();
	}
	else
	{//���浱ǰ����״̬��change2������time
		MQ[n].qu.front().leftime -= MQ[n].timeslice;//ʣ��ʱ�����
		refresh(MQ[n].qu.front());//���µ�programlist��ȥ
		change2(n);
		time += MQ[n].timeslice;
		printime();
		itfce.print();
	}
}

void MFQS::run()
{//ִ������
	int qn = check();
	if (qn == -1)
	{//����Ϊ��
		if (ptr < itfce.Size)
		{//����Ϊ�յ���û�д��������������
			start();
			run();
			qn = check();//�����run()����Ҫ����qn
		}
		else return;//����Ϊ���ҽ��̶���ȫ��ִ����
	}
	assert(qn >= 0);
	if (ptr < itfce.Size)
	{
		if (haveNewArrive(qn))
		{//�У��������,handle1,print,����ִ��run
			MQ[0].qu.push(itfce.prolist[ptr]);
			handle1(itfce.prolist[ptr].arrivetime, qn);
			//itfce.print();
			ptr++;
			run();
		}
		else
		{//�ޣ��ж�ʱ��Ƭ����ǰ�Ƿ�ǰ����ʣ��ʱ��Ϊ0
			if (MQ[qn].timeslice > MQ[qn].qu.front().leftime)//ʱ��Ƭ����ʣ��ʱ���ʾ����������
			{//handle2,����ִ��run
				handle2(qn);
				//itfce.print();
				run();
			}
			else
			{//����handle3,����ִ��run
				handle3(qn);
				//itfce.print();
				run();
			}
		}
	}
	else if (ptr == itfce.Size)
	{
		if (MQ[qn].timeslice > MQ[qn].qu.front().leftime)//ʱ��Ƭ����ʣ��ʱ���ʾ����������
		{//handle2
			handle2(qn);
			//itfce.print();
			run();
		}
		else
		{//����handle3,����ִ��run
			handle3(qn);
			//itfce.print();
			run();
		}
	}
}


