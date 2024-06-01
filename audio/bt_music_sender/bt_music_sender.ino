/*
  Streaming of sound data with Bluetooth to other Bluetooth device.
  We generate 2 tones which will be sent to the 2 channels.
  
  Copyright (C) 2020 Phil Schatzmann
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <math.h> 

#include "BluetoothA2DPSource.h"
#include "Queue.hpp"

//#define c3_frequency  130.81

BluetoothA2DPSource a2dp_source;

// The supported audio codec in ESP32 A2DP is SBC. SBC audio stream is encoded
// from PCM data normally formatted as 44.1kHz sampling rate, two-channel 16-bit sample data
/*
int32_t get_data_frames(Frame *frame, int32_t frame_count) {
	static float m_time = 0.0;
	float m_amplitude = 10000.0; // -32,768 to 32,767
	float m_deltaTime = 1.0 / 44100.0;
	float m_phase = 0.0;
	float pi_2 = PI * 2.0;
	// fill the channel data
	for (int sample = 0; sample < frame_count; ++sample) {
		float angle = pi_2 * c3_frequency * m_time + m_phase;
		frame[sample].channel2 = frame[sample].channel1;
		m_time += m_deltaTime;
	}
	// to prevent watchdog
	delay(1);

	return frame_count;
}
*/

//const int MAX_SIZE = 128;

Queue audioBuffer;

const int MIN_BUFFER = 128; // Minimum buffer samples
const int MAX_WAIT = 500; // Maximum wait time for buffering in milliseconds
const int BUFFER_SIZE = 256;
//const int BUFFER_OVERFLOW = 512; // Wrap-around size for buffer

bool isLowOrder;

uint8_t lowOrderByte;
uint8_t highOrderByte;

//int audioBuffer[BUFFER_SIZE];

int bufferCount = 0;
int stackPointer;

int32_t get_data_frames(Frame *frame, int32_t frame_count) {
	int sample;
	
	sample = 0;
	
	while (sample < frame_count) {
		readSerial();
		if (bufferCount > MIN_BUFFER) {
			sample = popBuffer(frame, sample);
		}
	}

	// to prevent watchdog
	delay(1);

	return frame_count;
}

int popBuffer(Frame *frame, int sample) {
	frame[sample].channel2 = frame[sample].channel1 = audioBuffer.dequeue();
	--bufferCount;
	
	return sample + 1;
}

void readSerial() {  
	if (Serial.available()) {
	int8_t temporary;

		temporary = Serial.read();
		if (isLowOrder) {
			lowOrderByte = temporary;
			isLowOrder = false;
		} else {
			highOrderByte = temporary;
			if (bufferCount >= BUFFER_SIZE) { // If buffer is full, drop old data
				audioBuffer.dequeue();
				--bufferCount;			
			}
			audioBuffer.enqueue((((uint16_t) highOrderByte) << 8) | (uint16_t) lowOrderByte);
			++bufferCount;
			isLowOrder = true;
		}
	}
}

void setup() {
	a2dp_source.set_auto_reconnect(false);
	a2dp_source.start("OontZ Angle 3 DS E0E", get_data_frames);
	a2dp_source.set_volume(30);
}

void loop() {
	//// To prevent watchdog in release > 1.0.6
	//delay(1000);
	readSerial();
	delay(100);
}
