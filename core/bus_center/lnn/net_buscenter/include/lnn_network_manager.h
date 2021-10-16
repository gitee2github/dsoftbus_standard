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

#ifndef LNN_NETWORK_MANAGER_H
#define LNN_NETWORK_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

#include "softbus_bus_center.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

int32_t LnnInitNetworkManager(void);

int32_t LnnInitNetworkManagerDelay(void);

void LnnNotifyAllTypeOffline(ConnectionAddrType type);

int32_t __attribute__ ((weak)) LnnInitIpNetwork(void);

int32_t __attribute__ ((weak)) LnnInitIpNetworkDelay(void);

void __attribute__ ((weak)) LnnCallIpDiscovery(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* LNN_NETWORK_MANAGER_H */
