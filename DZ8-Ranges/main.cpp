#include "range/v3/all.hpp"  // to je dovoljno za range-v3.
                             // Pojedini elementi se mogu "includati" i pojedinačno. 
#include "fmt/format.h"
#include <iostream>
#include <string>
#include <optional>
#include <numeric>

using namespace ranges; // Nikako ne uključivati std!

struct Planet {
    std::string name;
    double gravity;
};


int main() {
    {
        // 1. Generirajte niz 1,2,2,3,3,3.......9,9,9,9,9,9,9,9,9.  
        auto const niz = views::iota(1, 10)
            | views::transform([](int x) { return views::repeat_n(x, x); })
            | views::join
            | to<std::vector<int>>();

        fmt::print("Niz: {}\n", fmt::join(niz, ","));
    }

    {
        // 2. Nađi planet s gravitacijom 11.15 m²/s. Rezultat vratiti (i ispisati)
        //    kao std::optional<std::string>.
        Planet const v[] = {{"Venus",8.87}, {"Mars",3.711}, {"Jupiter", 24.92},{"Neptun", 11.15},
                        {"Saturn", 10.44}, {"Uran",8.69}};

        auto svi_planeti = v
            | views::filter([](const Planet& p) { return p.gravity == 11.15; })
            | views::transform([](const Planet& x)  -> std::optional<std::string> { return x.name; })
            | to<std::vector<std::optional<std::string>>>;

        for (const auto& planet : svi_planeti)
            if (planet)
                std::cout << "Pronaden planet s gravitacijom 11.15: " << *planet << std::endl;
            else
                std::cout << "Nema planeta sa zadatom gravitacijom." << std::endl;
    }

    {
        // 3. Aproksimirajte broj  π koristeći Gregory - Leibnizovu formulu 
        // π ≈ 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 - ...). Ispišite rezultat s 30 članova.
        auto niz = views::iota(0, 60)
            | views::remove_if([](int x) {return x % 2 == 0; })
            | views::transform([](int x)
                {   if (x % 4 == 3) { return -1.0 / x; }
                else { return 1.0 / x; }
                });

        auto suma = accumulate(niz, 0.0);
        fmt::print("Broj pi kao suma 30 clanova: {}\n", 4*suma);
    }

    {
        // 4. Selektirati domenu u web adresi i konverirati ju u string. Ispisati. 
        auto const str = std::string{ "https://en.test.org" }; // -> org
        // Zasto ova varijanta ne radi?
        //auto domena = str
        //    | views::split_when([](int x) { return x == '.'; })
        //    | views::reverse
        //    | views::take(1)
        //    | views::join
        //    | to<std::string>;
        auto domena = str
            | ranges::views::reverse
            | ranges::views::take_while([](char c) { return c != '.'; })
            | ranges::views::reverse
            | ranges::to<std::string>;

        std::cout << "Domena: " << domena << std::endl;
    }

    {
        //5.  Pretvoriti CAMELCASE string u SNAKE_CASE.
        std::string str{"ovoJeMojaCamelCaseVarijabla"};
           
        auto snake_case = str
            | views::transform([](char c) { return std::isupper(c) ? fmt::format("_{}", char(std::tolower(c))) : fmt::format("{}", c); })
            | views::join
            | to<std::string>;
              
        fmt::print("Snake case: {}\n", snake_case); // ovo_je_moja_camel_case_varijabla
    }

    {
        //6.  Izračunati euklidsku udaljenost između dvije točke zadana tipom std::array<double,n>. 
        std::array<double,4> a{1,2,3,4}, b{4,3,2,1};

        double euklidska_udaljenost = std::sqrt(accumulate(views::zip_with([](double x, double y) { return std::pow(x - y, 2); }, a, b), 0.0));
        fmt::print("Euklidska udaljenost zadanih tocaka na 4 znacajne znamenke: {:.4g}\n", euklidska_udaljenost);
    }

    {
    //7.  Provjeriti korektnost ISBN-13 broja (na primjer 9780306406157).
    //    Polazeći s lijeva na desno treba naizmjenično množiti znamenke s 1 odnosno 3.
    //    (Prvu znamenku množimo s 1, drugu s 3, treću s 1 itd. Broj ima 13 znamenaka.) 
	//    Zatim zbrojimo tako dobivenih 
    //    prvih 12 vrijednosti i izračunamo ostatak sume modulo 10. Ako je dobiveni broj nakon toga 
    //    različit od nule, oduzmemo ga od 10. Izračunata vrijednost mora biti jednaka 13. znamenci.
     
        long int n = 9780306406157;
     
    }
    {
        //8.  Naći i ispisati sve troznamenkaste narcisoidne brojeve. Broj je narcisoidan ako je 
        //    jednak sumi svojih znameni dignutih na treću potenciju. Na primjer, 153 = 1³ + 5³ + 3³.

        auto narcisoidni_brojevi = views::iota(100, 1000) | views::filter(
            [](int n) {
                auto digits = views::ints(0, 3) | views::transform(
                    [n](int i) {
                        int digit = n / static_cast<int>(std::pow(10, i)) % 10;
                        return static_cast<int>(std::pow(digit, 3));
                    }
                );
                return accumulate(digits, 0) == n;
            }
        ) | to<std::vector<int>>;

        fmt::print("Troznamenkasti narcisoidni brojevi: {}\n", fmt::join(narcisoidni_brojevi, ","));
    }

    return 0;
}
