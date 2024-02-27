#include <fmt/format.h>
#include <array>
#include <random>
#include <fmt/ostream.h>
#include <fstream>

using namespace std;
using fmt::print;

int main()
{
    // Generiranje uniformno generiranih slucajnih brojeva u rasponu 1-20 pomocu Mersenne Twister algoritma
    random_device sjeme;
    mt19937 generator(sjeme());
    uniform_int_distribution<std::mt19937::result_type> distribucija(1, 20);
    // Ispis liste integera pomocu fmt biblioteke
    array<int, 10> listaSlucajnih;
    print("\n");
    for(auto clan = 0; clan < listaSlucajnih.size(); clan++)
    {
        listaSlucajnih[clan] = distribucija(generator);
        print("{0}:{1:.>10} ** {1:.<10} ** {1:.^10} ** {1:.^#10x}\n", clan, listaSlucajnih[clan]);
    }
    print("\n");


    // Ispis broja pi s razlicitom preciznoscu i promjenjivom vidljivosti predznaka
    double pi = 3.141592653590;
    for(auto i = 0; i < 10; i++)
    {
        if(i % 2) // retci sa istaknutim predznakom
            print("{0}: {1:>+15.{2}f}\n", i, pi, i+3);
        else
            print("{0}: {1:>15.{2}f}\n", i, pi, i+3);
    }
    print("\n");


    // Zapis adresa triju int varijabli u ranije stvorenu datoteku "adrese.txt"
    int a, b, c;
    fstream izlaz("../log/adrese.txt");
    if(izlaz.is_open())
    {
        print(izlaz, "{} {} {}", static_cast<void*>(&a), static_cast<void*>(&b), static_cast<void*>(&c));
        izlaz.close();
    }
    else    // Bacanje greske u slucaju neuspjesnog otvaranja datoteke
        throw runtime_error("Neuspjesno otvaranje izlazne datoteke adrese.txt iz log foldera");

    return 0;
}