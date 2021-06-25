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
	cout << "\n进程执行完毕！";
	return 0;
}