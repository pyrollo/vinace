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


#include "c-carry-clock.hpp"
#include "time.h"
#include "sys/time.h"
#include <iostream>

CCarryClock::CCarryClock(CProcessor *processor) : CClock(processor) {
	for (int index = 0; index < CLOCK_STAT_PERIODS; index++)
		periods[period_number] = nsPeriod;
	period_number = 0;
}

void CCarryClock::set_frequency(float freq) {
	nsPeriod = (int)1000000000/freq; // Period in nanoseconds
}

// This could be any waiting method. 
// It should be close to the wanted period. Here, it is a 1 microsecond wait 
// that is far too much but works anyway (I did not manage to get a 10 or 100
// nanosecond wait).

void CCarryClock::wait() {
    static struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
//	usleep(1);
    select(0, NULL, NULL, NULL, &tv);
}

int CCarryClock::get_mean_period() { return mean_period; };


#define elapsed(time1, time2) ((time2).tv_nsec-(time1).tv_nsec+1000000000*((time2).tv_sec-(time1).tv_sec))

void CCarryClock::run() {
	int iter, period;
	int number_of_iterations = 1000;
    struct timeval tv;

	long carry_timens, wait_timeus;
	long total_expectedns, total_elapsedns, phase1_elapsedns;
	
	struct timespec start_time;
	struct timespec inter_time;
	struct timespec end_time;

	carry_timens = 0;
	clock_gettime (CLOCK_REALTIME, &start_time);
	
	printf("CCarryClock : Running.\n");
	while (running) {

		// Compute expected time for this loop
		total_expectedns = number_of_iterations * nsPeriod + carry_timens;

		// Cycle several times
		for (iter = 0; iter < number_of_iterations; iter++)
 			cycle();

		// Check time (including stats collecting)
		clock_gettime (CLOCK_REALTIME, &inter_time); 
		phase1_elapsedns = elapsed(start_time, inter_time);

		// See if waiting a while could be cool
		wait_timeus = (total_expectedns - phase1_elapsedns)/1000;
		if (wait_timeus > 0) {
			tv.tv_sec = wait_timeus / 1000000;
		    tv.tv_usec = wait_timeus % 1000000;
		    select(0, NULL, NULL, NULL, &tv);
		} else wait_timeus = 0;

		// End of time cycle
		clock_gettime (CLOCK_REALTIME, &end_time);

		// Compute time carried over
		total_elapsedns = elapsed(start_time, end_time);
		carry_timens = total_expectedns - total_elapsedns;

		// Begining of a new time cycle
		start_time = end_time; // Start of this cycle is the end of previous one

		// Collect statistics 
		period = total_elapsedns / number_of_iterations; // Period (ns)

		periods[period_number++] = period;
		if (period_number==CLOCK_STAT_PERIODS) period_number=0;

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
//			printf("Periods (ns/cycle): min=%d, max=%d, mean=%d\nCurrent: period=%d", min_period, max_period, mean_period, period);
		}
	}
	printf("CCarryClock : Stopped.\n");
}


