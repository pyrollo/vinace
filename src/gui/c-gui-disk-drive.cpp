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


#include <iostream>
#include "c-gui-disk-drive.hpp"

/* Pictures:
 *
 * The hole picture size is 300x178
 * The "open" picture is 79x123 and should be placed at 110,16
 * The "on" picture is 18x18 and should be placed at 56,120
 */

bool in_rect(int x, int y, Gdk::Rectangle *rect) {
	return
		(x>=rect->get_x() and
		x<=(rect->get_x()+rect->get_width()) and
		y>=rect->get_y() and
		y<=(rect->get_y()+rect->get_height()));
};

#define IMAGE_DRIVE_1 "resources/drive_1.jpg"
#define IMAGE_DRIVE_2 "resources/drive_2.jpg"
#define IMAGE_CLOSED  "resources/closed.jpg"
#define IMAGE_ON      "resources/on.jpg"

#define PIX_ON_X (63)
#define PIX_ON_Y (140)
#define PIX_CLOSED_X (129)
#define PIX_CLOSED_Y (20)

CGuiDiskDrive::CGuiDiskDrive(CDiskDrive *drive, bool drive2) {
	this->drive = drive;
	this->disk = new CFloppyDisk(); // Blank disk

	this->motor_on = drive->get_motor();
	this->loaded   = drive->loaded();

	// Pixbuf preparation
	if (drive2)
		mainpix =  Gdk::Pixbuf::create_from_file(IMAGE_DRIVE_2);
	else
		mainpix =  Gdk::Pixbuf::create_from_file(IMAGE_DRIVE_1);

	closedpix =  Gdk::Pixbuf::create_from_file(IMAGE_CLOSED);
	closedrect = new Gdk::Rectangle(PIX_CLOSED_X, PIX_CLOSED_Y, closedpix->get_width(), closedpix->get_height());

	onpix =  Gdk::Pixbuf::create_from_file(IMAGE_ON);
	onrect = new Gdk::Rectangle(PIX_ON_X, PIX_ON_Y, onpix->get_width(), onpix->get_height());
	pressed_rect = 0;

	// Text layout for displaying disk name
	layout_disk_name = create_pango_layout("disk name");
	layout_disk_name->set_text(disk->get_name());
	layout_disk_name->set_alignment(Pango::ALIGN_CENTER);
	layout_disk_name->set_width(mainpix->get_width()*PANGO_SCALE);

	// Adjust drawing area size
	set_size_request(mainpix->get_width(), mainpix->get_height());
	signal_expose_event().connect(sigc::mem_fun(this, &CGuiDiskDrive::on_expose_event));

	// Signals for clicking
	signal_button_press_event().connect(sigc::mem_fun(this, &CGuiDiskDrive::button_press));
	signal_button_release_event().connect(sigc::mem_fun(this, &CGuiDiskDrive::button_release));
	add_events(Gdk::BUTTON_PRESS_MASK|Gdk::BUTTON_RELEASE_MASK);

	// Become a Drag and Drop destination
	signal_drag_data_received().connect(sigc::mem_fun(this, &CGuiDiskDrive::on_drag_data_received));
	drag_dest_set(Gtk::DestDefaults(), Gdk::DragAction());
	signal_drag_drop().connect(sigc::mem_fun(this, &CGuiDiskDrive::on_drag_drop));
	signal_drag_motion().connect(sigc::mem_fun(this, &CGuiDiskDrive::on_drag_motion));

	// Observe the drive
	drive->subscribe(this);
}

CGuiDiskDrive::~CGuiDiskDrive() {
	drive->unsubscribe(this);
	delete closedrect;
	delete onrect;
	if (disk) delete disk;
}

void CGuiDiskDrive::notify(CObservable *obs) {
	if (obs == drive) {
		// Has a disk been ejected or loaded ?
		if (drive->loaded() != loaded) {
			this->loaded = drive->loaded();
			if (get_window())
		        get_window()->invalidate_rect(*closedrect, false);
		}

		// Has the motor been switched ?
		if (drive->get_motor() != motor_on) {
			this->motor_on   = drive->get_motor();
			if (get_window())
			    get_window()->invalidate_rect(*onrect, false);
		}
	}
}

void CGuiDiskDrive::load_disk(std::string filename) {
	delete disk;
	disk = new CFloppyDisk(filename);
	drive->eject();
	drive->insert(disk);
	layout_disk_name->set_text(disk->get_name());
	get_window()->invalidate_region(get_window()->get_visible_region()); // A bit brutal but changing disk wont append so often
}

void CGuiDiskDrive::on_drag_data_received(
        const Glib::RefPtr<Gdk::DragContext>& context, int, int,
        const Gtk::SelectionData& selection_data, guint, guint time)
{
	Glib::StringArrayHandle uris = selection_data.get_uris();
	if (uris.size() == 1) {
		Glib::ustring url = *(uris.begin());
		if (url.substr(0,7) == "file://") {
			load_disk(Glib::uri_unescape_string(url.substr(7).raw()));
			context->drop_finish(true, time);
			return;
		}
	}
	context->drop_finish(false, time);
}

bool CGuiDiskDrive::on_drag_drop(const Glib::RefPtr<Gdk::DragContext>& context,
	int, int, guint time) {
	drag_get_data(context, "text/uri-list", time);
	return true;
}

bool CGuiDiskDrive::on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context,
	int, int, guint time) {
    context->drag_status(Gdk::ACTION_COPY, time);
	return true;
}

//http://gtk.php.net/manual1/fr/html/gdk.gdkeventbutton.html
bool CGuiDiskDrive::button_press(GdkEventButton* event) {
	int x = (int)event->x;
	int y = (int)event->y;
	if (in_rect(x, y, closedrect)) {
		pressed_rect = closedrect;
	}
	return true;
}

bool CGuiDiskDrive::button_release(GdkEventButton* event) {
	int x = (int)event->x;
	int y = (int)event->y;

	if (event->button == 1
		and in_rect(x, y, closedrect)
		and pressed_rect == closedrect)
	{
		if (loaded)
			drive->eject();
		else
			drive->insert(disk);

		get_window()->invalidate_rect(*closedrect, false);
	}

	pressed_rect = 0;

	return true;
}

bool CGuiDiskDrive::on_expose_event(GdkEventExpose* ev)
{
	if (get_window()) {
		mainpix->render_to_drawable(get_window(), get_style()->get_black_gc(), 0, 0, 0, 0, -1, -1, Gdk::RGB_DITHER_NONE, 0, 0);
		if (motor_on)
			onpix->render_to_drawable(get_window(), get_style()->get_black_gc(), 0, 0, PIX_ON_X, PIX_ON_Y, -1, -1, Gdk::RGB_DITHER_NONE, 0, 0);

		if (loaded)
			closedpix->render_to_drawable(get_window(), get_style()->get_black_gc(), 0, 0, PIX_CLOSED_X, PIX_CLOSED_Y, -1, -1, Gdk::RGB_DITHER_NONE, 0, 0);

		get_window()->draw_layout(get_style()->get_white_gc(), 0,  mainpix->get_height() - 30, layout_disk_name);
	}
	return true;
}
