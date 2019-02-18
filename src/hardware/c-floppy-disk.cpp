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


#include "c-floppy-disk.hpp"
#include "gcr.h"
#include <fstream>
#include <iostream>

CFloppyDisk::CFloppyDisk() {
	set_name("Blank disk");
}
CFloppyDisk::CFloppyDisk(std::string filename) {
	set_name("Blank disk");
	load_from_dsk_file(filename);
}

std::string CFloppyDisk::get_name() {
	return name;
}

void CFloppyDisk::set_name(std::string name) {
	this->name = name;
}

BYTE CFloppyDisk::get_nibble(int track, int position) {
	if (in_disk(track, position))
		return nibbles[track][position];
	else
		return 0xFF;
}

void CFloppyDisk::set_nibble(int track, int position, BYTE nibble) {
	if (in_disk(track, position))
		nibbles[track][position] = nibble;
}

void CFloppyDisk::set_name_from_file(std::string filename) {
	size_t pos;
	std::string name = filename;
	pos = name.rfind('/');
	if (pos != std::string::npos) 
		name = name.substr(pos+1);
	pos = name.rfind('.');
	if (pos != std::string::npos) 
		name = name.substr(0,pos);
	set_name(name);
}

// Reads from a logical disk image
void CFloppyDisk::load_from_dsk_file(std::string filename) {

	BYTE logical_disk[DISK_IMAGE_SIZE];
	
    std::ifstream file (filename.c_str(), std::ios::in|std::ios::binary);
	if (file.is_open())
	{
		file.read ((char *)logical_disk, DISK_IMAGE_SIZE);
		file.close();
		if (file.fail()) {
			if (file.eof())
			    std::cout<< "CDiskDrive: Disk image file \""<< filename <<"\" too small !\n"<<std::endl;
			else
			    std::cout<< "CDiskDrive: Failed to read disk image file \""<< filename <<"\".\n"<<std::endl;
		}
    }
	else std::cout<< "CDiskDrive: Unable to load disk image file \""<< filename <<"\".\n"<<std::endl;	
	
	// Convert disk image to nibbles
	for (int track = 0; track < DISK_IMAGE_TRACKS; track++)
		SectorsToNibbles(logical_disk+track*DISK_IMAGE_TRACK_SIZE, nibbles[track], 254, track);
		 
	set_name_from_file(filename);
}

// "Understanding the Apple II" p9-27
