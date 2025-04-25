#include <stdio.h>


int main() {
    printf("欢迎进入计费管理系统!\n");

    int instruct;
    while (1) {
        instruct = printMenu();
        switch (instruct) {
            case 1:
                printf("添加卡\n");
                break;
            case 2:
                printf("查询卡\n");
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