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

#ifndef _C_CARRY_CLOCK_HPP_
#define _C_CARRY_CLOCK_HPP_

#include "c-clock.hpp"
#include "../core/c-observable.hpp"

#define CLOCK_STAT_PERIODS 100



// 
// CCarryClock
//
// A clock that runs many processor cycles at once and then waits to reach the
// wanted amount of time. Time difference between real and wanted time is 
// carried over to the next run.
//

class CCarryClock : public CClock, public CObservable
{
public:
	CCarryClock(CProcessor *processor);
	void run();
	void set_frequency(float freq);

	int get_mean_period();

protected:
	int nsPeriod;
	void wait();

private:
	// For statistics
	int periods[CLOCK_STAT_PERIODS];
	int period_number;

	int mean_period;
	int max_period;
	int min_period;
};

#endif // _C_CYCLE_AND_WAIT_CLOCK_HPP_
