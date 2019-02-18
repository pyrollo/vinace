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


#include "c-cycle-and-wait-clock.hpp"
#include "time.h"
#include "sys/time.h"
#include <iostream>

CCycleAndWaitClock::CCycleAndWaitClock(CProcessor *processor) : CClock(processor) {
	for (int index = 0; index < CLOCK_STAT_PERIODS; index++)
		periods[period_number] = nsPeriod;
	period_number = 0;
}

void CCycleAndWaitClock::set_frequency(float freq) {
	nsPeriod = (int)1000000000/freq; // Period in nanoseconds
}

// This could be any waiting method. 
// It should be close to the wanted period. Here, it is a 1 microsecond wait 
// that is far too much but works anyway (I did not manage to get a 10 or 100
// nanosecond wait).

void CCycleAndWaitClock::wait() {
    static struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1;
    select(0, NULL, NULL, NULL, &tv);
}

int CCycleAndWaitClock::get_mean_period() { return mean_period; };

void CCycleAndWaitClock::run() {
	int cpt, expc, expw, period;
	int iter, cycles;
    long elapsed;
	
	struct timespec now;
	struct timespec start;

	expw = 1; expc = 9999; 
	
	printf("CCycleAndWaitClock : Apple is running !\n");
	while (running) {
		// Reset counters and get start time
		cpt = 0; cycles = get_cyclecount();
		
		// Do some iterations, choosing between cycling or waiting according
		// to expw (expected waits) and expc (expected cycles) proportion.

		clock_gettime (CLOCK_REALTIME, &start); // Start time measure
		for (iter = 0; iter < 10000; iter++) {

			if (cpt>=0) {
				cpt -= expw; cycle(); // Cycle
			} else {			
				cpt += expc; wait(); // Wait
			}
		}
		clock_gettime (CLOCK_REALTIME, &now); // Stop time measure
		
		// Now, adjust expw and expc to reach the wanted period.

		cycles = (get_cyclecount() - cycles);

		elapsed = now.tv_nsec-start.tv_nsec+1000000000*(now.tv_sec-start.tv_sec);

		if (cycles) {
			period = elapsed/cycles;
			if (period >= nsPeriod) {
				expw --; expc ++; // More cycles, less waits
			} else {
				expw ++; expc --; // More waits, less cycles
			}

			// Collect statistics
			periods[period_number++] = period;
			if (period_number==CLOCK_STAT_PERIODS) period_number=0;

		} else {
			expw --; expc ++; // No cylces at all ! More cycles, less waits
			printf("No cycles this period !\n");
		}

		if (not period_number)
		{
			max_period = 0;
			min_period = 1000000000;
			int sumperiods = 0;
			for (int index = 0; index < CLOCK_STAT_PERIODS; index++) {
				sumperiods += periods[index];
				if (max_period < periods[index]) max_period = periods[index];
				if (min_period > periods[index]) min_period = periods[index];
			}
			mean_period = sumperiods/CLOCK_STAT_PERIODS;

			notifyUpdate();
//			printf("Periods (ns/cycle): min=%d, max=%d, mean=%d\nCurrent: period=%d, expc=%d, expw=%d\n", min_period, max_period, mean_period, period,expc,expw);
		}

	}
}
