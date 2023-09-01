//
// Created by vvbbnn00 on 2023/08/31.
//

#ifndef SCHOOLNAVIGATOR_MODELS_H
#define SCHOOLNAVIGATOR_MODELS_H

typedef long long int64;

// 位置信息
typedef struct {
    int id; // 地名ID
    char name[100]; // 地名
    char description[500]; // 地名描述
    int arrIndex; // 数组索引，保留字段
} Location;

// 速度信息
typedef struct {
    // 无拥堵时的速度
    double speedNormal;
    // 略微拥堵时的速度
    double speedSlight;
    // 拥堵时的速度
    double speedCongestion;
    // 严重拥堵时的速度
    double speedSerious;
} Speed;

typedef enum {
    // 无拥堵
    CONGESTION_NONE=0,
    // 略微拥堵
    CONGESTION_SLIGHT=1,
    // 拥堵
    CONGESTION_CONGESTION=2,
    // 严重拥堵
    CONGESTION_SERIOUS=3
} CongestionLevel;


// 路线信息
typedef struct {
    int id;
    // 路线速度
    Speed speed;
    // 是否有台阶
    bool hasStairs;
    // 距离
    double distance;
    // 拥堵时间段，以半小时为单位，共48个时间段
    CongestionLevel congestion[48];

    int startId;
    int endId;

    // 起点
    Location *start;
    // 终点
    Location *end;
} Route;


// 索引类型定义
typedef enum {
    INDEX_TYPE_INT64,       // 整数
    INDEX_TYPE_STRING,      // 字符串
    INDEX_TYPE_OBJECT       // 对象
} IndexType;

// 索引定义
typedef struct {
    int64 hash;             // 哈希值
    IndexType type;         // 索引类型
    void *data;             // 数据
} Index;

#endif //SCHOOLNAVIGATOR_MODELS_H
