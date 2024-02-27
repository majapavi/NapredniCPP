#include <iostream>
#include <concepts>
#include <type_traits>
#include <span>
using std::cout;
using std::endl;


//Zadatak 1. Napisati koncept FPCont koji ispituje da li tip T 
//           ima metode begin() i end(), da li se povratna vrijednost od 
//           begin() dade dereferncirati te da li daje nešto konvertibilno u 
//           tip double. 

// Varijanta sa izdvajanjem posebnog koncepta koja ne radi
//template <typename T>
//concept RefAndConvToDouble = requires{
//    { std::declval<std::remove_reference_t<T>> } -> std::convertible_to<double>;
//};

template <typename T>
concept FPCont = requires(T x) {
    x.begin();
    x.end();
    // Ovaj nacin sa ugnjezdavanjem uvjeta radi
    requires std::convertible_to<std::remove_reference_t<decltype(*(x.begin()))>, double>;
    
    // Ova varijanta sa izdvajanjem posebnog koncepta ne radi
    //{ x.begin() } noexcept -> RefAndConvToDouble;
};

// Klase X,Y i Z služe za testiranje Zadatka 1.
class X{};

struct Y{
    void begin(){}
    void end(){}
};

struct Z{
    float * begin(){ return nullptr;}
    float * end(){ return nullptr; }
};


// Zadatak 2.  Napraviti koncept Same<T,S> koji je istina ako su 
//             tipovi T i S jednaki nakon zanemarivanja dekoracija const i
//             volatile. 

template <typename T, typename S>
concept Same = requires{
    std::same_as< std::remove_cv<T>, std::remove_cv<S> >;
};


// Zadatak 3. Napisati koncept Number koji je istinit za sve cjelobrojne 
//          tipove i sve tipove s pokretnim zarezom, osim za tip bool
//          i sve moguće char tipove. (bool i char-ovi su cjelobrojni tipovi).

template<typename T>
concept Number = (std::floating_point<T> || std::integral<T>) &&
                !std::same_as<T, bool> && !std::same_as<T, char> &&
                !std::same_as<T, wchar_t> && !std::same_as<T, char8_t> &&
                !std::same_as<T, char16_t > && !std::same_as<T, char32_t > &&
                !std::same_as<T, signed char > && !std::same_as<T, unsigned char >;

// Neograničeni predložak funkcije mult() - samo kako ne bi imali grešku pri kompilaciji.
auto mult(auto x, auto y){
    std::cerr << "Greska. Tip od x = " << typeid(x).name()
              << ". Tip od y = " << typeid(y).name() << "\n";
    return -1;
}

// Ograničeni predložak funkcije mult()
auto mult(Number auto x, Number auto y) { return x * y; }


// ZADATAK 4. Napraviti koncept Pointer koji predstavlja pointer. Pri tome je pointer (po
//          našoj definiciji)  "void *" ili nešto što se dade dereferencirati. 
//          Napravite predložak "print(auto const & val)" koji ispisuje vrijednost val 
//          te napravite preopterećenje predloška print() koje uzima pokazivač val i ispisuje *val. 
//          Ispravno preopterećenje postići koristeći koncept Pointer. 

template <typename T>
concept Pointer = std::is_pointer_v<T>;

template <typename T>
concept NullPointer = Pointer<T> && std::is_null_pointer_v<T>;

void print(auto const& val) { cout << "val, obicno: " << val << endl; }
void print(Pointer auto const& val){ cout << "val, pointer: " << (*val) << endl; }
void print(NullPointer auto const& val){ cout << "null pointer se ne moze dereferencirati" << endl; }


// ZADATAK 5.           // OVO RJESENJE NIJE DOBRO
// Treba napisati koncept Two koji proizvoljan niz argumenata zadovoljava ako svi argumenti 
// pripadaju zadanom skupu od dva tipa int i char.

// Zaustavljanje rekurzije
bool twoHelper() { return 1; }

// Rekurzivni pomocni funkcijski predlozak 
template<typename T, typename... Args>
bool twoHelper(T t, Args... args) {
    if (std::same_as<T, int> || std::same_as<T, char>)
        return 0;
    return twoHelper(args...); // Rekurzivni poziv s preostalim argumentima
}

// Koncept za odredivanje jesu li svi elementi predani funkciji tipa char ili int
template <typename... Args>
concept Two = requires(Args... args){
    true == twoHelper(args...);         // ISPITUJE DALI USPOREDBA IMA SMISLA, A NE REZULTAT USPOREDBE
};

 //Pomocna testna funkcija koja može uzeti proizvoljan broj parametara ali samo 
 //tipa int i char (u bilo kojem poretku).
template<typename... Args>
requires Two<int, char, Args...>
void f1(Args... args){
	std::cout << "OK\n";
}


/* Zadatak 6. Napisati koncept InPlaceEngine koji provjerava da tip
 *          ima metodu process() koja kao argument uzima std::span<double>.
 *          Klasa Wrapper je parametrizirana s tipom koji mora imati 
 *          metodu process(). Zamijenite opći predložak Wrapper s 
 *          predloškom ograničenim na tipove koji zadovoljavaju 
 *          koncept InPlaceEngine. Implementirajte metodu Wrapper::reset()
 *          tako da poziva Engine::reset() ako takva metoda postoji,
 *          a ne radi ništa ako ne postoji. Pri tome koristiti
 *          samo if constexpr i requires!
 */

template <typename T>
concept InPlaceEngine = requires(T e){
    e.process(std::declval<std::span<double>>());
};

// Polazni kod klase Wrapper - neograničeni predložak
template <InPlaceEngine Engine>
class Wrapper{
    public:
        Wrapper(Engine e) : engine_(std::move(e)) {}
        void process(std::span<double> buf){
            engine_.process(buf);
        }
        void reset(){
            if constexpr(requires{engine_.reset();}){
                engine_.reset();
            }
            else {
                cout << "reset ne postoji" << endl;
            }
        }
    private:
        Engine engine_;
};

// Engine klase za testiranje.
struct Engine1{
    void process(std::span<double> sp){ std::cout << "process\n"; }
};
struct Engine2{
    void process(std::span<double> sp){ std::cout << "process\n"; }
    void reset() const { std::cout << "resret\n"; }
};
struct Engine3{
};


