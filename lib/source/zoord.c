/*
 * coord.c
 *
 *  Created on: Nov 5, 2024
 *      Author: kurumi
 */
#define COORD_GLOBAL
#include "zoord.h"
#include "math.h"

// 定义每个段的映射关系表


const SegmentMapping segment_map[] = {
    // SEG_A (顶部横段)
    {
        .valid_dirs = {LEFT, RIGHT},
        .coords = {{0,0}, {1,0}}  // 左右方向的坐标
    },
    // SEG_B (右上竖段)
    {
        .valid_dirs = {UP, DOWN},
        .coords = {{1,0}, {1,1}}  // 上下方向的坐标
    },
    // SEG_C (右下竖段)
    {
        .valid_dirs = {UP, DOWN},
        .coords = {{1,1}, {1,2}}  // 上下方向的坐标
    },
    // SEG_D (底部横段)
    {
        .valid_dirs = {LEFT, RIGHT},
        .coords = {{0,2}, {1,2}}  // 左右方向的坐标
    },
    // SEG_E (左下竖段)
    {
        .valid_dirs = {UP, DOWN},
        .coords = {{0,1}, {0,2}}  // 上下方向的坐标
    },
    // SEG_F (左上竖段)
    {
        .valid_dirs = {UP, DOWN},
        .coords = {{0,0}, {0,1}}  // 上下方向的坐标
    },
    // SEG_G (中间横段)
    {
        .valid_dirs = {LEFT, RIGHT},
        .coords = {{0,1}, {1,1}}  // 左右方向的坐标
    }
};

// 获取段的坐标
Point2D get_segment_coordinates(uint8_t digit, uint8_t segment, Direction direction) {
    Point2D result = {0, 0};  // 默认无效坐标
    segment = (uint8_t)log2(segment);


    // 查找方向对应的坐标索引
    uint8_t coord_index = -1;
    for(uint8_t i = 0; i < 2; i++) {
        if(segment_map[segment].valid_dirs[i] == direction) {
            coord_index = i;
            break;
        }
    }

    // 如果找到有效方向
    if(coord_index != -1) {
        result = segment_map[segment].coords[coord_index];
        // 根据数码管序号偏移坐标
        result.x += 2*digit;  // 假设数码管间距为3个单位
    }





    return result;
}

