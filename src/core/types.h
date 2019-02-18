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


#ifndef	_TYPES_H_
#define	_TYPES_H_

typedef	unsigned char	BYTE;
typedef	unsigned short	WORD;

inline BYTE dummy_byte() { return 0xFF; }

// Put state in b7
inline BYTE bool_to_b7(BYTE byte, bool state) {	return (state)?byte|0x80:byte&0x7F; }
inline BYTE bool_to_b7(bool state) { return bool_to_b7(dummy_byte(), state); }

#endif // _TYPES_H_