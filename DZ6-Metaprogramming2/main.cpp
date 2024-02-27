#include <fmt/format.h>
#include <type_traits>
#include <array>
#include <cassert>
#include <cstdint> // za (u)intptr_t
#include <vector>
#include <unordered_set>

// Metafunkcija koja vraća tip
template <typename T>
struct TypeIdentity {
    using type = T;
};

// Metafunkcija koja vraca vrijednost
template <class T, T v>
struct IntegralConstant {
    static constexpr T value = v;
};

template <bool B>
using BoolConstant_v = IntegralConstant<bool, B>;

using FalseType = BoolConstant_v<false>;
using TrueType = BoolConstant_v<true>;

/////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct RemoveReference : TypeIdentity<T> {};

template<typename T>
struct RemoveReference<T&> : TypeIdentity<T> {};

template<typename T>
struct RemoveReference<T&&> : TypeIdentity<T> {};

template<typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

/////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct AddLValueReference : TypeIdentity<T&> {};

template<typename T>
struct AddLValueReference<T&> : TypeIdentity<T&> {};

template<typename T>
struct AddLValueReference<T&&> : TypeIdentity<T&> {};

template<>
struct AddLValueReference<void> : TypeIdentity<void> {};

template<typename T>
using AddLValueReference_t = typename AddLValueReference<T>::type;

/////////////////////////////////////////////////////////////////////////////////////////
template<class T>
struct IsArray : FalseType {};

template<class T>
struct  IsArray<T[]> : TrueType {};
 
template<class T, std::size_t N >
struct IsArray<T[N]> : TrueType {};

template<typename T>
inline constexpr auto IsArray_v = IsArray<T>::value;

/////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename S, std::size_t N>
struct plusReturnTypeImplementation {
    using type = std::array< decltype(std::declval<T>() + std::declval<S>()), N>;
};

template<typename T, typename S, std::size_t N>
using retType_t = typename plusReturnTypeImplementation<T, S, N>::type;

template<typename T, typename S, std::size_t N>
retType_t<T, S, N> operator+(std::array<T, N> lista1, std::array<S, N> lista2)
{
    std::array<decltype(std::declval<T>() + std::declval<S>()), N> rez;
    for (std::size_t i = 0; i < N; i++)
        rez[i] = lista1[i] + lista2[i];

    return rez;
}

/////////////////////////////////////////////////////////////////////////////////////////
template<typename B>
TrueType Converter(const volatile B*);

template<typename B>
FalseType Converter(...);

template<typename B, typename D>
struct IsBaseOf{
    static constexpr bool value = decltype(Converter<B>(std::declval<D*>()))::value;
};

template<typename B>
struct IsBaseOf<B, void> : FalseType {};

template<typename D>
struct IsBaseOf<void, D> : FalseType {};

template<>
struct IsBaseOf<void, void> : FalseType {};

/////////////////////////////////////////////////////////////////////////////////////////
template<typename Container, typename = std::void_t<> >
struct HasConstReverseIterator : FalseType{};

template<typename Container>
struct HasConstReverseIterator<Container, std::void_t<typename Container::const_reverse_iterator> > : TrueType{};

template<typename Container>
constexpr inline bool hasConstReverseIterator_v = HasConstReverseIterator<Container>::value;

template<typename Container, bool = hasConstReverseIterator_v<Container> >
struct ConstReverseIteratorType {
    using type = typename Container::const_reverse_iterator;
};

template<typename Container>
struct ConstReverseIteratorType<Container, false>{};

/////////////////////////////////////////////////////////////////////////////////////////
template<typename T, typename = std::void_t<> >
struct hasMemberImpl_x : FalseType{};

template <typename T>
struct hasMemberImpl_x<T, std::void_t< decltype(std::declval<T>().x)> > : TrueType {};

template<typename T, typename R, typename... Args>
struct hasMemberImpl_x<T(R(Args...)), std::void_t<> > : FalseType {};

template<typename T>    
constexpr bool hasMember_x = hasMemberImpl_x<T>::value;



// Klase za testiranje
class B {};
class D : public B {};
class C{};

class E : public B, public C {};

class Base{
    public:
    virtual ~Base() = default;
};
class Derived : public Base {};


class AAA{
    public:
        int x;
};
class BBB{
    public:
        void x() const {}
};
class CCC{
    int y;
};


// Ne mijenjati main!
int main(){

    // 1. 
    static_assert(std::is_same_v<RemoveReference_t<int &>, int>);
    static_assert(std::is_same_v<RemoveReference_t<int &&>, int>);
    static_assert(std::is_same_v<RemoveReference_t<int const &>, int const>);

    // 2.
    static_assert(std::is_same_v<AddLValueReference_t<int>, int&>);
    static_assert(std::is_same_v<AddLValueReference_t<int&>, int&>);
    static_assert(std::is_same_v<AddLValueReference_t<int&&>, int&>);
    static_assert(std::is_same_v<AddLValueReference_t<void>, void>);

    // 3. 
    static_assert(IsArray_v<int> == false);
    static_assert(IsArray_v<int[]> == true);
    static_assert(IsArray_v<int[3]> == true);

    // 4.
    std::array<int,3> a{1,2,3};
    std::array<float,3> b{1.0f, 2.0f, 3.0f};
    auto c = a+b;
    assert(c[0] == 2);
    assert(c[1] == 4);
    assert(c[2] == 6);
    static_assert(std::is_same_v<std::decay_t<decltype(c[0])>, float>); 


    // 5. 
    static_assert( IsBaseOf<B,D>::value );
    static_assert( !IsBaseOf<B,C>::value );
    static_assert( IsBaseOf<B,B>::value );
    static_assert( !IsBaseOf<int,long>::value );
    static_assert( !IsBaseOf<void,void>::value );
    static_assert( !IsBaseOf<void,B>::value );

    static_assert( IsBaseOf<const B,D>::value );
    static_assert( IsBaseOf<B, volatile D>::value );
    
    static_assert( IsBaseOf<B,E>::value );
    static_assert( IsBaseOf<C,E>::value );

    // 6. 
    static_assert(hasConstReverseIterator_v<std::vector<int>>);
    static_assert(!hasConstReverseIterator_v<std::unordered_set<int>>);

    ConstReverseIteratorType<std::vector<int>>::type it;
//    ConstReverseIteratorType<std::unordered_set<int>>::type it1;  // ne kompilira se
   
    // 7. 

    static_assert(hasMember_x<AAA>);
    static_assert(!hasMember_x<BBB>);
    static_assert(!hasMember_x<CCC>);
   
    return 0;
}
