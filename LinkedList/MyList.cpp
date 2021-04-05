//
// Created by Zleal on 11/26/2019.
//
#include "MyList.h"
#include <vector>
#include <string>
#include <cctype>


using namespace std;

MyList::MyList()
{
    m_startPtr = nullptr;
}


MyList::~MyList()
{
    for(Node* current = m_startPtr; current != nullptr; current = current->getNode())
    {
        Node* tmp = current;
        cout << tmp->getString() << " was deleted" << endl;
        delete tmp;
    }
    m_startPtr = nullptr;

}

bool MyList::empty() const
{
    if(m_startPtr == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MyList::insert(string str)
{
    string new_string;
    for(int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }
    new_string = str;
    cout << "adding " << new_string << " to list" << endl;
    Node* new_node = new Node(new_string,nullptr);
    if(m_startPtr == nullptr)
    {
        m_startPtr = new_node;
    }
    else
    {
        Node* current = m_startPtr;
        Node* prior = nullptr;
        while(current != nullptr && current->getString() < new_node->getString())
        {

            prior = current;
            current = current->getNode();

        }
        if(current == m_startPtr)
        {
            new_node->setPtr(m_startPtr);
            m_startPtr = new_node;
        }
        else
        {
            new_node->setPtr(current);
            prior->setPtr(new_node);
        }

    }

}

void MyList::print() const
{
    if(m_startPtr != nullptr)
    {
        for(Node* current = m_startPtr; current != nullptr; current = current->getNode())
        {
            cout << current->getString() << endl;
            cout << "Address: " << current << endl;
        }
    }
    else if(m_startPtr == nullptr)
    {
        cout << "The list is empty" << endl;
    }

}

void MyList::remove(string str)
{
    string new_string;
    for(int i = 0; i < str.length(); i++)
    {
       str[i] = tolower(str[i]);
    }
    new_string = str;
    Node* node_to_delete = m_startPtr;
    Node* prior = nullptr;
    while(node_to_delete != nullptr && node_to_delete->getString() != new_string)
    {
        prior = node_to_delete;
        node_to_delete = node_to_delete->getNode();
    }
    if(node_to_delete == nullptr)
    {
        cout << "the list is empty or the node was not found" << endl;
        return;
    }
    else if(prior == nullptr)
    {
        m_startPtr = m_startPtr->getNode();
        delete node_to_delete;
    }
    else
    {
        prior->setPtr(node_to_delete->getNode());
        delete node_to_delete;
    }

}

void MyList::clear()
{
    for(Node* current = m_startPtr; current != nullptr; current = current->getNode())
    {
        Node* tmp = current;
        cout << tmp->getString() << " was deleted" << endl;
        delete tmp;
    }
    m_startPtr = nullptr;
}

MyList::MyList(const MyList &listToCopy)
{
    vector<string> arr;
    Node *copy_ptr = listToCopy.m_startPtr;
    m_startPtr = nullptr;
    for(Node* tmp = copy_ptr; tmp != nullptr; tmp = tmp->getNode())
    {
        arr.push_back(tmp->getString());
    }
    Node* new_node = new Node(arr.at(0),nullptr);
    if(m_startPtr == nullptr)
    {
        m_startPtr = new_node;
    }
    for(int i = 1; i < arr.size(); i++)
    {
        Node* head = m_startPtr;
        Node* prior = nullptr;
        Node* new_node = new Node(arr.at(i),nullptr);
        while(head != nullptr && head->getString() < arr.at(i))
        {

            prior = head;
            head = head->getNode();

        }
        if(head == m_startPtr)
        {
            new_node->setPtr(m_startPtr);
            m_startPtr = new_node;
        }
        else
        {
            new_node->setPtr(head);
            prior->setPtr(new_node);

        }
    }

}