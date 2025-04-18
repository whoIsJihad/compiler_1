#include <bits/stdc++.h>

using namespace std;
static unsigned int SDBMHash(string str)
{
    unsigned int hash = 0;
    unsigned int i = 0;
    unsigned int len = str.length();

    for (i = 0; i < len; i++)
    {
        hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}
class SymbolInfo
{
private:
    string name;
    string symbolType;
    SymbolInfo *next;


public:
    SymbolInfo(string name, string symbolType)
    {
        this->name = name;
        this->next = nullptr;
        this->symbolType = symbolType;
    }
    string getName()
    {
        return this->name;
    }
    string getType()
    {
        return this->symbolType;
    }

    void setName(string name)
    {
        this->name = name;
    }
    void setType(string symbolType)
    {
        this->symbolType = symbolType;
    }

    SymbolInfo *getNext()
    {
        return this->next;
    }
    void setNext(SymbolInfo *next)
    {
        this->next = next;
    }
};

class ScopeTable
{
    public:

    int numBuckets;
    SymbolInfo **hashTable;
    ScopeTable* parentScope;
    ScopeTable(int n)
    {
        hashTable = new SymbolInfo *;
        numBuckets = n;
        for (int i = 0; i < numBuckets; i++)
            hashTable[i] = nullptr;
    }

    bool Insert(SymbolInfo *symbol)
    {
        SymbolInfo *findSymbol = this->lookUp(*symbol);

        if (!findSymbol)
            return false;
        string name = symbol->getName();
        int index = SDBMHash(name) % this->numBuckets;
        SymbolInfo *temp = hashTable[index];
        if (temp == nullptr)
        {
            hashTable[index] = symbol;
            return true;
        }

        SymbolInfo *prev = nullptr;

        while (temp)
        {
            prev = temp;
            temp = temp->getNext();
        }

        prev->setNext(symbol);
        return true;
    }

    SymbolInfo *lookUp(SymbolInfo symbolInfo)
    {
        string name = symbolInfo.getName();

        int index = SDBMHash(name) % this->numBuckets;

        SymbolInfo *temp = hashTable[index];

        while (temp)
        {
            if (temp->getName() == name)
            {
                return temp;
            }
            temp = temp->getNext();
        }
        return nullptr;
    }

    bool Delete(SymbolInfo symbolInfo)
    {
        string name = symbolInfo.getName();

        int index = SDBMHash(name) % this->numBuckets;

        SymbolInfo *temp = hashTable[index];
        SymbolInfo *prev = nullptr;
        if (temp->getName() == name)
        {
            hashTable[index] = temp->getNext();
            return true;
        }
        while (temp)
        {
            if (temp->getName() == name)
            {
                // prev
                prev->setNext(temp->getNext());
                delete temp;
                return true;
            }
            prev = temp;
            temp = temp->getNext();
        }
        return false;
    }
    void Print()
    {
        for (int i = 0; i < numBuckets; i++)
        {
            SymbolInfo *temp = hashTable[i];
            while (temp)
            {
                cout << temp->getName() << " " << temp->getType() << endl;
            }
        }
    }
    ~ScopeTable()
    {
        for (int i = 0; i < numBuckets; i++)
        {
            SymbolInfo *temp = hashTable[i];
            SymbolInfo *prev = nullptr;

            while (temp)
            {
                prev = temp;
                delete prev;
                temp = temp->getNext();
            }
        }
        delete[] hashTable;
    }
};
int main()
{
    ScopeTable scopeTable(10);

    // Insert symbols
    SymbolInfo *symbol1 = new SymbolInfo("x", "int");
    SymbolInfo *symbol2 = new SymbolInfo("y", "float");
    SymbolInfo *symbol3 = new SymbolInfo("z", "char");

    cout << "Inserting symbols..." << endl;
    scopeTable.Insert(symbol1);
    scopeTable.Insert(symbol2);
    scopeTable.Insert(symbol3);

    // Print the scope table
    cout << "Printing ScopeTable:" << endl;
    scopeTable.Print();

    // Look up symbols
    cout << "Looking up symbols..." << endl;
    SymbolInfo *foundSymbol = scopeTable.lookUp(*symbol1);
    if (foundSymbol)
        cout << "Found: " << foundSymbol->getName() << " " << foundSymbol->getType() << endl;
    else
        cout << "Symbol not found." << endl;

    // Delete a symbol
    cout << "Deleting symbol 'y'..." << endl;
    if (scopeTable.Delete(*symbol2))
        cout << "Symbol 'y' deleted successfully." << endl;
    else
        cout << "Failed to delete symbol 'y'." << endl;

    // Print the scope table again
    cout << "Printing ScopeTable after deletion:" << endl;
    scopeTable.Print();

    return 0;

}