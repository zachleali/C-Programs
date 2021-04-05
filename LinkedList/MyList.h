//
// Created by Zleal on 11/26/2019.
//

#ifndef HW7_MYLIST_H
#define HW7_MYLIST_H
#include <iostream>
#include <string>
#include "Node.h"

using namespace std;


class MyList
{
private:
    Node *m_startPtr;

public:
    MyList();
    ~MyList();
   MyList(const MyList &listToCopy);
   void insert(string str);
   bool empty() const;
   void print() const;
   void remove(string str);
   void clear();


};


#endif //HW7_MYLIST_H
