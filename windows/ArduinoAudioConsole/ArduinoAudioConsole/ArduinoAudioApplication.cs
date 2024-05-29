using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArduinoAudioConsole {
	internal class ArduinoAudioApplication {
		static void Main(string[] args) {
			ArduinoAudio myAudio;

			//string filename = "F:\\Repositories\\temporary\\Joshua Colley singing Go The Distance [ ezmp3.cc ].mp3";
			string filename = "F:\\Repositories\\temporary\\JUST DO IT meme [ ezmp3.cc ].mp3";

			myAudio = new ArduinoAudio();
			myAudio.PlayFile(filename);

			Console.WriteLine("Press any key to continue...");
			Console.ReadKey();
		}
	}
}
