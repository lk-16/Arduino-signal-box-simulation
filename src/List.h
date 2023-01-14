/*Library für die Steuerung eines SpDrS60-Gleisbildstellpults
 *  mit dieser Library können Weichen, Signale etc. gesteuert werden
 *  Lennart Klüner 20.04.2022
 */

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
    struct ListNode
    {
        Gleissymbol *symbol;
        ListNode *next;
    };
    ListNode *head = nullptr;       /**<erstes Element der Liste*/
    ListNode *end = nullptr;        /**<letztes Element der List*/
    ListNode *preCurrent = nullptr; /**<Element vor dem Aktuellen*/
    ListNode *current = nullptr;    /**<aktuelles Element*/

public:
    List(/* args */);
    /**
     * @brief Hängt symbol an das Ende der Liste an.
     * 
     * @param symbol Gleissymbol, das am ende der Liste angehängt werden soll.
     */
    void append(Gleissymbol *symbol);
    void next();/**<Wenn die Liste nicht leer ist, wird das nächste Objekt das aktuelle Objekt*/
    void toFirst();
    /**
     * @brief true, wenn es ein aktuelles Objekt gibt,
     * sonst false.
     *
     * @return true, falls Zugriff moeglich, sonst false
     */
    boolean hasAccess();
    /**
     * @brief true, wenn die Liste keine Objekte enthält,
     * sonst false
     *
     * @return true, wenn die Liste leer ist, sonst false
     */
    boolean isEmpty();
    /**
     * @brief Gibt das aktuelle Gleissymbol zurück
     * 
     * @return Gleissymbol* 
     */
    Gleissymbol *getContent();
    /**
     * @brief Löscht das aktuelle Objekt
     * 
     */
    void remove();
};

#endif