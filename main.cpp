#include "MFQS.h"
#include <iostream>
using namespace std;

int main()
{
	Progmlist list;
	list.setsize();
	list.init();
	MFQS mfqs;
	mfqs.getprgmlist(list);
	mfqs.start();
	mfqs.run();
	cout << "\n����ִ����ϣ�";
	return 0;
}