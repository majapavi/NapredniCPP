#include "gtest/gtest.h"
#include "../src/array.h"
#include <iostream>			//cout
#include <utility>			//unique_ptr

#include <type_traits>
#include <memory>
#include <stdexcept>

// Definiranje tipova koji ce se testirati u testu parametriziranom tipovima
using AllTypes = ::testing::Types<char, int, long, float, double>;

// Definiranje fixture klase za testove parametrizirane tipovima
template <typename T>
class FixtureZaParametre : public testing::Test
{
public:
	Array<T, 4> LISTA_DEFAULT;

	T PARAMETAR;
	const std::size_t N_PARAM = 103;
	Array<T, 103> LISTA_PARAM;

	std::initializer_list<T> INIC_LISTA;
	const std::size_t N_INIC = 3;
	Array<T, 3> LISTA_INIC;

	void SetUp()
	{
		if (std::is_same_v<T, char>)
			PARAMETAR = 'G';
		else if (std::is_same_v<T, float>)
			PARAMETAR = -7.1;
		else if (std::is_same_v<T, double>)
			PARAMETAR = 22.565365;
		else if (std::is_same_v<T, long>)
			PARAMETAR = 4364245;
		else
			PARAMETAR = 70;
		
		LISTA_PARAM = Array<T, 103>(PARAMETAR);

		INIC_LISTA = {PARAMETAR, PARAMETAR, PARAMETAR};
		LISTA_INIC = Array<T, 3>({ PARAMETAR, PARAMETAR, PARAMETAR });
	}
};

TYPED_TEST_SUITE(FixtureZaParametre, AllTypes);

// Test za defaultni konstruktor
TYPED_TEST(FixtureZaParametre, DefaultniKonstruktor)
{
	// Testiranje alokacije
	EXPECT_EQ(this->LISTA_DEFAULT.size(), 4);
}

// Test za konstruktor s jednim parametrom
TYPED_TEST(FixtureZaParametre, ParametriziraniKonstruktor)
{
	// Unutar testa kreiramo novu listu jednaku kao u fixture klasi
	// kako bi bili sigurni da testiramo samo konstruktor
	Array<TypeParam, 103> lista(this->PARAMETAR);
	
	// Testiranje alokacije
	EXPECT_EQ(lista.size(), this->N_PARAM);

	// Testiranje inicijalizacije   BOLJE JE ISKORISTITI STATIC CAST
	if(std::is_same_v<TypeParam, float>)
		EXPECT_FLOAT_EQ(lista[8], this->PARAMETAR);
	else if(std::is_same_v<TypeParam, double>)
		EXPECT_DOUBLE_EQ(lista[8], this->PARAMETAR);
	else
		EXPECT_EQ(lista[8], this->PARAMETAR);
}

// Test za konstruktor s inicijalizacijskom listom
TYPED_TEST(FixtureZaParametre, KonstruktorInicijalizacijskomListom)
{
	Array<TypeParam, 3> lista({this->PARAMETAR, this->PARAMETAR, this->PARAMETAR});

	// Testiranje alokacije
	EXPECT_EQ(lista.size(), this->N_INIC);

	// Testiranje inicijalizacije
	if(std::is_same_v<TypeParam, float>)
		EXPECT_FLOAT_EQ(lista[0], this->PARAMETAR);
	else if(std::is_same_v<TypeParam, double>)
		EXPECT_DOUBLE_EQ(lista[0], this->PARAMETAR);
	else
		EXPECT_EQ(lista[0], this->PARAMETAR);
		
	// Testiranje bacanja iznimke kod predaje viska parametara u inicijalizacijskoj listi
	EXPECT_THROW((Array<TypeParam, 2>{this->PARAMETAR, this->PARAMETAR, this->PARAMETAR}), std::runtime_error);
}

// Test za operatore indeksiranja
TYPED_TEST(FixtureZaParametre, OperatorIndeksiranja)
{
	// Konstantna verzija
	EXPECT_EQ((this->LISTA_PARAM)[this->N_PARAM - 2], this->PARAMETAR);

	// Nekonstanta verzija
	(this->LISTA_PARAM)[this->N_PARAM - 5] = 1;
	EXPECT_EQ((this->LISTA_PARAM)[this->N_PARAM - 5], 1);

	// Listu ostavljamo nepromijenjenu nakon izvodenja testa
	(this->LISTA_PARAM)[this->N_PARAM - 5] = this->PARAMETAR;
	EXPECT_EQ((this->LISTA_PARAM)[this->N_PARAM - 2], this->PARAMETAR);
 }

TEST(neparametriziraniTestovi, operatorIndeksiranja)
{
    Array<std::unique_ptr<int>, 3> lista;
    Array<std::unique_ptr<int>, 3> novaLista;

	for(int i = 0; i < 3; i++)
	{
		lista[i] = std::make_unique<int>(i);
		novaLista[i] = std::move(lista[i]);
	}

    // Testiranje unistenosti pocetne liste
	EXPECT_EQ(lista[2], nullptr);

    // Testiranje premještanja
    EXPECT_NE(novaLista[1], nullptr);
	for(int i = 0; i < 3; i++)
	    EXPECT_EQ(*(novaLista[i]), i);
}


// Test za konstruktor kopije kopiranjem
TYPED_TEST(FixtureZaParametre, KonstruktorKopije)
{
	Array<TypeParam, 103> kopija1((this->LISTA_PARAM));
	Array<TypeParam, 3> kopija2(this->LISTA_INIC);

	// Testiranje ispravnog kopiranja
	EXPECT_EQ(kopija1[this->N_PARAM - 3], (this->LISTA_PARAM)[this->N_PARAM - 3]);
	EXPECT_EQ(kopija2[0], (this->LISTA_INIC)[0]);

	//Testiranje nepromijenjenosti kopirane liste
	EXPECT_EQ((this->LISTA_PARAM)[this->N_PARAM - 1], this->PARAMETAR);
	EXPECT_EQ((this->LISTA_INIC)[0], this->PARAMETAR);
}

// Test za operator pridruzivanja kopiranjem
TYPED_TEST(FixtureZaParametre, OperatorPridruzivanja)
{
	Array<TypeParam, 103> kopija1 = (this->LISTA_PARAM);
	Array<TypeParam, 3> kopija2(this->LISTA_INIC);

	// Testiranje ispravnog kopiranja
	EXPECT_EQ(kopija1[this->N_PARAM - 2], (this->LISTA_PARAM)[this->N_PARAM - 2]);
	EXPECT_EQ(kopija2[1], (this->LISTA_INIC)[1]);

	//Testiranje nepromijenjenosti kopirane liste
	EXPECT_EQ((this->LISTA_PARAM)[this->N_PARAM - 1], this->PARAMETAR);
	EXPECT_EQ((this->LISTA_INIC)[0], this->PARAMETAR);
}

// Test za for-range petlju
TYPED_TEST(FixtureZaParametre, ForRangePetlja)
{
	std::cout << "---------------------------------------------------------------------------------------" << std::endl;

	// Testiranje range-for petlje na listi stvorenoj defaultnim konstruktorom
	EXPECT_NO_THROW
	(
		std::cout << "Defaultna lista: ";
		for(TypeParam i : (this->LISTA_DEFAULT))
			std::cout << i << " ";
		std::cout << std::endl;
	);

	// Testiranje range-for petlje na listi stvorenoj konstruktorom s parametrom
	EXPECT_NO_THROW
	(
		std::cout << "Parametrizirana lista: ";
		for(TypeParam i : (this->LISTA_PARAM))
			std::cout << i << " ";
		std::cout << std::endl;
	);

	// Testiranje range-for petlje na listi stvorenoj konstruktorom s inicijalizacijskom listom
	EXPECT_NO_THROW
	(
		std::cout << "Inicijalizacijska lista: ";
		for(TypeParam i : (this->LISTA_INIC))
			std::cout << i << " ";
		std::cout << std::endl;
	);

	std::cout << "---------------------------------------------------------------------------------------" << std::endl;
}

// Test opearatora usporedbe na listama razlicitih ali usporedivih tipova
TEST(testovi, operatorUsporedbe)
{
	// Testiranje za floating point tipove
	Array<float, 12> l1(3.14);
	Array<double, 12> l2(3.14);
	Array<double, 12> l3(3.14159);

	EXPECT_TRUE(l1 == l2);
    EXPECT_FALSE(l2 == l3);
    EXPECT_FALSE(l1 == l3);
    EXPECT_FALSE(l2 == l3);

	l1[0] = 0;
	EXPECT_FALSE(l1 == l2);

	// Testiranje za cijele brojeve
	Array<int, 12> l4(3425245);
	Array<long, 12> l5(3425245);
	Array<long, 12> l6(336425245);

	EXPECT_TRUE(l4 == l5);
    EXPECT_FALSE(l4 == l6);
    EXPECT_FALSE(l5 == l6);

	// Testiranje za float i long
	EXPECT_FALSE(l1 == l5);

	// Testiranje za charove
	Array<char, 12> l7('X');
	Array<char, 12> l8('X');
	EXPECT_TRUE(l7 == l8);
	EXPECT_TRUE(l7 == l7);
	l8[0] = 'Y';
	EXPECT_FALSE(l7 == l8);

	// Testiranje za char i broj
	Array<char, 12> l9(3.14);
	EXPECT_FALSE(l1 == l9);
}
