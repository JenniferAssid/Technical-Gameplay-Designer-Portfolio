/**
* @file    list.c
* @author      Jennifer Assid
* DP email    jennifer.assid@digipen.edu
* course      CS 120
* section     A
* lab week    12
* @date

* @brief holds the functions that manipulate, assess, and output
                   various linked lists

*******************************************************************************/

#include <stdlib.h> /* malloc, free */
#include <stdio.h>  /* printf       */
#include "labs.h"

struct Node
{
    int number;        /* data portion    */
    struct Node* next; /* pointer portion */
};

/*******************************************************************************
   Function: add_front

Description: Adds a new node to the front of the list carrying data 'value'

     Inputs: **list - pointer to pointer to Node struct (head of linked list)
              value - data to be put in new node

    Outputs: None (node added to front of list)
*******************************************************************************/
void add_front(struct Node** list, int value)
{
    struct Node* newNode; /* initiates node to be added to front of list*/

    /* Allocates memory in heap for newNode */
    newNode = malloc(sizeof(struct Node));

    /* Places value into newNode data portion */
    newNode->number = value;

    /* Links the newNode next to current head of list */
    newNode->next = *list;

    /* updates pointer to be pointing to new head */
    *list = newNode;
}

/*******************************************************************************
   Function: print_list

Description: Prints the value of each node

     Inputs: *list - pointer to Node struct (head of linked list)

    Outputs: Print out of given linked list
*******************************************************************************/
void print_list(const struct Node* list)
{
    /* Iterates through the given list */
    while (list)
    {
        /* Prints the data portion of current node */
        printf("%3i", list->number);

        /* Sets up for next iteration to go over next node */
        list = list->next;
    }

    /* Prints a new line for readability */
    printf("\n");
}

/*******************************************************************************
   Function: free_list

Description: Frees all of the nodes in list

     Inputs: *list - pointer to Node struct (head of linked list)

    Outputs: None (frees values in given list)
*******************************************************************************/
void free_list(struct Node* list)
{
    struct Node* temp; /* temporary holder */

    /* Iterates through the given list */
    while (list)
    {
        /* sets the next node into a temporary hold */
        temp = list->next;

        /* clears current node */
        free(list);

        /* sets current node to the next node */
        list = temp;
    }
}

/*******************************************************************************
   Function: size_list

Description: Counts the number of nodes in list and returns the count

     Inputs: *list - pointer to Node struct (head of linked list)

    Outputs: number of nodes in list in an integer value
*******************************************************************************/
int size_list(const struct Node* list)
{
    int count = 0; /* running total */

    /* Iterates through the given list */
    while (list)
    {
        /* sets current node to next node */
        list = list->next;

        /* increments running total */
        count++;
    }

    /* returns total */
    return count;
}

/*******************************************************************************
   Function: sum_list

Description: Sums the values from each node in list and returns the sum

     Inputs: *list - pointer to Node struct (head of linked list)

    Outputs: sum of data portions of the nodes in given list in an integer value
*******************************************************************************/
int sum_list(const struct Node* list)
{
    int sum = 0; /* variable to hold calculated sum */

    /* Iterates through the given list */
    while (list)
    {
        /* Adds data portion of current node to current sum */
        sum += list->number;

        /* Sets current node to next node */
        list = list->next;
    }

    /* returns calculated sum */
    return sum;
}

/*******************************************************************************
   Function: add_back

Description: Adds a new node to the back of the list that contains 'value'

     Inputs: **list - pointer to pointer to Node struct (head of linked list)
              value - data to be put in new node

    Outputs: None (node added to back of the linked list)
*******************************************************************************/
void add_back(struct Node** list, int value)
{
    struct Node* newNode;  /* initiates node to be added to front of list */
    struct Node* lastNode; /* initiates node to act as temporary holder   */

    /* Allocates memory in heap for newNode */
    newNode = malloc(sizeof(struct Node));

    /* Places value into newNode data portion */
    newNode->number = value;

    /* Sets newNode next to NULL because node is going at the end of the list */
    newNode->next = NULL;

    /* Checks if list is empty */
    if (*list == NULL)

        /* Sets newNode to be the first node in the list */
        *list = newNode;

    else
    {
        /* Places current node into a temporary holder*/
        lastNode = *list;

        /* Iterates through the list */
        while (lastNode->next)
            /* Sets current node to next node */
            lastNode = lastNode->next;

        /* Places newNode at the end of the given list */
        lastNode->next = newNode;
    }

}

/*******************************************************************************
   Function: remove_item

Description: Removes the node in list with the first occurrence of value

     Inputs: **list - pointer to pointer to Node struct (head of linked list)
              value - data to be put in new node

    Outputs: None (list without node that was removed)
*******************************************************************************/
void remove_item(struct Node** list, int value)
{
    struct Node* temp = *list; /* initiates node to act as temporary holder   */
    struct Node* prev = temp;  /* initiates node for storage of previous node */

    /* Checks if list if empty */
    if (*list == NULL)

        /* Returns out of function */
        return;

    /* Checks if the first node is the first occurrence */
    if (temp && temp->number == value)
    {
        /* Updates the pointer to head of file */
        *list = temp->next;

        /* Frees the memory associated with current node */
        free(temp);

        /* Returns out of function*/
        return;
    }

    /* Iterates through the nodes that do not contain the value */
    while (temp && temp->number != value)
    {
        /* Sets previous node holder to hold the current node */
        prev = temp;

        /* Sets the temporary hold to hold the next node */
        temp = temp->next;
    }

    /* Checks if current node is empty */
    if (!temp)

        /* Returns out of function */
        return;

    /* Reestablishes chain with node removed */
    prev->next = temp->next;

    /* Frees memory associated with current node */
    free(temp);
}

/******* ADDED TO LINK WITH REST OF APPLICATION - 6/29/24 ******/
// Derived from main file given with lab from class - I DO NOT OWN THE FOLLOWING CODE
#include "PRNG.h"   /* RandomInt, ut_srand */


static void test_add_front(void)
{
    int i;
    struct Node* list = NULL;

    printf("\ntest_add_front ========================================\n");
    for (i = 1; i <= 10; i++)
    {
        printf("add %2i to front: ", i);
        add_front(&list, i);
        print_list(list);
    }

    free_list(list);
}

static void test_add_back(void)
{
    int i;
    struct Node* list = NULL;

    printf("\ntest_add_back ========================================\n");
    for (i = 1; i <= 10; i++)
    {
        printf("add %2i to back: ", i);
        add_back(&list, i);
        print_list(list);
    }

    free_list(list);
}

static void test_add_front_back(void)
{
    int i;
    struct Node* list = NULL;

    printf("\ntest_add_front_back ========================================\n");
    for (i = 1; i <= 5; i++)
    {
        printf("add %2i to front and back: ", i);
        add_front(&list, i);
        add_back(&list, i);
        print_list(list);
    }

    free_list(list);
}

static void test_remove(void)
{
    int i;
    struct Node* list = NULL;
    int items[] = { 5, 6, 10, 9, 2, 1, 3, 7, 8, 4 };

    printf("\ntest_remove ========================================\n");
    /* Populate the list */
    for (i = 1; i <= 10; i++)
        add_front(&list, i);

    printf("Start list : ");
    print_list(list);

    /* These values don't exist, but you need to handle that */
    remove_item(&list, 100);
    remove_item(&list, 99);

    /* Remove "random" order */
    for (i = 0; i < 10; i++)
    {
        printf("Removing %2i: ", items[i]);
        remove_item(&list, items[i]);
        print_list(list);
    }

    /* List should empty, but this is legal */
    printf("Removing  0: ");
    remove_item(&list, 1);
    print_list(list);

#if 1
    free_list(list); /* should be empty at this point           */
    free_list(NULL); /* This is legal and you need to handle it */
#endif
}

static void test_size(void)
{
    int i;
    struct Node* list = NULL;

    printf("\ntest_size ========================================\n");
    printf("Size is %2i: ", size_list(list));
    print_list(list);

    for (i = 1; i <= 10; i++)
    {
        add_front(&list, i);
        printf("Size is %2i: ", size_list(list));
        print_list(list);
    }

    free_list(list);
}

static void test_sum(void)
{
    int i, sum = 0;
    struct Node* list = NULL;

    printf("\ntest_sum ========================================\n");
    for (i = 1; i <= 10; i++)
        add_front(&list, i);

    sum = sum_list(list);
    print_list(list);
    printf("Sum is %i\n", sum);
    free_list(list);
}

static void test_A(void)
{
    int i;
    struct Node* list = NULL;
    int count = 50;
    int low = 1;
    int high = 20;

    printf("\ntest_A ========================================\n");
    for (i = 1; i <= count; i++)
        add_front(&list, RandomInt(low, high));

    print_list(list);
    printf("Size is %i.\n", size_list(list));
    printf("Sum is %i.\n", sum_list(list));

    for (i = 0; i < count; i++)
    {
        int r = RandomInt(low, high);
        printf("Remove %2i: ", r);
        remove_item(&list, r);
        print_list(list);
    }

    printf("Size is %i.\n", size_list(list));
    printf("Sum is %i.\n", sum_list(list));


    free_list(list);
}

void list(void)
{
    test_add_front();
    test_size();
    test_sum();
    test_add_back();
    test_add_front_back();
    test_remove();
    test_A();
}