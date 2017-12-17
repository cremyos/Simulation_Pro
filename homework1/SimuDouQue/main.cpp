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
#define MAXLIMIT 10000      /// 虽然题目没有规定队列长度限制，但是给了一个系统的上限

void initialize();              /// 初始化函数
void timing_Que1();                 /// 队列1时间函数
void timing_Que2();                 /// 队列2时间函数
void arrive_que1();                  /// 到达队列1
void arrive_que2();                  /// 到达队列2
void depart_que1();                  /// 离开队列1
void depart_que2();                  /// 离开队列2
void report();                  /// 报告函数
float Myexpon(float mean);        /// 指数分布函数

FILE *outfile;                  /// 报告文件


int num_event = 0;              ///< 事件数量，1、到达事件；2、离开事件；3、时间到事件

int next_event_type_que1;           ///< 下一事件类型队列1
int next_event_type_que2;           ///< 下一事件类型队列2

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

int num_que1_delayed;    /// regular队列1延时 num_que1_delayed
int num_que2_delayed;    /// express队列2延时 num_que2_delayed
float total_of_delays1;     /// 队列1总延时 total_of_delays1
float total_of_delays2;     /// 对列2总延时 total_of_delays2
float area_num_in_q1;      /// regular队列1中的数area_num_in_q1            
float area_num_in_q2;      /// express队列2中的数area_num_in_q2  

float area_server1_status;  /// 队列1的服务员服务时间
float area_server2_status;  /// 队列2的服务员服务时间

float time_que1_next_event[4] = {0};	/// 队列1下一事件
float time_que2_next_event[4] = {0};	/// 队列1下一事件

float time_arrival_que1[MAXLIMIT] = {0};    /// 队列1的到达时间
float time_arrival_que2[MAXLIMIT] = {0};    /// 队列2的到达时间
                                             

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
//        timing();


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
     * regular队列1延时 num_que1_delayed
     * express队列2延时 num_que2_delayed
     * 总延时 total_of_delays
     * regular队列1中的数area_num_in_q1
     * express队列2中的数area_num_in_q2
     **/
    num_que1_delayed = 0;
    num_que2_delayed = 0;
    total_of_delays1 = 0;
    total_of_delays2 = 0;
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
 * 队列1时间函数，下一事件表
 */
void timing_Que1()
{
    int i = 0;
    float min_time_next_event = 1.0e+29;

    next_event_type_que1 = 0;
    /**
     * 决定下一事件类型
     **/
    for(i = 1; i <= num_event; i++) {
        if(time_que1_next_event[i] < min_time_next_event) {
            min_time_next_event = time_que1_next_event[i];
            next_event_type_que1 = i;
        }

        if(0 == next_event_type_que1) {
            /**
             * 事件列表为空，停止仿真
             **/
            fprintf(outfile, "\nEvent list empty at time %f", Simulation_time);
            exit(1);
        }
    }

    Simulation_time = min_time_next_event;
}

/**
 * @brief timing
 * 队列2时间函数，下一事件表
 */
void timing_Que2()
{
    int i = 0;
    float min_time_next_event = 1.0e+29;

    next_event_type_que2 = 0;
    /**
     * 决定下一事件类型
     **/
    for(i = 1; i <= num_event; i++) {
        if(time_que2_next_event[i] < min_time_next_event) {
            min_time_next_event = time_que2_next_event[i];
            next_event_type_que2 = i;
        }

        if(0 == next_event_type_que2) {
            /**
             * 事件列表为空，停止仿真
             **/
            fprintf(outfile, "\nEvent list empty at time %f", Simulation_time);
            exit(1);
        }
    }

    Simulation_time = min_time_next_event;
}


/**
 * @brief arrive
 * 队列1到达事件
 */
void arrive_que1()
{
    float delay;

    time_que1_next_event[1] = Simulation_time + Myexpon(mean_interarrival1);

    if((server1_status == BUSY) && (server2_status == BUSY)) {
        /**
         * 判断两个队列的服务员是否都处于忙的状态
         **/

        ++num_in_q1;                        /// 入队列

        time_arrival_que1[num_in_q1] = Simulation_time;
    } else{
        /**
         * 队列1的服务员都空闲的时候，顾客到达延迟为0
         **/
        delay = 0.0;
        total_of_delays1 += delay;

        ++num_que1_delayed;
        server1_status = BUSY;

        time_que1_next_event[2] = Simulation_time + Myexpon(mean_server1);
    }
}

/**
 * @brief arrive
 * 队列2到达事件
 */
void arrive_que2()
{
    float delay;

    time_que2_next_event[1] = Simulation_time + Myexpon(mean_interarrival1);

    if((server1_status == BUSY) && (server2_status == BUSY)) {
        /**
         * 判断两个队列的服务员是否都处于忙的状态
         **/

        ++num_in_q1;                        /// 入队列

        time_arrival_que2[num_in_q1] = Simulation_time;
    } else {
        /**
         * 队列2的服务员都空闲的时候，顾客到达延迟为0
         * 但是队列1的顾客可以让队列2的服务员服务
         **/
        delay = 0.0;
        total_of_delays2 += delay;

        ++num_que2_delayed;
        server2_status = BUSY;

        time_que2_next_event[2] = Simulation_time + Myexpon(mean_server1);
    }
}

/**
 * @brief depart_que1
 * 队列1的离开事件
 */
void depart_que1()
{
    int i;
    float delay;

    if(num_in_q1 == 0){
        server1_status == IDLE;
        time_que1_next_event[2] = 1.0e+30;
    } else{
        num_in_q1--;
        delay = Simulation_time - time_arrival_que1[1];
        total_of_delays1 += delay;

        num_que1_delayed++;
        time_que1_next_event[2] = Simulation_time + Myexpon(mean_server1);

        for(i = 1; i <= num_in_q1; i++) {
            time_arrival_que1[i] = time_arrival_que1[i+1];
        }
    }
}


/**
 * @brief depart_que2
 * 队列2的离开事件
 */
void depart_que2()
{
    int i;
    float delay;

    if(num_in_q2 == 0){
        server2_status == IDLE;
        time_que2_next_event[2] = 1.0e+30;
    } else{
        num_in_q2--;
        delay = Simulation_time - time_arrival_que2[1];
        total_of_delays2 += delay;

        num_que2_delayed++;
        time_que2_next_event[2] = Simulation_time + Myexpon(mean_server1);

        for(i = 1; i <= num_in_q2; i++) {
            time_arrival_que2[i] = time_arrival_que2[i+1];
        }
    }
}


/**
 * @brief report
 * 报告
 */
void report()
{
    fprintf(outfile, "\n\nAverage delay in Queue1 %11.3f minutes\n\n", total_of_delays1/num_que1_delayed);
    fprintf(outfile, "\n\nAverage number in Queue1 %10.3f\n\n", area_num_in_q1/Simulation_time);
    fprintf(outfile, "\n\nServer utilization in Queue1 %15.3f\n\n", area_server1_status/Simulation_time);

    fprintf(outfile, "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    fprintf(outfile, "\n\nAverage delay in Queue2 %11.3f minutes\n\n", total_of_delays2/num_que2_delayed);
    fprintf(outfile, "\n\nAverage number in Queue2 %10.3f\n\n", area_num_in_q2/Simulation_time);
    fprintf(outfile, "\n\nServer utilization in Queue2 %15.3f\n\n", area_server2_status/Simulation_time);

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
