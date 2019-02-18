/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * apple
 * Copyright (C)  2009 <>
 * 
 * apple is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * apple is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "c-paddles.hpp"

// Paddle resolution : 0 is 1 by 1 (most accurate), 1 is 2 by 2, 2 is 4 by 4 and
// so on. Accurate is slow. 
#define PADDLE_RESOLUTION 3

// Paddles counters should decrease every 11 cycles.
#define PADDLE_CYCLES 11<<PADDLE_RESOLUTION

#include <iostream>

CPaddles::CPaddles(CClock *clock, CGameUnit *gameUnit) {
	this->gameUnit = gameUnit;
	gameUnit->subscribe(this);
	clock->set_timer(PADDLE_CYCLES, &timer_callback, (void *)this); 
}

CPaddles::~CPaddles() {
	// TODO : Remove clock timer 
}

void CPaddles::timer_callback(void *ptr) {
	((CPaddles *)(ptr))->countdown();
}

void CPaddles::notify(CObservable *obs) {
	if (gameUnit->get_ptrig())
		for (index = 0; index <4; index++) 
			paddle_counter[index] = paddle_position[index];
}

void CPaddles::set_paddle(int paddle, BYTE position) {
	if (paddle >=0 and paddle <4)
		paddle_position[paddle] = position >> PADDLE_RESOLUTION;
}

void CPaddles::countdown() {
	for (index = 0; index <4; index++)
		if (paddle_counter[index])
			paddle_counter[index]--;
	
//	printf("%d ",paddle_counter[0]);
	gameUnit->set_paddl0(paddle_counter[0]);
	gameUnit->set_paddl1(paddle_counter[1]);
	gameUnit->set_paddl2(paddle_counter[2]);
	gameUnit->set_paddl3(paddle_counter[3]);
}
