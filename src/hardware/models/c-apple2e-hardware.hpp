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

#ifndef _C_APPLE2E_HARDWARE_HPP_
#define _C_APPLE2E_HARDWARE_HPP_
#include "../../core/c-memory.hpp"
#include "../../core/models/c-apple2e-core.hpp"
#include "c-computer-hardware.hpp"
#include "../c-keyboard.hpp"
#include "../c-speaker.hpp"
#include "../video/c-video-output.hpp"
#include "../cards/c-card.hpp"
#include "../c-paddles.hpp"

class CApple2eHardware: public CComputerHardware
{
public:
	CApple2eHardware(CMemory *mainRom, CMemory *internalRom);
	~CApple2eHardware();

	virtual CApple2eCore *get_core() { return (CApple2eCore *)core; };

	void insertCard(int slot, CCard *card);
	void removeCard(int slot);

	// Devices
	CKeyboard    *keyboard;
	CPaddles     *paddles;
	CSpeaker     *speaker;
	CVideoOutput *colorvideo;
	CVideoOutput *bwvideo;

protected:
	CMemory *mainRom;
	CMemory *internalRom;
};

#endif // _C_APPLE2E_HARDWARE_HPP_
