#include <time.h>
#include <stdio.h>


int main(){
    time_t now = time(NULL);
    char* time_str = ctime(&now);  // 返回固定格式的字符串
    printf("当前时间是: %d", now);
    printf("当前时间是: %s", time_str);
    return 0;
}