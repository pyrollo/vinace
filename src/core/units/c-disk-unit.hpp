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

#ifndef _C_DISK_UNIT_HPP_
#define _C_DISK_UNIT_HPP_
#include "c-unit.hpp"

enum drive_action {
	action_none,
	action_motor,
	action_write,
	action_read,
	action_magnet
};

class CDriveInterface:public CObservable
{
public:
	CDriveInterface();

	drive_action get_action() { return action; };

	void set_protection(bool onoff) { this->protection = onoff; };
	bool get_protection() { return this->protection; };

	void set_motor(bool onoff);
	
	void set_magnet(char number, bool onoff);

	void ask_read();
	bool data_ready() { return ready; };

	void ask_write();

	void set_data(BYTE data) { ready=true; this->data=data; };
	BYTE get_data() { ready=false; return data; };

	bool onoff;
	char number;
protected:
	drive_action action;

	BYTE data;
	bool ready;

	bool protection;
};

class CDiskUnit:public CUnit
{
public:
	CDiskUnit();
	void write(BYTE addr, BYTE byte);
	BYTE read(BYTE addr);
	void reset();
	CDriveInterface *interfaces[2];

protected:
	CDriveInterface *selected;
	enum {mode_none, mode_read, mode_write, mode_check} mode;
	BYTE buffered_data;
private:
	void access(BYTE addr);
	BYTE dummy;
};

#endif // _C_DISK_UNIT_HPP_
