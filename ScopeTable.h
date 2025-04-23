#ifndef SCOPE_TABLE_H
#define SCOPE_TABLE_H

#include <string>
#include <iostream>
#include "SymbolInfo.h"
#include "HashFunctions.h"

class ScopeTable {
private:
    int id;
    int numBuckets;
    SymbolInfo** hashTable;
    ScopeTable* parentScope;

public:
    ScopeTable(int n, int id = 1) {
        hashTable = new SymbolInfo*[n];
        this->id = id;
        numBuckets = n;
        parentScope = nullptr;
        for (int i = 0; i < numBuckets; i++)
            hashTable[i] = nullptr;
    }

    ~ScopeTable() {
        for (int i = 0; i < numBuckets; i++) {
            SymbolInfo* temp = hashTable[i];
            SymbolInfo* prev = nullptr;
            while (temp) {
                prev = temp;
                temp = temp->getNext();
                delete prev;
            }
        }
        delete[] hashTable;
    }

    bool Insert(SymbolInfo* symbol) {
        int index = hash_func(symbol->getName(), numBuckets) % numBuckets;
        SymbolInfo* temp = hashTable[index];
        SymbolInfo* prev = nullptr;
        int i = 1;
        while (temp) {
            if (temp->getName() == symbol->getName()) {
                std::cout << "\t'" << symbol->getName() << "' already exists in the current ScopeTable" << std::endl;
                return false;
            }
            prev = temp;
            temp = temp->getNext();
            i++;
        }

        if (prev == nullptr) {
            hashTable[index] = symbol;
            std::cout << "\tInserted in ScopeTable# " << this->id << " at position " << index + 1 << ", " << i << std::endl;
            return true;
        }

        prev->setNext(symbol);
        std::cout << "\tInserted in ScopeTable# " << this->id << " at position " << index + 1 << ", " << i << std::endl;
        return true;
    }

    SymbolInfo* lookUp(std::string name) {
        std:: cout<<" the code is now in scope table look up function"<<std :: endl;
        if (hashTable == nullptr) {
            return nullptr;
        }
        int index = hash_func(name, numBuckets) % this->numBuckets;
        SymbolInfo* temp = hashTable[index];
        
        int i = 1;
        
        if (temp == nullptr) {
            return nullptr;
        }
        
        while (temp) {
            if (temp->getName() == name) {
                std::cout << "\t'" << name << "' found in ScopeTable at position " << index + 1 << " , " << i << std::endl;
                return temp;
            }
            temp = temp->getNext();
            i++;
        }

        return nullptr;
    }

    bool Delete(std::string name) {
        int index = hash_func(name, numBuckets) % this->numBuckets;
        SymbolInfo* temp = hashTable[index];
        SymbolInfo* prev = nullptr;
        if (temp && temp->getName() == name) {
            hashTable[index] = temp->getNext();
            delete temp;
            return true;
        }
        int i = 1;
        while (temp) {
            if (temp->getName() == name) {
                std::cout << "\tDeleted '" << name << "' from ScopeTable# " << this->id << " at position " << index + 1 << " ," << i << std::endl;
                prev->setNext(temp->getNext());
                delete temp;
                return true;
            }
            prev = temp;
            temp = temp->getNext();
        }
        std::cout << "\tNot found in the current ScopeTable" << std::endl;
        return false;
    }

    void Print(int tabCount = 0) {
        std::string tabs(tabCount, '\t');
        std::cout << tabs << "ScopeTable# " << this->id << std::endl;
        for (int i = 0; i < numBuckets; i++) {
            std::cout << tabs << i + 1 << "--> ";
            SymbolInfo* temp = hashTable[i];
            while (temp) {
                std::cout << "<" << temp->getName() << "," << temp->getType() << "> ";
                temp = temp->getNext();
            }
            std::cout << std::endl;
        }
    }

    void setParentScope(ScopeTable* scopeTable) {
        this->parentScope = scopeTable;
    }

    ScopeTable* getParentScope() {
        return this->parentScope;
    }

    int getNumBuckets() {
        return this->numBuckets;
    }

    int getID() {
        return this->id;
    }
};

#endif // SCOPE_TABLE_H