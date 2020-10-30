/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-26     OMEN       the first version
 */
#include "ArtWifi.h"

#include <wlan_mgnt.h>
#include <wlan_prot.h>
#include <wlan_cfg.h>

ArtWifi art_wifi;

static void art_wifi_init(ArtWifi *c_this) {
    c_this->net_ready = RT_NULL;
}

static void wifi_ready_callback(int event, struct rt_wlan_buff *buff, void *parameter) {
    rt_kprintf("%s\n", __FUNCTION__);
    rt_sem_release(art_wifi.net_ready);
}

static void wifi_connect_callback(int event, struct rt_wlan_buff *buff, void *parameter) {
    rt_kprintf("%s\n", __FUNCTION__);
    if ((buff != RT_NULL) && (buff->len == sizeof(struct rt_wlan_info))) {
        rt_kprintf("ssid : %s \n", ((struct rt_wlan_info *)buff->data)->ssid.val);
    }
}

static void wifi_connect_fail_callback(int event, struct rt_wlan_buff *buff, void *parameter) {
    rt_kprintf("%s\n", __FUNCTION__);
    if ((buff != RT_NULL) && (buff->len == sizeof(struct rt_wlan_info))) {
        rt_kprintf("ssid : %s \n", ((struct rt_wlan_info *)buff->data)->ssid.val);
    }
}

rt_err_t wifi_connect(void) {
    rt_err_t result = RT_EOK;

    art_wifi_init(&art_wifi);

    /* Configuring WLAN device working mode */
    rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION);
    /* station connect */
    rt_kprintf("start to connect ap ...\n");

    art_wifi.net_ready = rt_sem_create("net_ready", 0, RT_IPC_FLAG_FIFO);

    rt_wlan_register_event_handler(RT_WLAN_EVT_READY,
            wifi_ready_callback, RT_NULL);

    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED,
            wifi_connect_callback, RT_NULL);

    rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL,
            wifi_connect_fail_callback, RT_NULL);

    /* connect wifi */
    result = rt_wlan_connect(WLAN_SSID, WLAN_PASSWORD);

    if (result == RT_EOK) {
        /* waiting for IP to be got successfully  */
        result = rt_sem_take(art_wifi.net_ready, NET_READY_TIME_OUT);
        if (result == RT_EOK) {
            rt_kprintf("networking ready!\n");
        } else {
            rt_kprintf("wait ip got timeout!\n");
        }
        rt_wlan_unregister_event_handler(RT_WLAN_EVT_READY);
        rt_sem_delete(art_wifi.net_ready);

        rt_thread_delay(rt_tick_from_millisecond(5 * 1000));
        rt_kprintf("wifi disconnect test!\n");
    } else {
        rt_kprintf("connect failed!\n");
    }
    return result;
}

int wifi_connect_test(int argc, char *argv[]) {

    wifi_connect();
    return 0;
}
MSH_CMD_EXPORT(wifi_connect_test, connect test.);

