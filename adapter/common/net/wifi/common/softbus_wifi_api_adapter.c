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

#include "softbus_wifi_api_adapter.h"

#include <stdlib.h>
#include <string.h>

#include "securec.h"
#include "softbus_adapter_mem.h"
#include "softbus_errcode.h"
#include "softbus_log.h"
#include "wifi_device.h"

static int32_t ConvertSoftBusWifiConfFromWifiDev(const WifiDeviceConfig *sourceWifiConf, SoftBusWifiDevConf *wifiConf)
{
    if (strcpy_s(wifiConf->ssid, sizeof(wifiConf->ssid), sourceWifiConf->ssid) != EOK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "str copy ssid fail");
        return SOFTBUS_ERR;
    }

    if (memcpy_s(wifiConf->bssid, sizeof(wifiConf->bssid), sourceWifiConf->bssid,
        sizeof(sourceWifiConf->bssid)) != EOK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "mem copy bssid fail");
        return SOFTBUS_ERR;
    }

    if (strcpy_s(wifiConf->preSharedKey, sizeof(wifiConf->preSharedKey),
        sourceWifiConf->preSharedKey) != EOK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "str copy ssid fail");
        return SOFTBUS_ERR;
    }

    wifiConf->securityType = sourceWifiConf->securityType;
    wifiConf->isHiddenSsid = sourceWifiConf->isHiddenSsid;

    return SOFTBUS_OK;
}

static int32_t ConvertWifiDevConfFromSoftBusWifiConf(const SoftBusWifiDevConf *result, WifiDeviceConfig *wifiConf)
{
    if (strcpy_s(wifiConf->ssid, sizeof(wifiConf->ssid), result->ssid) != EOK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "str copy ssid fail");
        return SOFTBUS_ERR;
    }

    if (memcpy_s(wifiConf->bssid, sizeof(wifiConf->bssid),
        result->bssid, sizeof(result->bssid)) != EOK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "mem copy bssid fail");
        return SOFTBUS_ERR;
    }

    if (strcpy_s(wifiConf->preSharedKey, sizeof(wifiConf->preSharedKey),
        result->preSharedKey) != EOK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "str copy ssid fail");
        return SOFTBUS_ERR;
    }

    wifiConf->securityType = result->securityType;
    wifiConf->isHiddenSsid = result->isHiddenSsid;

    return SOFTBUS_OK;
}

int32_t SoftBusGetWifiDeviceConfig(SoftBusWifiDevConf *configList, uint32_t *num)
{
    WifiDeviceConfig *result = NULL;
    uint32_t wifiConfigSize;
    int32_t retVal;
    uint32_t i;

    if (configList == NULL) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "para configList is NULL");
        return SOFTBUS_ERR;
    }
    result = SoftBusMalloc(sizeof(WifiDeviceConfig) * WIFI_MAX_CONFIG_SIZE);
    if (result == NULL) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "malloc wifi device config fail");
        return SOFTBUS_ERR;
    }
    (void)memset_s(result, sizeof(WifiDeviceConfig) * WIFI_MAX_CONFIG_SIZE, 0,
                   sizeof(WifiDeviceConfig) * WIFI_MAX_CONFIG_SIZE);
    retVal = GetDeviceConfigs(result, &wifiConfigSize);
    if (retVal != SOFTBUS_OK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "malloc wifi device config fail");
        SoftBusFree(result);
        return SOFTBUS_ERR;
    }

    if (wifiConfigSize > WIFI_MAX_CONFIG_SIZE) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "wifi device config size is invalid.");
        SoftBusFree(result);
        return SOFTBUS_ERR;
    }

    for (i = 0; i < wifiConfigSize; i++) {
        if (ConvertSoftBusWifiConfFromWifiDev(result, configList) != SOFTBUS_OK) {
            SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "convert wifi config failed.");
            SoftBusFree(result);
            return SOFTBUS_ERR;
        }
        result++;
        configList++;
    }
    *num = wifiConfigSize;
    SoftBusFree(result);
    return SOFTBUS_OK;
}

int32_t SoftBusConnectToDevice(const SoftBusWifiDevConf *wifiConfig)
{
    WifiDeviceConfig wifiDevConfig;
    
    if (wifiConfig == NULL) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "para wifiConfig is NULL");
        return SOFTBUS_ERR;
    }
    (void)memset_s(&wifiDevConfig, sizeof(WifiDeviceConfig), 0, sizeof(WifiDeviceConfig));
    if (ConvertWifiDevConfFromSoftBusWifiConf(wifiConfig, &wifiDevConfig) != SOFTBUS_OK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "convert wifi config failed.");
        return SOFTBUS_ERR;
    }

    if (ConnectToDevice(&wifiDevConfig) != SOFTBUS_OK) {
        SoftBusLog(SOFTBUS_LOG_LNN, SOFTBUS_LOG_ERROR, "connect to wifi failed.");
        return SOFTBUS_ERR;
    }

    return SOFTBUS_OK;
}

int32_t SoftBusDisconnectDevice(void)
{
    return Disconnect();
}
