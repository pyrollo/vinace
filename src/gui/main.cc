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

#include <gtkmm.h>
#include "hardware/models/c-apple2e-hardware.hpp"
#include "hardware/cards/c-disk-controler-card.hpp"
#include "hardware/c-disk-drive.hpp"
#include "c-gui-green-monitor.hpp"
#include "c-gui-color-monitor.hpp"
#include "c-gui-keyboard.hpp"
#include "c-gui-numpad-joystick.hpp"
#include "c-gui-disk-drive.hpp"
#include "c-pulse-audio-sound.hpp"
#include <fstream>
#include <iostream>
#include "config.h"

void load_rom(CRomMemory *rom, const char *filename, int offset) {
    std::ifstream file (filename, std::ios::in|std::ios::binary);
	if (file.is_open())
	{
		file.seekg(offset);
		file.read((char *)rom->get_buffer(), rom->get_size());
		file.close();
    }
	else
		std::cout << "Unable to load ROM file \"" << filename << "\"." << std::endl;
}

int main(int argc, char *argv[])
{
	// Read rom file
	CRomMemory *lcRom   = new CRomMemory(0x3000);
	CRomMemory *intRom  = new CRomMemory(0x1000);
	CRomMemory *diskRom = new CRomMemory(0x0100);

	load_rom(lcRom,   "APPLE2E.ROM", 0x5000);
	load_rom(intRom,  "APPLE2E.ROM", 0x4000);
	load_rom(diskRom, "APPLE2E.ROM", 0x0600);

	// Build an Apple //e
	CApple2eHardware *apple = new CApple2eHardware(lcRom, intRom);
	CDiskControlerCard *controler = new CDiskControlerCard(diskRom);
	apple->insertCard(6, controler);
	CDiskDrive *drive1 = new CDiskDrive(apple->get_clock(), controler->get_interface(0));
	CDiskDrive *drive2 = new CDiskDrive(apple->get_clock(), controler->get_interface(1));

	// Build GUI
	Gtk::Main main(argc, argv);
	Gtk::Window window;
	window.set_title(VINACE_FULL_NAME);

	Gtk::VBox   box;
	window.add(box);

	box.property_can_focus().set_value(true);
    box.set_sensitive();
	box.grab_focus();

	CGuiMonitor *monitor = new CGuiColorMonitor(apple->colorvideo);
	box.add(*monitor);

	Gtk::HBox drivebox;
	box.add(drivebox);
	CGuiDiskDrive *gdrive1 = new CGuiDiskDrive(drive1, false);
	drivebox.add(*gdrive1);
	CGuiDiskDrive *gdrive2 = new CGuiDiskDrive(drive2, true);
	drivebox.add(*gdrive2);

	CGuiKeyboard *keyboard = new CGuiKeyboard(&box, apple->keyboard);
	CGuiNumpadJoystick *joystick = new CGuiNumpadJoystick(&box, apple->paddles);

	CPulseAudioSound *sound = new CPulseAudioSound(apple->speaker);

	window.show_all();

	// Run !
	sound->start();
	apple->start();
	main.run(window);

	// Delete roms
	delete lcRom;
	delete intRom;
	delete diskRom;

	return 0;
}
