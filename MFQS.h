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
}; // changes

struct Qu {
	std::queue<Progm> qu;
	unsigned timeslice = 1;
	unsigned priority = 1;
};

class Progmlist {
public:
	void setsize();//���ý��̶��г���
	void init();//��ʼ�����̶����н�����Ϣ
	void print();//��ӡ��ǰ������Ϣ
	std::vector<Progm> prolist;
	unsigned Size = 0;
};

class MFQS {
public:
	MFQS() {//��ʼ��
		for (size_t i = 0; i < Size; i++)
		{
			MQ[i].priority = i + 1;
			MQ[i].timeslice = (i + 1) * 2;
		}
	}
	unsigned size();//���ط������м���
	void printime();//��ӡϵͳ��ǰʱ��
	void getprgmlist(Progmlist prgmlist);//��ȡ���̶���
	void start();//����һ��������ӣ�����time,ptr
	void run();//ִ������
	int check();//��鵱ǰ������״̬��ȷ��Ӧ�����еĳ���
	bool haveNewArrive(int qn);//�Ƿ����µ������
	void change1(int qn);//���̵���1
	void change2(int qn);//���̵���2
	void handle1(unsigned arrivetime, int qn);//�¼�����1
	void handle2(int qn);//�¼�����2
	void handle3(int qn);//�¼�����3
	void refresh(Progm pgm);//����itfce����Ϣ
	//bool finished();//�жϽ��̶����Ƿ��Ѿ���ȫִ�����

private:
	Qu MQ[4];//�༶��������
	unsigned Size = 4;//���д�С
	unsigned time = 0;//ϵͳʱ��
	Progmlist itfce;//������̶��еĽӿ�
	unsigned ptr = 0;//ָ��ǰ����Ҫ��ӵĽ���
};