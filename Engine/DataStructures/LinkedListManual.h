//#pragma once
//
//// Linked list with manual memory allocation
//template <class T>
//class LinkedListManual
//{
//public:
//	struct LLMNode
//	{
//		T data;
//		LLMNode *next;
//	};
//
//private:
//	LLMNode *head;
//	UINT size;
//
//public:
//	LinkedList();
//
//	// Adds to end
//	void Add(LLMNode *newNode);
//	// Adds after "prev"
//	void Insert(LLMNode *prev, LLMNode *node);
//	void Remove(LLMNode *prev, LLMNode *node);
//
//	UINT GetSize() const;
//};
//
//template<class T>
//inline LinkedListManual<T>::LinkedList()
//{
//	head = nullptr;
//	size = 0;
//}
//
//template<class T>
//inline void LinkedListManual<T>::Add(LLMNode * newNode)
//{
//	if (head != nullptr)
//	{
//		node->next = head;
//	}
//	else
//	{
//		node->next = nullptr;
//	}
//
//	head = node;
//	size++;
//}
//
//template<class T>
//inline void LinkedListManual<T>::Insert(LLMNode * prev, LLMNode * node)
//{
//	if (prev == nullptr)
//	{
//		Add(node);
//	}
//	else
//	{
//		if (prev->next == nullptr)
//		{
//			prev->next = node;
//			node->next = nullptr;
//		}
//		else
//		{
//			node->next = prev->next;
//			prev->next = node;
//		}
//
//		size++;
//	}
//}
//
//template<class T>
//inline void LinkedListManual<T>::Remove(LLMNode * prev, LLMNode * node)
//{
//	if (prev == nullptr)
//	{
//		if (node->next == nullptr)
//		{
//			head = nullptr;
//		}
//		else
//		{
//			head = node->next;
//		}
//	}
//	else
//	{
//		prev->next = node->next;
//	}
//
//	size--;
//}
//
//template<class T>
//inline UINT LinkedListManual<T>::GetSize() const
//{
//	return size;
//}
