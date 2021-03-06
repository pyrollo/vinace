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
#ifndef _C_GUI_KEYBOARD_HPP_
#define _C_GUI_KEYBOARD_HPP_

#include <gtkmm.h>
#include "hardware/c-keyboard.hpp"

class CGuiKeyboard
{
public:
	CGuiKeyboard(Gtk::Widget *eventSource, CKeyboard *keyboard);
	~CGuiKeyboard();

	bool on_key_pressed(GdkEventKey* event);
	bool on_key_release(GdkEventKey* event);

protected:
	char get_ascii_key(GdkEventKey* event);

	Gtk::Widget *widget;
	CKeyboard *keyboard;
};

#endif // _C_GUI_KEYBOARD_HPP_
