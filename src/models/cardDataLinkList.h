#ifndef CARD_DATA_LINK_LIST_H
#define CARD_DATA_LINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

// 定义链表节点结构
typedef struct CardNode {
    CardData data;           // 卡数据
    struct CardNode* next;   // 指向下一个节点的指针
} CardNode;

// 定义链表结构
typedef struct CardList {
    CardNode* head;         // 头节点指针
    int size;              // 链表中的节点数量
} CardList;

// 初始化链表
CardList* initCardList();

// 添加卡到链表（在链表尾部添加）
int addCardToList(CardList* list, CardData card);

// 根据卡号查找卡
CardNode* findCardByNumber(CardList* list, const char* cardNumber);

// 根据卡号删除卡（标记为已删除）
int markCardAsDeleted(CardList* list, const char* cardNumber);

// 根据卡号物理删除卡（从链表中移除节点）
int removeCardFromList(CardList* list, const char* cardNumber);

// 更新卡信息
int updateCardInfo(CardList* list, const char* cardNumber, CardData newData);

// 获取链表大小
int getCardListSize(CardList* list);

// 打印所有卡信息
void printAllCards(CardList* list);

// 打印指定卡的信息
void printCardInfo(CardNode* node);

// 根据状态查找卡
CardNode** findCardsByStatus(CardList* list, int status, int* count);

// 释放链表内存
void freeCardList(CardList* list);

// 将链表保存到文件
int saveCardListToFile(CardList* list, const char* filename);

// 从文件加载链表
CardList* loadCardListFromFile(const char* filename);

// 链表实现函数

// 初始化链表
CardList* initCardList() {
    CardList* list = (CardList*)malloc(sizeof(CardList));
    if (list == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

// 添加卡到链表
int addCardToList(CardList* list, CardData card) {
    if (list == NULL) {
        return -1; // 链表不存在
    }

    // 检查卡号是否已存在且未删除
    CardNode* existing = findCardByNumber(list, card.aName);
    if (existing != NULL && existing->data.nDel != 1) {
        printf("卡号 %s 已存在！\n", card.aName);
        return -2; // 卡号已存在
    }

    // 创建新节点
    CardNode* newNode = (CardNode*)malloc(sizeof(CardNode));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        return -3; // 内存分配失败
    }

    // 复制卡数据
    newNode->data = card;
    newNode->next = NULL;

    // 如果链表为空，设置为头节点
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        // 否则找到最后一个节点并添加到末尾
        CardNode* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    list->size++;
    return 1; // 添加成功
}

// 根据卡号查找卡
CardNode* findCardByNumber(CardList* list, const char* cardNumber) {
    if (list == NULL || cardNumber == NULL || list->head == NULL) {
        return NULL;
    }

    CardNode* current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.aName, cardNumber) == 0) {
            return current; // 找到卡
        }
        current = current->next;
    }

    return NULL; // 未找到卡
}

// 标记卡为已删除
int markCardAsDeleted(CardList* list, const char* cardNumber) {
    CardNode* node = findCardByNumber(list, cardNumber);
    if (node == NULL) {
        return -1; // 卡不存在
    }

    node->data.nDel = 1; // 标记为已删除
    return 1; // 成功
}

// 物理删除卡（从链表中移除节点）
int removeCardFromList(CardList* list, const char* cardNumber) {
    if (list == NULL || cardNumber == NULL || list->head == NULL) {
        return -1; // 参数错误或链表为空
    }

    CardNode* current = list->head;
    CardNode* prev = NULL;

    // 如果头节点就是要删除的节点
    if (strcmp(current->data.aName, cardNumber) == 0) {
        list->head = current->next;
        free(current);
        list->size--;
        return 1; // 删除成功
    }

    // 查找要删除的节点
    while (current != NULL && strcmp(current->data.aName, cardNumber) != 0) {
        prev = current;
        current = current->next;
    }

    // 如果找到要删除的节点
    if (current != NULL) {
        prev->next = current->next;
        free(current);
        list->size--;
        return 1; // 删除成功
    }

    return -1; // 未找到要删除的节点
}

// 更新卡信息
int updateCardInfo(CardList* list, const char* cardNumber, CardData newData) {
    CardNode* node = findCardByNumber(list, cardNumber);
    if (node == NULL) {
        return -1; // 卡不存在
    }

    // 保留原卡号
    strcpy(newData.aName, node->data.aName);
    
    // 更新卡数据
    node->data = newData;
    return 1; // 成功
}

// 获取链表大小
int getCardListSize(CardList* list) {
    if (list == NULL) {
        return -1;
    }
    return list->size;
}

// 打印所有卡信息
void printAllCards(CardList* list) {
    if (list == NULL || list->head == NULL) {
        printf("卡列表为空！\n");
        return;
    }

    printf("卡号\t\t状态\t余额\t\t累计消费\t使用次数\t上次使用时间\n");
    
    CardNode* current = list->head;
    while (current != NULL) {
        if (current->data.nDel != 1) { // 只打印未删除的卡
            printf("%-10s\t%d\t%.2f\t\t%.2f\t\t%d\t\t%s", 
                current->data.aName, 
                current->data.nStatus, 
                current->data.fBalance, 
                current->data.fTotalUse, 
                current->data.nUseCount, 
                ctime(&current->data.tLast));
        }
        current = current->next;
    }
}

// 打印指定卡的信息
void printCardInfo(CardNode* node) {
    if (node == NULL) {
        printf("卡信息不存在！\n");
        return;
    }

    printf("卡号\t\t状态\t余额\t\t累计消费\t使用次数\t上次使用时间\n");
    printf("%-10s\t%d\t%.2f\t\t%.2f\t\t%d\t\t%s", 
        node->data.aName, 
        node->data.nStatus, 
        node->data.fBalance, 
        node->data.fTotalUse, 
        node->data.nUseCount, 
        ctime(&node->data.tLast));
}

// 根据状态查找卡
CardNode** findCardsByStatus(CardList* list, int status, int* count) {
    if (list == NULL || list->head == NULL || count == NULL) {
        *count = 0;
        return NULL;
    }

    // 首先计算匹配状态的卡的数量
    int matchCount = 0;
    CardNode* current = list->head;
    while (current != NULL) {
        if (current->data.nStatus == status && current->data.nDel != 1) {
            matchCount++;
        }
        current = current->next;
    }

    if (matchCount == 0) {
        *count = 0;
        return NULL;
    }

    // 分配指针数组
    CardNode** result = (CardNode**)malloc(sizeof(CardNode*) * matchCount);
    if (result == NULL) {
        printf("内存分配失败！\n");
        *count = 0;
        return NULL;
    }

    // 填充结果数组
    current = list->head;
    int index = 0;
    while (current != NULL && index < matchCount) {
        if (current->data.nStatus == status && current->data.nDel != 1) {
            result[index++] = current;
        }
        current = current->next;
    }

    *count = matchCount;
    return result;
}

// 释放链表内存
void freeCardList(CardList* list) {
    if (list == NULL) {
        return;
    }

    CardNode* current = list->head;
    CardNode* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(list);
}

// 将链表保存到文件
int saveCardListToFile(CardList* list, const char* filename) {
    if (list == NULL || filename == NULL) {
        return -1;
    }

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("无法打开文件 %s 进行写入！\n", filename);
        return -2;
    }

    // 写入链表大小
    fwrite(&list->size, sizeof(int), 1, file);

    // 写入所有节点数据
    CardNode* current = list->head;
    while (current != NULL) {
        fwrite(&current->data, sizeof(CardData), 1, file);
        current = current->next;
    }

    fclose(file);
    return 1;
}

// 从文件加载链表
CardList* loadCardListFromFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开文件 %s 进行读取！\n", filename);
        return NULL;
    }

    // 创建新链表
    CardList* list = initCardList();
    if (list == NULL) {
        fclose(file);
        return NULL;
    }

    // 读取链表大小
    int size;
    if (fread(&size, sizeof(int), 1, file) != 1) {
        printf("读取文件 %s 失败！\n", filename);
        free(list);
        fclose(file);
        return NULL;
    }

    // 读取所有节点数据
    for (int i = 0; i < size; i++) {
        CardData card;
        if (fread(&card, sizeof(CardData), 1, file) != 1) {
            printf("读取文件 %s 中的卡数据失败！\n", filename);
            freeCardList(list);
            fclose(file);
            return NULL;
        }
        addCardToList(list, card);
    }

    fclose(file);
    return list;
}

#endif // CARD_DATA_LINK_LIST_H