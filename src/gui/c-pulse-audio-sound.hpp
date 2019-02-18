/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * apple
 * Copyright (C)  2009 <>
 * 
 * apple is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * apple is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _C_PULSE_AUDIO_SOUND_HPP_
#define _C_PULSE_AUDIO_SOUND_HPP_
#include <pulse/simple.h>

#include "c-sound.hpp"

class CPulseAudioSound : public CSound
{
public:
	CPulseAudioSound(CSpeaker *speaker);
	void send_sound();
	void send_dummy(int samples);
protected:
	pa_simple *pulse;
	int lastpos;
};

#endif // _C_PULSE_AUDIO_SOUND_HPP_
