#pragma once
#include <Engine/DataStructures/DynamicArray.h>

// Simple event function, only int can be passed
typedef void (*EventFunction)(int);

class IEventHandler
{
public:
	virtual void Function(int a) = 0;
};


template <class T>
class Delegate
{
public:
	T Function;
};

typedef void(*FunctionVoid)();
typedef Delegate<FunctionVoid> DelegateVoid;


// TODO: template, delegates
class Event
{
private:
	// Stores all subscribed functions
	DynamicArray<IEventHandler*> subscribers;

public:
	// Empty constructor
	Event();
	// Default destructor
	~Event();

	// Allocate memory for arrays
	void Init();

	// Subscribe to this event.
	// When event is called then this function will be called too
	void operator+=(IEventHandler *function);

	// Calls all subscribers
	void operator()(int data) const;

	// Clear subscribers
	void Clear();
	// Deallocate
	void Delete();
};

inline Event::Event() { }

inline Event::~Event()
{
	Delete();
}

inline void Event::Init()
{
	subscribers.Init(8);
}

inline void Event::operator+=(IEventHandler *function)
{
	subscribers.Push(function);
}

inline void Event::operator()(int data) const
{
	int count = subscribers.GetSize();
	for (int i = 0; i < count; i++)
	{
		subscribers[i]->Function(data);
	}
}

inline void Event::Clear()
{
	subscribers.Clear();
}

inline void Event::Delete()
{
	subscribers.Delete();
}