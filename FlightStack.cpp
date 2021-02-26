#include "FlightStack.h"

FlightStack::FlightStack()
{
	topPtr = NULL;
}

FlightStack::~FlightStack()
{
	while (!isEmpty()) { pop(); }
}

FlightStack::FlightStack(const FlightStack& R)
{
	if (R.topPtr == NULL)
	{
		this->topPtr = NULL;
	}
	else
	{
		this->topPtr = new StackNode;
		this->topPtr->item = R.topPtr->item;
		StackNode* newPtr = this->topPtr;
		for (StackNode* origPtr = R.topPtr->next; origPtr != NULL; origPtr = origPtr->next)
		{
			newPtr->next = new StackNode;
			newPtr = newPtr->next;
			newPtr->item = origPtr->item;
		}
		newPtr->next = NULL;
	}
}

FlightStack& FlightStack::operator=(const FlightStack& R)
{
	if (R.topPtr == NULL)
	{
		topPtr = NULL;
	}
	else
	{
		topPtr = new StackNode;
		topPtr->item = R.topPtr->item;
		StackNode* newPtr = topPtr;
		for (StackNode* origPtr = R.topPtr->next; origPtr != NULL; origPtr = origPtr->next)
		{
			newPtr->next = new StackNode;
			newPtr = newPtr->next;
			newPtr->item = origPtr->item;
		}
		newPtr->next = NULL;
	}
	return *this;
}

bool FlightStack::getTop(std::string& stackTop) const
{
	if (isEmpty())
	{
		return false;
	}
	else
	{
		stackTop = topPtr->item;
		return true;
	}
}

bool FlightStack::isEmpty() const
{
	return topPtr == NULL;
}

bool FlightStack::pop()
{
	if (isEmpty())
	{
		return false;
	}
	else
	{
		StackNode* temp = topPtr;
		topPtr = topPtr->next;

		temp->next = NULL;
		delete temp;
		return true;
	}
}

bool FlightStack::pop(std::string& stackTop)
{
	if (isEmpty())
	{
		return false;
	}
	else
	{
		stackTop = topPtr->item;
		StackNode* temp = topPtr;
		topPtr = topPtr->next;

		temp->next = NULL;
		delete temp;
		return true;
	}
}

bool FlightStack::push(const std::string& newItem)
{
	StackNode* ptr = new StackNode;
	ptr->item = newItem;
	ptr->next = topPtr;
	topPtr = ptr;
	return true;
}
