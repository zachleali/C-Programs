#include <iostream>
#include <string>
#include "MyList.h"
#include "Node.h"

using namespace std;



int main()
{

    MyList list1 = MyList();
    list1.insert("A");
    list1.insert("B");
    list1.insert("C");
    list1.insert("D");
    cout << "printing main list: " << endl;
    list1.print();
    cout << endl;

    MyList copy = MyList(list1);
    cout << "printing copy list: " << endl;
    copy.print();
    cout << endl;




    return 0;
}