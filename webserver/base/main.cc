#include "thread.h"
#include <functional>
#include <stdio.h>
#include <string>
void pdata()
{
		printf("hello world\n");
}

int main()
{
		Thread* thread=new Thread(&pdata,"pdata");
		thread->start();
}
