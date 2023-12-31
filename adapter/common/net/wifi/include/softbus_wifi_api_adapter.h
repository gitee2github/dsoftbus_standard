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

#ifndef SOFTBUS_WIFI_API_ADAPTER_H
#define SOFTBUS_WIFI_API_ADAPTER_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define WIFI_MAX_SSID_LEN 33
#define WIFI_MAC_LEN 6
#define WIFI_MAX_KEY_LEN 65
#define WIFI_MAX_CONFIG_SIZE 10

typedef struct {
    char ssid[WIFI_MAX_SSID_LEN];
    unsigned char bssid[WIFI_MAC_LEN];
    char preSharedKey[WIFI_MAX_KEY_LEN];
    int32_t securityType;
    int32_t netId;
    int32_t isHiddenSsid;
} SoftBusWifiDevConf;

int32_t SoftBusGetWifiDeviceConfig(SoftBusWifiDevConf *configList, uint32_t *num);
int32_t SoftBusConnectToDevice(const SoftBusWifiDevConf *wifiConfig);
int32_t SoftBusDisconnectDevice(void);

#ifdef __cplusplus
}
#endif
#endif // SOFTBUS_WIFI_API_ADAPTER_H
