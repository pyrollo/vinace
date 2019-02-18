/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * main.cc is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * main.cc is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>
#include "hardware/models/c-apple2e-hardware.hpp"

int main(int argc, char *argv[])
{
	
	CApple2eHardware *apple = new CApple2eHardware("APPLE2E.ROM");
	apple->start();
	
	Gtk::Main main(argc, argv);
	Gtk::Window window;
    main.run(window);
	return 0;
}
