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

#include "c-speaker-unit.hpp"

//
// Speaker unit
//

void CSpeakerUnit::reset() {
	s_speaker = false;
	notifyUpdate();
};

void CSpeakerUnit::write(BYTE addr, BYTE byte) {
}

BYTE CSpeakerUnit::read(BYTE addr) {
	s_speaker = not s_speaker;
	notifyUpdate();
	return dummy_byte();
}