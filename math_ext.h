//
// Created by vvbbnn00 on 2023/08/31.
//

#ifndef SCHOOLNAVIGATOR_MATH_EXT_H
#define SCHOOLNAVIGATOR_MATH_EXT_H

#include <math.h>

/**
 * 获取两个整数的最大值
 * @param a
 * @param b
 * @return
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * 获取两个整数的最小值
 * @param a
 * @param b
 * @return
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}


/**
 * 安全的free
 * @param pptr
 */
void safe_free(void **pptr) {
    if (*pptr != NULL) {
        free(*pptr);
        *pptr = NULL;
    } else {
        // 报告错误或警告
    }
}


/**
 * 格式化时间字符串
 * @param total_seconds
 * @return
 */
char *format_time(double total_seconds) {
    // 定义用于存储天、小时、分钟和秒的变量
    int days, hours, minutes, seconds;

    // 计算天数、小时数、分钟数和秒数
    days = (int) (total_seconds / (24 * 3600));
    total_seconds = fmod(total_seconds, 24 * 3600);

    hours = (int) (total_seconds / 3600);
    total_seconds = fmod(total_seconds, 3600);

    minutes = (int) (total_seconds / 60);
    seconds = (int) fmod(total_seconds, 60);

    // 分配足够的内存空间来存储结果字符串
    // "xx天xx时xx分xx秒" 需要最多 50 个字符
    char *result = (char *) malloc(50 * sizeof(char));
    if (result == NULL) {
        return NULL;  // 内存分配失败
    }

    // 格式化时间字符串
    if (days > 0 && hours > 0) {
        snprintf(result, 50, "%d天%d时%d分%d秒", days, hours, minutes, seconds);
    } else if (days > 0) {
        snprintf(result, 50, "%d天%d分%d秒", days, minutes, seconds);
    } else if (hours > 0) {
        snprintf(result, 50, "%d时%d分%d秒", hours, minutes, seconds);
    } else {
        snprintf(result, 50, "%d分%d秒", minutes, seconds);
    }

    return result;
}


#endif //SCHOOLNAVIGATOR_MATH_EXT_H
