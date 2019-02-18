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

#include "c-gui-keyboard.hpp"

CGuiKeyboard::CGuiKeyboard(Gtk::Widget *eventSource, CKeyboard *keyboard) {
	this->widget = eventSource;
	this->keyboard = keyboard;
	this->widget->signal_key_press_event().connect(sigc::mem_fun(this, &CGuiKeyboard::on_key_pressed));
	this->widget->signal_key_release_event().connect(sigc::mem_fun(this, &CGuiKeyboard::on_key_release));
	this->widget->property_can_focus().set_value(true);
	this->widget->set_sensitive();
	this->widget->grab_focus();
}

CGuiKeyboard::~CGuiKeyboard() {
	// TODO : Disconnect signal handlers
}

// https://stage.maemo.org/svn/maemo/projects/haf/trunk/gtk+/gdk/gdkkeysyms.h


char CGuiKeyboard::get_ascii_key(GdkEventKey* event) {
	int key = event->keyval;
	
	// Control keys
	if (event->state & Gdk::CONTROL_MASK) {
		if (key > GDK_Z)
			key = key- GDK_a + GDK_A; // Shift key to uppercase
		
		if (key >= GDK_A and key <= GDK_Z) 
			return key & 0x1F;// Control-<key>
	}

	// Standard keys
	else 
		// Ascii characters
		if (key >= 0x20 and key<0x7F) return key;
		
		// Special char
		switch(event->keyval) {
			case GDK_Left:      return 0x08;
			case GDK_Tab:       return 0x09;
			case GDK_Down:      return 0x0A;
			case GDK_Up:        return 0x0B;
			case GDK_Return:    return 0x0D;
			case GDK_Right:     return 0x15;
			case GDK_Escape:    return 0x1B;
			case GDK_BackSpace: return 0x7F;
 		}
	return -1;
}

// TODO : Put the special keys in a configuration file

bool CGuiKeyboard::on_key_pressed(GdkEventKey* event) {
	char key;
	
	// Apple keys -> bound to game buttons
	if (event->keyval == GDK_Alt_L) { keyboard->press_open_apple();  return true; }
	if (event->keyval == 0xfe03 /*GDK_Alt_R*/ ) { keyboard->press_closed_apple(); return true; }

	
	// Ascii keys
	key = get_ascii_key(event);
	if (key>=0) {
		keyboard->press_key(key);
		return true;
	}

	// Reset : Ctrl + Scroll Lock
	if ((event->state & Gdk::CONTROL_MASK) and (event->keyval == GDK_Scroll_Lock or event->keyval == GDK_Break)) {
		keyboard->press_reset();
		return true;
	}

	return false;
}

bool CGuiKeyboard::on_key_release(GdkEventKey* event) {
	// Apple keys -> bound to game buttons
	if (event->keyval == GDK_Alt_L) { keyboard->release_open_apple(); return true; }
	if (event->keyval == 0xfe03 /*GDK_Alt_R*/) { keyboard->release_closed_apple(); return true; }

	// Normal keys
	if (get_ascii_key(event)>=0) {
		keyboard->release_key();
		return true;
	}
	return false;
}
