using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PseudoSerialAudio {
	struct Frame {
		public short channel1;
		public short channel2;
	}

	public static class Serial {
		public static byte read() {
			Random rnd = new Random();
			Byte[] b = new Byte[1];
			rnd.NextBytes(b);
			return b[0];
		}

		public static bool available() {
			return true;
		}
	}

	public class DebugSerialAudio {
		Queue audioBuffer;

		const int MIN_BUFFER = 128; // Minimum buffer samples
		const int MAX_WAIT = 500; // Maximum wait time for buffering in milliseconds
		const int BUFFER_SIZE = 256;
		//const int BUFFER_OVERFLOW = 512; // Wrap-around size for buffer

		bool isLowOrder;

		byte lowOrderByte;
		byte highOrderByte;

		//int audioBuffer[BUFFER_SIZE];

		int bufferCount = 0;
		int stackPointer;

		int get_data_frames(Frame[] frame, int frame_count) {
			short sample;

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

		short popBuffer(Frame[] frame, short sample) {
			frame[sample].channel2 = frame[sample].channel1 = (short)audioBuffer.dequeue();
			--bufferCount;

			return (short)(sample + 1);
		}


		void readSerial() {
			if (Serial.available()) {
				byte temporary;

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
					audioBuffer.enqueue((((int)highOrderByte) << 8) | (int)lowOrderByte);
					++bufferCount;
					isLowOrder = true;
				}
			}
		}

		void delay(int delay) {

		}

	}

	public class Stack {
		const int MAX_SIZE = 5;

		int[] arr;
		int topElement;

		public Stack() {
			topElement = -1;
			arr = new int[MAX_SIZE];
		}

		public bool isEmpty() {
			return (topElement == -1);
		}

		public bool isFull() {
			return (topElement == MAX_SIZE - 1);
		}

		public void push(int element) {
			if (!isFull()) {
				topElement++;
				arr[topElement] = element;
			}
		}

		public void pop() {
			if (!isEmpty()) {
				--topElement;
			} else {

			}
		}

		public int top() {
			if (!isEmpty()) {
				return arr[topElement];
			} else {
				return -1;
			}
		}
		
	} 

	public class Queue {
		private Stack stack1;
		private Stack stack2;

		public void enqueue(int x) {
			while (!stack1.isEmpty()) {
				stack2.push(stack1.top());
				stack1.pop();
			}

			stack1.push(x);

			while (!stack2.isEmpty()) {
				stack1.push(stack2.top());
				stack2.pop();
			}
		}

		public int dequeue() {
			if (stack1.isEmpty()) {
				return -1;
			}

			int x = stack1.top();
			stack1.pop();
			return x;
		}
	}
}
