using System;
using System.IO;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;

namespace ArduinoAudioConsole {
	internal class ArduinoAudio {
		
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
			NAudio.Wave.BlockAlignReductionStream stream;
			NAudio.Wave.DirectSoundOut output;

			using (reader = new NAudio.Wave.Mp3FileReader(filename)) {
				using (pcm = NAudio.Wave.WaveFormatConversionStream.CreatePcmStream(reader)) {
					////NAudio.Wave.WaveOut waveOut = new NAudio.Wave.WaveOut();
					//using (stream = new NAudio.Wave.BlockAlignReductionStream(pcm)) {
					//	using (output = new NAudio.Wave.DirectSoundOut()) {
					//		output.Init(stream);
					//		output.Play();
					//		while (output.PlaybackState == NAudio.Wave.PlaybackState.Playing) {
					//			Thread.Sleep(1000);
					//		}
					//	}
					//}

					
				}
			}
		}

	}

}
