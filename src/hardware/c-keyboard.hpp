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
 
#ifndef _C_KEYBOARD_HPP_
#define _C_KEYBOARD_HPP_

#include "src/core/units/c-game-unit.hpp"
#include "src/core/units/c-keyboard-unit.hpp"
#include "src/core/models/c-computer-core.hpp"

class CKeyboard
{
public:
	CKeyboard(CComputerCore *core, CKeyboardUnit *keyboard, CGameUnit *game);
	
	// Standard keys
	void press_key(BYTE asciiKey);
	void release_key();
	
	// Special keys
	void press_reset(); // Actually meaning CTRL-RESET on the Apple keyboard

	void press_open_apple();
	void release_open_apple();
	void press_closed_apple();
	void release_closed_apple();
	
protected:
	CKeyboardUnit *keyboard;
	CGameUnit     *game;
	CComputerCore *core;
};

#endif // _C_KEYBOARD_HPP_
