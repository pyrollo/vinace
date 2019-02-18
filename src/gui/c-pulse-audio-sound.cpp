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


// http://www.4front-tech.com/pguide/audio.html

#include "c-pulse-audio-sound.hpp"
#include <iostream>
//#include <unistd.h>
//#include <fcntl.h>

#define SOUND_PERIOD_USEC 20000
// 20kHz sound frequency
#define SOUND_FREQUENCY 20000

// Delay starting sound of one period
#define SAMPLES_SHIFT (SOUND_FREQUENCY*SOUND_PERIOD_USEC/1000000)

CPulseAudioSound::CPulseAudioSound(CSpeaker *speaker):CSound(speaker) {

	pa_sample_spec ss;
	ss.format = PA_SAMPLE_U8;
	ss.channels = 1;
	ss.rate = SOUND_FREQUENCY;
	
	pulse = pa_simple_new(NULL,               // Use the default server.
		                  "Vinace",           // Our application's name.
		                  PA_STREAM_PLAYBACK, // PA_STREAM_UPLOAD
		                  NULL,               // Use the default device.
		                  "Apple speaker sound",            // Description of our stream.
		                  &ss,                // Our sample format.
		                  NULL,               // Use default channel map
		                  NULL,               // Use default buffering attributes.
		                  NULL                // Ignore error code.
		                 );

	if (pulse)
		printf("CPulseAudioSound : Sound ready!\n");
	else
		printf("CPulseAudioSound : Unable to connect to PulseAudio -> No Sound.\n");

	// Send some data to Pulse Audio to be sure not to run out of buffer later.
//	send_dummy(SAMPLES_SHIFT);

	lastpos = speaker->get_pos();
}

void CPulseAudioSound::send_dummy(int samples) {
	static int error;
	unsigned char *dummy = new unsigned char[samples];
	for (int index=0; index < samples; index++)
		dummy[index]=0;

	if (pulse)
		pa_simple_write(pulse, dummy, samples, &error);	
	delete dummy;
}

void CPulseAudioSound::send_sound() {
	static int error;
	int newpos = speaker->get_pos();
	if (pulse)
		if (newpos < lastpos) {
			pa_simple_write(pulse, speaker->get_buffer()+lastpos, speaker->get_max_pos()-lastpos, &error);		
			lastpos = 0;
		}
		if (newpos > lastpos) {
			pa_simple_write(pulse, speaker->get_buffer()+lastpos, newpos-lastpos, &error);
			lastpos = newpos;
		}
}
