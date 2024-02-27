#include <iostream>
#include <cassert>
#include <type_traits>

//Zadatak 1.

// a) C++03, rekurzija, specijalizacije predlozaka struktura, bez constexpr funkcija
// predlozak strukture koja izracunava binomni koeficijent
template <unsigned int d, unsigned int k>
struct BinomialCoefficient{
    static const unsigned int value = BinomialCoefficient<d - 1, k - 1>::value + BinomialCoefficient<d - 1, k>::value;
};

// specijalizacije predloska za izracunavanje binomnog koeficijenta, zaustavlaju rekurziju
template<unsigned int d>
struct BinomialCoefficient<d, 0> {
    static const unsigned int value = 1;
};

template<unsigned int d>
struct BinomialCoefficient<d, d> {
    static const unsigned int value = 1;
};

// predlozak strukture za izracunavanje trazene dimenzije polinoma
template <unsigned int d, unsigned int k>
struct Dim {
    static const unsigned int value = BinomialCoefficient<d + k, k>::value;
};


// b) C++11, constexpr funkcija, jedan return, rekurzija
constexpr unsigned int binCoeff(unsigned int d, unsigned int k) {
    return (d != k && k != 0 && d != 0) ? (binCoeff(d - 1, k - 1) + binCoeff(d - 1, k)) : 1;
}

constexpr unsigned int dim(unsigned int d, unsigned int k){
    return binCoeff(d + k, k);
}


// c) C++14, constexpr funkcija, dozvoljeno vise returnova, bez rekurzije
constexpr unsigned int dim14(unsigned int d, unsigned int k)
{
    long unsigned int result = 1;
    unsigned int i;
    for (i = 1; i <= d + k; i++)
        result *= i;
    for (i = 1; i <= k; i++)
        result /= i;
    for (i = 1; i <= d; i++)
        result /= i;

    return result;
}



// Zadatak 2. 
// a. C++11 verzija

constexpr int64_t p = 31;
constexpr int64_t m = 1000000009;

// potrebna je pomocna funkcija jer osim pokazivaca s, trebamo pamtiti i potenciju i
constexpr int64_t hash11Implementation(const char* s, int64_t i)
{
    return (s[i] == '\0') ? 0 :
        (hash11Implementation(s, i+1) * p + static_cast<int64_t>(s[i]) - 96) % m;
}

constexpr int64_t hash11(const char * s) { return hash11Implementation(s, 0); }


// b. C++14 verzija 
constexpr int64_t hash14(const char * s) 
{
    int64_t result = 0;
    int64_t pPower = 1;

    for(int i = 0; s[i] != '\0'; i++)
    {
        // oduzimamo 96 jer je ascii kod znaka 'a' 97
        result = (result + (static_cast<int>(s[i]) - 96) * pPower) % m;
        pPower = (pPower * p) % m;
    }

    return result;
}



int main()
{
    // Ovaj kod ne brisati! Možete dodavati svoje testove.
    // Zadatak 1.
    // 1. 
    static_assert(Dim<0, 3>::value == 1);
    static_assert(Dim<3, 0>::value == 1);
    static_assert(Dim<3, 1>::value == 4);
    static_assert(Dim<3, 2>::value == 10);
    static_assert(Dim<3, 3>::value == 20);
    static_assert(Dim<5, 3>::value == 56);
    // 2.
    static_assert(dim(0, 3) == 1);
    static_assert(dim(3, 0) == 1);
    static_assert(dim(3, 1) == 4);
    static_assert(dim(3, 2) == 10);
    static_assert(dim(3, 3) == 20);
    static_assert(dim(5, 3) == 56);
    //3.
    static_assert(dim14(0, 3) == 1);
    static_assert(dim14(3, 0) == 1);
    static_assert(dim14(3, 1) == 4);
    static_assert(dim14(3, 2) == 10);
    static_assert(dim14(3, 3) == 20);
    static_assert(dim14(5, 3) == 56);
    

    // Zadatak 2.
    constexpr const char * s1="abcder";
    constexpr const char * s2="ertamabcdernn";
    constexpr const char * s3="afgtbbnjiocbfrewqdeloikjuhbvcr";
    constexpr const char * s4="utdtbbnjiocbzrewddegoimjeh";
    //std::cout << hash11(s1) << "\n";
    //std::cout << hash11(s2) << "\n";
    //std::cout << hash11(s3) << "\n";
    //std::cout << hash11(s4) << "\n";
    static_assert(hash11(s1) == 520064433);
    static_assert(hash11(s2) == 202527068);
    static_assert(hash11(s3) == 615720090);
    static_assert(hash11(s4) == 814096530);

    static_assert(hash14(s1)==hash11(s1));
    static_assert(hash14("ertamabcdernn")==hash11("ertamabcdernn"));
    static_assert(hash14(s3)==hash11(s3));
    static_assert(hash14(s4)==hash11(s4));

    static_assert(hash14(s1) == 520064433);
    static_assert(hash14(s2) == 202527068);
    static_assert(hash14(s3) == 615720090);
    static_assert(hash14(s4) == 814096530);

    return 0;
}
