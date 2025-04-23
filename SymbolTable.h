#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ScopeTable.h"

class SymbolTable {
private:
    ScopeTable* currentScopeTable;
    int cnt;

public:
    SymbolTable(ScopeTable* currentScopeTable) {
        cnt = 1;
        this->currentScopeTable = currentScopeTable;
    }

    SymbolTable() {
        this->currentScopeTable = nullptr;
    }

    ~SymbolTable() {
        ScopeTable* temp = currentScopeTable;
        while (temp) {
            ScopeTable* currentScopeTable = temp;
            temp = temp->getParentScope();
            delete currentScopeTable;
        }
        currentScopeTable = nullptr;
    }

    void enterScope() {
        if (currentScopeTable == nullptr) {
            currentScopeTable = new ScopeTable(10);
            return;
        }
        ScopeTable* newScopeTable = new ScopeTable(currentScopeTable->getNumBuckets(), cnt++);
        std::cout << "\tScopetable# " << newScopeTable->getID() << " created\n";
        newScopeTable->setParentScope(currentScopeTable);
        currentScopeTable = newScopeTable;
    }

    void exitScope() {
        if (currentScopeTable == nullptr) return;
        if (currentScopeTable->getParentScope() == nullptr) {
            delete currentScopeTable;
            currentScopeTable = nullptr;
            return;
        }
        ScopeTable* toDelete = currentScopeTable;
        currentScopeTable = currentScopeTable->getParentScope();
        delete toDelete;
    }

    bool Insert(SymbolInfo* symbol) {
        if (this->currentScopeTable == nullptr)
            return false;
        return this->currentScopeTable->Insert(symbol);
    }

    bool remove(std::string name) {
        if (!this->currentScopeTable)
            return false;
        return currentScopeTable->Delete(name);
    }

    void printCurrentScopeTable() {
        currentScopeTable->Print(1);
    }

    void printAllScopeTable() {
        ScopeTable* temp = currentScopeTable;
        int tabs = 1;
        while (temp) {
            temp->Print(tabs);
            tabs++;
            temp = temp->getParentScope();
        }
    }

    SymbolInfo* LookUp(std::string name) {
        int i=1;
        ScopeTable* temp = currentScopeTable;
        if (temp == nullptr) {
            return nullptr;
        }
        while (temp) {
            std::cout<<i++<<"th iteration inside lookup symbol table\n";
            SymbolInfo* symbol = temp->lookUp(name);
            if (symbol)
                return symbol;
            temp = temp->getParentScope();
        }
        std::cout << "\t'" << name << "' not found in any of the ScopeTables" << std::endl;
        return nullptr;
    }
   
};

#endif // SYMBOL_TABLE_H