#include "dijkstra.h"
#include "vector.h"
#include <conio.h>

/**
 * 获取地点信息
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
 * 获取拥堵信息
 * @param congestionLevel
 * @return
 */
char *getCongestionString(CongestionLevel congestionLevel) {
    switch (congestionLevel) {
        case CONGESTION_NONE:
            return "畅通";
        case CONGESTION_SLIGHT:
            return "轻度拥堵";
        case CONGESTION_CONGESTION:
            return "拥堵";
        case CONGESTION_SERIOUS:
            return "严重拥堵";
        default:
            return "未知";
    }
}


/**
 * 打印路线
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

    printf("\n\t 路线总览：");
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

    printf("\n\t 从 [%s] 至 [%s] 的路线如下，总耗时约 %s ：\n", start->name, end->name,
           format_time(weights[end->arrIndex] - weights[start->arrIndex]));
    for (int i = (int) ((long long) tmpVector.size & 0xffffffff) - 1; i > 0; i--) {
        int from = (int) ((long long) vector_get(&tmpVector, i) & 0xffffffff);
        int to = (int) ((long long) vector_get(&tmpVector, i - 1) & 0xffffffff);
        IndexListNode *routeNodeTmp = AVL_searchExact(routeIndexNode, locationList[from] * 10000 + locationList[to]);
        if (routeNodeTmp == NULL) {
            continue;
        }
        Route *route = routeNodeTmp->index.data;
        printf("\t - 从 [%s] 步行至 [%s]，约 %s ，路程 %.0f 米，当前状态[%s]%s\n",
               route->start->name,
               route->end->name,
               format_time(weights[to] - weights[from]),
               route->distance,
               getCongestionString(route->congestion[timespan]),
               route->hasStairs ? "，有台阶" : "");
    }
    printf("\t 到达目的地 [%s]\n", end->name);
}


/**
 * 展示所有地点
 */
void UI_PrintAllLocations() {
    NodeList *nodeList = NULL;
    AVL_inOrderTraverse(locationNode, &nodeList);
    printf("\n\t%-4s%-40s%s\n", "ID", "地名", "描述");
    printf("\t----------------------------------------------------------------------------------------------\n");
    for (NodeList *node = nodeList; node != NULL; node = node->next) {
        Location *location = node->indexNode->index.data;
        printf("\t%-4d%-40s%s\n", location->id, location->name, location->description);
    }
    free(nodeList);
}


/**
 * 获取起始地点和终点地点
 * @param start
 * @param end
 */
bool UI_getStartEndLocation(int *start, int *end) {
    printf("\t[+] 请输入起始地ID和终点地ID，用空格分隔：");
    char startChar[100], endChar[100];
    scanf("%s %s", startChar, endChar);
    *start = strtol(startChar, NULL, 10);
    *end = strtol(endChar, NULL, 10);
    if (getLocation(*start) == NULL || getLocation(*end) == NULL) {
        printf("\t[+] 输入的地点ID不合法。（按任意键继续）\n");
        *start = -1;
        *end = -1;
        getch();
        return false;
    }
    if (*start == *end) {
        printf("\t[+] 起始地点和终点地点不能相同。（按任意键继续）\n");
        *start = -1;
        *end = -1;
        getch();
        return false;
    }
    return true;
}


/**
 * 获取时间
 * @return 以半小时为单位的时间单位数，如果输入不合法则返回-1
 */
int UI_getTime() {
    printf("\t[+] 请输入24小时制时间，例如11:45：");
    char timeChar[100];
    fflush(stdin);
    scanf("%s", timeChar);

    // 检查是否包含一个且仅一个冒号
    int colon_count = 0;
    for (int i = 0; i < strlen(timeChar); ++i) {
        if (timeChar[i] == ':') {
            colon_count++;
        }
    }
    if (colon_count != 1) {
        printf("\t[+] 输入的时间格式不正确。（按任意键继续）\n");
        getch();
        return -1;
    }

    // 分割输入字符串
    char *hourStr = strtok(timeChar, ":");
    char *minuteStr = strtok(NULL, ":");

    if (hourStr == NULL || minuteStr == NULL) {
        printf("\t[+] 输入的时间格式不正确。（按任意键继续）\n");
        getch();
        return -1;
    }

    // 转换字符串到整数
    int hour = strtol(hourStr, NULL, 10);
    int minute = strtol(minuteStr, NULL, 10);

    // 检查时间是否合法
    if (hour < 0 || hour >= 24 || minute < 0 || minute >= 60) {
        printf("\t[+] 输入的时间不合法。（按任意键继续）\n");
        getch();
        return -1;
    }

    // 将时间转换为以半小时为一个单位的单位数
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

        // 初始化Dijkstra地图
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

        printf("\n\t[+] 按ESC键退出，按其他键继续\n");
        getch();
        if (getch() == 27) {
            break;
        }
    }
}
