#include <stdio.h>
#include "models/model.h"

#define MAX_CARD_NUM 100 // 假设最多100张卡

CardData cardData[MAX_CARD_NUM]; 

int main() {
    printf("欢迎进入计费管理系统!\n");

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

    // 将卡信息添加到数组中
    for (int i = 0; i < MAX_CARD_NUM; i++) {
        if (cardData[i].nDel != 1) { // 找到一个已删除的位置
            cardData[i] = card;
            printf("--------添加的卡信息如下-------\n");
            printf("卡号\t密码\t状态\t开卡金额\n");
            printf("%s\t%s\t%d\t%.2f\n", cardData[i].aName, cardData[i].aPassword, cardData[i].nStatus, cardData[i].fBalance);
            return 0;
        }
        if (i == 99) {
            printf("卡片已满，无法添加！\n");
            return -1;
        }
    }

}


int queryCard() {
    printf("----------查询卡-----------\n");
    char cardNum[19];
    printf("请输入查询的卡号：");
    scanf("%s", cardNum);

    for (int i = 0; i < MAX_CARD_NUM; i++) {
        if (strcmp(cardData[i].aName, cardNum) == 0 && cardData[i].nDel != 1) {
            printf("--------查询的卡信息如下-------\n");
            // 增加宽度，考虑中文字符占用两个宽度的情况
            printf("卡号\t\t状态\t余额\t\t累计消费\t使用次数\t上次使用时间\n");
            
            // 直接使用制表符对齐，并确保每个字段有足够的空间
            printf("%-10s\t%d\t%.2f\t\t%.2f\t\t%d\t\t%s", 
                  cardData[i].aName, 
                  cardData[i].nStatus, 
                  cardData[i].fBalance, 
                  cardData[i].fTotalUse, 
                  cardData[i].nUseCount, 
                  ctime(&cardData[i].tLast));
            return 0;
        }
    }
    printf("未找到该卡号！\n");
    return -1;
}