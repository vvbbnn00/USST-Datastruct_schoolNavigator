//
// Created by vvbbnn00 on 2023/08/31.
//

#ifndef SCHOOLNAVIGATOR_MATH_EXT_H
#define SCHOOLNAVIGATOR_MATH_EXT_H

#include <math.h>

/**
 * ��ȡ�������������ֵ
 * @param a
 * @param b
 * @return
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * ��ȡ������������Сֵ
 * @param a
 * @param b
 * @return
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}


/**
 * ��ȫ��free
 * @param pptr
 */
void safe_free(void **pptr) {
    if (*pptr != NULL) {
        free(*pptr);
        *pptr = NULL;
    } else {
        // �������򾯸�
    }
}


/**
 * ��ʽ��ʱ���ַ���
 * @param total_seconds
 * @return
 */
char *format_time(double total_seconds) {
    // �������ڴ洢�졢Сʱ�����Ӻ���ı���
    int days, hours, minutes, seconds;

    // ����������Сʱ����������������
    days = (int) (total_seconds / (24 * 3600));
    total_seconds = fmod(total_seconds, 24 * 3600);

    hours = (int) (total_seconds / 3600);
    total_seconds = fmod(total_seconds, 3600);

    minutes = (int) (total_seconds / 60);
    seconds = (int) fmod(total_seconds, 60);

    // �����㹻���ڴ�ռ����洢����ַ���
    // "xx��xxʱxx��xx��" ��Ҫ��� 50 ���ַ�
    char *result = (char *) malloc(50 * sizeof(char));
    if (result == NULL) {
        return NULL;  // �ڴ����ʧ��
    }

    // ��ʽ��ʱ���ַ���
    if (days > 0 && hours > 0) {
        snprintf(result, 50, "%d��%dʱ%d��%d��", days, hours, minutes, seconds);
    } else if (days > 0) {
        snprintf(result, 50, "%d��%d��%d��", days, minutes, seconds);
    } else if (hours > 0) {
        snprintf(result, 50, "%dʱ%d��%d��", hours, minutes, seconds);
    } else {
        snprintf(result, 50, "%d��%d��", minutes, seconds);
    }

    return result;
}


#endif //SCHOOLNAVIGATOR_MATH_EXT_H
