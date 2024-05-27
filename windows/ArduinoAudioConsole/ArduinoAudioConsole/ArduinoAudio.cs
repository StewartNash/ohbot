using System;
using System.IO;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;

namespace ArduinoAudioConsole {
	internal class ArduinoAudio {
		public void PlayFile(string filename) {
			NAudio.Wave.Mp3FileReader reader = new NAudio.Wave.Mp3FileReader(filename);
			NAudio.Wave.WaveStream pcm = NAudio.Wave.WaveFormatConversionStream.CreatePcmStream(reader);
			//NAudio.Wave.WaveOut waveOut = new NAudio.Wave.WaveOut();
			NAudio.Wave.BlockAlignReductionStream stream = new NAudio.Wave.BlockAlignReductionStream(pcm);
			NAudio.Wave.DirectSoundOut output = new NAudio.Wave.DirectSoundOut();
			output.Init(stream);
			output.Play();
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
	}

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
}
