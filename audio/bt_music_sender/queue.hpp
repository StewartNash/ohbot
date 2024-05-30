#include "stack.hpp"

class Queue {
	private:
		Stack stack1;
		Stack stack2;
	public:
		void enqueue(int x);
		int dequeue();
};
