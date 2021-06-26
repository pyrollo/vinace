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


#ifndef _C_DISK_DRIVE_HPP_
#define _C_DISK_DRIVE_HPP_
#include "gcr.h"
#include "core/c-observable.hpp"
#include "core/c-observer.hpp"
#include "core/units/c-disk-unit.hpp"
#include "c-floppy-disk.hpp"
#include "c-clock.hpp"
/*
#define DISK_IMAGE_SECTORS 16
#define DISK_IMAGE_TRACK_SIZE (DISK_IMAGE_SECTORS*256)
#define DISK_IMAGE_TRACKS  35
#define DISK_IMAGE_SIZE (DISK_IMAGE_TRACKS*DISK_IMAGE_TRACK_SIZE)
*/

// A Disk II drive
class CDiskDrive: public CObservable, public CObserver
{
public:
	CDiskDrive(CClock *clock, CDriveInterface *interface);
	~CDiskDrive();

	static void timer_callback(void *ptr);

	void notify(CObservable *obs);

	void next_nibble();

	// On the physical word side
	void insert(CFloppyDisk *disk);
	void eject();
	bool loaded();
	bool get_motor();

	CFloppyDisk *get_disk();

protected:
	void set_motor(bool on);
	void set_magnet(int magnet, bool on);
	BYTE read_nibble();
	void write_nibble(BYTE data);

	CClock *clock;

	// Interface which the drive is connected to
	CDriveInterface *interface;

	// The disk loaded in the drive
	CFloppyDisk *disk;

	// Drive status
	bool motor;         // Motor on/of
	int stepper_status; // Step engine status
	int head_position;  // Physical head position (1/8 tracl)
	int disk_position;  // Position in track (rotation)
	bool nibble_ready;  // Is there a nibble ready to be read ?
};

#endif // _C_DISK_DRIVE_HPP_
