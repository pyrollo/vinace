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

#ifndef _C_PADDLES_HPP_
#define _C_PADDLES_HPP_
#include "../core/c-observable.hpp"
#include "../core/units/c-game-unit.hpp"
#include "../hardware/c-clock.hpp"

class CPaddles: public CObserver
{
public:
	CPaddles(CClock *clock, CGameUnit *gameUnit);
	~CPaddles();
	static void timer_callback(void *ptr);
	void set_paddle(int paddle, BYTE position);
//	void set_button(int button, bool position);
	void notify(CObservable *obs);
	void countdown();
protected:
	CGameUnit *gameUnit;
	BYTE paddle_position[4];
	BYTE paddle_counter[4];
private:
	int index;
};

#endif // _C_PADDLES_HPP_
