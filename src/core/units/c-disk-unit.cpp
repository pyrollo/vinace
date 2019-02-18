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

#include "c-disk-unit.hpp"

//
// Duplicate drive interface
//

CDriveInterface::CDriveInterface() {
	action = action_none;
	protection = false;
	ready = false;
}

void CDriveInterface::set_motor(bool onoff) { 
	action=action_motor; 
	this->onoff=onoff; 
	notifyUpdate(); 
}

void CDriveInterface::set_magnet(char number, bool onoff) { 
	action=action_magnet; 
	this->number=number; 
	this->onoff=onoff; 
	notifyUpdate(); 
}

void CDriveInterface::ask_read() { 
	action = action_read; 
	notifyUpdate(); 
}

void CDriveInterface::ask_write() {
	action = action_write;
	notifyUpdate();
}

CDiskUnit::CDiskUnit() {
	interfaces[0] = new CDriveInterface();
	interfaces[1] = new CDriveInterface();
	selected = interfaces[0];
	mode = mode_none;
	dummy = 0;
}

//
// Main controler
//

// Stop motors on reset
void CDiskUnit::reset() {
	interfaces[0]->set_motor(false);
	interfaces[1]->set_motor(false);
}

void CDiskUnit::access(BYTE addr) {

	// Switches common to read and write
	
	// Stepper command
	if (addr < 0x08)
		selected->set_magnet(addr>>1, addr&0x01);
	else
		switch (addr) {
			// Motor
			case 0x08: selected->set_motor(false); break;
			case 0x09: selected->set_motor(true); break;

			// Drive select
			case 0x0A: selected = interfaces[0]; break;
			case 0x0B: selected = interfaces[1]; break;
			
			// Mode
			case 0x0C: mode = mode_read;  break;
			case 0x0D: mode = mode_check; break;
			case 0x0E: mode = mode_read;  break;
			case 0x0F: mode = mode_write; break;
		}

		if (addr==0x0C and mode == mode_write) selected->ask_write();

	notifyUpdate();
}

BYTE CDiskUnit::read(BYTE addr) {
	BYTE byte = dummy_byte()&0x7F;
	
	if (addr==0x0E and mode == mode_check) 
		byte = bool_to_b7(selected->get_protection());
	
	if (addr==0x0C and mode == mode_read) {
		selected->ask_read();
		if (selected->data_ready())
			byte = selected->get_data();
		else
			// The value must change in order to pass motor on test 
			// For this test, the software usualy waits for data to change.
			// Lack of change induces very poor read performances
			// TODO : This should be managed somewhere else (in disk drive for
			//        example).
			byte = (dummy++)&0x7F;
	}

	access(addr);
	return byte;
}

void CDiskUnit::write(BYTE addr, BYTE byte){
	if (addr==0x0D or addr==0x0F)
		selected->set_data(byte);
	access(addr);
}
