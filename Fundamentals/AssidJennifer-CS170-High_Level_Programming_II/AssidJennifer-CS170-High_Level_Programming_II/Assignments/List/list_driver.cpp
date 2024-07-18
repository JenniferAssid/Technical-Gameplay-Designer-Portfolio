// I DO NOT OWN THE FOLLOWING CODE - IT IS BASED OFF OF CODE PROVIDED TO THE CLASS VIA A DRIVER FILE TO TEST FUNCTIONALITY

#include "List.h"
#include <iostream>

void List_TestPushFront(void)
{
  std::cout << "List_TestPushFront..." << std::endl;

  int size = 5;
  CS170::List list;
  for (int i = 1; i <= size; i++)
    list.push_front(i);

  std::cout << list;
  std::cout << "Items in the list: " << list.size() << std::endl;
  std::cout << std::endl;
}

void List_TestPushBack(void)
{
  std::cout << "List_TestPushBack..." << std::endl;

  int size = 5;
  CS170::List list;
  for (int i = 1; i <= size; i++)
    list.push_back(i);

  std::cout << list;
  std::cout << "Items in the list: " << list.size() << std::endl;
  std::cout << std::endl;
}

void List_TestPushFrontBack(void)
{
  std::cout << "List_TestPushFrontBack..." << std::endl;

  int size = 10;
  CS170::List list;
  for (int i = 1; i <= size; i++)
  {
    list.push_front(i);
    list.push_back(i * 5);
  }

  std::cout << list;
  std::cout << "Items in the list: " << list.size() << std::endl;
  std::cout << std::endl;
}

void List_TestPopFront(void)
{
  std::cout << "List_TestPopFront..." << std::endl;

  int size = 5;
  CS170::List list;
  for (int i = 1; i <= size; i++)
    list.push_front(i);

  std::cout << list;
  while (!list.empty())
  {
    int item = list.front();
    std::cout << "First item was: " << item << std::endl;
    list.pop_front();
    std::cout << "New list:";
    std::cout << list;
    std::cout << "Items in the list: " << list.size() << std::endl;
  }
  std::cout << std::endl;
}

void List_TestArray1(void)
{
  std::cout << "List_TestArray1..." << std::endl;

  int array[] = { 4, 7, 12, 5, 9, 23, 7, 11, 15, 2 };
  int size = static_cast<int>(sizeof(array) / sizeof(*array));

  // Construct from array
  CS170::List list(array, size);

  std::cout << list;
  std::cout << "Items in the list: " << list.size() << std::endl;
  std::cout << std::endl;
}

void List_TestArray2(void)
{
  std::cout << "List_TestArray2..." << std::endl;

  const int array[] = { 4, 7, 12, 5, 9, 23, 7, 11, 15, 2 };
  int size = static_cast<int>(sizeof(array) / sizeof(*array));

  // Construct from array
  CS170::List list(array, size);

  std::cout << list;
  std::cout << "Items in the list: " << list.size() << std::endl;
  std::cout << std::endl;
}

void List_TestCopyConstructor1(void)
{
  std::cout << "List_TestCopyConstructor1..." << std::endl;
  int size = 10;
  CS170::List list1;
  for (int i = 1; i <= size; i++)
    list1.push_front(i * 3);

  std::cout << "List 1: ";
  std::cout << list1;

  CS170::List list2(list1);
  std::cout << "List 2: ";
  std::cout << list2;

  std::cout << std::endl;
}

void List_TestCopyConstructor2(void)
{
  std::cout << "List_TestCopyConstructor2..." << std::endl;
  int size = 10;
  CS170::List list1;
  for (int i = 1; i <= size; i++)
    list1.push_front(i * 3);

  std::cout << "List 1: ";
  std::cout << list1;

  const CS170::List list2(list1);
  std::cout << "List 2: ";
  std::cout << list2;

  if (list2.empty())
    std::cout << "List 2 is empty\n";
  else
    std::cout << "List 2 is not empty\n";

  std::cout << "Items in List2: ";
  std::cout << list2.size();
  std::cout << std::endl;

  std::cout << std::endl;
}

void List_TestAssignment1(void)
{
  std::cout << "List_TestAssignment1..." << std::endl;
  int size = 10;
  CS170::List list1, list2;
  for (int i = 1; i <= size; i++)
    list1.push_front(i * 2);

  std::cout << "Before assignment:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;

  list2 = list1;
  std::cout << "After assignment:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;

  std::cout << std::endl;
}

void List_TestAssignment2(void)
{
  std::cout << "List_TestAssignment2..." << std::endl;
  int size = 10;
  CS170::List list1, list2, list3;
  for (int i = 1; i <= size; i++)
    list1.push_front(i * 2);

  std::cout << "Before assignment:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "List 3: ";
  std::cout << list3;

  list3 = list2 = list1;
  std::cout << "After assignment:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "List 3: ";
  std::cout << list3;

  std::cout << std::endl;
}

void List_TestAddition1(void)
{
  std::cout << "List_TestAddition1..." << std::endl;

  int size = 5;
  CS170::List list1, list2;
  for (int i = 1; i <= size; i++)
  {
    list1.push_front(i);
    list2.push_front(i * 10);
  }
  std::cout << "Before addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;

  CS170::List list3;
  list3 = list1 + list2;
  std::cout << "After addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "List 3: ";
  std::cout << list3;
  std::cout << std::endl;
}

void List_TestAddition2(void)
{
  std::cout << "List_TestAddition2..." << std::endl;

  int size = 5;
  CS170::List list1, list2;
  for (int i = 1; i <= size; i++)
  {
    list1.push_front(i);
    list2.push_front(i * 10);
  }

  std::cout << "Before addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;

  const CS170::List list3(list1 + list2);
  std::cout << "List 3: ";
  std::cout << list3;

  CS170::List list4;
  list4 = list1 + list2 + list3;
  std::cout << "After addition:" << std::endl;
  std::cout << "List 4: ";
  std::cout << list4;
  std::cout << "Items in the list: " << list4.size() << std::endl;
  std::cout << std::endl;
}

void List_TestAddAssign1(void)
{
  std::cout << "List_TestAddAssign1..." << std::endl;

  int size = 5;
  CS170::List list1, list2;
  for (int i = 1; i <= size; i++)
  {
    list1.push_front(i);
    list2.push_front(i * 10);
  }

  std::cout << "Before addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;

  list1 += list2;
  std::cout << "After addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "Items in the list: " << list1.size() << std::endl;
  std::cout << std::endl;
}

void List_TestAddAssign2(void)
{
  std::cout << "List_TestAddAssign2..." << std::endl;

  int size = 5;
  CS170::List list1, list2;
  for (int i = 1; i <= size; i++)
    list1.push_front(i);

  std::cout << "Before addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;

  list1 += list2;
  std::cout << "After addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "Items in the list: " << list1.size() << std::endl;
  std::cout << std::endl;
}

void List_TestAddAssign3(void)
{
  std::cout << "List_TestAddAssign3..." << std::endl;

  int size = 5;
  CS170::List list1, list2;
  for (int i = 1; i <= size; i++)
    list1.push_front(i);

  std::cout << "Before addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;

  list2 += list1;
  std::cout << "After addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "Items in the list: " << list1.size() << std::endl;
  std::cout << std::endl;
}

void List_TestAddAssign4(void)
{
  std::cout << "List_TestAddAssign4..." << std::endl;

  int size = 5;
  CS170::List list1, list2, list3;
  for (int i = 1; i <= size; i++)
  {
    list1.push_front(i);
    list2.push_front(i * 2);
    list3.push_front(i * 5);
  }

  std::cout << "Before addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "List 3: ";
  std::cout << list3;

  list1 += list2 += list3;
  std::cout << "After addition:" << std::endl;
  std::cout << "List 1: ";
  std::cout << list1;
  std::cout << "List 2: ";
  std::cout << list2;
  std::cout << "List 3: ";
  std::cout << list3;
  std::cout << "Items in the list: " << list1.size() << std::endl;
  std::cout << std::endl;
}

void List_TestSubscript1(void)
{
  std::cout << "List_TestSubscript1..." << std::endl;

  int size = 10;
  CS170::List list1;
  for (int i = 1; i <= size; i++)
    list1.push_back(i);

  std::cout << "list[0] = " << list1[0] << std::endl;
  std::cout << "list[5] = " << list1[5] << std::endl;
  std::cout << "list[9] = " << list1[9] << std::endl;
  std::cout << std::endl;
}

void List_TestSubscript2(void)
{
  std::cout << "List_TestSubscript2..." << std::endl;

  int size = 10;
  CS170::List list1;
  for (int i = 1; i <= size; i++)
    list1.push_back(i);

  std::cout << "Original list:\n";
  std::cout << list1;

  list1[0] = 10;
  list1[5] = 20;
  list1[9] = 30;
  list1[7] = list1[2];

  std::cout << "Modified list:\n";
  std::cout << list1;

  std::cout << std::endl;
}