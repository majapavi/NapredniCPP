#pragma once 

#include <coroutine>
#include <cstddef>  // za std::byte
#include <string>
#include <utility> // za std::exchange()
#include <optional>
#include <cassert>
#include <iostream>

class Parser
{
public:
    struct Promise;
    using promise_type = Promise;
    using PromiseTypeHandle = std::coroutine_handle<promise_type>;

private:
    PromiseTypeHandle parserHandle;

public:
    struct Promise
    {
        std::optional<std::byte> current_byte; // ulazni podatak, preko co_await std::byte{}; 
        std::string current_frame; // povratna vrijednost korutine, preko co_yield frame;

        auto get_return_object() { return PromiseTypeHandle::from_promise(*this); }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() {
            throw std::runtime_error(std::string("Parsing error after ESC.\n"));
        }
        auto yield_value(std::string frame) { // sigurnije uzimati frame po vrijednosti
            current_frame = std::move(frame); // premjestiti ulaznu vrijednost u lokalnu varijablu korutine
            return std::suspend_always{}; // nako yieldanja suspendiramo korutinu kako bi u mainu ispisali
        }

        // koristimo return_void, a ne return _value, jer korutina nigdje ne koristi izraz co_return val;
         void return_void() {}
        
        // vraca odgovarajuci awaiter
        // b nema neko značenje. metoda set_byte() postavlja byte u current_byte!
        auto await_transform(std::byte b) { return ValueAwaiter{current_byte}; }
    };

    struct ValueAwaiter {
        // treba nam optional jer byte mozda jos nije spreman, tu provjeravamo jel je
        // v je zapravo referenca na current_byte
        std::optional<std::byte> & v;
        
        // provjera je li byte vec spreman, ako je - nastavi s obradom, ako nije - rezimiraj korutinu
        bool await_ready() { return v.has_value(); }
        void await_suspend(auto) {}
        // moramo izbristai v (current_byte) prije nego sto ga vratimo
        std::byte await_resume() {
            auto tmp = v.value();
            // If std::optional<T> object contains a value, destroy that value as if by value().T::~T(). Otherwise, there are no effects. 
            v.reset();
            return tmp; 
        }
    };

    Parser(PromiseTypeHandle h) : parserHandle{ h } { }
    ~Parser() { if (parserHandle) parserHandle.destroy(); }
    Parser(Parser const&) = delete;
    Parser& operator=(Parser const&) = delete;

    // vraca string koji predstavlja procitani podatak (pohranjen u _Promise_ objektu)
    // ukoliko citanje podatka jos nije zavrsilo vraca prazan string
    std::string get_frame() const {
        auto tmp = parserHandle.promise().current_frame;
        // nakon sto vrati string, mora ga obrisati
        parserHandle.promise().current_frame.clear();
        return tmp;
    }

    void set_byte(std::byte c){
        parserHandle.promise().current_byte = c;
        // samo rezimira ako korutina nije gotova
        if( !parserHandle.done() ) parserHandle.resume();
    }
};
