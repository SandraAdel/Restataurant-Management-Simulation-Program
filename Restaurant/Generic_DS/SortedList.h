#pragma once
#ifndef __SortedList_H_
#define _SortedList_H_
#include "PriorityData.h"
#include "Node.h"

template <class T>

class SortedList
{
	Node<PriorityData<T>>* head;
	Node<PriorityData<T>>* GetNodeAt ( int position ) const;
	Node<PriorityData<T>>* GetPrevOf ( const T & anEntry ) const;

public:

	SortedList ();

	bool isEmpty () const;
	int getLength () const;
	bool insertSortedAscendingly ( const PriorityData<T> & newEntry );
	bool insertSortedDescendingly ( const PriorityData<T> & newEntry );
	bool remove (const T & anEntry);     
	void clear ();
	bool getEntry ( int position, PriorityData<T> & anEntry ) const;
	bool replace ( int position, const T & newEntry, T & oldEntry ); 
	T* toArray ( int & count );

	virtual ~SortedList ();
};

template <class T>
SortedList<T> :: SortedList ()
{
	head = nullptr;
}

template <class T>
Node<PriorityData<T>>* SortedList<T> :: GetNodeAt ( int position ) const
{
	if ( position < 0 || position >  getLength () -1  )
	{
		return nullptr;
	}

	Node<PriorityData<T>>* nptr = head;
	for ( int i=0; i<position; i++ )
	{
		nptr = nptr ->getNext();
	}

	return nptr;
}

template <class T>
Node<PriorityData<T>>* SortedList<T> :: GetPrevOf ( const T & anEntry ) const
{
	Node<PriorityData<T>>* nptr = head;
	if ( nptr == nullptr || nptr ->getNext () == nullptr )
	{
		return nullptr;
	}

	while ( nptr ->getNext() != nullptr && nptr ->getNext() ->getItem().getData() != anEntry )
	{
		nptr = nptr ->getNext();
	}

	if ( nptr ->getNext() == nullptr )
	{
		return nullptr;
	}
	else 
	{
		return nptr; 
	}
}

template <class T>
bool SortedList<T> :: insertSortedAscendingly ( const PriorityData<T> & newEntry )  
{
	Node<PriorityData<T>>* newNode = new Node<PriorityData<T>> ( newEntry );
	if ( newNode == nullptr )
	{
		return false;
	}

	Node<PriorityData<T>>* nptr = head;

	if ( nptr == nullptr )
	{
		head = newNode;
	}
	else if ( newEntry.getPriority() < nptr ->getItem().getPriority())
	{
		newNode ->setNext( head );
		head = newNode;
	}
	else 
	{
		while ( nptr ->getNext() != nullptr && nptr ->getNext() ->getItem().getPriority() <= newEntry.getPriority() )
		{
			nptr = nptr ->getNext();
		}

		newNode ->setNext ( nptr ->getNext() );
		nptr ->setNext ( newNode );
	}
	return true;
}

template <class T>
bool SortedList<T> :: insertSortedDescendingly ( const PriorityData<T> & newEntry )  
{
	Node<PriorityData<T>>* newNode = new Node<PriorityData<T>> ( newEntry );
	if ( newNode == nullptr )
	{
		return false;
	}

	Node<PriorityData<T>>* nptr = head;

	if ( nptr == nullptr )
	{
		head = newNode;
	}
	else if ( newEntry.getPriority() > nptr ->getItem().getPriority())
	{
		newNode ->setNext( head );
		head = newNode;
	}
	else 
	{
		while ( nptr ->getNext() != nullptr && nptr ->getNext() ->getItem().getPriority() >= newEntry.getPriority() )
		{
			nptr = nptr ->getNext();
		}

		newNode ->setNext ( nptr ->getNext() );
		nptr ->setNext ( newNode );
	}
	return true;
}

template <class T>
bool SortedList<T> :: remove ( const T & anEntry )
{
	if ( head == nullptr )
	{
		return false;
	}

	if ( head ->getItem().getData() == anEntry )
	{
		Node<PriorityData<T>>* nodeToDelete = head;
		head = head ->getNext();
		delete nodeToDelete;
		return true;
	}

	Node<PriorityData<T>>* prevNode = GetPrevOf ( anEntry );

	if ( prevNode == nullptr )
	{
		return false;
	}
	else
	{
		Node<PriorityData<T>>* nodeToDelete = prevNode ->getNext();
		prevNode ->setNext ( nodeToDelete ->getNext() );
		delete nodeToDelete;
		return true;
	}

}

template <class T>
bool SortedList<T> :: isEmpty () const
{
	return ( head == nullptr );
}

template <class T>
int SortedList<T> :: getLength () const
{
	int count = 0;
	Node<PriorityData<T>>* nptr = head;
	while ( nptr != nullptr )
	{
		count++;
		nptr = nptr ->getNext();
	}
	return count;
}

template <class T>
void SortedList<T> :: clear ()
{
	PriorityData<T> element;
	while ( !isEmpty() )
	{
		getEntry(0,element);
		remove(element.getData());
	}
}

template <class T>
bool SortedList<T> :: getEntry ( int position, PriorityData<T> & anEntry ) const
{
	Node<PriorityData<T>>* required = GetNodeAt ( position );
	if ( required == nullptr )
	{
		return false;
	}

	anEntry = required ->getItem();
	return true;

}

template <class T>
bool SortedList<T> :: replace ( int position, const T & newEntry, T & oldEntry )
{
	Node<PriorityData<T>>* required = GetNodeAt ( position );

	if ( required == nullptr )
	{
		return false;
	}

	oldEntry= required ->getItem().getData();
	PriorityData<T> item ( newEntry, required ->getItem().getPriority());
	required ->setItem ( item );
	return true;
}

template <class T>
T* SortedList<T> :: toArray ( int & count )
{
	count = getLength();
	T* Arr= new T[count];
	Node<PriorityData<T>>* p = head;
	for(int i=0; i<count;i++)
	{
		Arr[i] = p->getItem().getData();
		p = p->getNext();
	}
	return Arr;
}

template <class T>
SortedList<T> :: ~SortedList ()
{
	clear();
}
#endif