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


#ifndef _C_FLOPPY_DISK_HPP_
#define _C_FLOPPY_DISK_HPP_
#include <iostream>
#include "../core/types.h"

#define DISK_IMAGE_SECTORS 16
#define DISK_IMAGE_TRACK_SIZE (DISK_IMAGE_SECTORS*256)
#define DISK_IMAGE_TRACKS  35
#define DISK_IMAGE_SIZE (DISK_IMAGE_TRACKS*DISK_IMAGE_TRACK_SIZE)

#define DISK_TRACK_NIBBLES 6392 
//We do not manage half tracks...
#define DISK_TRACKS 40

/*
	CFloppyDisk represents a floppy disks. The floppy disk is stored in nibbles
	and not in bytes. This is closer to reality.  
*/

class CFloppyDisk {
public:
	CFloppyDisk();
	CFloppyDisk(std::string filename);
	std::string get_name();
	void set_name(std::string name);

	bool get_protected() { return false; };

	BYTE get_nibble(int track, int position);
	void set_nibble(int track, int position, BYTE nibble);

	int get_track_maximum() { return DISK_TRACKS; };
	int get_position_maximum() { return DISK_TRACK_NIBBLES; };

	bool in_disk(int track, int position) { return track>=0 and track<get_track_maximum() and position>=0 and position<get_position_maximum(); };

	void set_name_from_file(std::string filename);
	void load_from_dsk_file(std::string filename);

protected:
	std::string name;
	// The huge entire disk in memory!
	BYTE nibbles[DISK_TRACKS][DISK_TRACK_NIBBLES];
};

#endif // _C_FLOPPY_DISK_HPP_
