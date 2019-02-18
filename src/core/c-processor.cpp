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

#include "c-processor.hpp"

CProcessor::CProcessor(CMemory *memory, int number_of_signals) {
	this->memory = memory;
	this->number_of_signals = number_of_signals;
	
	// Initializing signals
	signals = new int[number_of_signals];
	
	for (int signal=0; signal < number_of_signals; signal++)
		signals[signal] = 0;
	
	// Other members
	cycles = 0;
	waiting = false;
	
	// Issue a first reset to initialize processor
	reset();
}

CProcessor::~CProcessor() {
	delete signals;
}

void CProcessor::signal(int signal) {
	if (signal < number_of_signals)
	signals[signal]++;
}

void CProcessor::cycle() {
	// Beware thats a =, not a ==
	if (waiting = cycles)
		cycles--; // Wait for the last instruction to finish
	else {
		if (process_signals()); // First, process pending signals
			process_instruction(); // If no signal, process instruction
		if (cycles) cycles--; // Begin cycle countdown
	}
}