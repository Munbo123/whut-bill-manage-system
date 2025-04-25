#include <time.h>

// 卡信息
typedef struct CardData {
    char aName[19];     // 卡号，不能为空
    char aPassword[9]; // 密码，不能为空
    int nStatus;        // 卡状态，0表示未上机，1表示正在上机，2表示注销，3表示失效
    time_t tStart;       // 开卡时间
    time_t tEnd;         // 截止时间
    double fTotalUse;   // 累计消费金额
    time_t tLast;     // 上次使用时间
    int nUseCount;      // 上机次数
    double fBalance;     // 余额
    int nDel;       // 删除标志，0表示未创建，1表示已创建，2表示已注销，3表示已失效   

} CardData;


// 计费信息
typedef struct ChargeData {
    char aName[19];     // 卡号，不能为空
    time_t tStart;       // 上机时间
    time_t tEnd;         // 下机时间
    double fAmount;      // 计费金额
    int nStatus;        // 计费状态，0表示未结算，1表示已结算
    int nDel;       // 删除标志，0表示未删除，1表示已删除
} ChargeData;


// 计费标准信息
typedef struct ChargeStandard {
    time_t tStart;       // 开始时间
    time_t tEnd;         // 结束时间
    int unit;         // 最小计费单元
    double charge;      // 每个计费单元的收费
    int rateType;     // 计费类型，0-普通，1-包夜,2-包天
    int nDel;       // 删除标志，0表示未删除，1表示已删除
} ChargeStandard;


// 充值/退费信息
typedef struct RechargeData {
    char aName[19];     // 卡号，不能为空
    time_t operatorTime; // 操作时间
    int operationType; // 操作类型，0表示充值，1表示退费
    double amount;      // 操作金额
    int nDel;       // 删除标志，0表示未删除，1表示已删除
} RechargeData;


// 管理员信息
typedef struct AdminData {
    char aName[19];     // 管理员账号，不能为空
    char aPassword[9]; // 管理员密码，不能为空
    int privilege;   // 管理员权限等级，0表示普通管理员，1表示超级管理员
    int nDel;       // 删除标志，0表示未删除，1表示已删除
} AdminData;


