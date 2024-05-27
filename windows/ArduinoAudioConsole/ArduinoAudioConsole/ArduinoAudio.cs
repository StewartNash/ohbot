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


		/*
		public void PlayFile(string filename) {
			NAudio.Wave.AudioFileReader audioFile;
			NAudio.Wave.WaveOutEvent output;

			using (audioFile = new NAudio.Wave.AudioFileReader(filename))
			using (output = new NAudio.Wave.WaveOutEvent()) {
				output.Init(audioFile);
				output.Play();
				while (output.PlaybackState == NAudio.Wave.PlaybackState.Playing) {
					Thread.Sleep(1000);
				}
			}
		}

		public void PlayStream(string filename) {
			Console.WriteLine("loading and parsing MP3 file...");
			MP3Sharp.MP3Stream stream = new MP3Sharp.MP3Stream(filename);
			NAudio.Wave.WaveFormat waveFormat = new NAudio.Wave.WaveFormat(stream.Frequency, stream.ChannelCount);
			Console.WriteLine("allocating playback cache...");
			FastWaveBuffer fastWaveBuffer = new FastWaveBuffer(waveFormat, (int)stream.Length);
			Console.WriteLine("populating playback cache...");
			stream.CopyTo(fastWaveBuffer);
			fastWaveBuffer.Seek(0, SeekOrigin.Begin);
			Console.WriteLine("unloading MP3 file...");
			stream.Dispose();
			Console.WriteLine("prepairing player...");
			NAudio.Wave.WaveOutEvent waveOutEvent = new NAudio.Wave.WaveOutEvent();
			waveOutEvent.Init(fastWaveBuffer);
			waveOutEvent.Volume = 1;
			Console.WriteLine("arming ManualResetEvent...");
			ManualResetEvent manualResetEvent = new ManualResetEvent(false);
			waveOutEvent.PlaybackStopped += (object sender, NAudio.Wave.StoppedEventArgs e) => {
				manualResetEvent.Set();
			};
			Console.WriteLine("done!");
			waveOutEvent.Play();
			manualResetEvent.WaitOne();
		}
		*/
	}

	/*
	public sealed class FastWaveBuffer : MemoryStream, NAudio.Wave.IWaveProvider {
		public FastWaveBuffer(NAudio.Wave.WaveFormat waveFormat, byte[] bytes) : base(bytes) {
			WaveFormat = waveFormat;
		}
		public FastWaveBuffer(NAudio.Wave.WaveFormat waveFormat, int size = 4096) : base() {
			WaveFormat = waveFormat;
			Capacity = size;
		}
		public NAudio.Wave.WaveFormat WaveFormat {
			get;
		}
	}
	*/

}
