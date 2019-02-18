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

#ifndef _C_CLOCK_HPP_
#define _C_CLOCK_HPP_

#include <pthread.h>
#include "../core/c-processor.hpp"

//
// Clock
//
// A clock that runs a processor and can trigger periodical tasks using timers
//

struct clockTimer {
	int period;               // Number of cycles beetwen two calls
	int remaining;            // Cycles remaining before next call
	void (*callback)(void *); // Pointer to the callback function
	void *ptr;                // Void pointer given as first argument to the callback
	clockTimer *next;         // Next timer in the chained list
};

class CClock
{
public:
	CClock(CProcessor *processor);
	~CClock();
	virtual void run() = 0;
	void start();
	void stop();
	void cycle();
	bool is_running() { return running; }; 
	void set_timer(int period, void (*callback)(void *), void *ptr);
	unsigned int get_cyclecount() { return cyclecount; }; // Get processor clock cycle count (CClocks must update this counter correctly)
	static void *run_thread(void* clock);

protected:
	bool running;
	pthread_t thread;
	unsigned int cyclecount;

private:
	CProcessor *processor;
	clockTimer *timers;

	// Instanciated members for performance purpose
	clockTimer *timer;
};

#endif // _C_CLOCK_HPP_
