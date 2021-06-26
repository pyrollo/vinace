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

#include "c-apple2e-core.hpp"
#include "core/c-processor6502.hpp"
#include "core/busses/c-io-rom-bus.hpp"
#include "core/busses/c-language-card-bus.hpp"
#include "core/busses/c-main-memory-bus.hpp"
#include "core/busses/c-slot-rom-bus.hpp"

//
// IO UNIT
//

CApple2eIoUnit::CApple2eIoUnit() {
	// Apple ][+ units
	slots        = new CSlotsUnit();
	keyboard     = new CKeyboardUnit();
	game         = new CGameUnit();
	speaker      = new CSpeakerUnit();
	graphicMode  = new CGraphicModeUnit();
	languageCard = new CLanguageCardUnit();

	// Apple //e extra units
	textMode     = new CTextModeUnit();
	ioRom        = new CIoRomUnit();

	// Apple ][+ IOU Map:
	assign_read(      0x00,       keyboard);
	assign_read_write(0x10,       keyboard);
	assign_read(      0x30,       speaker);
	assign_read(      0x40,       game);
	assign_read_write(0x50, 0x57, graphicMode);
	assign_read_write(0x58, 0x5F, game);
	assign_read(      0x61, 0x67, game);
	assign_read(      0x69, 0x70, game);
	assign_read_write(0x80, 0x8F, languageCard);
	assign_read_write(0x90, 0xFF, slots);

	// Apple //e extra IOU Map
	assign_write(     0x06, 0x07, ioRom);
	assign_write(     0x0A, 0x0B, ioRom);
	assign_write(     0x0C, 0x0F, textMode);
	assign_read(      0x11, 0x12, languageCard);
	assign_read(      0x15,       ioRom);
	assign_read(      0x17,       ioRom);
	assign_read(      0x1E, 0x1F, textMode);

	reset();
}

CApple2eIoUnit::~CApple2eIoUnit() {
	delete slots;
	delete keyboard;
	delete game;
	delete speaker;
	delete graphicMode;
	delete languageCard;
	delete textMode;
	delete ioRom;
}

void CApple2eIoUnit::reset() {
	slots->reset();
	keyboard->reset();
	game->reset();
	speaker->reset();
	graphicMode->reset();
	languageCard->reset();
	textMode->reset();
	ioRom->reset();
}

//
// The computer
//

CApple2eCore::CApple2eCore(CMemory *lcRom, CMemory *intRom) {
	// ROM : internal and language card
	this->intRom = intRom;
	this->lcRom = lcRom;

	// Input Output Unit
	iou = new CApple2eIoUnit();

	// RAM : main and language card
	ram   = new CRamMemory(0x10000); // To be replaced by a AuxiliaryBus for 128K ram
	lcRam = new CMemoryProxy(ram, 0xC000);

	slotBus = new CSlotRomBus();
	ioBus   = new CIoRomBus(iou->ioRom, slotBus, intRom);
	lcBus   = new CLanguageCardBus(iou->languageCard, lcRom, lcRam);

	// memory as seen by the processor
	memory = new CMainMemoryBus(ram, iou, ioBus, lcBus);

	// 6502 processor
	processor = new CProcessor6502(memory);

	// Issue a first reset
	reset();
}

  CApple2eCore::~CApple2eCore() {
	delete processor;
	delete memory;
	delete lcBus;
	delete ioBus;
	delete slotBus;
	delete lcRam;
	delete ram;
	delete iou;
}

void CApple2eCore::reset() {
	iou->reset();
	processor->signal(SIGNAL_RESET);
}

void CApple2eCore::insertCard(int slot, CUnit *cardUnit, CMemory *cardRom, CMemory *cardRomExt) {
	iou->slots->insert_card(slot, cardUnit);
	slotBus->insert_card(slot, cardRom, cardRomExt);
};

void CApple2eCore::removeCard(int slot) {
	iou->slots->remove_card(slot);
	slotBus->remove_card(slot);
};
