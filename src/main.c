#include <stdio.h>
#include <string.h>
#include "models/model.h"
#include "models/cardDataLinkList.h"

// 移除原来的数组定义
// 定义全局链表变量
CardList* cardList = NULL;

int printMenu();
int addCard();
int queryCard();

int main() {
    printf("欢迎进入计费管理系统!\n");
    
    // 正确初始化链表
    cardList = initCardList();
    if (cardList == NULL) {
        printf("初始化链表失败，系统退出！\n");
        return -1;
    }
    
    // 尝试从文件加载卡数据
    CardList* loadedList = loadCardListFromFile("cards.dat");
    if (loadedList != NULL) {
        freeCardList(cardList); // 释放原链表
        cardList = loadedList;  // 使用加载的链表
        printf("已从文件加载卡数据！\n");
    }

    int instruct;
    while (1) {
        instruct = printMenu();
        switch (instruct) {
            case 1:
                printf("添加卡\n");
                addCard();
                break;
            case 2:
                printf("查询卡\n");
                queryCard();
                break;
            case 3:
                printf("上机\n");
                break;
            case 4:
                printf("下机\n");
                break;
            case 5:
                printf("充值\n");
                break;
            case 6:
                printf("退费\n");
                break;
            case 7:
                printf("查询统计\n");
                break;
            case 8:
                printf("注销卡\n");
                break;
            case 0:
                printf("退出系统\n");
                // 保存卡数据到文件
                saveCardListToFile(cardList, "cards.dat");
                // 释放链表内存
                freeCardList(cardList);
                return 0;
            default:
                printf("无效的输入，请重新选择！\n");
        }
    }

    return 0;
}

int printMenu() {
    printf("\n********** 菜单 **********\n");
    printf("1. 添加卡\n");
    printf("2. 查询卡\n");
    printf("3. 上机\n");
    printf("4. 下机\n");
    printf("5. 充值\n");
    printf("6. 退费\n");
    printf("7. 查询统计\n");
    printf("8. 注销卡\n");
    printf("0. 退出\n");

    int instruct;
    printf("请选择菜单编号0~8：");
    scanf("%d", &instruct);

    return instruct;
}

int addCard() {
    printf("----------添加卡-----------\n");
    CardData card;
    printf("请输入卡号：");
    scanf("%s", card.aName);
    printf("请输入密码：");
    scanf("%s", card.aPassword);
    printf("请输入开卡金额：");
    scanf("%lf", &card.fBalance);
    card.nStatus = 0; // 未上机
    card.tStart = time(NULL); // 开卡时间
    card.tEnd = 0; // 截止时间
    card.fTotalUse = 0.0; // 累计金额
    card.tLast = 0; // 上次使用时间
    card.nUseCount = 0; // 上机次数
    card.nDel = 0; // 未删除

    // 将卡信息添加到链表中
    int result = addCardToList(cardList, card);
    if (result == 1) { // 成功返回1
        printf("--------添加的卡信息如下-------\n");
        printf("卡号\t密码\t状态\t开卡金额\n");
        printf("%s\t%s\t%d\t%.2f\n", card.aName, card.aPassword, card.nStatus, card.fBalance);
        return 0;
    } else {
        if (result == -2) {
            printf("卡号已存在！\n");
        } else {
            printf("添加卡失败！\n");
        }
        return -1;
    }
}

int queryCard() {
    printf("----------查询卡-----------\n");
    char cardNum[19];
    printf("请输入查询的卡号：");
    scanf("%s", cardNum);

    // 使用正确的函数findCardByNumber而不是findCardInList
    CardNode* cardNode = findCardByNumber(cardList, cardNum);
    if (cardNode != NULL && cardNode->data.nDel != 1) {
        printf("--------查询的卡信息如下-------\n");
        // 直接调用我们实现的打印卡信息函数
        printCardInfo(cardNode);
        return 0;
    } else {
        printf("未找到该卡号！\n");
        return -1;
    }
}

