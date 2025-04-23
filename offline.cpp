#include <iostream>
#include <limits>

#include "HashFunctions.h"
#include "SymbolTable.h"
#include "Tokenizer.h"

unsigned int (*hash_func)(std::string, unsigned int) = SDBMHash; // Define the default hash function

int main(int argc, char* argv[])
{
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
    std::cout << "\tScopeTable# " << currentScopeTable->getID() << " created" << std::endl;
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
                res += "," + tokenisedCommand.tokens[numTokens - 1] + "<==(";
                for (int i = 3; i < numTokens - 1; i++)
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
                std::cout << "Number of parameters mismatch for the command D" << std::endl;
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
            std::cout << "Exiting program." << std::endl;
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