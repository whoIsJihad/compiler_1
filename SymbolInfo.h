#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include <string>

class SymbolInfo {
private:
    std::string name;
    std::string symbolType;
    SymbolInfo* next;

public:
    SymbolInfo(std::string name, std::string symbolType) {
        this->name = name;
        this->next = nullptr;
        this->symbolType = symbolType;
    }

    std::string getName() {
        return this->name;
    }

    std::string getType() {
        return this->symbolType;
    }

    void setName(std::string name) {
        this->name = name;
    }

    void setType(std::string symbolType) {
        this->symbolType = symbolType;
    }

    SymbolInfo* getNext() {
        return this->next;
    }

    void setNext(SymbolInfo* next) {
        this->next = next;
    }
};

#endif // SYMBOL_INFO_H