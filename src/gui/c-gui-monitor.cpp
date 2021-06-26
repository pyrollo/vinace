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


#include "c-gui-monitor.hpp"

CGuiMonitor::CGuiMonitor(CVideoOutput *vo, const char *imagefile) {
	this->vo = vo;
	this->background =  Gdk::Pixbuf::create_from_file(imagefile);
	this->rendered = background->copy(); // Copy background

	xmargin = (background->get_width() - VIDEO_OUTPUT_WIDTH)/2;
	ymargin = background->get_height()/2 - VIDEO_OUTPUT_HEIGHT; // Pixels are twice as hight than they are wide
	set_size_request(rendered->get_width(), rendered->get_height());

	Glib::signal_timeout().connect(sigc::mem_fun(this, &CGuiMonitor::on_timer_event), /*40*/ 80 );
}

bool CGuiMonitor::on_expose_event(GdkEventExpose* ev) {
	rendered->render_to_drawable(this->get_window(), this->get_style()->get_black_gc(), 0, 0, 0, 0, -1, -1, Gdk::RGB_DITHER_NONE, 0, 0);
	return true;
}

bool CGuiMonitor::on_timer_event() {
	render();
	return true;
}

#define ADD_COL(c1, c2) (((c1)+(c2)<256)?((c1)+(c2)):255)

//#define ADD_COL(c1, c2) (c2)

void CGuiMonitor::render() {
	vo->render();

	int x, y, pixaddr, basey;
	int r, g ,b;

	int multx = rendered->get_n_channels();
	int multy = rendered->get_rowstride();

	char   *vopixels = vo->get_pixels();
	guint8 *pbpixels = rendered->get_pixels();
	guint8 *bgpixels = background->get_pixels();

	// Reset top and left borders to background
	pixaddr = multx*xmargin + multy*(ymargin-1);
	for (x = 0; x<VIDEO_OUTPUT_WIDTH; x++) {
		pbpixels[pixaddr]   = bgpixels[pixaddr];
		pbpixels[pixaddr+1] = bgpixels[pixaddr+1];
		pbpixels[pixaddr+2] = bgpixels[pixaddr+2];
		pixaddr+=multx;
	}
	pixaddr = multx*(xmargin-1) + multy*(ymargin);
	for (y = 0; y<VIDEO_OUTPUT_HEIGHT; y++) {
		pbpixels[pixaddr]   = bgpixels[pixaddr];
		pbpixels[pixaddr+1] = bgpixels[pixaddr+1];
		pbpixels[pixaddr+2] = bgpixels[pixaddr+2];
		pixaddr+=multy;
		pbpixels[pixaddr]   = bgpixels[pixaddr];
		pbpixels[pixaddr+1] = bgpixels[pixaddr+1];
		pbpixels[pixaddr+2] = bgpixels[pixaddr+2];
		pixaddr+=multy;
	}

	// Draw screen
	for (y = 0; y<VIDEO_OUTPUT_HEIGHT; y++) {
		basey = VIDEO_OUTPUT_WIDTH*y;
		pixaddr = multx*xmargin + multy*(y*2+ymargin);

		for (x = 0; x<VIDEO_OUTPUT_WIDTH; x++) {
			get_color(vopixels[x+basey], r, g, b);

			// Center pixel
			pbpixels[pixaddr  ] = ADD_COL(bgpixels[pixaddr  ],r);
			pbpixels[pixaddr+1] = ADD_COL(bgpixels[pixaddr+1],g);
			pbpixels[pixaddr+2] = ADD_COL(bgpixels[pixaddr+2],b);

			// Fade color
			r=r>>2; g=g>>2; b=b>>2;

			// Top pixel (mix with existing)
			pbpixels[pixaddr-multy  ] = ADD_COL(pbpixels[pixaddr-multy  ],r);
			pbpixels[pixaddr-multy+1] = ADD_COL(pbpixels[pixaddr-multy+1],g);
			pbpixels[pixaddr-multy+2] = ADD_COL(pbpixels[pixaddr-multy+2],b);

			// Left pixel (mix with existing)
			pbpixels[pixaddr-multx  ] = ADD_COL(pbpixels[pixaddr-multx  ],r);
			pbpixels[pixaddr-multx+1] = ADD_COL(pbpixels[pixaddr-multx+1],g);
			pbpixels[pixaddr-multx+2] = ADD_COL(pbpixels[pixaddr-multx+2],b);

			// Bottom pixel (mix with background)
			pbpixels[pixaddr+multy  ] = ADD_COL(bgpixels[pixaddr+multy  ],r);
			pbpixels[pixaddr+multy+1] = ADD_COL(bgpixels[pixaddr+multy+1],g);
			pbpixels[pixaddr+multy+2] = ADD_COL(bgpixels[pixaddr+multy+2],b);

			// Right pixel (mix with background)
			pbpixels[pixaddr+multx  ] = ADD_COL(bgpixels[pixaddr+multx  ],r);
			pbpixels[pixaddr+multx+1] = ADD_COL(bgpixels[pixaddr+multx+1],g);
			pbpixels[pixaddr+multx+2] = ADD_COL(bgpixels[pixaddr+multx+2],b);

			pixaddr += multx; // Next pixel please !
		}
	}
	// All the screen is dirty (almost... could be improved by creating a rectangle for the display)
	if (get_window())
		get_window()->invalidate_region(get_window()->get_visible_region());
}
