/*****************************************************************************
 * Copyright (C) 1994-2007 YAE
 * $Id$
 *
 * Author: Doug Kwan <chun_tak@yahoo.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************
 *
 * gcr.h
 * GCR encoding/decoding utility
 *
 */

/*
 * raw track
 * this is obtained by
 * 14.31818MHz / 14 / 32 / 8
 *
 */
#define RAW_TRACK_BYTES 6392 
#include "../core/types.h"

void SectorsToNibbles( BYTE *sectors, BYTE *nibbles, int volume, int track );
int NibblesToSectors( BYTE *nibbles, BYTE *sectors, int volume, int track );
