/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <stdio.h>
#include <string.h>
#include "securec.h"
#include "common_list.h"
#include "softbus_errcode.h"
#include "softbus_adapter_mem.h"
#include "softbus_log.h"
#include "softbus_hidumper.h"

typedef struct {
    ListNode node;
    char moduleName[SOFTBUS_MODULE_NAME_LEN];
    char helpInfo[SOFTBUS_MODULE_HELP_LEN];
    DumpHandlerFunc dumpHandler;
} HandlerNode;

static LIST_HEAD(g_hidumperhander_list);

static void SoftBusDumpShowHelp(int fd)
{
    dprintf(fd, "Usage: hidumper -s 4700 -a \"[Option]\" \n");
    dprintf(fd, "  Option: [-h] ");
    ListNode *item = NULL;
    LIST_FOR_EACH(item, &g_hidumperhander_list) {
        HandlerNode *itemNode = LIST_ENTRY(item, HandlerNode, node);
        dprintf(fd, "| [");
        dprintf(fd, "%s", itemNode->moduleName);
        dprintf(fd, "]");
    }
    dprintf(fd, "\n");

    item = NULL;
    LIST_FOR_EACH(item, &g_hidumperhander_list) {
        HandlerNode *itemNode = LIST_ENTRY(item, HandlerNode, node);
        dprintf(fd, "\t\t");
        dprintf(fd, "%s", itemNode->moduleName);
        dprintf(fd, "\t\t");
        dprintf(fd, "%s", itemNode->helpInfo);
        dprintf(fd, "\n");
    }
}

void SoftBusDumpErrInfo(int fd, const char *argv)
{
    dprintf(fd, "the command %s is invalid, please input again!\n", argv);
}

void SoftBusDumpSubModuleHelp(int fd, char *moduleName, ListNode *varList)
{
    dprintf(fd, "Usage: hidumper -s 4700 -a \" %s [Option] \n", moduleName);
    dprintf(fd, "  Option: [-h]  | [-l <");
    ListNode *item = NULL;
    LIST_FOR_EACH(item, varList) {
        SoftBusDumpVarNode *itemNode = LIST_ENTRY(item, SoftBusDumpVarNode, node);
        dprintf(fd, "%s |", itemNode->varName);
    }
    dprintf(fd, ">]\n");
    dprintf(fd, "   -h         List all the dump item in %s module\n", moduleName);
    dprintf(fd, "   -l <item>  Dump the item in %s module, item is nesessary\n", moduleName);
}

SoftBusDumpVarNode *SoftBusCreateDumpVarNode(char *varName, SoftBusVarDumpCb cb)
{
    SoftBusDumpVarNode *varNode = SoftBusCalloc(sizeof(SoftBusDumpVarNode));
    if (varNode == NULL) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "SoftBusCreateDumpVarNode malloc fail.");
        return NULL;
    }
    ListInit(&varNode->node);
    if (strcpy_s(varNode->varName, SOFTBUS_DUMP_VAR_NAME_LEN, varName) != EOK) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "SoftBusCreateDumpVarNode set varName  %s fail.", varName);
        SoftBusFree(varNode);
        return NULL;
    }

    varNode->dumpCallback = cb;

    return varNode;
}

int SoftBusAddDumpVarToList(char *dumpVar, SoftBusVarDumpCb cb, ListNode *varList)
{
    if (strlen(dumpVar) >= SOFTBUS_DUMP_VAR_NAME_LEN || cb == NULL) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "SoftBusRegDiscDumpCb invalid param");
        return SOFTBUS_ERR;
    }

    SoftBusDumpVarNode *varNode = SoftBusCreateDumpVarNode(dumpVar, cb);
    if (varNode == NULL) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "SoftBusRegDiscDumpCb node create fail");
        return SOFTBUS_ERR;
    }
    varNode->dumpCallback = cb;
    ListTailInsert(varList, &varNode->node);

    return SOFTBUS_OK;
}

void SoftBusReleaseDumpVar(ListNode *varList)
{
    if (varList == NULL) {
        return;
    }
    ListNode *item = NULL;
    ListNode *nextItem = NULL;
    LIST_FOR_EACH_SAFE(item, nextItem, varList) {
        SoftBusDumpVarNode *varNode = LIST_ENTRY(item, SoftBusDumpVarNode, node);
        ListDelete(&varNode->node);;
        SoftBusFree(varNode);
    }
    SoftBusFree(varList);
}

static HandlerNode *CreateHiDumperHandlerNode(char *moduleName, char *helpInfo, DumpHandlerFunc handler)
{
    HandlerNode *handlerNode = SoftBusCalloc(sizeof(HandlerNode));
    if (handlerNode == NULL) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "CreateHiDumperHandlerNode malloc fail.");
        return NULL;
    }
    ListInit(&handlerNode->node);
    if (strcpy_s(handlerNode->moduleName, SOFTBUS_MODULE_NAME_LEN, moduleName) != EOK) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "CreateHiDumperHandlerNode get moduleName fail.");
        SoftBusFree(handlerNode);
        return NULL;
    }

    if (strcpy_s(handlerNode->helpInfo, SOFTBUS_MODULE_NAME_LEN, helpInfo) != EOK) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "CreateHiDumperHandlerNode get helpInfo fail");
        SoftBusFree(handlerNode);
        return NULL;
    }
    handlerNode->dumpHandler = handler;

    return handlerNode;
}

void SoftBusHiDumperDeInit(void)
{
    ListNode *item = NULL;
    ListNode *nextItem = NULL;
    LIST_FOR_EACH_SAFE(item, nextItem, &g_hidumperhander_list) {
        HandlerNode *handlerNode = LIST_ENTRY(item, HandlerNode, node);
        ListDelete(&handlerNode->node);;
        SoftBusFree(handlerNode);
    }
    SoftBusFree(&g_hidumperhander_list);
}

int SoftBusRegHiDumperHandler(char *moduleName, char *helpInfo, DumpHandlerFunc handler)
{
    if (strlen(moduleName) >= SOFTBUS_MODULE_NAME_LEN || strlen(helpInfo) >= SOFTBUS_MODULE_HELP_LEN ||
        handler == NULL) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "SoftBusRegHiDumperHandler invalid param");
        return SOFTBUS_ERR;
    }
    HandlerNode *handlerNode = CreateHiDumperHandlerNode(moduleName, helpInfo, handler);
    if (handlerNode == NULL) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "SoftBusRegHiDumperHandler node create fail");
        return SOFTBUS_ERR;
    }
    ListTailInsert(&g_hidumperhander_list, &handlerNode->node);
    return SOFTBUS_OK;
}

int SoftBusDumpProcess(int fd, int argc, const char **argv)
{
    if (fd <= 0 || argv == NULL) {
        SoftBusLog(SOFTBUS_LOG_CONN, SOFTBUS_LOG_ERROR, "SoftBusDumpProcess: param invalid ");
        return SOFTBUS_ERR;
    }

    if (argc == 0 || strcmp(argv[0], "-h")) {
        SoftBusDumpShowHelp(fd);
        return SOFTBUS_OK;
    }

    const char **argvPtr = NULL;
    if (argc == 1) {
        *argvPtr = NULL;
    } else {
        argvPtr = &argv[1];
    }
    int argcNew = argc - 1;

    ListNode *item = NULL;
    int isModuleExist = SOFTBUS_DUMP_NOT_EXIST;
    LIST_FOR_EACH(item, &g_hidumperhander_list) {
        HandlerNode *itemNode = LIST_ENTRY(item, HandlerNode, node);
        if (strcmp(itemNode->moduleName, argv[0]) == 0) {
            itemNode->dumpHandler(fd, argcNew, argvPtr);
            isModuleExist = SOFTBUS_DUMP_EXIST;
            break;
        }
    }

    if (isModuleExist == SOFTBUS_DUMP_NOT_EXIST) {
        SoftBusDumpErrInfo(fd, argv[0]);
        SoftBusDumpShowHelp(fd);
    }
    
    return SOFTBUS_OK;
}
