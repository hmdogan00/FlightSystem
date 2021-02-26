#pragma once
#include <string>

class FlightStack
{
public:
	FlightStack();
	FlightStack(const FlightStack& R);
	~FlightStack();
	FlightStack& operator= (const FlightStack& R);
	bool isEmpty() const;
	bool push(const std::string& newItem);
	bool pop();
	bool pop(std::string& stackTop);
	bool getTop(std::string& stackTop) const;

private:
	struct StackNode
	{
		std::string item;
		StackNode* next;
	};

	StackNode* topPtr;
};

