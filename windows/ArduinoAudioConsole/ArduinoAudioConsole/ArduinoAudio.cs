using System;
using System.IO;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;

namespace ArduinoAudioConsole {
	internal class ArduinoAudio {
		static SerialPort mySerialPort;

		public ArduinoAudio(SerialPort serialPort) {
			mySerialPort = serialPort;
		}
		
		public void PlayFile(string filename) {
			NAudio.Wave.Mp3FileReader reader;
			NAudio.Wave.WaveStream pcm;
			NAudio.Wave.BlockAlignReductionStream stream;
			NAudio.Wave.DirectSoundOut output;

			using (reader = new NAudio.Wave.Mp3FileReader(filename)) {
				using (pcm = NAudio.Wave.WaveFormatConversionStream.CreatePcmStream(reader)) {
					//NAudio.Wave.WaveOut waveOut = new NAudio.Wave.WaveOut();
					using (stream = new NAudio.Wave.BlockAlignReductionStream(pcm)) {
						using (output = new NAudio.Wave.DirectSoundOut()) {
							output.Init(stream);
							output.Play();
							while (output.PlaybackState == NAudio.Wave.PlaybackState.Playing) {
								Thread.Sleep(1000);
							}
						}
					}
				}
			}
		}

		public void PlayStream(string filename) {
			NAudio.Wave.Mp3FileReader reader;
			NAudio.Wave.WaveStream pcm;
			//MemoryStream stream;

			//using (stream = new MemoryStream()) {
			//	using (reader = new NAudio.Wave.Mp3FileReader(filename)) {
			//		using (pcm = NAudio.Wave.WaveFormatConversionStream.CreatePcmStream(reader)) {
			//			byte[] output;

			//			output = new byte[pcm.Length];
			//			pcm.Position = 0;
			//			pcm.Read(output, 0, output.Length);
			//			stream.Write(output, 0, output.Length);
			//			mySerialPort.Write(output, 0, output.Length);
			//		}
			//	}
			//}

			using (reader = new NAudio.Wave.Mp3FileReader(filename)) {
				using (pcm = NAudio.Wave.WaveFormatConversionStream.CreatePcmStream(reader)) {
					byte[] output;

					output = new byte[pcm.Length];
					pcm.Position = 0;
					pcm.Read(output, 0, output.Length);
					mySerialPort.Write(output, 0, output.Length);
				}
			}
		}

	}

}
