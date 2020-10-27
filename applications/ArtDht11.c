/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-27     OMEN       the first version
 */

#include "ArtDht11.h"

#include <rtdevice.h>
#include "drv_common.h"
#include "sensor.h"
#include "sensor_dallas_dht11.h"

#include "ArtOneNet.h"

#define DHT11_DATA_PIN    GET_PIN(B, 12)

ArtDht11 art_dht11;

static void art_dht11_read_entry(void *parameter) {
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;
    rt_uint8_t get_data_freq = 1; /* 1Hz */

    dev = rt_device_find("temp_dht11");
    if(dev == RT_NULL) {
        return;
    }

    if(rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK) {
        rt_kprintf("open device failed!\n");
        return;
    }

    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)(&get_data_freq));

    while(1) {
        res = rt_device_read(dev, 0, &sensor_data, 1);

        if(res != 1) {
            rt_kprintf("read data failed! result is %d\n", res);
            rt_device_close(dev);
            return;
        } else {
            if(sensor_data.data.temp >= 0) {
                art_dht11.temp_= (sensor_data.data.temp & 0xffff) >> 0;
                art_dht11.humi_ = (sensor_data.data.temp & 0xffff0000) >> 16;
                //rt_kprintf("temp:%d, humi:%d\n" ,art_dht11.temp_, art_dht11.humi_);
                rt_event_send(art_onenet.recvdata_event_, DS18B20_GET_DATA_EVENT);
            }
        }
    }
}

static int ArtDht11ReadDataThread(void) {
    art_onenet.recvdata_event_ = rt_event_create("Get Data Event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(art_onenet.recvdata_event_);

    art_dht11.thread_ = rt_thread_create("dht11 thread", art_dht11_read_entry, RT_NULL,
            DHT11_THREAD_STACK_SIZE, DHT11_THREAD_PRIORITY, DHT11_THREAD_TIMESLICE);

    if(art_dht11.thread_ != RT_NULL) {
        rt_thread_startup(art_dht11.thread_ );
    }
}
INIT_APP_EXPORT(ArtDht11ReadDataThread);

static int ArtDht11RtHwPort(void) {
    struct rt_sensor_config cfg;

    cfg.intf.user_data = (void *)DHT11_DATA_PIN;

    rt_hw_dht11_init("dht11", &cfg);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(ArtDht11RtHwPort);
