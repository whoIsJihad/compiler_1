#include <bits/stdc++.h>
using namespace std;
unsigned int (*hash_func)(std::string, unsigned int) = SDBMHash; // Define the default hash function

unsigned int SDBMHash(std::string str, unsigned int num_buckets) {
    unsigned int hash = 0;
    unsigned int len = str.length();

    for (unsigned int i = 0; i < len; i++) {
        hash = ((str[i]) + (hash << 6) + (hash << 16) - hash) % num_buckets;
    }

    return hash;
}

unsigned int simpleHash(std::string s, unsigned int numBucket) {
    const int p = 31;
    const int m = 1e9 + 9;
    unsigned int hash_value = 0;
    long long p_pow = 1;
    for (char c : s) {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value % numBucket;
}

unsigned int djb2(std::string str, unsigned int numBucket) {
    unsigned int hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % numBucket;
}


class TokenArray {
    public:
    std::string* tokens;
    size_t size;
    ~TokenArray(){
        delete [] tokens;
    }
};
class Tokenizer {
private:
    std::string input_;

    // Helper to trim leading/trailing spaces
    std::string trim(const std::string& str) {
        size_t start = 0;
        size_t end = str.length();
        while (start < end && str[start] == ' ') ++start;
        while (end > start && str[end - 1] == ' ') --end;
        return start < end ? str.substr(start, end - start) : "";
    }

    // Helper to count tokens
    size_t countTokens() {
        size_t count = 0;
        bool in_token = false;
        for (size_t i = 0; i < input_.length(); ++i) {
            if (input_[i] == ' ') {
                in_token = false;
            } else if (!in_token) {
                in_token = true;
                ++count;
            }
        }
        return count;
    }

public:
    Tokenizer(const std::string& input) : input_(trim(input)) {}

    // Structure to return array and size
   

    // Returns a dynamically allocated array of tokens
    TokenArray getTokens() {
        TokenArray result = { nullptr, 0 };
        
        // Count tokens to allocate exact size
        result.size = countTokens();
        if (result.size == 0) return result;

        // Allocate array
        result.tokens = new std::string[result.size];
        
        // Tokenize
        std::string token;
        bool in_token = false;
        size_t token_index = 0;

        for (size_t i = 0; i < input_.length(); ++i) {
            char c = input_[i];
            if (c == ' ') {
                if (in_token) {
                    result.tokens[token_index++] = token;
                    token.clear();
                    in_token = false;
                }
                continue;
            }
            in_token = true;
            token += c;
        }

        // Add last token if exists
        if (in_token) {
            result.tokens[token_index] = token;
        }

        return result;
    }

    // Static method to free the token array
    static void freeTokens(TokenArray& arr) {
        delete[] arr.tokens;
        arr.tokens = nullptr;
        arr.size = 0;
    }
};




class SymbolInfo
{
private:
    std::string name;
    std::string symbolType;
    SymbolInfo *next;

public:
    SymbolInfo(std::string name, std::string symbolType)
    {
        this->name = name;
        this->next = nullptr;
        this->symbolType = symbolType;
    }

    std::string getName()
    {
        return this->name;
    }

    std::string getType()
    {
        return this->symbolType;
    }

    void setName(std::string name)
    {
        this->name = name;
    }

    void setType(std::string symbolType)
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
    int id;
    int numBuckets;
    SymbolInfo **hashTable;
    ScopeTable *parentScope;
    int numCollision;

public:
    ScopeTable(int n, int id = 1)
    {
        hashTable = new SymbolInfo *[n];
        this->id = id;
        numBuckets = n;
        parentScope = nullptr;
        this->numCollision = 0;
        for (int i = 0; i < numBuckets; i++)
            hashTable[i] = nullptr;
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

    bool Insert(SymbolInfo *symbol)
    {
        int index = hash_func(symbol->getName(), numBuckets) % numBuckets;
        SymbolInfo *temp = hashTable[index];
        if (temp != nullptr)
            numCollision++;
        SymbolInfo *prev = nullptr;
        int i = 1;
        while (temp)
        {
            if (temp->getName() == symbol->getName())
            {
                return false;
            }
            prev = temp;
            temp = temp->getNext();
            i++;
        }

        if (prev == nullptr)
        {
            hashTable[index] = symbol;
            return true;
        }

        prev->setNext(symbol);
        return true;
    }

    SymbolInfo *lookUp(std::string name)
    {
        if (hashTable == nullptr)
        {
            return nullptr;
        }
        int index = hash_func(name, numBuckets) % this->numBuckets;
        SymbolInfo *temp = hashTable[index];

        int i = 1;

        if (temp == nullptr)
        {
            return nullptr;
        }

        while (temp)
        {
            if (temp->getName() == name)
            {
                numCollision++;

                return temp;
            }
            temp = temp->getNext();
            i++;
        }
        numCollision++;

        return nullptr;
    }

    bool Delete(std::string name)
    {
        int index = hash_func(name, numBuckets) % this->numBuckets;
        SymbolInfo *temp = hashTable[index];
        SymbolInfo *prev = nullptr;
        int i = 1;

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
                prev->setNext(temp->getNext());
                numCollision++;
                delete temp;
                return true;
            }
            i++;
            prev = temp;
            temp = temp->getNext();
        }
        if (!hashTable[index])
            numCollision++;
        return false;
    }

    void Print(int tabCount = 0)
    {
        std::string tabs(tabCount, '\t');
        for (int i = 0; i < numBuckets; i++)
        {
            SymbolInfo *temp = hashTable[i];
            while (temp)
            {
                temp = temp->getNext();
            }
            
        }
    }

    void setParentScope(ScopeTable *scopeTable)
    {
        this->parentScope = scopeTable;
    }

    ScopeTable *getParentScope()
    {
        return this->parentScope;
    }

    int getNumBuckets()
    {
        return this->numBuckets;
    }

    int getID()
    {
        return this->id;
    }
    int getNumCollision(){
        return numCollision;
    }
};


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
        newScopeTable->setParentScope(currentScopeTable);
        currentScopeTable = newScopeTable;
    }

    void exitScope()
    {
        if (currentScopeTable == nullptr || currentScopeTable->getParentScope() == nullptr)
            return;

        ScopeTable *toDelete = currentScopeTable;

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
        return nullptr;
    }

    int getTotalCollision()
    {
        ScopeTable *temp = currentScopeTable;
        int total = 0;
        while (temp)
        {
            total += temp->getNumCollision();
            temp = temp->getParentScope();
        }
        return total + deletedScopeTableCollision;
    }
};

int main(int argc,char * argv[]){
    std::string input, output, hash_name;
    if (argc == 3)
    {

        input = argv[1];
        output = argv[2];
        hash_func = SDBMHash;
    }
    else if (argc == 4)
    {
        input = argv[1];
        output = argv[2];
        hash_name = argv[3];
        if (hash_name == "SDBMHash")
        {
            hash_func = SDBMHash;
        }
        else if (hash_name == "simpleHash")
        {
            hash_func = simpleHash;
        }
        else if (hash_name == "djb2")
        {
            hash_func = djb2;
        }
    }
    std::cout << input << " " << output << " " << hash_name << std::endl;

    freopen(input.c_str(), "r", stdin);
    freopen(output.c_str(), "w", stdout);
    int numberOfBuckets;
    std::cin >> numberOfBuckets;
    
    ScopeTable* currentScopeTable = new ScopeTable(numberOfBuckets);
    std::cout << "\n\tScopeTable# " << currentScopeTable->getID() << " created" << std::endl;
    SymbolTable* symbolTable = new SymbolTable(currentScopeTable);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    int cmdCount = 1;

    while (true)
    {
        
        std::string cmd;
        std::getline(std::cin, cmd);
        std::cout << "Cmd " << cmdCount++ << ": " << cmd << "\n";
        Tokenizer tokenizer(cmd);
        TokenArray tokenisedCommand = tokenizer.getTokens();
        char ch = tokenisedCommand.tokens[0][0];
        switch (ch)
        {
        case 'I':
        {
            std::string symbolName, symbolType;
            symbolName = tokenisedCommand.tokens[1];
            if (tokenisedCommand.size > 2)
                symbolType = tokenisedCommand.tokens[2];
            else
                symbolType = "";
            std::string res = "";
            if (symbolType == "FUNCTION")
            {
                int numTokens = tokenisedCommand.size;
                res += "," + tokenisedCommand.tokens[3] + "<==(";
                for (int i = 4; i < numTokens ; i++)
                {
                    res += (tokenisedCommand.tokens[i] + ",");
                }
                res.pop_back();
                res.push_back(')');
            }

            if (symbolType == "STRUCT" || symbolType == "UNION")
            {
                int numTokens = tokenisedCommand.size;
                res += ",{";
                for (int i = 3; i < numTokens - 1; i = i + 2)
                {
                    res += ("(" + tokenisedCommand.tokens[i] + "," + tokenisedCommand.tokens[i + 1] + "),");
                }
                res.pop_back();
                res += "}";
            }

            SymbolInfo* symbol = new SymbolInfo(symbolName, symbolType + res);
            bool insertTed=symbolTable->Insert(symbol);
            if(!insertTed) delete symbol;
            break;
        }
        case 'L':
        {
            if (tokenisedCommand.size != 2)
            {
                std::cout << "\tNumber of parameters mismatch for the command L" << std::endl;
                break;
            }
            std::string name = tokenisedCommand.tokens[1];
            SymbolInfo* symbol = symbolTable->LookUp(name);
            break;
        }
        case 'D':
        {
            if (tokenisedCommand.size != 2)
            {
                std::cout << "\tNumber of parameters mismatch for the command D" << std::endl;
                break;
            }
            std::string name = tokenisedCommand.tokens[1];
            symbolTable->remove(name);
            break;
        }
        case 'P':
        {
            char option = tokenisedCommand.tokens[1][0];
            if (option == 'C')
            {
                symbolTable->printCurrentScopeTable();
            }
            else if (option == 'A')
            {
                symbolTable->printAllScopeTable();
            }
            break;
        }
        case 'S':
        {
            symbolTable->enterScope();
            break;
        }
        case 'E':
        {
            symbolTable->exitScope();
            break;
        }
        case 'Q':
        {
            delete symbolTable;
            return 0;
        }
        default:
        {

            std::cout << "Invalid command." << std::endl;
            break;
        }

        }
        
    }

    return 0;
}