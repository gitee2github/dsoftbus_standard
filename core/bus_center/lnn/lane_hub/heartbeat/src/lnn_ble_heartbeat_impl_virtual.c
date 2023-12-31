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

#include "lnn_ble_heartbeat.h"

#include "softbus_errcode.h"
#include "softbus_log.h"

int32_t LnnInitBleHeartbeat(const LnnHeartbeatImplCallback *callback)
{
    (void)callback;
    SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_INFO, "ble heartbeat stub impl init");
    return SOFTBUS_OK;
}

int32_t LnnOnceBleHbBegin(void)
{
    SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_INFO, "ble heartbeat stub impl beat once");
    return SOFTBUS_NOT_IMPLEMENT;
}

int32_t LnnOnceBleHbEnd(void)
{
    SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_INFO, "ble heartbeat stub impl beat end");
    return SOFTBUS_NOT_IMPLEMENT;
}

int32_t LnnStopBleHeartbeat(void)
{
    SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_INFO, "ble heartbeat stub impl beat stop");
    return SOFTBUS_NOT_IMPLEMENT;
}

int32_t LnnDeinitBleHeartbeat(void)
{
    SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_INFO, "ble heartbeat stub impl deinit");
    return SOFTBUS_OK;
}
