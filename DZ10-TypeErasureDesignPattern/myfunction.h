#pragma once
#include <utility>
#include <memory>

template <typename Sgn>
class MyFunction;

template <typename Ret, typename... Args>
class MyFunction<Ret(Args...)>
{
    // Definiramo tipove pokazivaca na funkcije koje koristimo za type-erasue obrazac
    // Parametra tipa char* koji se pojavljuju sluze za pretvorbu pokazivaca na neki
    // Funkcijski oblik i isto su dio type-erasure obrasca
    typedef Ret(*izvedi_tErased)(char*, Args&&...);
    typedef void (*stvori_tErased)(char*, char*);
    typedef void (*unisti_tErased)(char*);

    // Generičke funkcije svjesne tipa za pozivanje
    // Specijalizacije ovih funkcija nece biti kompatibilne s prethodno
    // deklariranim tipovima pokazivaca na funkcije, pa nam je potreban kast.
    template <typename Functor>
    static Ret izvediTemplate(Functor* fn, Args&&... args)
    {
        //// Kako radi ovo sa forward, obicna varijanta mi ne radi
        return (*fn)(std::forward<Args>(args)...);
    }

    template <typename Functor>
    static void stvoriTemplate(Functor* stvori_dst, Functor* stvori_src)
    {
        // Funktor mora biti kopirajuce konstruiran
        //// Zasto?
        new (stvori_dst) Functor(*stvori_src);
    }

    template <typename Functor>
    static void unistiTemplate(Functor* f)
    {
        f->~Functor();
    }

    // Pokazivaci na funkcije za pohranu ponasanja vezanog uz stvarni tip funkcije
    izvedi_tErased izvediImpl;
    stvori_tErased kreirajImpl;
    unisti_tErased unistiImpl;

    // Cuvanje pokazivaca na char kako bi se obrisale informacije o stvarnom tipu
    // Takoder, trebamo cuvati velicinu memorije povezanu s stvarnim pokazivacem
    //// Zasto tu stvari rade sa char[], ali ne i sa char*
    std::unique_ptr<char[]> data_ptr;
    size_t data_size;

public:
    MyFunction()
        : izvediImpl(nullptr)
        , kreirajImpl(nullptr)
        , unistiImpl(nullptr)
        , data_ptr(nullptr)
        , data_size(0)
    {}

    // Konstrukcija iz bilo kojeg tipa funkcijskog objekta
    template <typename Functor>
    MyFunction(Functor f)
        //// Koja je razlika izmedu static_cast i reinterpret_cast
        //// i zasto je tu potrebna ova druga
        : izvediImpl(reinterpret_cast<izvedi_tErased>(izvediTemplate<Functor>))
        , kreirajImpl(reinterpret_cast<stvori_tErased>(stvoriTemplate<Functor>))
        , unistiImpl(reinterpret_cast<unisti_tErased>(unistiTemplate<Functor>))
        , data_ptr(new char[sizeof(Functor)])
        , data_size(sizeof(Functor))
    {
        // Kopiranje funkcijskog objekta u interno skladiste uz gubitak podatka o tipu
        this->kreirajImpl(this->data_ptr.get(), reinterpret_cast<char*>(&f));
    }

    // Kopirajuci konstruktor
    MyFunction(MyFunction const& rhs)
        : izvediImpl(rhs.izvediImpl)
        , kreirajImpl(rhs.kreirajImpl)
        , unistiImpl(rhs.unistiImpl)
        , data_size(rhs.data_size)
    {
        if (this->izvediImpl) {
            // Kad izvor nije null funkcija, kopirajte njegov interni funkcijski objekt
            this->data_ptr.reset(new char[this->data_size]);
            this->kreirajImpl(this->data_ptr.get(), rhs.data_ptr.get());
        }
    }

    // Konstruktor premjestanjem
    MyFunction(MyFunction&& rhs) noexcept
        : izvediImpl(rhs.izvediImpl)
        , kreirajImpl(rhs.kreirajImpl)
        , unistiImpl(rhs.unistiImpl)
        , data_size(rhs.data_size)
    {
        if (this->izvediImpl) {
            this->data_ptr = std::move(rhs.data_ptr);
            rhs.izvediImpl = nullptr;
            rhs.kreirajImpl = nullptr;
            rhs.unistiImpl = nullptr;
            rhs.data_size = 0;
        }
    }

    ~MyFunction()
    {
        if (data_ptr != nullptr) {
            this->unistiImpl(this->data_ptr.get());
        }
    }

    // Ostali konstruktori, od nullptr, od pokazivača na funkciju
    //// Malo objasniti zasto ovo pokriva sve ostale slucajeve
    Ret operator()(Args&&... args)
    {
        return this->izvediImpl(this->data_ptr.get(), std::forward<Args>(args)...);
    }

    // Pridruzivanje kopiranjem
    MyFunction& operator=(const MyFunction& rhs)
    {
        if (this != &rhs) {
            this->izvediImpl = rhs.izvediImpl;
            this->kreirajImpl = rhs.kreirajImpl;
            this->unistiImpl = rhs.unistiImpl;
            this->data_size = rhs.data_size;

            if (this->izvediImpl) {
                this->data_ptr.reset(new char[this->data_size]);
                this->kreirajImpl(this->data_ptr.get(), rhs.data_ptr.get());
            }
        }

        return *this;
    }

    // Pridruzivanje premjestanjem
    MyFunction& operator=(MyFunction&& rhs) noexcept
    {
        if (this != &rhs) {
            this->izvediImpl = rhs.izvediImpl;
            this->kreirajImpl = rhs.kreirajImpl;
            this->unistiImpl = rhs.unistiImpl;
            this->data_size = rhs.data_size;

            if (this->izvediImpl) {
                this->data_ptr = std::move(rhs.data_ptr);
                rhs.izvediImpl = nullptr;
                rhs.kreirajImpl = nullptr;
                rhs.unistiImpl = nullptr;
                rhs.data_size = 0;
            }
        }

        return *this;
    }
};
