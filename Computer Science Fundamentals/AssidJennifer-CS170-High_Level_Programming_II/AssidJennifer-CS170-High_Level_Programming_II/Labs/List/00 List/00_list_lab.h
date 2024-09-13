#pragma once
////////////////////////////////////////////////////////////////////////////////
#ifndef LIST_LAB_H
#define LIST_LAB_H
////////////////////////////////////////////////////////////////////////////////

namespace CS170
{
    namespace ListLab
    {
        struct Node
        {
            int number; // data portion
            Node* next; // pointer portion
        };

        // Adds a node to the end of the list
        void AddToEnd(Node** pList, int value);

        // Adds a node to the front of the list
        void AddToFront(Node** pList, int value);

        // Returns the number of nodes in the list
        int Count(Node* list);

        // Finds an item and returns the node that contains it
        Node* FindItem(Node* list, int value);

        // Frees (deletes) all of the nodes in the list
        void FreeList(Node* pList);

        // Prints all of the nodes values
        void PrintList(Node* list);

        // Inserts a new node at the specified position
        void Insert(Node** pList, int value, int position);

    } // namespace ListLab
} // namespace CS170

void list_lab_0(void);

#endif // LIST_H
