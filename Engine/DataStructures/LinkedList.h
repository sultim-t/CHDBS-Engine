#pragma once

#define FOREACHLINKEDLIST(baseclass, member, list) \
for (baseclass *member = list.PointToHead(); member != NULL; member = list.GetNext())


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

	LinkedListNode *head;
	LinkedListNode *last;
	int size;

	// For iterator
	mutable LinkedListNode *pointer;
public:
	LinkedList();
	~LinkedList();

	T &operator[](unsigned index);
	const T &operator[](unsigned index) const;

	// Allocates memory for node, adds it to list
	void Add(T data);
	void Remove(const T &data);
	bool Find(const T &data);

	int GetSize() const;
	void Clear();

	// Reset iterator
	T *PointToHead() const;
	// Iterate over list
	T *GetNext() const;
	T *GetHead() const;
	T *GetTail() const;
};

template<class T>
inline LinkedList<T>::LinkedList()
{
	head = last = NULL;
	size = 0;
}

template<class T>
inline LinkedList<T>::~LinkedList()
{
	Clear();
}

template<class T>
inline T &LinkedList<T>::operator[](unsigned index)
{
	ASSERT(index >= size);

	LinkedListNode *p = head;
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

	LinkedListNode *p = head;
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
		delete head;

		head = newHead;

        size--;
        return;
	}

	// not head
	LinkedListNode *p = head;
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
		last->next = NULL;

		size--;
		return;
	}
}

template<class T>
inline bool LinkedList<T>::Find(const T &data)
{
	LinkedListNode *p = head;

	while (p != NULL)
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
	LinkedListNode *newNode = new LinkedListNode(data, NULL);

	if (last != NULL)
	{
		last->next = newNode;
		last = newNode;
	}
	else
	{
		head = last = newNode;
	}

	size++;
}

template<class T>
inline int LinkedList<T>::GetSize() const
{
	return size;
}

template<class T>
inline void LinkedList<T>::Clear()
{
	// delete all nodes
	LinkedListNode *p = head;

	while (p != NULL)
	{
		LinkedListNode *toRemove = p;
		p = p->next;
		delete toRemove;
	}

	head = last = NULL;
	size = 0;
}

template<class T>
inline T *LinkedList<T>::GetNext() const
{
	LinkedListNode *next = pointer->next;

	if (next != NULL)
	{
		pointer = next;
		return &(next->data);
	}
	else
	{
		return NULL;
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
	pointer = head;
	return &(head->data);
}
