/**
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Name: SimuDouQue
 * Author: 吕跃鹏(Lniper)
 * Date: 2017-12-09
 * Description: 高级语言仿真一个双队列系统，并计算队列的平均延时，
 *              队列中的平均人数，每一个结算员的利用率。
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 **/
#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "lcgrand.h"

using namespace std;

#define BUSY    1           /// 忙状态
#define IDLE    0           /// 空闲状态

void initialize();              /// 初始化函数
void timing();                 /// 时间函数
void arrive();                  /// 到达队列
void depart();                  /// 离开队列
void report();                  /// 报告函数
float Myexpon(float mean);        /// 指数分布函数

FILE *outfile;                  /// 报告文件


int num_event = 0;              ///< 事件数量

float mean_interarrival1 = 2.1;        /// Regular队列1的到达时间间隔均值2.1
float mean_interarrival2 = 1.1;        /// Express队列2的到达时间间隔均值1.1

float mean_server1 = 2.0;        /// Regular队列1的服务时间均值2.0
float mean_server2 = 0.9;        /// Express队列2的服务时间均值2.0

float server1_status;						/// 两个队列服务员，初始都是空闲 server1_status, server2_status
float server2_status;						/// 两个队列服务员，初始都是空闲 server1_status, server2_status   
int num_in_q1;                /// regular队列1中的顾客数 num_in_q1                       
int num_in_q2;                /// express队列2中的顾客数 num_in_q2                       
float time_last_event1;         /// 队列1最后事件                                      
float time_last_event2;         /// 队列2最后事件       

int num_custs1_delayed;    /// regular队列1延时 num_custs1_delayed      
int num_custs2_delayed;    /// express队列2延时 num_custs2_delayed      
float total_of_delays;     /// 总延时 total_of_delays                    
float area_num_in_q1;      /// regular队列1中的数area_num_in_q1            
float area_num_in_q2;      /// express队列2中的数area_num_in_q2  

float time_que1_next_event[4] = {0};	/// 队列1下一事件
float time_que2_next_event[4] = {0};	/// 队列1下一事件
                                             

float Simulation_time_Count = 480;     /// 仿真时间8小时 = 480min
float Simulation_time = 0;             /// 仿真时间计数器

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    outfile = fopen("OutputFile.txt", "w");

    num_event = 3;

    fprintf(outfile, "Double-server queueing system\n");
    fprintf(outfile, "Mean interarrival Queue1 time %11.3f minutes\n\n", mean_interarrival1);
    fprintf(outfile, "Mean server Queue1 time %16.3f minutes\n\n", mean_server1);

    fprintf(outfile, "Mean interarrival Queue2 time %11.3f minutes\n\n", mean_interarrival2);
    fprintf(outfile, "Mean server Queue2 time %16.3f minutes\n\n", mean_server2);

    initialize();

    while(Simulation_time < Simulation_time_Count) {
        /**
         * Determine the next event.
         **/
        timing();


    }


    fclose(outfile);

    return a.exec();
}

/**
 * @brief initialize
 * 初始化函数，初始化仿真时钟
 * 初始化状态变量
 * 初始化统计计数器
 * 初始化事件表
 */
void initialize()
{
    /**
     * 初始化仿真时间计数器
     **/
    Simulation_time = 0;

    /**
     * 初始化状态变量
     * 两个队列服务员，初始都是空闲 server1_status, server2_status
     * regular队列1中的顾客数 num_in_q1
     * express队列2中的顾客数 num_in_q2
     * 初始化队列1最后事件
     * 初始化队列2最后事件
     **/
    server1_status = IDLE;
    server2_status = IDLE;
    num_in_q1 = 0;
    num_in_q2 = 0;
    time_last_event1 = 0;
    time_last_event2 = 0;

    /**
     * 初始化统计计数器
     * regular队列1延时 num_custs1_delayed
     * express队列2延时 num_custs2_delayed
     * 总延时 total_of_delays
     * regular队列1中的数area_num_in_q1
     * express队列2中的数area_num_in_q2
     **/
    num_custs1_delayed = 0;
    num_custs2_delayed = 0;
    total_of_delays = 0;
    area_num_in_q1 = 0;
    area_num_in_q2 = 0;

    /**
     * 初始化事件表
     * time_que1_next_event队列1的事件表，[1]:到达；[2]：离开；[3]:调度时间结束
     * time_que2_next_event队列2的事件表，[1]:到达；[2]：离开；[3]:调度时间结束
     **/
    time_que1_next_event[1] = Simulation_time + Myexpon(mean_interarrival1);
    time_que1_next_event[2] = 1.0e+30;
    time_que1_next_event[3] = Simulation_time_Count;

    time_que2_next_event[1] = Simulation_time + Myexpon(mean_interarrival1);
    time_que2_next_event[2] = 1.0e+30;
    time_que2_next_event[3] = Simulation_time_Count;

}

/**
 * @brief timing
 * 时间函数，下一事件表
 */
void timing()
{

}

/**
 * @brief Myexpon
 * @param mean
 * @return
 * 产生指数分布的随机变量
 */
float Myexpon(float mean)
{
    return -mean * log(lcgrand(1));
}
