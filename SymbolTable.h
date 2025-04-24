#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ScopeTable.h"

class SymbolTable
{
private:
    ScopeTable *currentScopeTable;
    int cnt;
    int deletedScopeTableCollision;

public:
    SymbolTable(ScopeTable *currentScopeTable)
    {
        cnt = 1;
        ++cnt;
        deletedScopeTableCollision = 0;
        this->currentScopeTable = currentScopeTable;
    }

    SymbolTable()
    {
        this->currentScopeTable = nullptr;
    }

    ~SymbolTable()
    {
        ScopeTable *temp = currentScopeTable;
        while (temp)
        {
            std::cout<<"\tScopeTable# "<<temp->getID()<<" removed"<<std::endl;
            ScopeTable *currentScopeTable = temp;
            temp = temp->getParentScope();
            delete currentScopeTable;
        }
        currentScopeTable = nullptr;
    }

    void enterScope()
    {
        if (currentScopeTable == nullptr)
        {
            currentScopeTable = new ScopeTable(10);
            return;
        }
        ScopeTable *newScopeTable = new ScopeTable(currentScopeTable->getNumBuckets(), cnt++);
        std::cout << "\tScopeTable# " << newScopeTable->getID() << " created\n";
        newScopeTable->setParentScope(currentScopeTable);
        currentScopeTable = newScopeTable;
    }

    void exitScope()
    {
        if (currentScopeTable == nullptr || currentScopeTable->getParentScope() == nullptr)
            return;

        ScopeTable *toDelete = currentScopeTable;
        std::cout<<"\tScopeTable# "<<currentScopeTable->getID()<<" removed"<<std::endl;
        
        deletedScopeTableCollision += currentScopeTable->getNumCollision();
        currentScopeTable = currentScopeTable->getParentScope();
        delete toDelete;
    }

    bool Insert(SymbolInfo *symbol)
    {
        if (this->currentScopeTable == nullptr)
            return false;
        return this->currentScopeTable->Insert(symbol);
    }

    bool remove(std::string name)
    {
        if (!this->currentScopeTable)
            return false;
        return currentScopeTable->Delete(name);
    }

    void printCurrentScopeTable()
    {
        currentScopeTable->Print(1);
    }

    void printAllScopeTable()
    {
        ScopeTable *temp = currentScopeTable;
        int tabs = 1;
        while (temp)
        {
            temp->Print(tabs);
            tabs++;
            temp = temp->getParentScope();
        }
    }

    SymbolInfo *LookUp(std::string name)
    {
        int i = 1;
        ScopeTable *temp = currentScopeTable;
        if (temp == nullptr)
        {
            return nullptr;
        }
        while (temp)
        {
            SymbolInfo *symbol = temp->lookUp(name);
            if (symbol)
                return symbol;
            temp = temp->getParentScope();
        }
        std::cout << "\t'" << name << "' not found in any of the ScopeTables" << std::endl;
        return nullptr;
    }

    int getTotalCollision()
    {
        // std::cout<<"Inside total collision counter function\n";
        ScopeTable *temp = currentScopeTable;
        int total = 0;
        while (temp)
        {
            // std::cout<<"Current scope collision" << temp->getNumCollision()<<std::endl;
            total += temp->getNumCollision();
            temp = temp->getParentScope();
        }
        return total+deletedScopeTableCollision;
    }
};

#endif // SYMBOL_TABLE_H