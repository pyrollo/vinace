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

#ifndef _C_CHARSET_HPP_
#define _C_CHARSET_HPP_
#include "src/core/types.h"

enum char_mode {
	mode_normal,
	mode_inverse,
	mode_flash
};

class CCharset
{
public:
	virtual BYTE* get_char(BYTE code) = 0;
	virtual char_mode get_mode(BYTE code) = 0;
};

class C2ePrimaryCharset: public CCharset
{
public:
	BYTE* get_char(BYTE code);
	char_mode get_mode(BYTE code);
};

class C2eAlternativeCharset: public CCharset
{
public:
	BYTE* get_char(BYTE code);
	char_mode get_mode(BYTE code);
};

#endif // _C_CHARSET_HPP_
