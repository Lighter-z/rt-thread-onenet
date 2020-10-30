/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-27     OMEN       the first version
 */
#ifndef APPLICATIONS_ARTDHT11_H_
#define APPLICATIONS_ARTDHT11_H_

#include <rtthread.h>

#define DHT11_THREAD_STACK_SIZE       (1024)                                //线程栈大小
#define DHT11_THREAD_PRIORITY         (RT_THREAD_PRIORITY_MAX / 2 - 2)      //优先级
#define DHT11_THREAD_TIMESLICE        20                                    //时间片

typedef struct {
    rt_thread_t thread_;

    uint8_t temp_;
    uint8_t humi_;

}ArtDht11;

extern ArtDht11 art_dht11;

#endif /* APPLICATIONS_ARTDHT11_H_ */
