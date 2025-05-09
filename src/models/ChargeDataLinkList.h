#ifndef CHARGE_DATA_LINK_LIST_H
#define CHARGE_DATA_LINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

// 定义链表节点结构
typedef struct ChargeNode {
    ChargeData data;           // 计费数据
    struct ChargeNode* next;   // 指向下一个节点的指针
} ChargeNode;

// 定义链表结构
typedef struct ChargeList {
    ChargeNode* head;         // 头节点指针
    int size;                 // 链表中的节点数量
} ChargeList;

// 初始化链表
ChargeList* initChargeList();

// 添加计费记录到链表（在链表尾部添加）
int addChargeToList(ChargeList* list, ChargeData charge);

// 根据卡号查找计费记录
ChargeNode** findChargesByCardNumber(ChargeList* list, const char* cardNumber, int* count);

// 根据卡号和状态查找计费记录
ChargeNode** findChargesByCardNumberAndStatus(ChargeList* list, const char* cardNumber, int status, int* count);

// 根据计费记录的开始时间范围查找
ChargeNode** findChargesByTimeRange(ChargeList* list, time_t startTime, time_t endTime, int* count);

// 标记计费记录为已删除
int markChargeAsDeleted(ChargeList* list, ChargeNode* node);

// 更新计费记录
int updateChargeInfo(ChargeList* list, ChargeNode* node, ChargeData newData);

// 获取链表大小
int getChargeListSize(ChargeList* list);

// 打印所有计费记录
void printAllCharges(ChargeList* list);

// 打印指定计费记录
void printChargeInfo(ChargeNode* node);

// 释放链表内存
void freeChargeList(ChargeList* list);

// 将链表保存到文件
int saveChargeListToFile(ChargeList* list, const char* filename);

// 从文件加载链表
ChargeList* loadChargeListFromFile(const char* filename);

// 链表实现函数

// 初始化链表
ChargeList* initChargeList() {
    ChargeList* list = (ChargeList*)malloc(sizeof(ChargeList));
    if (list == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

// 添加计费记录到链表
int addChargeToList(ChargeList* list, ChargeData charge) {
    if (list == NULL) {
        return -1; // 链表不存在
    }

    // 创建新节点
    ChargeNode* newNode = (ChargeNode*)malloc(sizeof(ChargeNode));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        return -3; // 内存分配失败
    }

    // 复制计费数据
    newNode->data = charge;
    newNode->next = NULL;

    // 如果链表为空，设置为头节点
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        // 否则找到最后一个节点并添加到末尾
        ChargeNode* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    list->size++;
    return 1; // 添加成功
}

// 根据卡号查找计费记录
ChargeNode** findChargesByCardNumber(ChargeList* list, const char* cardNumber, int* count) {
    if (list == NULL || cardNumber == NULL || count == NULL || list->head == NULL) {
        *count = 0;
        return NULL;
    }

    // 计算匹配卡号的记录数量
    int matchCount = 0;
    ChargeNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.aName, cardNumber) == 0 && current->data.nDel != 1) {
            matchCount++;
        }
        current = current->next;
    }

    if (matchCount == 0) {
        *count = 0;
        return NULL;
    }

    // 分配指针数组
    ChargeNode** result = (ChargeNode**)malloc(sizeof(ChargeNode*) * matchCount);
    if (result == NULL) {
        printf("内存分配失败！\n");
        *count = 0;
        return NULL;
    }

    // 填充结果数组
    current = list->head;
    int index = 0;
    while (current != NULL && index < matchCount) {
        if (strcmp(current->data.aName, cardNumber) == 0 && current->data.nDel != 1) {
            result[index++] = current;
        }
        current = current->next;
    }

    *count = matchCount;
    return result;
}

// 根据卡号和状态查找计费记录
ChargeNode** findChargesByCardNumberAndStatus(ChargeList* list, const char* cardNumber, int status, int* count) {
    if (list == NULL || cardNumber == NULL || count == NULL || list->head == NULL) {
        *count = 0;
        return NULL;
    }

    // 计算匹配条件的记录数量
    int matchCount = 0;
    ChargeNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.aName, cardNumber) == 0 && 
            current->data.nStatus == status && 
            current->data.nDel != 1) {
            matchCount++;
        }
        current = current->next;
    }

    if (matchCount == 0) {
        *count = 0;
        return NULL;
    }

    // 分配指针数组
    ChargeNode** result = (ChargeNode**)malloc(sizeof(ChargeNode*) * matchCount);
    if (result == NULL) {
        printf("内存分配失败！\n");
        *count = 0;
        return NULL;
    }

    // 填充结果数组
    current = list->head;
    int index = 0;
    while (current != NULL && index < matchCount) {
        if (strcmp(current->data.aName, cardNumber) == 0 && 
            current->data.nStatus == status && 
            current->data.nDel != 1) {
            result[index++] = current;
        }
        current = current->next;
    }

    *count = matchCount;
    return result;
}

// 根据计费记录的开始时间范围查找
ChargeNode** findChargesByTimeRange(ChargeList* list, time_t startTime, time_t endTime, int* count) {
    if (list == NULL || count == NULL || list->head == NULL) {
        *count = 0;
        return NULL;
    }

    // 计算匹配时间范围的记录数量
    int matchCount = 0;
    ChargeNode* current = list->head;
    while (current != NULL) {
        if (current->data.tStart >= startTime && 
            current->data.tStart <= endTime && 
            current->data.nDel != 1) {
            matchCount++;
        }
        current = current->next;
    }

    if (matchCount == 0) {
        *count = 0;
        return NULL;
    }

    // 分配指针数组
    ChargeNode** result = (ChargeNode**)malloc(sizeof(ChargeNode*) * matchCount);
    if (result == NULL) {
        printf("内存分配失败！\n");
        *count = 0;
        return NULL;
    }

    // 填充结果数组
    current = list->head;
    int index = 0;
    while (current != NULL && index < matchCount) {
        if (current->data.tStart >= startTime && 
            current->data.tStart <= endTime && 
            current->data.nDel != 1) {
            result[index++] = current;
        }
        current = current->next;
    }

    *count = matchCount;
    return result;
}

// 标记计费记录为已删除
int markChargeAsDeleted(ChargeList* list, ChargeNode* node) {
    if (list == NULL || node == NULL) {
        return -1; // 参数错误
    }

    node->data.nDel = 1; // 标记为已删除
    return 1; // 成功
}

// 更新计费记录
int updateChargeInfo(ChargeList* list, ChargeNode* node, ChargeData newData) {
    if (list == NULL || node == NULL) {
        return -1; // 参数错误
    }
    
    // 保留原卡号和删除标志
    strcpy(newData.aName, node->data.aName);
    newData.nDel = node->data.nDel;
    
    // 更新计费数据
    node->data = newData;
    return 1; // 成功
}

// 获取链表大小
int getChargeListSize(ChargeList* list) {
    if (list == NULL) {
        return -1;
    }
    return list->size;
}

// 打印所有计费记录
void printAllCharges(ChargeList* list) {
    if (list == NULL || list->head == NULL) {
        printf("计费记录为空！\n");
        return;
    }

    printf("卡号\t\t上机时间\t\t下机时间\t\t计费金额\t状态\n");
    
    ChargeNode* current = list->head;
    while (current != NULL) {
        if (current->data.nDel != 1) { // 只打印未删除的记录
            char startTimeStr[30] = "未开始";
            char endTimeStr[30] = "未结束";
            
            if (current->data.tStart != 0) {
                struct tm *startTime = localtime(&current->data.tStart);
                strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", startTime);
            }
            
            if (current->data.tEnd != 0) {
                struct tm *endTime = localtime(&current->data.tEnd);
                strftime(endTimeStr, sizeof(endTimeStr), "%Y-%m-%d %H:%M:%S", endTime);
            }
            
            printf("%-10s\t%s\t%s\t%.2f\t\t%d\n", 
                current->data.aName, 
                startTimeStr,
                endTimeStr,
                current->data.fAmount, 
                current->data.nStatus);
        }
        current = current->next;
    }
}

// 打印指定计费记录
void printChargeInfo(ChargeNode* node) {
    if (node == NULL) {
        printf("计费记录不存在！\n");
        return;
    }

    printf("卡号\t\t上机时间\t\t下机时间\t\t计费金额\t状态\n");
    
    char startTimeStr[30] = "未开始";
    char endTimeStr[30] = "未结束";
    
    if (node->data.tStart != 0) {
        struct tm *startTime = localtime(&node->data.tStart);
        strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", startTime);
    }
    
    if (node->data.tEnd != 0) {
        struct tm *endTime = localtime(&node->data.tEnd);
        strftime(endTimeStr, sizeof(endTimeStr), "%Y-%m-%d %H:%M:%S", endTime);
    }
    
    printf("%-10s\t%s\t%s\t%.2f\t\t%d\n", 
        node->data.aName, 
        startTimeStr,
        endTimeStr,
        node->data.fAmount, 
        node->data.nStatus);
}

// 释放链表内存
void freeChargeList(ChargeList* list) {
    if (list == NULL) {
        return;
    }

    ChargeNode* current = list->head;
    ChargeNode* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(list);
}

// 将链表保存到文件
int saveChargeListToFile(ChargeList* list, const char* filename) {
    if (list == NULL || filename == NULL) {
        return -1;
    }

    FILE* file = fopen(filename, "w"); // 改为文本模式写入
    if (file == NULL) {
        printf("无法打开文件 %s 进行写入！\n", filename);
        return -2;
    }

    // 写入链表大小
    fprintf(file, "%d\n", list->size);

    // 写入所有节点数据
    ChargeNode* current = list->head;
    while (current != NULL) {
        fprintf(file, "%s,%ld,%ld,%.2f,%d,%d\n",
               current->data.aName,
               current->data.tStart,
               current->data.tEnd,
               current->data.fAmount,
               current->data.nStatus,
               current->data.nDel);
        current = current->next;
    }

    fclose(file);
    return 1;
}

// 从文件加载链表
ChargeList* loadChargeListFromFile(const char* filename) {
    FILE* file = fopen(filename, "r"); // 改为文本模式读取
    if (file == NULL) {
        printf("无法打开文件 %s 进行读取！\n", filename);
        return NULL;
    }

    // 创建新链表
    ChargeList* list = initChargeList();
    if (list == NULL) {
        fclose(file);
        return NULL;
    }

    // 读取链表大小
    int size;
    if (fscanf(file, "%d\n", &size) != 1) {
        printf("读取文件 %s 失败！\n", filename);
        free(list);
        fclose(file);
        return NULL;
    }

    // 读取所有节点数据
    for (int i = 0; i < size; i++) {
        ChargeData charge;
        if (fscanf(file, "%[^,],%ld,%ld,%lf,%d,%d\n",
                  charge.aName,
                  &charge.tStart,
                  &charge.tEnd,
                  &charge.fAmount,
                  &charge.nStatus,
                  &charge.nDel) != 6) {
            printf("读取文件 %s 中的计费数据失败！\n", filename);
            freeChargeList(list);
            fclose(file);
            return NULL;
        }
        addChargeToList(list, charge);
    }

    fclose(file);
    return list;
}

#endif // CHARGE_DATA_LINK_LIST_H