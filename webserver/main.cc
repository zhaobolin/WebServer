#include "thread.h"
#include <functional>
#include <stdio.h>
#include <string>
void pdata()
{
		printf("hello world\n");
		printf("子线程tid的值为:%d\n线程名为:%s\n",currentthread::tid(),currentthread::name());
}

int main()
{
		printf("主线程tid的值为:%d\n线程名为:%s\n",currentthread::tid(),currentthread::name());
		Thread* thread=new Thread(&pdata,"pdata");
		thread->start();
}
