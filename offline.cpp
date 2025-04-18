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
unsigned long long simpleHash(const std::string& s) {
    const int p = 31;
    const int m = 1e9 + 9;
    unsigned long long hash_value = 0;
    long long p_pow = 1;
    for (char c : s) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

unsigned long long djb2(const std::string& str) {
    unsigned long long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
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
    private:
    int numBuckets;
    SymbolInfo **hashTable;
    ScopeTable *parentScope;
    public:
    ScopeTable(int n)
    {
        hashTable = new SymbolInfo *[n];
        numBuckets = n;
        for (int i = 0; i < numBuckets; i++)
            hashTable[i] = nullptr;
    }

    bool Insert(SymbolInfo *symbol)
    {
        SymbolInfo *findSymbol = this->lookUp(symbol->getName());

        if (findSymbol)
            return false;
        string name = symbol->getName();
        int index = SDBMHash(name) % this->numBuckets;
        SymbolInfo *temp = hashTable[index];
        if (temp == nullptr)
        {
            hashTable[index] = symbol;
            return true;
        }

        symbol->setNext(hashTable[index]);
        hashTable[index]=symbol;
        return  true;
    }

    SymbolInfo *lookUp(string name)
    {

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

    bool Delete(string name)
    {

        int index = SDBMHash(name) % this->numBuckets;

        SymbolInfo *temp = hashTable[index];
        SymbolInfo *prev = nullptr;
        if (temp && temp->getName() == name)
        {
            hashTable[index] = temp->getNext();
            delete temp;
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
                temp = temp->getNext();
            }
        }
    }

    void setParentScope(ScopeTable * scopeTable){
        this->parentScope=scopeTable;
    }

    ScopeTable* getParentScope(){
        return this->parentScope;
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
                temp = temp->getNext();
                delete prev;
            }
        }
        delete[] hashTable;
    }
};


class SymbolTable{
    private:
    ScopeTable*  currentScopeTable;
    SymbolTable(ScopeTable *currentScopeTable){
        this->currentScopeTable=currentScopeTable;
    }
    SymbolTable(){
        this->currentScopeTable=nullptr;
    }
    void enterScope(){
        ScopeTable *newScopeTable=new ScopeTable(10);
        newScopeTable->setParentScope(currentScopeTable);
        currentScopeTable=newScopeTable;
    }

    void removeScope(){
        ScopeTable* toDelete=currentScopeTable;
        currentScopeTable=currentScopeTable->getParentScope();
        delete toDelete;
    }

    bool insert(SymbolInfo* symbol){
        return this->currentScopeTable->Insert(symbol);
    }

    bool remove(string name){
        return currentScopeTable->Delete(name);

    }

    void printCurrentScopeTable(){
        currentScopeTable->Print();
    }

    void printAllScopeTable(){
        ScopeTable* temp=currentScopeTable;
        while(temp){
            temp->Print();
            temp=temp->getParentScope();
        }
    }
    SymbolInfo* LookUp(string name){
        ScopeTable * temp=currentScopeTable;

        while(temp){
            SymbolInfo * symbol=temp->lookUp(name);
            if(symbol) return symbol;
            temp=temp->getParentScope();
        }
        return nullptr;
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
    SymbolInfo *foundSymbol = scopeTable.lookUp(symbol1->getName());
    if (foundSymbol)
        cout << "Found: " << foundSymbol->getName() << " " << foundSymbol->getType() << endl;
    else
        cout << "Symbol not found." << endl;

    // Delete a symbol
    cout << "Deleting symbol 'y'..." << endl;
    if (scopeTable.Delete(symbol2->getName()))
        cout << "Symbol 'y' deleted successfully." << endl;
    else
        cout << "Failed to delete symbol 'y'." << endl;

    // // Print the scope table again
    cout << "Printing ScopeTable after deletion:" << endl;
    scopeTable.Print();
    return 0;
}