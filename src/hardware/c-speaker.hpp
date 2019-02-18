/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * Vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _C_SPEAKER_HPP_
#define _C_SPEAKER_HPP_

#include "c-clock.hpp"
#include "../core/units/c-speaker-unit.hpp"

// 0,5 s buffering at 8000kHz
#define SPEAKER_BUFFER_SIZE 4096

// This class simply records the changes of the apple speaker, synchronized to
// the processor clock to avoid sound transforation due to low accuracy of the
// clock.

class CSpeaker
{
public:
	CSpeaker(CClock *clock, CSpeakerUnit *speakerUnit);
	~CSpeaker();
	static void timer_callback(void *ptr);
	void record();
	int get_pos();
	int get_max_pos();
	unsigned char* get_buffer();

protected:
	CClock *clock;
	CSpeakerUnit *speakerUnit;

	unsigned char buffer[SPEAKER_BUFFER_SIZE];
	int pos;
	short realstate;
};

#endif // _C_SPEAKER_HPP_
