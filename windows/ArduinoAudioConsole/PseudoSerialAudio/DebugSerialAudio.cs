using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PseudoSerialAudio {

	public class DebugSerialAudio {

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
