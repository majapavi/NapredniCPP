#include "parser_coro.h"

#include <vector>
#include <iostream>

// User defined literals
std::byte operator""_B(char c){ return std::byte(c); }
std::byte operator""_B(unsigned long long int c){ return std::byte(c); }

int main(){
     std::vector<std::byte> fakeBytes{
                           0x70_B, ESC, SOF, ESC,
                           'H'_B, 'e'_B, 'l'_B, 'l'_B, 'o'_B, ESC, SOF,
                           0x7_B, ESC, SOF, 'W'_B, 'o'_B, 'r'_B, 'l'_B, 'd'_B, ESC, SOF,
                           0x67_B, 'g'_B,'g'_B, 
                           ESC, SOF, ESC, 'H'_B, 'e'_B, 'r'_B, 'e'_B, ','_B, ESC, SOF,
                           ESC, SOF, 'I'_B,  ESC, SOF,
                           ESC, SOF, 'a'_B, 'm'_B, ESC, SOF, 'g'_B, 'o'_B};

   
     Parser parser = parse();
    for(auto b : fakeBytes)
    {
        parser.set_byte(b);
        
        if(auto const& frame = parser.get_frame(); frame.length() > 0)
            std::cout << frame << "\n"; // bivši callback
    }

    return 0;
}
