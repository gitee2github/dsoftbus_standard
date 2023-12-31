/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "lnn_network_manager.h"

#include "lnn_ip_network_impl.h"
#include "softbus_errcode.h"
#include "softbus_log.h"

typedef enum {
    LNN_NETWORK_IMPL_TYPE_IP,
    LNN_NETWORK_IMPL_TYPE_MAX,
} LnnNetworkImplType;

typedef struct {
    int32_t (*InitNetworkImpl)(void);
    int32_t (*InitNetworkDelayImpl)(void);
    int32_t (*DeinitNetworkImpl)(void);
    void (*NotifyOfflineMsg)(void);
} NetworkImpl;

static NetworkImpl g_networkImpl[LNN_NETWORK_IMPL_TYPE_MAX] = {
    [LNN_NETWORK_IMPL_TYPE_IP] = {
        .InitNetworkImpl = LnnInitIpNetwork,
        .InitNetworkDelayImpl = LnnInitIpNetworkDelay,
        .DeinitNetworkImpl = LnnDeinitIpNetwork,
        .NotifyOfflineMsg = LnnNotifyOfflineMsg,
    },
};

int32_t LnnInitNetworkManager(void)
{
    uint32_t i;

    for (i = 0; i < LNN_NETWORK_IMPL_TYPE_MAX; ++i) {
        if (g_networkImpl[i].InitNetworkImpl == NULL) {
            continue;
        }
        if (g_networkImpl[i].InitNetworkImpl() != SOFTBUS_OK) {
            SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "init network impl(%d) failed\n", i);
            return SOFTBUS_ERR;
        }
    }
    return SOFTBUS_OK;
}

int32_t LnnInitNetworkManagerDelay(void)
{
    uint32_t i;

    for (i = 0; i < LNN_NETWORK_IMPL_TYPE_MAX; ++i) {
        if (g_networkImpl[i].InitNetworkDelayImpl == NULL) {
            continue;
        }
        if (g_networkImpl[i].InitNetworkDelayImpl() != SOFTBUS_OK) {
            SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "init network delay impl(%d) failed\n", i);
            return SOFTBUS_ERR;
        }
    }
    return SOFTBUS_OK;
}

void LnnDeinitNetworkManager(void)
{
    uint32_t i;

    for (i = 0; i < LNN_NETWORK_IMPL_TYPE_MAX; ++i) {
        if (g_networkImpl[i].DeinitNetworkImpl == NULL) {
            continue;
        }
        if (g_networkImpl[i].DeinitNetworkImpl() != SOFTBUS_OK) {
            SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "deinit network impl(%d) failed\n", i);
        }
    }
}

void LnnNotifyAllTypeOffline(ConnectionAddrType type)
{
    (void)type;
    uint32_t i;
    for (i = 0; i < LNN_NETWORK_IMPL_TYPE_MAX; ++i) {
        if (g_networkImpl[i].NotifyOfflineMsg == NULL) {
            continue;
        }
        g_networkImpl[i].NotifyOfflineMsg();
    }
}