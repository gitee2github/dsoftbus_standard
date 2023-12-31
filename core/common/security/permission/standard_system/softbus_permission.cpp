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

#include "softbus_permission.h"

#include <sys/types.h>
#include <unistd.h>

#include "softbus_adapter_mem.h"
#include "softbus_errcode.h"
#include "softbus_log.h"

int32_t TransPermissionInit(void)
{
    return SOFTBUS_OK;
}

void TransPermissionDeinit(void)
{
}

int32_t CheckTransPermission(pid_t callingUid, pid_t callingPid,
    const char *pkgName, const char *sessionName, uint32_t actions)
{
    return SOFTBUS_OK;
}

int32_t CheckTransSecLevel(const char *mySessionName, const char *peerSessionName)
{
    return SOFTBUS_OK;
}

bool CheckDiscPermission(pid_t callingUid, const char *pkgName)
{
    return true;
}

bool CheckBusCenterPermission(pid_t callingUid, const char *pkgName)
{
    return true;
}
