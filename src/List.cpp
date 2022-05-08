#include "List.h"

/**Mit der Klasse List kann eine Liste von Gleissymbolen erstellt werden.
 */

List::List(/* args */)
{
}
void List::append(Gleissymbol *symbol)
{
    if (symbol != nullptr)
    {
        end->next->symbol = symbol; // fülle next in end mit symbol
        end = end->next;            // setzte next auf ende
        if (isEmpty())
        {

            head = end;
        }
    }
}

void List::next()
{
    if (hasAccess())
    {
        preCurrent = current;
        current = current->next;
    }
}

void List::toFirst()
{
    current = head;
    preCurrent = head;
}

boolean List::hasAccess()
{
    return current != nullptr;
}

boolean List::isEmpty()
{
    return head == nullptr;
}

Gleissymbol *List::getContent()
{
    return current->symbol;
}

void List::remove()
{
    ListNode *p = head;
    if (current == head) // wenn am beginn
    {
        p = head;
        head = head->next;
        current = head;
        preCurrent = head;
        delete p;
    }
    else // sonst
    {
        preCurrent->next = current->next;
        p = current;
        current = current->next;
        delete p;
    }
}