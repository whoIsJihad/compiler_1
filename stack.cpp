#include <bits/stdc++.h>

using namespace std;

class Obj
{
public:
    int x;
    Obj *next;
    Obj(int x)
    {
        this->next = nullptr;
        this->x = x;
    }
};

class Stack
{
public:
    Obj *head;
    Stack()
    {
        head = nullptr;
    }

    void push(int x)
    {
        Obj *newObj = new Obj(x);

        // head->next=newObj;
        newObj->next = head;
        head = newObj;
    }

    Obj *top()
    {
        return head;
    }

    void pop()
    {
        if (head == nullptr)
            return;
        Obj *temp = head;
        head = head->next;
        delete temp;
        return;
    }

    void clear()
    {
        Obj *temp = head;
        while (temp)
        {
            Obj *del = temp;
            temp = temp->next;
            delete del;
        }
        head=nullptr;


    }
};

int main()
{
}