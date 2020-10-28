/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-26     OMEN       the first version
 */

#include "ArtOneNet.h"

#include <stdlib.h>
#include <onenet.h>

#include "ArtDht11.h"
#include "ArtBh1750.h"

ArtOneNet art_onenet;

//onenet 回调响应函数
static void ArtOneNetCmdRspCb(uint8_t *recv_data, size_t recv_size, uint8_t **resp_data, size_t *resp_size) {

}

static void art_onenet_upload_entry(void *parameter) {
    int value = 0;
    rt_uint32_t set = 0;

    rt_sem_take(art_onenet.init_sem_, RT_WAITING_FOREVER);
    /* 后面用不到这个信号量了，把它删除了，回收资源 */
    rt_sem_delete(art_onenet.init_sem_);

    /* 500ms上传一次数据 */
    rt_thread_delay(rt_tick_from_millisecond(500));

    while(1) {
//        value = rand() % 100;
        if(rt_event_recv(art_onenet.recvdata_event_, (DS18B20_GET_DATA_EVENT | BH1750_GET_DATA_EVENT), RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &set) == RT_EOK) {
            rt_thread_delay(rt_tick_from_millisecond(300));

            if (onenet_mqtt_upload_digit("temperature", art_dht11.temp_) < 0) {
                LOG_E("upload has an error, stop uploading");
                break;
            } else {
                LOG_D("buffer : {\"temperature\":%d}", art_dht11.temp_);
            }

            rt_thread_delay(rt_tick_from_millisecond(300));

            if (onenet_mqtt_upload_digit("humi", art_dht11.humi_) < 0) {
              LOG_E("upload has an error, stop uploading");
              break;
            } else {
                LOG_D("buffer : {\"humi\":%d}", art_dht11.humi_);
            }

            rt_thread_delay(rt_tick_from_millisecond(300));

            if (onenet_mqtt_upload_digit("light", art_bh1750.data_) < 0) {
              LOG_E("upload has an error, stop uploading");
              break;
            } else {
                LOG_D("buffer : {\"light\":%d}", art_dht11.humi_);
            }



        }
    }
}

static void ArtOneNetUploadThread(ArtOneNet *c_this) {
    /* 设置 onenet 回调响应函数 */
    onenet_set_cmd_rsp_cb(ArtOneNetCmdRspCb);

    c_this->thread_ = rt_thread_create("OneNet_Thread", art_onenet_upload_entry, RT_NULL,
            ONENET_THREAD_STACK_SIZE, ONENET_THREAD_PRIORITY, ONENET_THREAD_TIMESLICE);

    if(c_this->thread_!= RT_NULL) {
      rt_thread_startup(c_this->thread_);
    }
}
MSH_CMD_EXPORT(ArtOneNetUploadThread, Upload Data To OneNet);

static void art_mqtt_init_entry(void *parameter) {

    uint8_t failed_times = 0;

    while(1) {
        if(!onenet_mqtt_init()) {
            rt_sem_release(art_onenet.init_sem_);
            return;
        }
        rt_thread_mdelay(100);
        LOG_E("onenet mqtt init failed %d times", failed_times++);
    }
}

static void ArtOneNetInitThread(ArtOneNet *c_this) {
    c_this->init_sem_ = rt_sem_create("mqtt_sem", RT_NULL, RT_IPC_FLAG_FIFO);
    RT_ASSERT(c_this->init_sem_);

    c_this->init_thread_ = rt_thread_create("mqtt_thread", art_mqtt_init_entry, RT_NULL,
                                             ONENET_THREAD_STACK_SIZE, ONENET_THREAD_PRIORITY, ONENET_THREAD_TIMESLICE);
    if (c_this->init_thread_ != RT_NULL) {
      rt_thread_startup(c_this->init_thread_);
    }
}

void ArtOneNetUploaData(void) {

    ArtOneNetInitThread(&art_onenet);
    ArtOneNetUploadThread(&art_onenet);
}

