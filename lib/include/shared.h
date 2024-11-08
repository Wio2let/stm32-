/*
 * shared.h
 *
 *  Created on: Nov 5, 2024
 *      Author: kurumi
 */

#ifndef INC_SHARED_H_
#define INC_SHARED_H_
typedef  enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
}Direction;

typedef struct {
    uint8_t x;
    uint8_t y;
} Point2D;
typedef struct {
	uint8_t n;
	uint8_t section;
}Position;
Point2D get_segment_coordinates(uint8_t, uint8_t, Direction);

#endif /* INC_SHARED_H_ */
