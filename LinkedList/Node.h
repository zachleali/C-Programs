//
// Created by Zleal on 11/26/2019.
//

#ifndef HW7_NODE_H
#define HW7_NODE_H
#include <iostream>
#include<list>
#include <string>

using namespace std;

class Node
{
private:
    string m_data;
    Node *m_nextPtr = nullptr;

public:
    explicit Node(string input_string, Node *tmp_ptr)
    {
        m_data = input_string;
        m_nextPtr = tmp_ptr;

    }

    void setString(string input_string)
    {
        m_data = input_string;
    }

    void setPtr(Node *tmp_ptr)
    {
        m_nextPtr = tmp_ptr;
    }

    string getString() const
    {
        return m_data;
    }

    Node* getNode()
    {
        return m_nextPtr;
    }

};

#endif //HW7_NODE_H
