
#include "common.h"
#include "logger.h"
#include "threadpool.h"
#include <fcntl.h>
#include <stdio.h>

using namespace std;

//
// rm ./threadpool_test &&  g++ -g *.cpp -o threadpool_test -lpthread -std=c++14
//

void taskFunc(void)
{
    // LOG( "threadID  %llu ...  ", pthread_self());
    static int64_t count = 0;
    // int64_t count =0;
    // count++;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // LOG( "count = %ld", ++count);
}

FILE *g_pfile = NULL;

int main()
{
    if (!g_pfile)
    {
        g_pfile = fopen("./1234.txt", "a+");
        if (g_pfile == NULL)
        {
            perror("fopen...");
        }
    }

    // FILE_LOG_DBG(g_pfile, "hello", "2222", "4444");
    // LogConsole::LogDebug("hahah", 2, "12345", "adad");
    // CONSOLE_LOG_DBG("123\n");
    // CONSOLE_LOG_DBG("%d %d %s\n",222,333,"12344");

    // char tmp[32] = {};
    // sprintf(tmp, "%s", __TIMESTAMP__);
    // sprintf(tmp, "%lu", __TIMESTAMP__);linux
    // cout << tmp << endl;
    // cout << __TIMESTAMP__ << endl;

    // fclose(g_pfile);
    // return 0;

    //

    // ThreadPool pool(3, 10);   // 最大10个线程,
    ThreadPool pool(3, 30); // 最大30个线程,
    for (int i = 0; i < 10000; i++) //== 10000个线程测试  "======= increase thread  ======="  "======= decrease thread  =======",
    {
        pool.AddTask(std::bind(taskFunc));
        // pool.AddTask(Task(taskFunc, pa));
    }

    // 在析构 ~ThreadPool() 中有一个 [ while( m_busyNum != 0) { sleep() }  +  sleep(1) ] 的阻塞, 会保证线程池中所有线程的正常退出,
    // 如果 main() 想要任务执行完, 那么就必须给予线程池足够的时间, 不让线程池析构掉,
    this_thread::sleep_for(chrono::seconds(50)); // 让线程池有足够的时间来执行完任务队列, 否则当前线程退出时候,销毁ThreadPool 对象发送 SIGKILL信号 ,

    return 0;
}
