#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <iostream>     //cout
#include <cmath>        //abs

template <typename T, std::size_t N>
class Array
{
private:
    T podaci[N];

public:
    template <typename P, typename R, std::size_t Q>
    friend bool operator==(Array<P, Q> const&, Array<R, Q> const&);

    // Dodijeljeni konstruktor (bez inicijalizacije)
    Array() {}

    // Konstruktor koji uzima vrednost za inicijalizaciju svih elemenata
    explicit Array(const T& vrijednost)
    {
        for (T& element : podaci)
            element = vrijednost;
    }

    // Konstruktor inicijalizacijskom listom
    Array(std::initializer_list<T> vrijednosti)
    {
        if (vrijednosti.size() > N)
            throw std::runtime_error("Broj elemenata u inicijalizacijskoj listi je veci od duljine liste");

        std::copy(vrijednosti.begin(), vrijednosti.end(), podaci);
    }

    // Konstruktor kopije kopiranjem
    Array(const Array& kopirana)
    {
        std::copy(kopirana.podaci, kopirana.podaci + N, podaci);
    }

    // Operator pridruživanja kopiranjem
    Array& operator=(const Array& kopirana)
    {
        if (this != &kopirana)
            std::copy(kopirana.podaci, kopirana.podaci + N, podaci);

        return *this;
    }

    // Operator indeksiranja
    T& operator[](std::size_t indeks)
    {
        if (indeks >= N)
            throw std::out_of_range("U listi ne postoji tako velik indeks");

        return podaci[indeks];
    }

    // Konstantni operator indeksiranja
    const T& operator[](std::size_t indeks) const
    {
        if (indeks >= N)
            throw std::out_of_range("U listi ne postoji tako velik indeks");

        return podaci[indeks];
    }

    // Iteratori za range-for petlju
    T* begin() { return podaci; }
    T* end() { return podaci + N; }
    const T* begin() const { return podaci; }
    const T* end() const { return podaci + N; }

    // Getter velicine liste
    std::size_t size() { return N; }
};

// Simetricni operator usporedbe (liste iste duljine)
template <typename P, typename R, std::size_t N>
bool operator==(Array<P, N> const& prvaLista, Array<R, N> const& drugaLista)
{
    if((std::is_same<P, float>::value || std::is_same<P, double>::value)
        && (std::is_same<R, float>::value || std::is_same<R, double>::value))
    {
        const auto relativniEpsilon = 0.0001;    // 0.01%
        for(std::size_t i = 0; i < N; i++)
        {
            auto a = std::abs(prvaLista[i]);
            auto b = std::abs(drugaLista[i]);
            auto redVelicine = (a > b ? a : b);
            if(std::abs(prvaLista[i] - drugaLista[i]) > relativniEpsilon * redVelicine)
                return false;
        }

        return true;
    }
    else
    {
        for(std::size_t i = 0; i < N; i++)
            if(prvaLista[i] != drugaLista[i])
                return false;
        return true;
    }
}