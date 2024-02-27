**Zadatak**.  Ovaj zadatak ima za cilj upoznavanje s {fmt} bibliotekom.
Kroz cijeli se kolegij podrazumijeva da student zna kako koristiti {fmt}
biblioteku za formatiranje ispisa. Sve potrebne informacije o biblioteci
se mogu naći na sljedećim linkovima:

- [web stranica kolegija](https://web.math.pmf.unizg.hr/nastava/napcpp/format.html)
- [fmt.dev](https://fmt.dev/latest/index.html)
- [hackingcpp.com](https://hackingcpp.com/cpp/libs/fmt.html)

Zadatak je nužno napraviti bez korištenja IO-stream biblioteke.

a. Generirajte slučajni niz od 10 cijelih brojeva između 1 i 20  i spremite ih u jedan `std::array<int>`. Zatim ih ispište na sljedeći način:

````{verbatim}
 0:........13 ** 13........ ** ....13.... ** ...0xd....
 1:.........7 ** 7......... ** ....7..... ** ...0x7....
 2:........11 ** 11........ ** ....11.... ** ...0xb....
 3:.........8 ** 8......... ** ....8..... ** ...0x8....
 4:........10 ** 10........ ** ....10.... ** ...0xa....
 5:........15 ** 15........ ** ....15.... ** ...0xf....
 6:.........7 ** 7......... ** ....7..... ** ...0x7....
 7:.........6 ** 6......... ** ....6..... ** ...0x6....
 8:........13 ** 13........ ** ....13.... ** ...0xd....
 9:........13 ** 13........ ** ....13.... ** ...0xd....
````

Svaki broj je ispisan sa širinom 10, a u zadnjem stupcu je heksadecimalni zapis brojeva. 

b. Ispišite broj `pi` 10 puta na sljedeći način: 

````{verbatim}
 0:           3.142
 1:         +3.1416
 2:         3.14159
 3:       +3.141593
 4:       3.1415927
 5:     +3.14159265
 6:     3.141592654
 7:   +3.1415926536
 8:   3.14159265359
 9: +3.141592653590
````
c. Definirajte tri varijable tipa int. Otvorite datoteku imena `adrese.txt` i u nju ispišite adrese tih varijabli. Za otvaranje datoteke koristite `std::ofstream`. 
