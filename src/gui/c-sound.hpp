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

#ifndef _C_SOUND_HPP_
#define _C_SOUND_HPP_

#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "../hardware/c-speaker.hpp"

// Sound launches a specific thread that reads data from CSpeaker and sends them
// to the sound device.

class CSound
{
public:
	CSound(CSpeaker *speaker);
	void start();
	void stop();
	void run();

	virtual void send_sound() = 0;
	static void *thread_manager(void* sound);

protected:
	CSpeaker *speaker;  
	pthread_t thread;
	bool running;
};

#endif // _C_SOUND_HPP_
