#include <stdio.h>
#include <string.h>
#include <math.h>
#include "models/model.h"
#include "models/cardDataLinkList.h"
#include "models/ChargeDataLinkList.h"
#include "models/RechargeDataLinkList.h"


// 移除原来的数组定义
// 定义全局链表变量
CardList* cardList = NULL;

// 定义计费信息的链表变量
ChargeList* chargeList = NULL;

// 定义充值/退费信息的链表变量
RechargeList* rechargeList = NULL;

// 定义每小时费率（元/小时）
const double HOURLY_RATE = 5.0;

// 定义函数原型
int printMenu();
int addCard();
int queryCard();
int startUse();
int endUse();
int rechargeCard(); // 充值功能
int refundCard();   // 退费功能
int cancelCard();   // 注销卡功能
double calculateFee(time_t startTime, time_t endTime);

int main() {
    printf("欢迎进入计费管理系统!\n");
    
    // 初始化卡数据链表
    cardList = initCardList();
    if (cardList == NULL) {
        printf("初始化卡数据链表失败，系统退出！\n");
        return -1;
    }
    
    // 初始化计费数据链表
    chargeList = initChargeList();
    if (chargeList == NULL) {
        printf("初始化计费数据链表失败，系统退出！\n");
        freeCardList(cardList);
        return -1;
    }
    
    // 初始化充值/退费数据链表
    rechargeList = initRechargeList();
    if (rechargeList == NULL) {
        printf("初始化充值/退费数据链表失败，系统退出！\n");
        freeCardList(cardList);
        freeChargeList(chargeList);
        return -1;
    }
    
    // 尝试从文件加载卡数据
    CardList* loadedCardList = loadCardListFromFile("cards.dat");
    if (loadedCardList != NULL) {
        freeCardList(cardList); // 释放原链表
        cardList = loadedCardList;  // 使用加载的链表
        printf("已从文件加载卡数据！\n");
    }
    
    // 尝试从文件加载计费数据
    ChargeList* loadedChargeList = loadChargeListFromFile("charges.dat");
    if (loadedChargeList != NULL) {
        freeChargeList(chargeList); // 释放原链表
        chargeList = loadedChargeList;  // 使用加载的链表
        printf("已从文件加载计费数据！\n");
    }
    
    // 尝试从文件加载充值/退费数据
    RechargeList* loadedRechargeList = loadRechargeListFromFile("recharges.dat");
    if (loadedRechargeList != NULL) {
        freeRechargeList(rechargeList); // 释放原链表
        rechargeList = loadedRechargeList;  // 使用加载的链表
        printf("已从文件加载充值/退费数据！\n");
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
                startUse();
                break;
            case 4:
                printf("下机\n");
                endUse();
                break;
            case 5:
                printf("充值\n");
                rechargeCard();
                break;
            case 6:
                printf("退费\n");
                refundCard();
                break;
            case 7:
                printf("查询统计\n");
                break;
            case 8:
                printf("注销卡\n");
                cancelCard();
                break;
            case 0:
                printf("退出系统\n");
                // 保存卡数据到文件
                saveCardListToFile(cardList, "cards.dat");
                // 保存计费数据到文件
                saveChargeListToFile(chargeList, "charges.dat");
                // 保存充值/退费数据到文件
                saveRechargeListToFile(rechargeList, "recharges.dat");
                // 释放链表内存
                freeCardList(cardList);
                freeChargeList(chargeList);
                freeRechargeList(rechargeList);
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

int startUse() {
    printf("----------上机-----------\n");
    char cardNum[19];
    char password[9];
    printf("请输入上机的卡号：");
    scanf("%s", cardNum);
    printf("请输入密码：");
    scanf("%s", password);

    // 查找卡号
    CardNode* cardNode = findCardByNumber(cardList, cardNum);
    if (cardNode != NULL && cardNode->data.nDel != 1) {
        // 验证密码
        if (strcmp(cardNode->data.aPassword, password) != 0) {
            printf("密码错误！\n");
            return -1;
        }
        
        // 检查卡状态
        if (cardNode->data.nStatus == 0) { // 未上机
            cardNode->data.nStatus = 1; // 设置为正在上机
            cardNode->data.tLast = time(NULL); // 更新上次使用时间
            printf("卡号 %s 上机成功！\n", cardNum);
        } else {
            printf("卡号 %s 已经在使用中！\n");
        }

        // 输出上机信息
        printf("--------上机信息如下-------\n");
        printf("卡号\t密码\t状态\t余额\t累计消费\t上机次数\n");
        printf("%s\t%s\t%d\t%.2f\t%.2f\t%d\n", 
            cardNode->data.aName, 
            cardNode->data.aPassword, 
            cardNode->data.nStatus, 
            cardNode->data.fBalance, 
            cardNode->data.fTotalUse, 
            cardNode->data.nUseCount);

    } else {
        printf("未找到该卡号！\n");
        return -1;
    }
}

int endUse() {
    printf("----------下机-----------\n");
    char cardNum[19];
    printf("请输入下机的卡号：");
    scanf("%s", cardNum);

    // 查找卡号
    CardNode* cardNode = findCardByNumber(cardList, cardNum);
    if (cardNode == NULL || cardNode->data.nDel == 1) {
        printf("未找到该卡号！\n");
        return -1;
    }

    // 检查卡状态
    if (cardNode->data.nStatus != 1) { // 不是正在上机状态
        printf("该卡不在上机状态，无法下机！\n");
        return -2;
    }

    // 获取当前时间作为下机时间
    time_t endTime = time(NULL);
    
    // 计算费用
    double fee = calculateFee(cardNode->data.tLast, endTime);
    
    // 检查余额是否足够
    if (cardNode->data.fBalance < fee) {
        printf("余额不足！需要 %.2f 元，但余额仅 %.2f 元\n", fee, cardNode->data.fBalance);
        printf("请先充值后再下机！\n");
        return -3;
    }
    
    // 创建计费记录
    ChargeData charge;
    strcpy(charge.aName, cardNode->data.aName);
    charge.tStart = cardNode->data.tLast; // 上机时间
    charge.tEnd = endTime; // 下机时间
    charge.fAmount = fee; // 计费金额
    charge.nStatus = 1; // 已结算
    charge.nDel = 0; // 未删除
    
    // 添加计费记录
    if (addChargeToList(chargeList, charge) != 1) {
        printf("添加计费记录失败！\n");
        return -4;
    }
    
    // 更新卡信息
    cardNode->data.fBalance -= fee; // 扣除费用
    cardNode->data.fTotalUse += fee; // 累计消费增加
    cardNode->data.nUseCount++; // 上机次数增加
    cardNode->data.nStatus = 0; // 设置为未上机状态
    
    // 显示下机结果
    printf("下机成功！\n");
    printf("--------下机信息如下-------\n");
    printf("卡号：%s\n", cardNode->data.aName);
    printf("上机时间：%s", ctime(&cardNode->data.tLast));
    printf("下机时间：%s", ctime(&endTime));
    
    // 计算使用时间（小时和分钟）
    double timeUsedSec = difftime(endTime, cardNode->data.tLast);
    int hours = (int)(timeUsedSec / 3600);
    int minutes = (int)((timeUsedSec - hours * 3600) / 60);
    
    printf("使用时长：%d小时%d分钟\n", hours, minutes);
    printf("消费金额：%.2f元\n", fee);
    printf("卡内余额：%.2f元\n", cardNode->data.fBalance);
    
    return 0;
}

// 计算上机费用
// startTime: 上机时间
// endTime: 下机时间
// 返回值: 费用（元）
double calculateFee(time_t startTime, time_t endTime) {
    // 计算使用时间（秒）
    double timeUsed = difftime(endTime, startTime);
    
    // 将秒转换为小时，并进位取整（不足1小时按1小时计费）
    double hoursUsed = ceil(timeUsed / 3600);
    
    // 计算费用
    double fee = hoursUsed * HOURLY_RATE;
    
    return fee;
}

int rechargeCard() {
    printf("----------充值-----------\n");
    char cardNum[19];
    char password[9];
    double amount;

    printf("请输入卡号：");
    scanf("%s", cardNum);
    printf("请输入密码：");
    scanf("%s", password);
    
    // 查找卡号
    CardNode* cardNode = findCardByNumber(cardList, cardNum);
    if (cardNode == NULL || cardNode->data.nDel == 1) {
        printf("未找到该卡号！\n");
        return -1;
    }

    // 验证密码
    if (strcmp(cardNode->data.aPassword, password) != 0) {
        printf("密码错误！\n");
        return -2;
    }

    // 检查卡状态是否有效
    if (cardNode->data.nStatus == 2 || cardNode->data.nStatus == 3) {
        printf("该卡已注销或失效，无法充值！\n");
        return -3;
    }

    // 输入充值金额
    printf("请输入充值金额：");
    scanf("%lf", &amount);

    // 验证充值金额
    if (amount <= 0) {
        printf("充值金额必须大于0！\n");
        return -4;
    }

    // 更新卡余额
    double oldBalance = cardNode->data.fBalance;
    cardNode->data.fBalance += amount;

    // 创建充值记录
    RechargeData recharge;
    strcpy(recharge.aName, cardNode->data.aName);
    recharge.operatorTime = time(NULL);
    recharge.operationType = 0; // 0表示充值
    recharge.amount = amount;
    recharge.nDel = 0;

    // 添加充值记录
    if (addRechargeToList(rechargeList, recharge) != 1) {
        // 充值记录添加失败，恢复原余额
        cardNode->data.fBalance = oldBalance;
        printf("充值记录添加失败！\n");
        return -5;
    }

    // 显示充值成功信息
    printf("充值成功！\n");
    printf("--------充值信息如下-------\n");
    printf("卡号：%s\n", cardNode->data.aName);
    printf("充值金额：%.2f元\n", amount);
    printf("充值前余额：%.2f元\n", oldBalance);
    printf("当前余额：%.2f元\n", cardNode->data.fBalance);
    printf("充值时间：%s", ctime(&recharge.operatorTime));

    return 0;
}

int refundCard() {
    printf("----------退费-----------\n");
    char cardNum[19];
    char password[9];
    double amount;

    printf("请输入卡号：");
    scanf("%s", cardNum);
    printf("请输入密码：");
    scanf("%s", password);
    
    // 查找卡号
    CardNode* cardNode = findCardByNumber(cardList, cardNum);
    if (cardNode == NULL || cardNode->data.nDel == 1) {
        printf("未找到该卡号！\n");
        return -1;
    }

    // 验证密码
    if (strcmp(cardNode->data.aPassword, password) != 0) {
        printf("密码错误！\n");
        return -2;
    }

    // 检查卡状态是否有效
    if (cardNode->data.nStatus == 2 || cardNode->data.nStatus == 3) {
        printf("该卡已注销或失效，无法退费！\n");
        return -3;
    }
    
    // 检查卡是否在上机状态
    if (cardNode->data.nStatus == 1) {
        printf("该卡正在上机中，请先下机后再退费！\n");
        return -4;
    }

    // 显示当前余额
    printf("当前卡内余额：%.2f元\n", cardNode->data.fBalance);
    
    // 输入退费金额
    printf("请输入退费金额（最多%.2f元）：", cardNode->data.fBalance);
    scanf("%lf", &amount);

    // 验证退费金额
    if (amount <= 0) {
        printf("退费金额必须大于0！\n");
        return -5;
    }
    
    // 验证退费金额是否超过余额
    if (amount > cardNode->data.fBalance) {
        printf("退费金额超过卡内余额，无法退费！\n");
        return -6;
    }

    // 更新卡余额
    double oldBalance = cardNode->data.fBalance;
    cardNode->data.fBalance -= amount;

    // 创建退费记录
    RechargeData refund;
    strcpy(refund.aName, cardNode->data.aName);
    refund.operatorTime = time(NULL);
    refund.operationType = 1; // 1表示退费
    refund.amount = amount;
    refund.nDel = 0;

    // 添加退费记录
    if (addRechargeToList(rechargeList, refund) != 1) {
        // 退费记录添加失败，恢复原余额
        cardNode->data.fBalance = oldBalance;
        printf("退费记录添加失败！\n");
        return -7;
    }

    // 显示退费成功信息
    printf("退费成功！\n");
    printf("--------退费信息如下-------\n");
    printf("卡号：%s\n", cardNode->data.aName);
    printf("退费金额：%.2f元\n", amount);
    printf("退费前余额：%.2f元\n", oldBalance);
    printf("当前余额：%.2f元\n", cardNode->data.fBalance);
    printf("退费时间：%s", ctime(&refund.operatorTime));

    return 0;
}

int cancelCard() {
    printf("----------注销卡-----------\n");
    char cardNum[19];
    char password[9];
    
    printf("请输入要注销的卡号：");
    scanf("%s", cardNum);
    printf("请输入密码：");
    scanf("%s", password);
    
    // 查找卡号
    CardNode* cardNode = findCardByNumber(cardList, cardNum);
    if (cardNode == NULL) {
        printf("未找到该卡号！\n");
        return -1;
    }

    // 检查卡是否已被注销
    if (cardNode->data.nDel == 1) {
        printf("该卡已被注销！\n");
        return -2;
    }

    // 验证密码
    if (strcmp(cardNode->data.aPassword, password) != 0) {
        printf("密码错误！\n");
        return -3;
    }

    // 检查卡是否在上机状态
    if (cardNode->data.nStatus == 1) {
        printf("该卡正在上机中，请先下机后再注销！\n");
        return -4;
    }

    // 显示卡信息
    printf("--------要注销的卡信息如下-------\n");
    printCardInfo(cardNode);
    
    // 确认是否注销
    char confirm;
    printf("确认要注销该卡吗？(y/n): ");
    scanf(" %c", &confirm);
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("已取消注销操作！\n");
        return -5;
    }
    
    // 如果卡内有余额，需要退还余额
    if (cardNode->data.fBalance > 0) {
        printf("该卡还有余额 %.2f 元，将进行退费操作。\n", cardNode->data.fBalance);
        
        // 创建退费记录
        RechargeData refund;
        strcpy(refund.aName, cardNode->data.aName);
        refund.operatorTime = time(NULL);
        refund.operationType = 1; // 1表示退费
        refund.amount = cardNode->data.fBalance;
        refund.nDel = 0;

        // 添加退费记录
        if (addRechargeToList(rechargeList, refund) != 1) {
            printf("退费记录添加失败，请稍后再试！\n");
            return -6;
        }
        
        printf("已退还余额 %.2f 元。\n", cardNode->data.fBalance);
        
        // 清空余额
        cardNode->data.fBalance = 0;
    }
    
    // 更新卡状态
    cardNode->data.nStatus = 2; // 2表示已注销
    cardNode->data.nDel = 1;    // 标记为已删除
    cardNode->data.tEnd = time(NULL); // 记录注销时间
    
    printf("卡号 %s 已成功注销！\n", cardNode->data.aName);
    printf("注销时间：%s", ctime(&cardNode->data.tEnd));
    
    // 显示卡使用统计信息
    printf("--------卡使用统计信息-------\n");
    printf("累计上机次数：%d\n", cardNode->data.nUseCount);
    printf("累计消费金额：%.2f元\n", cardNode->data.fTotalUse);
    
    return 0;
}


