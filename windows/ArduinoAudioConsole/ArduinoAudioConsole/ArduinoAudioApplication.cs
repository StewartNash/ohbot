#define DEBUG_VERSION

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArduinoAudioConsole {
	internal class ArduinoAudioApplication {
		static System.IO.Ports.SerialPort mySerialPort;
		static PseudoSerialAudio.PseudoSerialPort myPseudoSerialPort;

		static void Main(string[] args) {
			ArduinoAudio myAudio;

#if DEBUG_VERSION
			myPseudoSerialPort = new PseudoSerialAudio.PseudoSerialPort();

			myPseudoSerialPort.PortName = "COM3";
			myPseudoSerialPort.BaudRate = 9600;
			myPseudoSerialPort.Open();

			mySerialPort = null;

			//string filename = "F:\\Repositories\\temporary\\Joshua Colley singing Go The Distance [ ezmp3.cc ].mp3";
			string filename = "F:\\Repositories\\temporary\\JUST DO IT meme [ ezmp3.cc ].mp3";

			myAudio = new ArduinoAudio(myPseudoSerialPort);
			//myAudio.PlayFile(filename);
			myAudio.PlayStream(filename);

			Console.WriteLine("Press any key to continue...");
			Console.ReadKey();
#else
			mySerialPort = new System.IO.Ports.SerialPort();
			mySerialPort.PortName = "COM3";
			mySerialPort.BaudRate = 9600;
			mySerialPort.Open();

			myPseudoSerialPort = null;

			//string filename = "F:\\Repositories\\temporary\\Joshua Colley singing Go The Distance [ ezmp3.cc ].mp3";
			string filename = "F:\\Repositories\\temporary\\JUST DO IT meme [ ezmp3.cc ].mp3";

			myAudio = new ArduinoAudio(mySerialPort);
			//myAudio.PlayFile(filename);
			myAudio.PlayStream(filename);

			Console.WriteLine("Press any key to continue...");
			Console.ReadKey();
#endif
		}
	}
}
