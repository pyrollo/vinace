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

#ifndef _C_APPLE2E_CORE_HPP_
#define _C_APPLE2E_CORE_HPP_
#include "c-computer-core.hpp"
#include "../c-io-unit.hpp"

#include "../units/c-game-unit.hpp"
#include "../units/c-graphic-mode-unit.hpp"
#include "../units/c-io-rom-unit.hpp"
#include "../units/c-keyboard-unit.hpp"
#include "../units/c-language-card-unit.hpp"
#include "../units/c-slots-unit.hpp"
#include "../units/c-speaker-unit.hpp"
#include "../units/c-text-mode-unit.hpp"
#include "../busses/c-slot-rom-bus.hpp"

class CApple2eIoUnit: public CIoUnit {
public:
	CApple2eIoUnit();
	~CApple2eIoUnit();
	
	void reset();

	CSlotsUnit        *slots;
	CKeyboardUnit     *keyboard;
	CGameUnit         *game;
	CSpeakerUnit      *speaker;
	CGraphicModeUnit  *graphicMode;
	CLanguageCardUnit *languageCard;
	CTextModeUnit     *textMode;
	CIoRomUnit        *ioRom;

};

class CApple2eCore: public CComputerCore 
{
public:
	CApple2eCore(CMemory *lcRom, CMemory *intRom);
	~CApple2eCore();
	
	void reset();
	
	// In core, a card is a unit plus some rom
	void insertCard(int slot, CUnit *cardUnit, CMemory *cardRom, CMemory *cardRomExt);
	void removeCard(int slot);

	CMemory *rom; // The whole ROM
	CApple2eIoUnit *iou;
	
protected:
	// Memory proxies and busses
	CMemory *intRom;  // Internal ROM ($C100-$CFFF)
	CMemory *lcRom;   // Language card ROM ($D000-FFFF)
	CMemory *ram;     // The whole RAM (64K)
	CMemory *lcRam;   // Language card RAM
	CMemory *ioBus;   // Slots ROM or internal ROM bus
	CMemory *lcBus;   // Language card bus
	CSlotRomBus *slotBus; // Slots ROMs bus
};

#endif // _C_APPLE2E_CORE_HPP_
