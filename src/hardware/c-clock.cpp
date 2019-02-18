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

// Some tips about time measures
// http://www.opengroup.org/onlinepubs/009695399/functions/timer_create.html
// http://www.opengroup.org/onlinepubs/009695399/functions/clock_getcpuclockid.html
// http://manpagesfr.free.fr/man/man2/clock_nanosleep.2.html
// http://ww2.it.nuigalway.ie/staff/h_melvin/realtime/SampleCode.htm

#include "c-clock.hpp"
#include <iostream>

//
// Clock
//
// A clock that runs a processor and can trigger periodical tasks using timers
//

CClock::CClock(CProcessor *processor) {
	this->processor = processor;
	timers = 0;
	running = false;
}

CClock::~CClock() {
	clockTimer *timer = timers;
	clockTimer *next;
	while (timer) {
		next = timer->next;
		delete timer;
		timer = next;
	}
}


void CClock::start() {
	running = true;
	// TODO : Pthread seems to allow real time thread launching... explore that way !
	if (pthread_create(&(this->thread), 
					   NULL, 
					   CClock::run_thread,
					   (void *)this
					   )) {
		running = false;
		printf("CClock : Could not start thread. Apple is not running.\n");
	}
}

void CClock::stop() {
	running = false;	
}

void CClock::cycle() {
	processor->cycle();
	cyclecount++;

	// Clock timer management
	timer = timers;
	
	while (timer) {
		if (!timer->remaining--) {
			timer->remaining = timer->period; // Reset timer
			(*timer->callback)(timer->ptr);   // and call callback
		}
		timer = timer->next; // Next timer
	}
}

void CClock::set_timer(int period, void (*callback)(void *), void *ptr) {
	// Create a new timer
	clockTimer *timer = new clockTimer();
	timer->remaining = period;
	timer->period    = period;
	timer->callback  = callback;
	timer->ptr       = ptr;

	// And place it in the timers list
	timer->next = timers;
	timers = timer;
}

// This is the entry point of the running clock thread
void *CClock::run_thread(void* clock) {
	((CClock*)clock)->run();
	return 0;
}
