#include <exception>
#include "IndexedList.h"
#include "ListIterator.h"

// Komplexitaet: theta(n)
IndexedList::IndexedList() {
	//TODO - Implementation
    this->cap = 20;
    this->head = -1;
    this->tail = -1;
    this->firstEmpty = 0;
    this->nodes = new DLLANode [this->cap];
    for (int index = 0; index < this->cap - 1; index++)
        this->nodes[index].next = index + 1;
    this->nodes[this->cap-1].next = -1;

    for (int index = this->cap - 1; index >= 1; index--)
        this->nodes[index].prev = index -1;
    this->nodes[0].prev = -1;
}

// Komplexitaet: theta(n)
int IndexedList::size() const {
    //TODO - Implementation
    if (!IndexedList::isEmpty())
    {
        int current_pos = this->head;
        int count = 1;
        while (nodes[current_pos].next != -1)
        {
            count++;
            current_pos = nodes[current_pos].next;
        }
        return count;
    }
    return 0;
}

// Komplexitaet: theta(1)
bool IndexedList::isEmpty() const {
    //TODO - Implementation
	return this->head == -1;
}

// Bester Fall: theta(1)  -  gesuchte Position ist 0
// Durchschnittlicher Fall: theta(n)
// Schlimmster Fall: theta(n)  - gesuchte Position ist die letzte
// Komplexitaet: O(n)
TElem IndexedList::getElement(int pos) const {
    //TODO - Implementation
    if (pos >= 0) {
        int count = 0;
        int current_pos = this->head;
        while (current_pos != -1 && count != pos) {
            current_pos = nodes[current_pos].next;
            count++;
        }
        if (current_pos != -1)
            return nodes[current_pos].info;
    }
    std::exception E;
    throw E;
}

// Bester Fall: theta(1)  -  gesuchte Position ist 0
// Durchschnittlicher Fall: theta(n)
// Schlimmster Fall: theta(n)  - gesuchte Position ist die letzte
// Komplexitaet: O(n)
TElem IndexedList::setElement(int pos, TElem e)
{
    //TODO - Implementation
    if (pos >= 0) {
        int count = 0;
        int current_pos = this->head;
        while (current_pos!=-1 && count != pos) {
            current_pos = nodes[current_pos].next;
            count++;
        }
        TElem old_value = nodes[current_pos].info;
        nodes[current_pos].info = e;
        return old_value;
    }
    std::exception E;
    throw E;
}

// Komplexitaet: theta(1)
int IndexedList::allocate()
{
    // gibt die erste leere Position zurueck, aktualisiert firstEmpty
    int newElem = this->firstEmpty;
    if (newElem != -1)
    {
        this->firstEmpty = nodes[firstEmpty].next;
        if (this->firstEmpty != -1)
            nodes[firstEmpty].prev = -1;

        nodes[newElem].prev = -1;
        nodes[newElem].next = -1;
    }
    return  newElem;
}

// Komplexitaet: theta(1) amortisiert
void IndexedList::addToEnd(TElem e) {
    //TODO - Implementation
    int newElem = IndexedList::allocate();
    if (newElem == -1)
    {
        // es gibt keine leere Position -> resize
        auto *newElements = new DLLANode [this->cap*2];
        for (int index = 0; index < IndexedList::size(); index++)
            newElements[index] = nodes[index];

        for (int index = this->cap; index < this->cap*2-1; index++)
            newElements[index].next = index + 1;
        newElements[this->cap*2-1].next = -1;

        for (int index = 2 * this->cap - 1; index >= this->cap; index--)
            newElements[index].prev = index -1;

        this->nodes = newElements;
        this->firstEmpty = this->cap;
        this->cap *= 2;
        newElem = IndexedList::allocate();
    }
    nodes[newElem].info = e;
    if (IndexedList::isEmpty())
    {
        // head, tail aendern
        this->head = newElem;
        nodes[head].info = e;
        nodes[head].next = -1;
        this->tail = this->head;
    }
    else
    {
        nodes[tail].next = newElem;
        nodes[ newElem].prev = tail;
        tail = newElem;
        nodes [tail].next = -1;
        nodes [tail].info = e;
  }
}

// Bester Fall: theta(n) - wegen size() Aufruf   // falls wir speichern size in Representation->theta(1) pos=0 -> head aendern
// Durchschnittlicher Fall: theta(n)
// Schlimmster Fall: theta(n)
// Komplexitaet: theta(n)
void IndexedList::addToPosition(int pos, TElem e) {
    //TODO - Implementation
    if (pos < 0 || pos > IndexedList::size())
    {
        std::exception E;
        throw E;
    }
    int newElem = IndexedList::allocate();
    if (newElem == -1)
    {
        // resize
        auto *newElements = new DLLANode [this->cap*2];
        for (int index = 0; index < IndexedList::size(); index++)
            newElements[index] = nodes[index];

        for (int index = this->cap; index < this->cap*2-1; index++)
            newElements[index].next = index + 1;
        newElements[this->cap*2-1].next = -1;

        for (int index = 2 * this->cap - 1; index >= this->cap; index--)
            newElements[index].prev = index -1;

        this->nodes = newElements;
        this->firstEmpty = this->cap;
        this->cap *= 2;
        newElem = IndexedList::allocate();
    }
    nodes[newElem].info = e;
    if (pos == 0)
    {
        // leere Liste -> head und tail aendern
        if (this->head == -1)
        {
            this->head = newElem;
            this->tail = newElem;
        }
            // nur head aendern
        else
        {
            nodes[newElem].next = this->head;
            nodes[head].prev = newElem;
            this->head = newElem;
        }
    }
    else
    {
        int current_pos = this->head;
        int count = 0;
        // hoert vor der gesuchten Position auf
        while (current_pos!= -1 && count < pos-1)
        {
            current_pos = nodes[current_pos].next;
            count ++;
        }
        if (current_pos!= -1)
        {
            int nodNext = nodes[current_pos].next;
            nodes[newElem].next = nodNext;
            nodes[newElem].prev = current_pos;
            nodes[current_pos].next = newElem;
            // tail aendern
            if (nodNext == -1)
                this->tail = newElem;
            else
                nodes[nodNext].prev = newElem;
        }
        else
            free(newElem);
    }
}

// Position freigegeben
// Komplexitaet: theta(1)
void IndexedList::free(int pos)
{
    nodes[pos].next = this->firstEmpty;
    nodes[pos].prev = -1;
    if (this->firstEmpty == -1)
        nodes[firstEmpty].prev = pos;
    this->firstEmpty = pos;
}

// Bester Fall: theta(1)  - Element aus der erste Position entfernen
// Durchschnittlicher Fall: theta(n)
// Schlimmster Fall: theta(n)  - List durchlaufen
// Komplexitaet: O(n)
TElem IndexedList::remove(int pos) {
    if(!IndexedList::isEmpty() && pos >=0)
    {
        TElem deletedElem;
        // wenn die Position auf dem Head liegt
        if(pos == 0)
        {
            // wenn es nur 1 Element gibt -> head, tail aendern
            if (this->head == this->tail)
            {
                deletedElem = nodes[head].info;
                int ElementToFree = this -> head;
                this->head =-1;
                this->tail =-1;
                IndexedList::free(ElementToFree);
                return deletedElem;
            }
            else
                // erstes Element loeschen -> head aendern
            {
                deletedElem = nodes[head].info;
                int ElementToFree = this -> head;
                this->head = nodes[head].next;
                nodes[head].prev = -1;
                IndexedList::free(ElementToFree);
                return deletedElem;
            }
        }
        else
        {
            int current_pos = this->head;
            int count = 0;
            while (current_pos!=-1 && count != pos)
            {
                current_pos = nodes[current_pos].next;
                count++;
            }
            // wenn die Position auf dem Tail liegt
            if(current_pos == this->tail)
            {
                deletedElem = nodes[tail].info;
                int ElementToFree = this -> tail;
                tail = nodes[tail].prev;
                nodes[tail].next = -1;
                IndexedList::free(ElementToFree);
                return deletedElem;
            }
            else if (current_pos!=-1) {
                deletedElem = nodes[current_pos].info;
                nodes[nodes[current_pos].prev].next = nodes[current_pos].next;
                nodes[nodes[current_pos].next].prev = nodes[current_pos].prev;
                IndexedList::free(current_pos);
                return deletedElem;
            }
        }
    }
        std::exception E;
        throw E;
}

// Bester Fall: theta(1)  -  das gesuchte Element ist auf Position 0
// Durchschnittlicher Fall: theta(n)
// Schlimmster Fall: theta(n) - gesuchte Position ist das letzte
// Komplexitaet: O(n)
int IndexedList::search(TElem e) const{
    int count = 0;
    TElem current = this -> head;
    while (current != -1 and nodes[current].info != e)
    {
        current = nodes[current].next;
        count ++;
    }
    if (current == -1)
        return -1;
    else
        return count;
}

ListIterator IndexedList::iterator() const {
    return ListIterator(*this);        
}

IndexedList::~IndexedList() {
	//TODO - Implementation
    delete[] this->nodes;
}

IndexedList::IndexedList (const IndexedList &list)
{
    this->cap = list.cap;
    this->head = list.head;
    this->tail = list.tail;
    this->firstEmpty = list.firstEmpty;

    this->nodes = new DLLANode [this->cap];
    for (int index = 0; index < this->cap; index++)
    {
        this->nodes[index].next = list.nodes[index].next;
        this->nodes[index].prev = list.nodes[index].prev;
    }
}