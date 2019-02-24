#pragma once

namespace currentthread
{
		extern __thread int t_cacheTid();//线程的真实ID
		extern __thread char t_tidString[32];//tid的整型
		extern __thread int t_tidStringLength;//string类型tid的长度
		extern __thread const char* t_threadName;//线程的名字
		void cacheTid();
		inline int tid()
		{
				if(__builtin_expect(t_catchedTid==0,0))
						//__builitin_expect的作用是告诉编译器
						//t_catchedTid==0的概率大
				{
						cacheTid();
				}
				return t_cacheTid;
		}
		inline const char* tidString()
		{
				return t_tidString;
		}
		inline int tidStringLength()
		{
				return t_tidStringLength;
		}
		inline const char* name()
		{
				return t_threadName;
		}
}
