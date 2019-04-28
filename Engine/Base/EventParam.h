#pragma once
#include <Engine/DataStructures/DynamicArray.h>
#include "Delegate.h"

template <class D>
class EventParam
{
private:
	// Stores all subscribed functions
	// Must be allocated before adding
	// Will be deleted on event destruction
	DynamicArray<IDelegateParam<D>*> subscribers;

public:
	// Empty constructor
	EventParam();
	// Default destructor
	~EventParam();

	// Allocate memory for arrays
	void Init();

	// Create delegate for function in object and subscribe it to this event
	template <class T>
	void Subscribe(T *object, void (T::*objectFunction)(const D *data));

	// Calls all subscribers
	void operator()(const D *data) const;

	// Get all subscribers in this event
	const DynamicArray<IDelegateParam<D>*> &GetSubscribers() const;

	// Clear subscribers
	void Clear();
	// Deallocate
	void Delete();
};

template<class D>
inline EventParam<D>::EventParam()
{ }

template<class D>
inline EventParam<D>::~EventParam()
{
	Delete();
}

template<class D>
inline void EventParam<D>::Init()
{
	subscribers.Init(8);
}

template<class D>
inline void EventParam<D>::operator()(const D * data) const
{
	int count = subscribers.GetSize();
	for (int i = 0; i < count; i++)
	{
		subscribers[i]->Invoke(data);
	}
}

template<class D>
inline const DynamicArray<IDelegateParam<D>*>& EventParam<D>::GetSubscribers() const
{
	return subscribers;
}

template<class D>
inline void EventParam<D>::Clear()
{
	for (int i = 0; i < subscribers.GetSize(); i++)
	{
		delete subscribers[i];
	}

	subscribers.Clear();
}

template<class D>
inline void EventParam<D>::Delete()
{
	// clear data
	Clear();
	// delete array
	subscribers.Delete();
}

template<class D>
template<class T>
inline void EventParam<D>::Subscribe(T * object, void(T::* objectFunction)(const D *data))
{
	IDelegateParam<D> *newDelegate = new DelegateParam<T, D>(object, objectFunction);
	subscribers.Push(newDelegate);
}
