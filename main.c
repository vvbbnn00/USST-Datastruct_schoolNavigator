#include "dijkstra.h"
#include "vector.h"
#include <conio.h>

/**
 * ��ȡ�ص���Ϣ
 * @param id
 * @return
 */
Location *getLocation(int id) {
    IndexListNode *node = AVL_searchExact(locationNode, id);
    if (node == NULL) {
        return NULL;
    }
    return node->index.data;
}

/**
 * ��ȡӵ����Ϣ
 * @param congestionLevel
 * @return
 */
char *getCongestionString(CongestionLevel congestionLevel) {
    switch (congestionLevel) {
        case CONGESTION_NONE:
            return "��ͨ";
        case CONGESTION_SLIGHT:
            return "���ӵ��";
        case CONGESTION_CONGESTION:
            return "ӵ��";
        case CONGESTION_SERIOUS:
            return "����ӵ��";
        default:
            return "δ֪";
    }
}


/**
 * ��ӡ·��
 */
void printRoute(int startIndex, int endIndex, const int *locationList,
                const int *previous,
                const double *weights, const int timespan) {

    Location *start = getLocation(startIndex);
    Location *end = getLocation(endIndex);

    long long currentNode = end->arrIndex;
    Vector tmpVector;
    vector_init(&tmpVector);

    while (currentNode != -1) {
        vector_push_back(&tmpVector, (void *) currentNode);
        currentNode = previous[currentNode];
    }

    printf("\n\t ·��������");
    for (int i = (int) ((long long) tmpVector.size & 0xffffffff) - 1; i > 0; i--) {
        int from = (int) ((long long) vector_get(&tmpVector, i) & 0xffffffff);
        int to = (int) ((long long) vector_get(&tmpVector, i - 1) & 0xffffffff);
        IndexListNode *routeNodeTmp = AVL_searchExact(routeIndexNode, locationList[from] * 10000 + locationList[to]);
        if (routeNodeTmp == NULL) {
            continue;
        }
        Route *route = routeNodeTmp->index.data;
        printf("[%s] -> ", route->start->name);
    }
    printf("[%s]\n", end->name);

    printf("\n\t �� [%s] �� [%s] ��·�����£��ܺ�ʱԼ %s ��\n", start->name, end->name,
           format_time(weights[end->arrIndex] - weights[start->arrIndex]));
    for (int i = (int) ((long long) tmpVector.size & 0xffffffff) - 1; i > 0; i--) {
        int from = (int) ((long long) vector_get(&tmpVector, i) & 0xffffffff);
        int to = (int) ((long long) vector_get(&tmpVector, i - 1) & 0xffffffff);
        IndexListNode *routeNodeTmp = AVL_searchExact(routeIndexNode, locationList[from] * 10000 + locationList[to]);
        if (routeNodeTmp == NULL) {
            continue;
        }
        Route *route = routeNodeTmp->index.data;
        printf("\t - �� [%s] ������ [%s]��Լ %s ��·�� %.0f �ף���ǰ״̬[%s]%s\n",
               route->start->name,
               route->end->name,
               format_time(weights[to] - weights[from]),
               route->distance,
               getCongestionString(route->congestion[timespan]),
               route->hasStairs ? "����̨��" : "");
    }
    printf("\t ����Ŀ�ĵ� [%s]\n", end->name);
}


/**
 * չʾ���еص�
 */
void UI_PrintAllLocations() {
    NodeList *nodeList = NULL;
    AVL_inOrderTraverse(locationNode, &nodeList);
    printf("\n\t%-4s%-40s%s\n", "ID", "����", "����");
    printf("\t----------------------------------------------------------------------------------------------\n");
    for (NodeList *node = nodeList; node != NULL; node = node->next) {
        Location *location = node->indexNode->index.data;
        printf("\t%-4d%-40s%s\n", location->id, location->name, location->description);
    }
    free(nodeList);
}


/**
 * ��ȡ��ʼ�ص���յ�ص�
 * @param start
 * @param end
 */
bool UI_getStartEndLocation(int *start, int *end) {
    printf("\t[+] ��������ʼ��ID���յ��ID���ÿո�ָ���");
    char startChar[100], endChar[100];
    scanf("%s %s", startChar, endChar);
    *start = strtol(startChar, NULL, 10);
    *end = strtol(endChar, NULL, 10);
    if (getLocation(*start) == NULL || getLocation(*end) == NULL) {
        printf("\t[+] ����ĵص�ID���Ϸ������������������\n");
        *start = -1;
        *end = -1;
        getch();
        return false;
    }
    if (*start == *end) {
        printf("\t[+] ��ʼ�ص���յ�ص㲻����ͬ�����������������\n");
        *start = -1;
        *end = -1;
        getch();
        return false;
    }
    return true;
}


/**
 * ��ȡʱ��
 * @return �԰�СʱΪ��λ��ʱ�䵥λ����������벻�Ϸ��򷵻�-1
 */
int UI_getTime() {
    printf("\t[+] ������24Сʱ��ʱ�䣬����11:45��");
    char timeChar[100];
    fflush(stdin);
    scanf("%s", timeChar);

    // ����Ƿ����һ���ҽ�һ��ð��
    int colon_count = 0;
    for (int i = 0; i < strlen(timeChar); ++i) {
        if (timeChar[i] == ':') {
            colon_count++;
        }
    }
    if (colon_count != 1) {
        printf("\t[+] �����ʱ���ʽ����ȷ�����������������\n");
        getch();
        return -1;
    }

    // �ָ������ַ���
    char *hourStr = strtok(timeChar, ":");
    char *minuteStr = strtok(NULL, ":");

    if (hourStr == NULL || minuteStr == NULL) {
        printf("\t[+] �����ʱ���ʽ����ȷ�����������������\n");
        getch();
        return -1;
    }

    // ת���ַ���������
    int hour = strtol(hourStr, NULL, 10);
    int minute = strtol(minuteStr, NULL, 10);

    // ���ʱ���Ƿ�Ϸ�
    if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60) {
        printf("\t[+] �����ʱ�䲻�Ϸ������������������\n");
        getch();
        return -1;
    }

    // ��ʱ��ת��Ϊ�԰�СʱΪһ����λ�ĵ�λ��
    int unit = hour * 2 + minute / 30;
    return unit;
}


int main() {
    system("chcp 65001 && chcp 936 && cls && echo off");

    initData();

    DijkstraRoute *routeList;
    int *locationList;
    size_t routeNum, locationNum;

    int timespan;

    while (true) {
        system("cls");
        UI_PrintAllLocations();
        printf("\n");

        timespan = UI_getTime();
        if (timespan == -1) {
            continue;
        }

        int start, end;
        if (!UI_getStartEndLocation(&start, &end)) {
            continue;
        }

        // ��ʼ��Dijkstra��ͼ
        initDijkstraMap(routeNode, locationNode, timespan, &routeNum, &routeList, &locationNum, &locationList);
        double *weights = malloc(locationNum * sizeof(double));
        int *previous = malloc(locationNum * sizeof(int));
        dijkstra(start, locationNum, routeNum, routeList, weights, previous);
        printRoute(start, end, locationList, previous, weights, timespan);

        free(weights);
        free(previous);
        free(routeList);
        free(locationList);
        routeList = NULL;
        locationList = NULL;

        printf("\n\t[+] ��ESC���˳���������������\n");
        getch();
        if (getch() == 27) {
            break;
        }
    }
}
