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

#ifndef _C_PROCESSOR_HPP_
#define _C_PROCESSOR_HPP_

#define SIGNAL_RESET 0 // Reset signal

#include "c-memory.hpp"

class CProcessor
{
public:
	CProcessor(CMemory *memory, int number_of_signals = 0);
	~CProcessor();

	void cycle(); // Play one machine cycle (may do nothing)
	void signal(int signal); // Send a signal (IRQ, Reset, ...) to the processor

	// Access to internal processor state
	int get_cycles() { return cycles; };
	bool is_waiting() { return waiting; };

protected:
	CMemory *memory; // Memory (or bus) accessed by the processor

	int number_of_signals;
	int *signals; // Pending signals
	int cycles;	  // Remaining cycles before end of instruction
	bool waiting; // Indicates if cycle() has just waited

	virtual void reset() {} ; // Reset the processor
	virtual void process_instruction() {}; // Execute the next opcode
	virtual bool process_signals() {}; // Process signals, returns true if no signal and false if there was one

private:

};

#endif // _C_PROCESSOR_HPP_
