/*
 * coord.h
 *
 *  Created on: Nov 5, 2024
 *      Author: kurumi
 */


#ifndef COORD_H_
#define COORD_H_

#include "main.h"
#include "shared.h"

#ifdef COORD_GLOBAL
	#define COORD_EXTERN
#else
	#define COORD_EXTERN extern
#endif

// 定义段常量
#define SEG_A 0  // 顶部横段
#define SEG_B 1  // 右上竖段
#define SEG_C 2  // 右下竖段
#define SEG_D 3  // 底部横段
#define SEG_E 4  // 左下竖段
#define SEG_F 5  // 左上竖段
#define SEG_G 6  // 中间横段

// 定义结构体存储2D坐标

// 定义段的有效方向和对应坐标的结构体
typedef struct {
    uint8_t valid_dirs[2];    // 该段的两个有效方向
    Point2D coords[2];        // 对应的坐标
} SegmentMapping;




#endif /* COORD_H_ */
