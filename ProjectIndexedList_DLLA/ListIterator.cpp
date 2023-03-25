#include "ListIterator.h"
#include "IndexedList.h"
#include <exception>

// Komplexitaet: theta(1)
ListIterator::ListIterator(const IndexedList& list) : list(list){
   //TODO - Implementation
    this->current = list.head;
}

// Komplexitaet: theta(1)
void ListIterator::first(){
    //TODO - Implementation
    this->current = list.head;
}

// Komplexitaet: theta(1)
void ListIterator::next(){
    //TODO - Implementation
    if (ListIterator::valid())
        this->current = this->list.nodes[current].next;
    else
    {
        std::exception E;
        throw E;
    }
}

// Komplexitaet: theta(1)
bool ListIterator::valid() const{
    //TODO - Implementation
    return this->current != -1;
}

// Komplexitaet: theta(1)
TElem ListIterator::getCurrent() const{
   //TODO - Implementation
    if (ListIterator::valid())
    {
        return this->list.nodes[current].info;
    }
    else
    {
        std::exception E;
        throw E;
    }
}