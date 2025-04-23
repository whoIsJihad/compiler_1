
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

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

#endif // TOKENIZER_H