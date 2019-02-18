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


#ifndef _C_GUI_DISK_DRIVE_HPP_
#define _C_GUI_DISK_DRIVE_HPP_
#include "../hardware/c-disk-drive.hpp"
#include "../core/c-observer.hpp"
#include <gtkmm.h>

class CGuiDiskDrive: public Gtk::DrawingArea, public CObserver
{
public:
	CGuiDiskDrive(CDiskDrive *drive, bool drive2);
	~CGuiDiskDrive();
	void notify(CObservable *obs);
	bool on_expose_event(GdkEventExpose* ev);
	void eject();
	
	bool button_press(GdkEventButton* event);
	bool button_release(GdkEventButton* event);

	void load_disk(std::string filename);
	
	void on_drag_data_received(
        const Glib::RefPtr<Gdk::DragContext>& context, int, int,
        const Gtk::SelectionData& selection_data, guint, guint time);

	bool on_drag_drop(const Glib::RefPtr<Gdk::DragContext>& context,
		int, int, guint time); 	

	bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context,
		int, int, guint time); 	
	
protected:
	CDiskDrive *drive;
	CFloppyDisk *disk;
	
	Glib::RefPtr<Pango::Layout> layout_disk_name;
	Glib::RefPtr<Gdk::Pixbuf> mainpix;
	Glib::RefPtr<Gdk::Pixbuf> onpix;
	Glib::RefPtr<Gdk::Pixbuf> closedpix;

	Gdk::Rectangle *onrect;
	Gdk::Rectangle *closedrect;
	
	bool motor_on;
	bool loaded;

private:
	Gdk::Rectangle *pressed_rect;
};

#endif // _C_GUI_DISK_DRIVE_HPP_
