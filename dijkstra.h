//
// Created by vvbbnn00 on 2023/09/01.
//

#ifndef SCHOOLNAVIGATOR_DIJKSTRA_H
#define SCHOOLNAVIGATOR_DIJKSTRA_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "avl.h"
#include "models.h"
#include <stdarg.h>
#include "data.h"
#include <stdlib.h>
#include <float.h>


typedef struct {
    int from, to;
    double weight;
    Route *route;
} DijkstraRoute;


/**
 * 获取路线的速度
 * @param route
 * @param currentTimeSlot
 * @return
 */
double getSpeed(Route *route, int currentTimeSlot) {
    CongestionLevel congestionLevel = route->congestion[currentTimeSlot];
    switch (congestionLevel) {
        case CONGESTION_NONE:
            return route->speed.speedNormal;
        case CONGESTION_SLIGHT:
            return route->speed.speedSlight;
        case CONGESTION_CONGESTION:
            return route->speed.speedCongestion;
        case CONGESTION_SERIOUS:
            return route->speed.speedSerious;
        default:
            return 0.00001;
    }
}

/**
 * 初始化Dijkstra地图
 * @param routeVector
 * @param locations
 * @param timespan
 * @param routeNum
 * @param routeList
 * @param locationNum
 * @param locationList
 * @return
 */
char initDijkstraMap(Vector *routeVector, AVLNode *locations, int timespan,
                     size_t *routeNum, DijkstraRoute **routeList, size_t *locationNum, int **locationList) {
    int currentArrIndex = 0;
    Vector tmpVector;
    vector_init(&tmpVector);
    // 加载路线到变长数组中
    NodeList *nodeList = NULL;
    AVL_inOrderTraverse(locations, &nodeList);
    for (NodeList *node = nodeList; node != NULL; node = node->next) {
        Location *location = node->indexNode->index.data;
        location->arrIndex = currentArrIndex;
        currentArrIndex++;
        vector_push_back(&tmpVector, location);
    }
    free(nodeList);
    // 转到locationList
    *locationList = calloc(tmpVector.size, sizeof(int));
    for (int i = 0; i < tmpVector.size; i++) {
        Location *location = vector_get(&tmpVector, i);
        (*locationList)[i] = location->id;
    }
    *locationNum = tmpVector.size;
    // 转到routeList
    *routeList = calloc(routeVector->size, sizeof(DijkstraRoute));
    *routeNum = routeVector->size;
    for (int i = 0; i < routeVector->size; i++) {
        Route *route = vector_get(routeVector, i);
        DijkstraRoute *dijkstraRoute = *routeList + i;
        dijkstraRoute->from = route->start->arrIndex;
        dijkstraRoute->to = route->end->arrIndex;
        dijkstraRoute->route = route;
        dijkstraRoute->weight = (route->distance / getSpeed(route, timespan)) * (1 + (route->hasStairs ? 0.5 : 0));
    }
    vector_free(&tmpVector);
    return 1;
}

/**
 * Dijkstra算法
 * @param startNode
 * @param locationNum
 * @param routeNum
 * @param routeList
 * @param distances
 * @param previous
 */
void dijkstra(int startNode, size_t locationNum, size_t routeNum, DijkstraRoute *routeList, double *distances, int *previous) {
    Location *start = AVL_searchExact(locationNode, startNode)->index.data;

    bool *visited = malloc(locationNum * sizeof(bool));
    for (int i = 0; i < locationNum; ++i) {
        distances[i] = DBL_MAX;
        previous[i] = -1;
        visited[i] = false;
    }
    distances[start->arrIndex] = 0;

    for (int i = 0; i < locationNum; ++i) {
        double minDist = DBL_MAX;
        int currentNode = -1;

        // Find the unvisited node with the smallest distance
        for (int j = 0; j < locationNum; ++j) {
            if (!visited[j] && distances[j] < minDist) {
                minDist = distances[j];
                currentNode = j;
            }
        }

        if (currentNode == -1) {
            break;
        }

        visited[currentNode] = true;

        // Update distances for neighbors of the current node
        for (int j = 0; j < routeNum; ++j) {
            if (routeList[j].from == currentNode) {
                int neighbor = routeList[j].to;
                double alt = distances[currentNode] + routeList[j].weight;
                if (alt < distances[neighbor]) {
                    distances[neighbor] = alt;
                    previous[neighbor] = currentNode;
                }
            }
        }
    }

    free(visited);
}


#endif //SCHOOLNAVIGATOR_DIJKSTRA_H
