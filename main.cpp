#include "MFQS.h"
#include <iostream>
using namespace std;

int main()
{// make a change
	Progmlist list;
	list.setsize();
	list.init();
	MFQS mfqs;
	mfqs.getprgmlist(list);
	mfqs.start();
	mfqs.run();
	cout << "\n����ִ����ϣ�";
	return 0;
}// make changes