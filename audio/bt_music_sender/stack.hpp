const int MAX_SIZE = 5; // Maximum size of the stack

class Stack {
	private:
		int arr[MAX_SIZE];
		int topElement;
	public:
		Stack(); 
		bool isEmpty(); 
		bool isFull();
		void push(int element);
		int top();
};
