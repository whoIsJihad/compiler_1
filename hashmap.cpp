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
unsigned long long strongHash(const std::string &key, int tableSize)
{
    const unsigned long long p = 131; // Larger prime base
    unsigned long long hash = 0;

    for (char ch : key)
    {
        hash = hash * p + ch;
    }

    return hash % tableSize;
}
class Hashmap
{
public:
    int size;
    Obj **pointers;
    Hashmap(int size)
    {
        this->size = size;
        pointers = new Obj *[size];
        for (int i = 0; i < size; i++)
        {
            pointers[i] = nullptr;
        }
    }

    bool insert(string key, Obj *ptr)
    {
        int index = strongHash(key, this->size);
        Obj *temp = pointers[index];
        if (temp == nullptr)
        {
            pointers[index] = ptr;
            return true;
        }

        Obj *prev = nullptr;

        while (temp)
        {
            prev = temp;
            temp = temp->next;
        }
        prev->next = ptr;
        return true;
    }

    Obj *find(string key)
    {
        int index = strongHash(key, this->size);
        Obj *temp = pointers[index];
        if (temp == nullptr)
        {
            return nullptr;
        }

        Obj *prev = nullptr;

        while (temp)
        {
            prev = temp;
            temp = temp->next;
        }

        return prev;
    }
};

int main()
{
    Hashmap *hashmap = new Hashmap(4);
    Obj *var1 = new Obj(911);
    Obj *var2 = new Obj(13);
    Obj *var3 = new Obj(42);
    Obj *var4 = new Obj(69);
    Obj *var5 = new Obj(1000000);

    hashmap->insert("moonshadow", var1);
    hashmap->insert("atherion", var2);
    hashmap->insert("gold man", var3);
    hashmap->insert("iron man", var4);
    hashmap->insert("opo man", var5);

    cout << var3 << " " << hashmap->find("gold man") << endl;
}