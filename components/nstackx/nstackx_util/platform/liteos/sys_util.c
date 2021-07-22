/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "nstackx_util.h"
#include "nstackx_error.h"
#include "nstackx_log.h"
#include "securec.h"
#include "sys_epoll.h"

#define TAG "nStackXUtil"

static const char *g_illegalPathString[] = {
    "/../",
};

static const char *g_illegalPathHeadString[] = {
    "../",
};

uint8_t IsFileNameLegal(const char *fileName)
{
    if (fileName == NULL || strlen(fileName) == 0) {
        return NSTACKX_FALSE;
    }

    for (uint32_t idx = 0; idx < sizeof(g_illegalPathHeadString) / sizeof(g_illegalPathHeadString[0]); idx++) {
        if (g_illegalPathHeadString[idx] == NULL || strlen(fileName) < strlen(g_illegalPathHeadString[idx])) {
            continue;
        }
        if (memcmp(fileName, g_illegalPathHeadString[idx], strlen(g_illegalPathHeadString[idx])) == 0) {
            LOGE(TAG, "illegal filename");
            return NSTACKX_FALSE;
        }
    }

    for (uint32_t idx = 0; idx < sizeof(g_illegalPathString) / sizeof(g_illegalPathString[0]); idx++) {
        if (g_illegalPathString[idx] == NULL || strlen(fileName) < strlen(g_illegalPathString[idx])) {
            continue;
        }
        if (strstr(fileName, g_illegalPathString[idx]) != NULL) {
            LOGE(TAG, "illegal filename");
            return NSTACKX_FALSE;
        }
    }
    return NSTACKX_TRUE;
}

int32_t GetCpuNum(void)
{
    /* @todo: liteos support multiple cpu core */
    return 1;
}

void StartThreadBindCore(int32_t cpu)
{
    /* @todo: liteos support thread bind core */
    (void)cpu;
}

void SetThreadName(const char *name)
{
    /* liteos only support set thread name when create */
    (void)name;
}

void BindThreadToTargetMask(pid_t tid, uint32_t cpuMask)
{
}

void SetMaximumPriorityForThread(void)
{
}

void ClockGetTime(clockid_t id, struct timespec *tp)
{
    if (clock_gettime(id, tp) != 0) {
        LOGE(TAG, "clock_gettime error: %d", errno);
    }
}

int32_t SemInit(sem_t *sem, int pshared, unsigned int value)
{
    return sem_init(sem, pshared, value);
}

void SemGetValue(sem_t *sem, int *sval)
{
    if (sem_getvalue(sem, sval) != 0) {
        LOGE(TAG, "sem get error: %d", errno);
    }
}

void SemPost(sem_t *sem)
{
    if (sem_post(sem) != 0) {
        LOGE(TAG, "sem post error: %d", errno);
    }
}

void SemWait(sem_t *sem)
{
    if (sem_wait(sem) != 0) {
        LOGE(TAG, "sem wait error: %d", errno);
    }
}

void SemDestroy(sem_t *sem)
{
    if (sem_destroy(sem) != 0) {
        LOGE(TAG, "sem destroy error: %d", errno);
    }
}

int32_t PthreadCreate(pthread_t *tid, const pthread_attr_t *attr, void *(*entry)(void *), void *arg)
{
    return pthread_create(tid, attr, entry, arg);
}

void PthreadJoin(pthread_t thread, void **retval)
{
    if (pthread_join(thread, retval) != 0) {
        LOGE(TAG, "pthread_join failed error: %d", errno);
    }
}

int32_t PthreadMutexInit(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    return pthread_mutex_init(mutex, attr);
}

void PthreadMutexDestroy(pthread_mutex_t *mutex)
{
    if (pthread_mutex_destroy(mutex) != 0) {
        LOGE(TAG, "pthread mutex destroy error: %d", errno);
    }
}

int32_t PthreadMutexLock(pthread_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

int32_t PthreadMutexUnlock(pthread_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}

void CloseDesc(int32_t desc)
{
    CloseDescClearEpollPtr(desc);
    if (close(desc) != 0) {
        LOGE(TAG, "close desc error : %d", errno);
    }
}

int32_t SetSocketNonBlock(SocketDesc fd)
{
    int32_t flag;

    flag = fcntl(fd, F_GETFL, 0);
    if (flag < 0) {
        LOGE(TAG, "fcntl GETFL error");
        return NSTACKX_EFAILED;
    }

    if (fcntl(fd, F_SETFL, (unsigned int)flag | O_NONBLOCK) < 0) {
        LOGE(TAG, "fcntl SETFL error");
        return NSTACKX_EFAILED;
    }
    return NSTACKX_EOK;
}