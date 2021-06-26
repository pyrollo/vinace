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

#include "c-disk-drive.hpp"
#include <iostream>

//#define CYCLES_PER_NIBBLE 32
#define CYCLES_PER_NIBBLE 32

//http://www.doc.ic.ac.uk/~ih/doc/stepper/others/example3/diskii_specs.html

// Begin copy from YAE
#define NO_OF_PHASES 8
#define MAX_PHYSICAL_TRACK_NO (40*NO_OF_PHASES)

static int	stepper_movement_table[16][NO_OF_PHASES] = {
	{  0,  0,  0,  0,  0,  0,  0,  0 },	/* all electromagnets off */
	{  0, -1, -2, -3,  0,  3,  2,  1 },	/* EM 1 on */
	{  2,  1,  0, -1, -2, -3,  0,  3 },	/* EM 2 on */
	{  1,  0, -1, -2, -3,  0,  3,  2 },	/* EMs 1 & 2 on */
	{  0,  3,  2,  1,  0, -1, -2, -3 },	/* EM 3 on */
	{  0, -1,  0,  1,  0, -1,  0,  1 },	/* EMs 1 & 3 on */
	{  3,  2,  1,  0, -1, -2, -3,  0 },	/* EMs 2 & 3 on */
	{  2,  1,  0, -1, -2, -3,  0,  3 },	/* EMs 1, 2 & 3 on */
	{ -2, -3,  0,  3,  2,  1,  0, -1 },	/* EM 4 on */
	{ -1, -2, -3,  0,  3,  2,  1,  0 },	/* EMs 1 & 4 on */
	{  0,  1,  0, -1,  0,  1,  0, -1 },	/* EMs 2 & 4 */
	{  0, -1, -2, -3,  0,  3,  2,  1 },	/* EMs 1, 2 & 4 on */
	{ -3,  0,  3,  2,  1,  0, -1, -2 },	/* EMs 3 & 4 on */
	{ -2, -3,  0,  3,  2,  1,  0, -1 },	/* EMs 1, 3 & 4 on */
	{  0,  3,  2,  1,  0, -1, -2, -3 },	/* EMs 2, 3 & 4 on */
	{  0,  0,  0,  0,  0,  0,  0,  0 } };	/* all electromagnets on */
// End copy from YAE

CDiskDrive::CDiskDrive(CClock *clock, CDriveInterface *interface) {
	this->clock = clock;
	this->interface = interface;
	if (interface) {
		interface->set_protection(false);
		interface->subscribe(this);
	}

	motor = false;
	nibble_ready = false;
	disk = 0;
	stepper_status = 0;
	head_position = 0;
	disk_position = 0;

	clock->set_timer(CYCLES_PER_NIBBLE, &timer_callback, this);
}

CDiskDrive::~CDiskDrive() {
	if (interface)
		interface->unsubscribe(this);
	// TODO : Remove clock timer
}

void CDiskDrive::timer_callback(void *ptr) {
	((CDiskDrive *)(ptr))->next_nibble();
}

// An action has been requested by the interface
void CDiskDrive::notify(CObservable *obs) {
	if (obs == interface) {
		switch (interface->get_action()) {
			case action_motor:
				set_motor(interface->onoff);
				break;
			case action_magnet:
				set_magnet(interface->number, interface->onoff);
				break;
			case action_write:
				if (motor and interface->data_ready())
					write_nibble(interface->get_data());
				break;
			case action_read:
				if (nibble_ready) {
					interface->set_data(read_nibble());
					nibble_ready = false;
				}
				break;
		}
	}
}


void CDiskDrive::insert(CFloppyDisk *disk) {
	this->disk = disk;
	if (interface)
		if (disk)
			interface->set_protection(disk->get_protected());
		else
			interface->set_protection(false); // If no disk disk II protection sensor is in unprotected position
	notifyUpdate();
}

void CDiskDrive::eject() {
	this->disk = 0;
	if (interface)
		interface->set_protection(false); // If no disk disk II protection sensor is in unprotected position
	notifyUpdate();
}

bool CDiskDrive::loaded() {
	return this->disk != 0;
}

CFloppyDisk *CDiskDrive::get_disk() {
	return this->disk;
};

bool CDiskDrive::get_motor() {
	return motor;
}

// TODO : Actually, motor does not stops immediately, there is a slight delay.
// TODO : Test the behavior of the controler when motor off is requested, and motor is not yet off.
void CDiskDrive::set_motor(bool on) {
	motor = on;
	notifyUpdate();
}

void CDiskDrive::set_magnet(int magnet, bool on)
{
	if ( on )
		stepper_status |= (1<<magnet);
	else
		stepper_status &= ~(1<<magnet);

	if ( motor ) {

		head_position +=  stepper_movement_table[stepper_status][head_position & 0x07];

		if ( head_position < 0 ) {
			printf("CDiskDrive: Reached first track !\n");
	  		head_position = 0;
		}
		else if ( head_position >= MAX_PHYSICAL_TRACK_NO ) {
			printf("CDiskDrive: Reached last track !\n");
	  		head_position = MAX_PHYSICAL_TRACK_NO-1;
		}
		printf("CDiskDrive : head position %02x\n", head_position);
	}
	notifyUpdate();
}

void CDiskDrive::next_nibble() {
	static long missed;
	if (motor) {
		disk_position ++;
		if (nibble_ready)
			missed++;
		else
			if (missed) {
				printf("Missed %ld at %d\n", missed, disk_position);
				missed=0;
		}
		if (disk_position >= disk->get_position_maximum()) {
			disk_position = 0; printf("Turn completed\n");}
		nibble_ready = true;
	}
}

BYTE CDiskDrive::read_nibble() {
	BYTE data;
	if (!disk) return 0;
	if (!motor) return 0;

 	data = disk->get_nibble(head_position >> 2, disk_position);
	return data;
}

void CDiskDrive::write_nibble(BYTE data) {

	if (!disk) return;
	if (!motor) return;

	// TODO :manage write protect
	// TODO :manage write !

}
