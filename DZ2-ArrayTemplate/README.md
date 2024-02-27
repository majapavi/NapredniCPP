**Zadatak**. U ovom zadatku vježbamo pisanje testova. Svi elementi jezika trebaju biti poznati od ranije. 

Porebno je implementirati predložak klase `Array<T,N>` koja predstavlja statički alocirano polje od `N` elemenata  tipa `T`. Tip indeksa je `std::size_t`.

Zahtjevi na klasu su sljedeći:

1. Klasa mora imati dodijeljeni konstruktor (koji ne uzima argumente) i konstruktor koji uzima vrijednost kojom inicijalizira sve elemente.  Dodijeljeni konstruktor ne mora inicijalizirati vrijednosti.
1. Klasa ima konstruktor inicijalizacijskom listom: na primjer, `Array<char,3> arr{'a','b'};`, koji u listi dozvoljava manji broj inicijalizacija, ali ne i veći. Ako je broj elemenata u inicijalizacijskoj listi veći od `N` izbacuje se `std::runtime` izuzetak.
1. Klasa  ima konstruktor kopije kopiranjem i operator pridruživanja kopiranjem. Operacije kopiranja i pridruživanja  premještanjem nisu potrebne. 
1. Klasa  ima operatore indeksiranja.
1. Klasa dozvoljava upotrebu _range-for_ petlje.
1. Klasa ima operator `==` definiran kao slobodna funkcija.  Operator može uspoređivati polja različitih tipova i iste dimenzije ukoliko su tipovi usporedivi.

Napisati testove koji testiraju sljedeće:

1. Konstruktore. Testirati dodijeljeni konstruktor i konstruktor koji uzima jedan parametar parametriziranim testom za tipove: char, int, long, float, double. 
1. Testirati konstruktor inicijalizacijskom listom. Testirati izbacivanje izuzetka kod suviše velikog broja inicijalizacijskih vrijednosti parametriziranim testom za tipove: char, int, long, float, double. 
1. Testirati operator indeksiranja (`[]`) u konstantnoj i nekonstantnoj verziji. Testirati operator indeksiranja na tipu elemenata koji se može samo premještati (npr. `std::unique_ptr<int>`, koristeći `std::move()`).
1. Testirati konstruktor kopije parametriziranim testom za tipove: char, int, long, float, double.
1. Testirati operator pridruživanja parametriziranim testom za tipove: char, int, long, float, double. 
1. Testirati _range-for_ petlju parameriziranim testom za tipove: char, int, long, float, double.
1. Testirati operator uspoređivanja za dva različita, ali usporediva tipa.
