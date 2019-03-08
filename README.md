# WebServer

## 一个多线程服务器项目

### 目前实现的内容:
+ 不可拷贝的基类
+ 封装了当前线程的一些全局变量,包括线程名,tid等参数
+ 封装互斥锁以及持有互斥锁的对象
+ 封装条件变量
+ 封装了linux的线程
+ 实现了EventLoop事件循环
+ 封装了要监听的文件描述符
+ 封装了EPOLL的IO复用类
+ 完善EventLoop,通过与channel,epoll交互实现了reactor模式
