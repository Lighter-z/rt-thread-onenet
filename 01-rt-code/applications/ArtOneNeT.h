/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-26     OMEN       the first version
 */
#include <rtthread.h>
#include <rtdbg.h>

#ifndef APPLICATIONS_ARTONENET_H_
#define APPLICATIONS_ARTONENET_H_

#define ONENET_THREAD_STACK_SIZE       (1024  * 2)                          //线程栈大小
#define ONENET_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX / 2 - 2)     //优先级
#define ONENET_THREAD_TIMESLICE        20                                    //时间片

#define DS18B20_GET_DATA_EVENT (1 << 3)
#define BH1750_GET_DATA_EVENT  (1 << 5)

typedef struct {

    rt_thread_t thread_;
    rt_thread_t init_thread_;         /* mqtt初始化的线程的句柄 */

    rt_sem_t init_sem_;
    rt_event_t recvdata_event_;

}ArtOneNet;

extern ArtOneNet art_onenet;

void ArtOneNetUploaData(void);

#endif /* APPLICATIONS_ARTONENET_H_ */
