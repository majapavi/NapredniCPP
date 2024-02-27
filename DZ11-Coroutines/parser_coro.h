#pragma once

#include <coroutine>
#include <cstddef>  // za std::byte
#include <string>
#include <stdexcept>

#include "parser.h"

static const std::byte ESC{'H'};   // escape
static const std::byte SOF{ 0x10 };  // start of frame

Parser parse()
{
    while(true) {
        std::byte b = co_await std::byte{};
        if(b != ESC)
            continue;

        b = co_await std::byte{};
        if(b != SOF)
            continue;
        
        std::string frame{};
        while(true){
            b = co_await std::byte{};
            if(b == ESC){
                b = co_await std::byte{};
                if(b == SOF){
                    co_yield frame;
                    break;
                }
                else if(b != ESC)
                    throw std::runtime_error("Greška u ulaznom toku.");  // Greška 
            }
            frame += static_cast<char>(b);
        } // end while
    } // end while
}

