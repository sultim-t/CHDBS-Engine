#pragma once
#include <Engine/DataStructures/DynamicArray.h>
#include "Delegate.h"

class Event
{
private:
	// Stores all subscribed functions
	// Must be allocated before adding
	// Will be deleted on event destruction
	DynamicArray<IDelegate*> subscribers;

public:
	// Empty constructor
	Event();
	// Default destructor
	~Event();

	// Allocate memory for arrays
	void Init();

	// Create delegate for static function and subscribe it to this event
	void operator+=(void(*staticFunction)());

	// Create delegate for static function and subscribe it to this event
	void Subscribe(void(*staticFunction)());

	// Create delegate for function in object and subscribe it to this event
	template <class T>
	void Subscribe(T *object, void (T::*objectFunction)());

	// Calls all subscribers
	void operator()() const;

	// Get all subscribers in this event
	const DynamicArray<IDelegate*> &GetSubscribers() const;


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

inline void Event::operator()() const
{
	int count = subscribers.GetSize();
	for (int i = 0; i < count; i++)
	{
		subscribers[i]->Invoke();
	}
}

inline const DynamicArray<IDelegate*>& Event::GetSubscribers() const
{
	return subscribers;
}

inline void Event::Clear()
{
	for (int i = 0; i < subscribers.GetSize(); i++)
	{
		delete subscribers[i];
	}

	subscribers.Clear();
}

inline void Event::Delete()
{
	// clear data
	Clear();
	// delete array
	subscribers.Delete();
}

inline void Event::operator+=(void(*staticFunction)())
{
	Subscribe(staticFunction);
}

inline void Event::Subscribe(void(*staticFunction)())
{
	IDelegate *newDelegate = new DelegateStatic(staticFunction);
	subscribers.Push(newDelegate);
}

template<class T>
inline void Event::Subscribe(T * object, void(T::* objectFunction)())
{
	IDelegate *newDelegate = new Delegate<T>(object, objectFunction);
	subscribers.Push(newDelegate);
}
