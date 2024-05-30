#include "stack.hpp"

Stack::Stack() {
	topElement = -1; // Initialize topElement to -1 to indicate an empty stack
}

bool Stack::isEmpty() {
	return (topElement == -1);
}

bool Stack::isFull() {
	return (topElement == MAX_SIZE - 1);
}

void Stack::push(int element) {
	if (!isFull()) {
		topElement++;
		arr[topElement] = element;
	} else {
		// Stack is full
		//Serial.println("Stack is full. Cannot push element.");
	}
}

void Stack::pop() {
	if (!isEmpty()) {
		return arr[topElement];
		topElement--;
	} else {
		// Stack is empty
		//Serial.println("Stack is empty. Cannot pop from empty stack.");
	}
}

int Stack::top() {
	if (!isEmpty()) {
		return arr[topElement];
	} else {
		//Serial.println("Stack is empty.");
		return -1; // In this example, we consider -1 as an invalid value.
	}
}
