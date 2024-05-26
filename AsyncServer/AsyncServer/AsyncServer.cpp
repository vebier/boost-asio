
#include"CServer.h"
#include <iostream>
#include<boost/asio.hpp>
#include<csignal>
#include<thread>
#include<mutex>
#include "LogicSystem.h"
#include "AsioThreadPool.h"
using namespace std;

bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
    try
    {
        auto pool= AsioThreadPool::GetInstance();
        boost::asio::io_context  io_context;
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&io_context,pool](auto, auto) {
            io_context.stop();
            pool->Stop();
            unique_lock<mutex> lock(mutex_quit);
            bstop = true;
            cond_quit.notify_one();
            });
        CServer s(pool->GetIOService(), 10086);
        {
            unique_lock<mutex> lock(mutex_quit);
            while (!bstop)
            {
                cond_quit.wait(lock);
            }
        }
       
    }
    catch (const std::exception& e)
    {
        cout << "exception code is " << e.what() << endl;
    }
   
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

