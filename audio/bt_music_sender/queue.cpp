#include "queue.hpp"

void Queue::enqueue(int x) {
	// Move all elements from stack1 to stack2
	while (!stack1.isEmpty()) {
		stack2.push(stack1.top());
		stack1.pop();
	}

	// Push item into stack1
	stack1.push(x);

	// Push everything back to stack1
	while (!stack2.isEmpty()) {
		stack1.push(stack2.top());
		stack2.pop();
	}
}

// Dequeue an item from the queue
int Queue::dequeue() {
	// If first stack is empty
	if (stack1.isEmpty()) {
		//Serial.println("Queue is empty.");
		return -1;
	}

	// Return the top of stack1
	int x = stack1.top();
	stack1.pop();
	return x;
}
