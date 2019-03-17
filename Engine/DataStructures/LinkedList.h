#pragma once

#include <memory>

#define FOREACHLINKEDLIST(baseclass, member, list) \
for (baseclass *member = list.PointToHead(); member != nullptr; member = list.GetNext())


template <class T>
class LinkedList
{
private:
	struct LinkedListNode
	{
		T data;
		LinkedListNode *next;

		LinkedListNode(T data, LinkedListNode *next)
		{
			this->data = data;
			this->next = next;
		}
	};

	std::shared_ptr<LinkedListNode> head;
	LinkedListNode *last;
	int size;

	// For iterator
	mutable LinkedListNode *pointer;
public:
	inline LinkedList();
	inline ~LinkedList();

	// Copies pointers
	// Note: doesn't copy elements from source
	inline void operator=(const LinkedList<T> &source);
	inline T &operator[](unsigned index);
	inline const T &operator[](unsigned index) const;

	// Allocates memory for node, adds it to list
	inline void Add(T data);
	inline void Remove(const T &data);
	inline bool Find(const T &data);

	inline int GetSize() const;
	inline void Delete();

	// Reset iterator
	inline T *PointToHead() const;
	// Iterate over list
	inline T *GetNext() const;
	inline T *GetHead() const;
	inline T *GetTail() const;
};

template<class T>
inline LinkedList<T>::LinkedList() :
	last(nullptr),
	size(0)
{ }

template<class T>
inline LinkedList<T>::~LinkedList()
{
	Delete();
}

template<class T>
inline void LinkedList<T>::operator=(const LinkedList<T> &source)
{
	// copy shared ptr
	this->head = source.head;
	
	// copy other data
	this->last = source.last;
	this->pointer = source.pointer;
	this->size = source.size;
}

template<class T>
inline T &LinkedList<T>::operator[](unsigned index)
{
	ASSERT(index >= size);

	LinkedListNode *p = head.get();
	for (unsigned i = 0; i < index; i++)
	{	
		p = p->next;
	}

	return p->data;
}

template<class T>
inline const T &LinkedList<T>::operator[](unsigned index) const
{
	ASSERT(index >= size);

	LinkedListNode *p = head.get();
	for (unsigned i = 0; i < index; i++)
	{
		p = p->next;
	}

	return p->data;
}

template<class T>
inline void LinkedList<T>::Remove(const T &data)
{
	if (size == 0)
	{
		return;
	}

	// check head
	if (head->data == data)
	{
		LinkedListNode *newHead = head->next;

		// delete head if possible
		// and reassign
		head.reset(newHead);

        size--;
        return;
	}

	// not head
	LinkedListNode *p = head.get();
	while (p->next != last)
	{
		if (p->next->data == data)
		{
			// store pointer
			LinkedListNode *nextNext = p->next->next;

            // delete pointer to next
			delete p->next;

            // reasign pointer to next
			p->next = nextNext;

			size--;
			return;
		}

		p = p->next;
	}

	// check last
	if (last->data == data)
	{
		delete last;

		// "p" is previous of "last"
		last = p;
		last->next = nullptr;

		size--;
		return;
	}
}

template<class T>
inline bool LinkedList<T>::Find(const T &data)
{
	LinkedListNode *p = head.get();

	while (p != nullptr)
	{
		if (p->data == data)
		{
			return true;
		}

		p = p->next;
	}

	return false;
}

template<class T>
inline void LinkedList<T>::Add(T data)
{
	LinkedListNode *newNode = new LinkedListNode(data, nullptr);

	if (last != nullptr)
	{
		last->next = newNode;
		last = newNode;
	}
	else
	{
		head.reset(newNode);
		last = newNode;
	}

	size++;
}

template<class T>
inline int LinkedList<T>::GetSize() const
{
	return size;
}

template<class T>
inline void LinkedList<T>::Delete()
{
	// delete all nodes
	LinkedListNode *p = head;

	while (p != nullptr)
	{
		LinkedListNode *toRemove = p;
		p = p->next;
		delete toRemove;
	}

	// to defaults
	head.reset();
	last = nullptr;
	size = 0;
}

template<class T>
inline T *LinkedList<T>::GetNext() const
{
	LinkedListNode *next = pointer->next;

	if (next != nullptr)
	{
		pointer = next;
		return &(next->data);
	}
	else
	{
		return nullptr;
	}
}

template<class T>
inline T * LinkedList<T>::GetHead() const
{
	return &(head->data);
}

template<class T>
inline T * LinkedList<T>::GetTail() const
{
	return &(last->data);
}

template<class T>
inline T * LinkedList<T>::PointToHead() const
{
	pointer = head.get();
	return &(head->data);
}
