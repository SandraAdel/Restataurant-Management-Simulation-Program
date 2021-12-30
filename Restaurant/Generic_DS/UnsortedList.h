#pragma once
#ifndef __UnSortedList_H_
#define _UnSortedList_H_
#include "Node.h"

template <class T>

class UnsortedList
{
	Node<T>* head;
	Node<T>* GetNodeAt ( int position ) const;

public:

	UnsortedList ();

	bool isEmpty () const;
	int getLength () const;
	bool insert ( int newPosition, const T & newEntry );
	bool remove ( int position );         
	void clear ();
	bool getEntry ( int position, T & anEntry ) const;
	bool replace ( int position, const T & newEntry, T & oldEntry ); 
	T* toArray ( int & count );

	virtual ~UnsortedList ();
};

template <class T>
UnsortedList<T> :: UnsortedList ()
{
	head = nullptr;
}

template <class T>
Node<T>* UnsortedList<T> :: GetNodeAt ( int position ) const
{
	if ( position < 0 || position >  getLength () -1  )
	{
		return nullptr;
	}

	Node<T>* nptr = head;
	for ( int i=0; i<position; i++ )
	{
		nptr = nptr ->getNext();
	}

	return nptr;
}


template <class T>
bool UnsortedList<T> :: insert ( int newPosition, const T & newEntry )
{
	if ( newPosition < 0 || newPosition > getLength() )
	{
		return false;
	}

	Node<T>* newNode = new Node<T> ( newEntry );
	if ( newNode == nullptr )
	{
		return false;
	}

	if ( newPosition == 0 )
	{
		newNode ->setNext( head );
		head = newNode;
	}
	else 
	{
		Node<T>* prevNode = GetNodeAt ( newPosition -1 );
		newNode ->setNext ( prevNode ->getNext() );
		prevNode ->setNext ( newNode );
	}

	return true;
}

template <class T>
bool UnsortedList<T> :: remove ( int position )
{
	if ( position < 0 || position > getLength()- 1 )
	{
		return false;
	}

	Node<T>* nodeToDelete = nullptr;
	if ( position == 0 )
	{
		nodeToDelete = head;
		head = head ->getNext();
	}
	else 
	{
		Node<T>* prevNode = GetNodeAt ( position -1 );
		nodeToDelete = prevNode ->getNext();
		prevNode ->setNext ( nodeToDelete ->getNext() );
	}

	delete nodeToDelete;
	return true;
}

template <class T>
bool UnsortedList<T> :: isEmpty () const
{
	return ( head == nullptr );
}

template <class T>
int UnsortedList<T> :: getLength () const
{
	int count = 0;
	Node<T>* nptr = head;
	while ( nptr != nullptr )
	{
		count++;
		nptr = nptr ->getNext();
	}
	return count;
}

template <class T>
void UnsortedList<T> :: clear ()
{
	while ( !isEmpty() )
	{
		remove(0);
	}
}

template <class T>
bool UnsortedList<T> :: getEntry ( int position, T & anEntry ) const
{
	Node<T>* required = GetNodeAt ( position );
	if ( required == nullptr )
	{
		return false;
	}

	anEntry = required ->getItem();
	return true;

}

template <class T>
bool UnsortedList<T> :: replace ( int position, const T & newEntry, T & oldEntry )
{
	Node<T>* required = GetNodeAt ( position );

	if ( required == nullptr )
	{
		return false;
	}

	oldEntry= required ->getItem();
	required ->setItem ( newEntry );
	return true;
}

template <class T>
T* UnsortedList<T> :: toArray ( int & count )
{
	count = getLength();
	T* Arr= new T[count];
	Node<T>* p = head;
	for(int i=0; i<count;i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}

template <class T>
UnsortedList<T> :: ~UnsortedList ()
{
	clear();
}
#endif