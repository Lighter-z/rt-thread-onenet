/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-26     OMEN       the first version
 */
#ifndef APPLICATIONS_ART_WIFI_H_
#define APPLICATIONS_ART_WIFI_H_

#include <rthw.h>
#include <rtthread.h>

#define WLAN_SSID               "chuangshiji-708"
#define WLAN_PASSWORD           "csj666666"
#define NET_READY_TIME_OUT       (rt_tick_from_millisecond(15 * 1000))

typedef struct {
    rt_sem_t net_ready;
}ArtWifi;

rt_err_t wifi_connect(void);

#endif /* APPLICATIONS_WIFI_H_ */
