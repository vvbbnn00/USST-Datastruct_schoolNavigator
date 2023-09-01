//
// Created by vvbbnn00 on 2023/08/31.
//

#ifndef SCHOOLNAVIGATOR_DATA_H
#define SCHOOLNAVIGATOR_DATA_H

#include <stdbool.h>
#include <string.h>
#include "avl.h"
#include "models.h"
#include <stdarg.h>
#include "vector.h"

AVLNode *locationNode = NULL;
AVLNode *routeIndexNode = NULL;
Vector *routeNode = NULL;

bool addLocationToAVL(int id, char *name, char *description) {
    Location *location = calloc(1, sizeof(Location));
    location->id = id;
    strcpy(location->name, name);
    strcpy(location->description, description);
    locationNode = AVL_insertNode(locationNode, id, INDEX_TYPE_OBJECT, location);
    return true;
}


bool addRouteToVector(Vector *vector, int startID, int endId, double distance, bool hasStairs, Speed speed,
                      bool isDouble, ...) {
    Route *route = calloc(1, sizeof(Route));
    route->startId = startID;
    route->endId = endId;

    route->start = AVL_searchExact(locationNode, startID)->index.data;
    route->end = AVL_searchExact(locationNode, endId)->index.data;

    route->distance = distance;
    route->hasStairs = hasStairs;
    route->speed = speed;

    va_list congestionList;
    va_start(congestionList, isDouble);
    CongestionLevel *congestion = calloc(48, sizeof(CongestionLevel));

    while (true) {
        int index = va_arg(congestionList, int);
        CongestionLevel congestionLevel = va_arg(congestionList, CongestionLevel);
        if (index == -1) {
            break;
        }
        congestion[index] = congestionLevel;
    }
    va_end(congestionList);

    memcpy(route->congestion, congestion, sizeof(CongestionLevel) * 48);
    vector_push_back(vector, route);
    routeIndexNode = AVL_insertNode(routeIndexNode, startID * 10000 + endId, INDEX_TYPE_OBJECT, route);

    if (isDouble) {
        Route *route2 = calloc(1, sizeof(Route));
        route2->startId = endId;
        route2->endId = startID;

        route2->start = route->end;
        route2->end = route->start;

        route2->distance = distance;
        route2->hasStairs = hasStairs;
        route2->speed = speed;
        memcpy(route2->congestion, congestion, sizeof(CongestionLevel) * 48);
        vector_push_back(vector, route2);
        routeIndexNode = AVL_insertNode(routeIndexNode, endId * 10000 + startID, INDEX_TYPE_OBJECT, route2);
    }

    return true;
}

void initData() {
    Speed speed = {1.8055, 1.3888, 0.5722, 0.2166}; // m/s

    routeNode = calloc(1, sizeof(Vector));
    vector_init(routeNode);

    addLocationToAVL(1, "516号大门", "上海理工大学 军工路校区 516号大门");
    addLocationToAVL(2, "新图书馆", "新图书馆，位于计算中心附近");
    addLocationToAVL(3, "第一教学楼", "上海理工大学第一教学楼");
    addLocationToAVL(4, "光电信息与计算机工程学院", "上海理工大学光电信息与计算机工程学院学院楼");
    addLocationToAVL(5, "毛主席像", "上海理工大学的标志性建筑之一");
    addLocationToAVL(6, "暖屋超市", "位于第三学生公寓附近，是一家非24小时营业的超市");
    addLocationToAVL(7, "第三学生公寓一号楼", "上海理工大学第三学生公寓一号楼，男生宿舍，有四个单元");
    addLocationToAVL(8, "第一食堂", "上海理工大学第一食堂，靠近第三学生公寓");

    addRouteToVector(routeNode, 1, 2, 1200, false, speed, true,
                     16, CONGESTION_SLIGHT,
                     17, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     35, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 1, 3, 330, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     23, CONGESTION_CONGESTION,
                     24, CONGESTION_CONGESTION,
                     25, CONGESTION_SLIGHT,
                     26, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_CONGESTION,
                     35, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 1, 4, 830, false, speed, true, -1);
    addRouteToVector(routeNode, 1, 5, 500, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     23, CONGESTION_SLIGHT,
                     24, CONGESTION_SLIGHT,
                     25, CONGESTION_SLIGHT,
                     26, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     35, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 1, 6, 700, false, speed, true,
                     23, CONGESTION_SLIGHT,
                     24, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 1, 7, 1300, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     23, CONGESTION_SLIGHT,
                     24, CONGESTION_SLIGHT,
                     25, CONGESTION_SLIGHT,
                     26, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     35, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 1, 8, 720, false, speed, true,
                     21, CONGESTION_CONGESTION,
                     22, CONGESTION_CONGESTION,
                     23, CONGESTION_SERIOUS,
                     24, CONGESTION_SERIOUS,
                     25, CONGESTION_CONGESTION,
                     26, CONGESTION_CONGESTION,
                     33, CONGESTION_CONGESTION,
                     34, CONGESTION_SERIOUS,
                     35, CONGESTION_CONGESTION,
                     -1);
    addRouteToVector(routeNode, 2, 3, 600, false, speed, true,
                     16, CONGESTION_SLIGHT,
                     17, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     35, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 2, 3, 600, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 2, 4, 400, false, speed, true, -1); // 无拥堵
    addRouteToVector(routeNode, 2, 5, 800, false, speed, true,
                     21, CONGESTION_CONGESTION,
                     22, CONGESTION_CONGESTION,
                     33, CONGESTION_CONGESTION,
                     34, CONGESTION_CONGESTION,
                     -1);
    addRouteToVector(routeNode, 2, 6, 950, false, speed, true,
                     21, CONGESTION_SERIOUS,
                     22, CONGESTION_SERIOUS,
                     33, CONGESTION_SERIOUS,
                     34, CONGESTION_SERIOUS,
                     -1);
    addRouteToVector(routeNode, 2, 7, 1300, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 2, 8, 500, true, speed, true,
                     21, CONGESTION_SERIOUS,
                     22, CONGESTION_SERIOUS,
                     33, CONGESTION_SERIOUS,
                     34, CONGESTION_SERIOUS,
                     -1);
    addRouteToVector(routeNode, 3, 4, 350, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 3, 5, 780, false, speed, true,
                     21, CONGESTION_CONGESTION,
                     22, CONGESTION_CONGESTION,
                     33, CONGESTION_CONGESTION,
                     34, CONGESTION_CONGESTION,
                     -1);
    addRouteToVector(routeNode, 3, 6, 890, false, speed, true,
                     21, CONGESTION_SERIOUS,
                     22, CONGESTION_SERIOUS,
                     33, CONGESTION_SERIOUS,
                     34, CONGESTION_SERIOUS,
                     -1);
    addRouteToVector(routeNode, 3, 7, 1250, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 3, 8, 650, true, speed, true,
                     21, CONGESTION_SERIOUS,
                     22, CONGESTION_SERIOUS,
                     33, CONGESTION_SERIOUS,
                     34, CONGESTION_SERIOUS,
                     -1);
    addRouteToVector(routeNode, 4, 5, 530, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 4, 6, 470, false, speed, true,
                     21, CONGESTION_CONGESTION,
                     22, CONGESTION_CONGESTION,
                     33, CONGESTION_CONGESTION,
                     34, CONGESTION_CONGESTION,
                     -1);
    addRouteToVector(routeNode, 4, 7, 1100, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 4, 8, 580, true, speed, true,
                     21, CONGESTION_SERIOUS,
                     22, CONGESTION_SERIOUS,
                     33, CONGESTION_SERIOUS,
                     34, CONGESTION_SERIOUS,
                     -1);
    addRouteToVector(routeNode, 5, 6, 440, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 5, 7, 1050, false, speed, true,
                     21, CONGESTION_CONGESTION,
                     22, CONGESTION_CONGESTION,
                     33, CONGESTION_CONGESTION,
                     34, CONGESTION_CONGESTION,
                     -1);
    addRouteToVector(routeNode, 5, 8, 510, false, speed, true,
                     21, CONGESTION_SERIOUS,
                     22, CONGESTION_SERIOUS,
                     33, CONGESTION_SERIOUS,
                     34, CONGESTION_SERIOUS,
                     -1);
    addRouteToVector(routeNode, 6, 7, 300, false, speed, true,
                     21, CONGESTION_SLIGHT,
                     22, CONGESTION_SLIGHT,
                     33, CONGESTION_SLIGHT,
                     34, CONGESTION_SLIGHT,
                     -1);
    addRouteToVector(routeNode, 6, 8, 480, false, speed, true,
                     21, CONGESTION_CONGESTION,
                     22, CONGESTION_CONGESTION,
                     33, CONGESTION_CONGESTION,
                     34, CONGESTION_CONGESTION,
                     -1);
    addRouteToVector(routeNode, 7, 8, 290, false, speed, true,
                     21, CONGESTION_SERIOUS,
                     22, CONGESTION_SERIOUS,
                     33, CONGESTION_SERIOUS,
                     34, CONGESTION_SERIOUS,
                     -1);
}

#endif //SCHOOLNAVIGATOR_DATA_H
