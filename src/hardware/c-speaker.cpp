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

#include "c-speaker.hpp"

CSpeaker::CSpeaker(CClock *clock, CSpeakerUnit *speakerUnit) {
	this->clock = clock;
	this->speakerUnit = speakerUnit;
	
	this->pos = 0;
	this->realstate=0;
	
	clock->set_timer(50, &timer_callback, this); 
}

CSpeaker::~CSpeaker() {
	// TODO : Remove clock timer
}

void CSpeaker::timer_callback(void *ptr) {
	((CSpeaker *)(ptr))->record();
}

void CSpeaker::record() {
	realstate = (realstate + (realstate<<1) + (realstate<<2) + ((speakerUnit->get_speaker())?0x00:0xff) )>>3; // Very simple lowpass filter...
	buffer[pos++] = realstate;
	if (pos == SPEAKER_BUFFER_SIZE) pos = 0;
};

int CSpeaker::get_pos() {
	return pos;
}

int CSpeaker::get_max_pos() {
	return SPEAKER_BUFFER_SIZE-1;
}

unsigned char* CSpeaker::get_buffer() {
	return buffer;
}

