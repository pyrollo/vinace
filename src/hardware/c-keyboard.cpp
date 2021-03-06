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

#include "c-keyboard.hpp"

CKeyboard::CKeyboard(CComputerCore *core, CKeyboardUnit *keyboard, CGameUnit *game) {
	this->core = core;
	this->keyboard = keyboard;
	this->game = game;
}
	
void CKeyboard::press_key(BYTE asciiKey) {
	keyboard->press_key(asciiKey);
}

void CKeyboard::release_key() {
	keyboard->release_key();
}

void CKeyboard::press_reset() {
	core->reset();
}

void CKeyboard::press_open_apple() {
	game->set_pb0(true);
}

void CKeyboard::release_open_apple() {
	game->set_pb0(false);
}

void CKeyboard::press_closed_apple() {
	game->set_pb1(true);
}

void CKeyboard::release_closed_apple() {
	game->set_pb1(false);
}
