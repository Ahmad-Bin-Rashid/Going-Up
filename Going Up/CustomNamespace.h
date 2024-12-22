#pragma once

#include <iostream>
#include <string>

namespace ct {

    // Custom Output Stream Class
    class CustomCout {
    public:
        CustomCout& operator|(const std::string& str) {
            std::cout << str;
            return *this;
        }

        CustomCout& operator|(const char* str) {
            std::cout << str; 
            return *this;
        }

        CustomCout& operator|(int value) {
            std::cout << value;
            return *this;
        }

        CustomCout& operator|(double value) {
            std::cout << value;
            return *this;
        }

        CustomCout& operator|(char ch) {
            std::cout << ch;
            return *this;
        }

        CustomCout& operator|(size_t value) {
            std::cout << value;
            return *this;
        }
    };

    CustomCout p;
}