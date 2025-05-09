#ifndef RECHARGE_DATA_LINK_LIST_H
#define RECHARGE_DATA_LINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

// 定义链表节点结构
typedef struct RechargeNode {
    RechargeData data;         // 充值/退费数据
    struct RechargeNode* next; // 指向下一个节点的指针
} RechargeNode;

// 定义链表结构
typedef struct RechargeList {
    RechargeNode* head;        // 头节点指针
    int size;                  // 链表中的节点数量
} RechargeList;

// 初始化链表
RechargeList* initRechargeList();

// 添加充值/退费记录到链表（在链表尾部添加）
int addRechargeToList(RechargeList* list, RechargeData recharge);

// 根据卡号查找充值/退费记录
RechargeNode** findRechargesByCardNumber(RechargeList* list, const char* cardNumber, int* count);

// 根据卡号和操作类型查找充值/退费记录
RechargeNode** findRechargesByCardNumberAndType(RechargeList* list, const char* cardNumber, int operationType, int* count);

// 根据充值/退费记录的时间范围查找
RechargeNode** findRechargesByTimeRange(RechargeList* list, time_t startTime, time_t endTime, int* count);

// 获取链表大小
int getRechargeListSize(RechargeList* list);

// 打印所有充值/退费记录
void printAllRecharges(RechargeList* list);

// 打印指定充值/退费记录
void printRechargeInfo(RechargeNode* node);

// 释放链表内存
void freeRechargeList(RechargeList* list);

// 将链表保存到文件
int saveRechargeListToFile(RechargeList* list, const char* filename);

// 从文件加载链表
RechargeList* loadRechargeListFromFile(const char* filename);

// 链表实现函数

// 初始化链表
RechargeList* initRechargeList() {
    RechargeList* list = (RechargeList*)malloc(sizeof(RechargeList));
    if (list == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

// 添加充值/退费记录到链表
int addRechargeToList(RechargeList* list, RechargeData recharge) {
    if (list == NULL) {
        return -1; // 链表不存在
    }

    // 创建新节点
    RechargeNode* newNode = (RechargeNode*)malloc(sizeof(RechargeNode));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        return -3; // 内存分配失败
    }

    // 复制充值/退费数据
    newNode->data = recharge;
    newNode->next = NULL;

    // 如果链表为空，设置为头节点
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        // 否则找到最后一个节点并添加到末尾
        RechargeNode* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    list->size++;
    return 1; // 添加成功
}

// 根据卡号查找充值/退费记录
RechargeNode** findRechargesByCardNumber(RechargeList* list, const char* cardNumber, int* count) {
    if (list == NULL || cardNumber == NULL || count == NULL || list->head == NULL) {
        *count = 0;
        return NULL;
    }

    // 计算匹配卡号的记录数量
    int matchCount = 0;
    RechargeNode* current = list->head;
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
    RechargeNode** result = (RechargeNode**)malloc(sizeof(RechargeNode*) * matchCount);
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

// 根据卡号和操作类型查找充值/退费记录
RechargeNode** findRechargesByCardNumberAndType(RechargeList* list, const char* cardNumber, int operationType, int* count) {
    if (list == NULL || cardNumber == NULL || count == NULL || list->head == NULL) {
        *count = 0;
        return NULL;
    }

    // 计算匹配条件的记录数量
    int matchCount = 0;
    RechargeNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.aName, cardNumber) == 0 && 
            current->data.operationType == operationType && 
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
    RechargeNode** result = (RechargeNode**)malloc(sizeof(RechargeNode*) * matchCount);
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
            current->data.operationType == operationType && 
            current->data.nDel != 1) {
            result[index++] = current;
        }
        current = current->next;
    }

    *count = matchCount;
    return result;
}

// 根据充值/退费记录的时间范围查找
RechargeNode** findRechargesByTimeRange(RechargeList* list, time_t startTime, time_t endTime, int* count) {
    if (list == NULL || count == NULL || list->head == NULL) {
        *count = 0;
        return NULL;
    }

    // 计算匹配时间范围的记录数量
    int matchCount = 0;
    RechargeNode* current = list->head;
    while (current != NULL) {
        if (current->data.operatorTime >= startTime && 
            current->data.operatorTime <= endTime && 
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
    RechargeNode** result = (RechargeNode**)malloc(sizeof(RechargeNode*) * matchCount);
    if (result == NULL) {
        printf("内存分配失败！\n");
        *count = 0;
        return NULL;
    }

    // 填充结果数组
    current = list->head;
    int index = 0;
    while (current != NULL && index < matchCount) {
        if (current->data.operatorTime >= startTime && 
            current->data.operatorTime <= endTime && 
            current->data.nDel != 1) {
            result[index++] = current;
        }
        current = current->next;
    }

    *count = matchCount;
    return result;
}

// 获取链表大小
int getRechargeListSize(RechargeList* list) {
    if (list == NULL) {
        return -1;
    }
    return list->size;
}

// 打印所有充值/退费记录
void printAllRecharges(RechargeList* list) {
    if (list == NULL || list->head == NULL) {
        printf("充值/退费记录为空！\n");
        return;
    }

    printf("卡号\t\t操作时间\t\t操作类型\t金额\n");
    
    RechargeNode* current = list->head;
    while (current != NULL) {
        if (current->data.nDel != 1) { // 只打印未删除的记录
            char operTimeStr[30] = "未记录";
            
            if (current->data.operatorTime != 0) {
                struct tm *operTime = localtime(&current->data.operatorTime);
                strftime(operTimeStr, sizeof(operTimeStr), "%Y-%m-%d %H:%M:%S", operTime);
            }
            
            printf("%-10s\t%s\t%s\t%.2f\n", 
                current->data.aName, 
                operTimeStr,
                current->data.operationType == 0 ? "充值" : "退费",
                current->data.amount);
        }
        current = current->next;
    }
}

// 打印指定充值/退费记录
void printRechargeInfo(RechargeNode* node) {
    if (node == NULL) {
        printf("充值/退费记录不存在！\n");
        return;
    }

    printf("卡号\t\t操作时间\t\t操作类型\t金额\n");
    
    char operTimeStr[30] = "未记录";
    
    if (node->data.operatorTime != 0) {
        struct tm *operTime = localtime(&node->data.operatorTime);
        strftime(operTimeStr, sizeof(operTimeStr), "%Y-%m-%d %H:%M:%S", operTime);
    }
    
    printf("%-10s\t%s\t%s\t%.2f\n", 
        node->data.aName, 
        operTimeStr,
        node->data.operationType == 0 ? "充值" : "退费",
        node->data.amount);
}

// 释放链表内存
void freeRechargeList(RechargeList* list) {
    if (list == NULL) {
        return;
    }

    RechargeNode* current = list->head;
    RechargeNode* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(list);
}

// 将链表保存到文件
int saveRechargeListToFile(RechargeList* list, const char* filename) {
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
    RechargeNode* current = list->head;
    while (current != NULL) {
        fprintf(file, "%s,%ld,%d,%.2f,%d\n",
               current->data.aName,
               current->data.operatorTime,
               current->data.operationType,
               current->data.amount,
               current->data.nDel);
        current = current->next;
    }

    fclose(file);
    return 1;
}

// 从文件加载链表
RechargeList* loadRechargeListFromFile(const char* filename) {
    FILE* file = fopen(filename, "r"); // 改为文本模式读取
    if (file == NULL) {
        printf("无法打开文件 %s 进行读取！\n", filename);
        return NULL;
    }

    // 创建新链表
    RechargeList* list = initRechargeList();
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
        RechargeData recharge;
        if (fscanf(file, "%[^,],%ld,%d,%lf,%d\n",
                  recharge.aName,
                  &recharge.operatorTime,
                  &recharge.operationType,
                  &recharge.amount,
                  &recharge.nDel) != 5) {
            printf("读取文件 %s 中的充值/退费数据失败！\n", filename);
            freeRechargeList(list);
            fclose(file);
            return NULL;
        }
        addRechargeToList(list, recharge);
    }

    fclose(file);
    return list;
}

#endif // RECHARGE_DATA_LINK_LIST_H