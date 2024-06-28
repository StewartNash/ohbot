#include <SPI.h>

#include <SD.h>
#include <sd_defines.h>
#include <sd_diskio.h>

#include <AudioConfig.h>
#include <AudioLogger.h>
#include <AudioTools.h>

#include <A2DPVolumeControl.h>
#include <BluetoothA2DP.h>
#include <BluetoothA2DPCommon.h>
#include <BluetoothA2DPOutput.h>
#include <BluetoothA2DPSink.h>
#include <BluetoothA2DPSinkQueued.h>
#include <BluetoothA2DPSource.h>
#include <config.h>
#include <SoundData.h>

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
//#include "BluetoothA2DPSource.h"
#include <math.h> 

#define c3_frequency  130.81

BluetoothA2DPSource a2dp_source;

// The supported audio codec in ESP32 A2DP is SBC. SBC audio stream is encoded
// from PCM data normally formatted as 44.1kHz sampling rate, two-channel 16-bit sample data
int32_t get_data_frames(Frame *frame, int32_t frame_count) {
    static float m_time = 0.0;
    float m_amplitude = 10000.0;  // -32,768 to 32,767
    float m_deltaTime = 1.0 / 44100.0;
    float m_phase = 0.0;
    float pi_2 = PI * 2.0;
    // fill the channel data
    for (int sample = 0; sample < frame_count; ++sample) {
        float angle = pi_2 * c3_frequency * m_time + m_phase;
        frame[sample].channel1 = m_amplitude * sin(angle);
        frame[sample].channel2 = frame[sample].channel1;
        m_time += m_deltaTime;
    }
    // to prevent watchdog
    delay(1);

    return frame_count;
}


void setup() {
  a2dp_source.set_auto_reconnect(false);
  a2dp_source.start("LEXON MINO L", get_data_frames);  
  a2dp_source.set_volume(30);
}

void loop() {
  // to prevent watchdog in release > 1.0.6
  delay(1000);
}

/*
#include "BluetoothA2DPSource.h"
#include <SPI.h>
#include <SD.h>

BluetoothA2DPSource a2dp_source;
File sound_file;
const char* file_name = "/audio.raw";
const int sd_ss_pin = 5;
const int frame_size_bytes = sizeof(int16_t) * 2;

// callback used by A2DP to provide the sound data
int32_t get_sound_data(Channels* data, int32_t len) {
  // the data in the file must be in int16 with 2 channels 
  size_t result_len_bytes = sound_file.read((uint8_t*)data, len * frame_size_bytes );
  // result is in number of frames
  int32_t result_len = result_len_bytes / frame_size_bytes;
  ESP_LOGD("get_sound_data", "%d -> %d",len );
  return result_len;
}

// Arduino Setup
void setup(void) {
  Serial.begin(115200);

  // Setup SD and open file
  SD.begin(sd_ss_pin);
  sound_file = SD.open(file_name, FILE_READ);

  // start the bluetooth
  Serial.println("starting A2DP...");
  a2dp_source.start("MyMusic", get_sound_data);  
}

// Arduino loop - repeated processing 
void loop() {
}
*/
