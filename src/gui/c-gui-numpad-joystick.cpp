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

#include "c-gui-numpad-joystick.hpp"

CGuiNumpadJoystick::CGuiNumpadJoystick(Gtk::Widget *eventSource, CPaddles *paddles) {
	this->widget = eventSource;
	this->paddles = paddles;
	this->widget->signal_key_press_event().connect(sigc::mem_fun(this, &CGuiNumpadJoystick::on_key_pressed));
	this->widget->signal_key_release_event().connect(sigc::mem_fun(this, &CGuiNumpadJoystick::on_key_release));
	for (int key=0; key<10; key++) keys[key]=false;
}

CGuiNumpadJoystick::~CGuiNumpadJoystick() {
	// TODO : Disconnect signal handlers
}

// https://stage.maemo.org/svn/maemo/projects/haf/trunk/gtk+/gdk/gdkkeysyms.h

// TODO : Put the special keys in a configuration file

bool CGuiNumpadJoystick::set_key(int key, bool onoff) {
	BYTE value;
	
	switch (key) {
//		case GDK_KP_0:     paddles->set_button(0, onoff); return true;
//		case GDK_KP_Enter: paddles->set_button(1, onoff); return true;

		case GDK_KP_1: keys[1] = onoff; break;
		case GDK_KP_2: keys[2] = onoff; break;
		case GDK_KP_3: keys[3] = onoff; break;
		case GDK_KP_4: keys[4] = onoff; break;
		case GDK_KP_5: keys[5] = onoff; break;
		case GDK_KP_6: keys[6] = onoff; break;
		case GDK_KP_7: keys[7] = onoff; break;
		case GDK_KP_8: keys[8] = onoff; break;
		case GDK_KP_9: keys[9] = onoff; break;
		
		default:return false;
	}

	value = 0x80;
	if (keys[7] or keys[4] or keys[1]) value  = 0x00;
	if (keys[9] or keys[6] or keys[3]) value += 0x7F;
	paddles->set_paddle(0, value);
	
	value = 0x80;
	if (keys[7] or keys[8] or keys[9]) value  = 0x00;
	if (keys[1] or keys[2] or keys[3]) value += 0x7F;
	paddles->set_paddle(1, value);
	
	return true;
}

bool CGuiNumpadJoystick::on_key_pressed(GdkEventKey* event) {
	return set_key(event->keyval, true);
}

bool CGuiNumpadJoystick::on_key_release(GdkEventKey* event) {
	return set_key(event->keyval, false);
}
