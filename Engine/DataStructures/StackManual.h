#pragma once

// Linked list with manual memory allocation
template <class T>
class StackManual
{
public:
	struct SMNode
	{
		T data;
		SMNode *next;
	};

private:
	SMNode *head;
	UINT size;

public:
	StackManual();

	void Push(SMNode *newNode);
	SMNode *Pop();

	UINT GetSize() const;
};

template<class T>
inline StackManual<T>::StackManual()
{
	head = nullptr;
	size = 0;
}

template<class T>
inline void StackManual<T>::Push(SMNode * newNode)
{
	newNode->next = head;
	head = newNode;
}

template<class T>
typename StackManual<T>::SMNode * StackManual<T>::Pop()
{
	SMNode *toPop = head;
	head = head->next;

	return toPop;
}

template<class T>
inline UINT StackManual<T>::GetSize() const
{
	return size;
}
