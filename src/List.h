

#ifndef List_h
#define List_h

#include "Gleissymbol.h"


/**
 * Mit der Klasse List kann eine Liste von Gleissymbolen erstellt werden.
 * @author Lennart Klüner
 * @file List.h
 * @date 16.04.2022
 */
class List
{
private:
    /* data */
    struct ListNode{
        Gleissymbol *symbol;
        ListNode *next;
        };
    ListNode *head = nullptr;
    ListNode *end = nullptr;
    ListNode *preCurrent = nullptr;
    ListNode *current = nullptr;
public:
    List(/* args */);
    void append(Gleissymbol *symbol);
    void next();
    void toFirst();
    boolean hasAccess();
    boolean isEmpty();
    Gleissymbol *getContent();
    void remove();
};


#endif