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

#include "c-sound.hpp"
#include <iostream>
//#include <unistd.h>
//#include <fcntl.h>

#define SOUND_PERIOD_USEC 20000
// 20kHz sound frequency
#define SOUND_FREQUENCY 20000

// Delay starting sound of one period
#define SAMPLES_SHIFT (SOUND_FREQUENCY*SOUND_PERIOD_USEC/1000000)

CSound::CSound(CSpeaker *speaker) {
	this->speaker = speaker;
	running = false;
}

void CSound::start() {
	// TODO : Pthread seems to allow real time thread launching... explore that way !
	if (pthread_create(&(this->thread), 
					   NULL, 
					   CSound::thread_manager,
					   (void *)this
					   )) {
		printf("CSound : Could not start thread -> No sound.\n");
	}
}

void CSound::stop() {
	running = false;
}

void *CSound::thread_manager(void* sound) {
	((CSound*)sound)->run();
	return 0;
}

void CSound::run() {
	running = true;
	while(running) {
		usleep(SOUND_PERIOD_USEC); // Sleep 20ms before sending sound again
		send_sound();
	}
}
