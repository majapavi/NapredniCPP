**Zadatak.**  Parsiranje niza okteta. 

Ulazni niz okteta predstavlja poruku kodiranu prema protokolu koji opisujemo niže. U datoteci `parser_classic.cpp`
nalazi se opisani protokol kodiran u funkciji `processNextByte()`. U datoteci se nalazi i test primjer u `main()` funkciji. 
U datoteci `parser_coro.h` se nalazi isti protokol kodiran kao korutina `parse()`.
Zadatak je napraviti infrastrukturu koja će omogućiti da korutina `parse()` obavi traženu zadaću.

**PROTOKOL**: Ovdje opisani protokol je jednostavna verzija zamišljenog mrežnog protokola koji poruku razbija na pakete (okvire).

Ulazni tok podataka je podijeljen u pakete (okvire, frames). Početak
poruke je označen s dva okteta: ESC i SOF (start of frame). Kraj poruke
je također označen s oktetima ESC i SOF. 
ESC je znak za izuzetak s kojim se označava da sljedeći znak nije dio 
paketa već je to kontrolni znak. Koristimo ga ispred SOF, koji je naš jedini kontrolni znak, ali i ispred EOF znaka
kada se nađe u paketu kako bi signalizirao da sljedeći ESC znak nije znak za izuzetak već bio paketa.
Na taj je način moguće slati ne samo oktete koji predstavljaju znakove veći sve moguće oktete. Znakovi koji nisu unutar paketa se ignoriraju.

U datoteci **parser_classic.cpp** gornji je protokol implementiran kao obična funkcija koja pri svakom pozivu čita
jedan oktet s ulaznog toka. Kako funkcija mora pamtiti stanje toka oslanja se na statičke varijable. 
**IMPLEMENTACIJA**: Imamo dva stanja: možemo biti unutar poruke koju prenosi okvir (header) ili izvan nje.
Ako smo izvan poruke onda ignoriramo oktete. Unutar poruke sve znakove zapisujemo u string `frame`, osim eventualno znaka ESC.
ESC može prethoditi oktetu ESC ili oktetu SOF. Zato imamo statičku varijablu `wasESC_in` koja pamti je li prethodni znak bio ESC.
Svaki znak samo ubacujemo u string `frame` osim ako prethodni znak nije ESC. Ako iza ESC nije došao SOF ili ESC onda imamo grešku u nizu i izbacujemo izuzetak.
Kada nismo u poruci opet imam varijablu koja kaže je li prethodni znak bio ESC. Tu sada koristim drugu zastavicu `wasESC_out`
neovisnu o onom što se dešava u unutrašnjosti poruke. Izvan poruke nije greška ako ESC slijedi bilo što.
Taj ESC jednostavno ignoriramo i čekamo da dođemo do niza ESC SOF koji označava početak poruke. 

U datoteci **parser_coro.h**  je algoritam iz `parser_classic.cpp` zapisan u obliku korutine. Korutina `parse()` ne čita samo jedan znak
već sve znakove i stoga ne mora pamtiti stanje između dva poziva. Uočite da je kod vrlo _prirodan_. 


Zadatak je konstruirati klasu `Parser` koja predstavlja sučelje korutine `parse()`. `Parser` ima
_Promis_ tip koji lijeno pokreće korutinu
i služi za transfer podataka iz korutine i u korutinu.
Korutina šalje podatak tipa std::string koji predstavlja poruku koristeći `co_yield` izraz.
Ulazni podatak je sljedeći oktet koji se dobiva pomoću `co_await std::byte{}` izraza. 
Za unos novog okteta moramo u _Promise_ objekt uvesti `await_transform(std::byte)` metodu koja će vratiti odgovarajući _Awaiter_.
Klasa `Parser` ima dvije metode za transer podataka:
metoda `get_frame()` vraća string koji predstavlja pročitani podatak (pohranjen u _Promise_ objektu).
Ukoliko čitanje podatka još nije završilo  vraća prazan string.
Metoda `set_byte(std::byte c)` postavlja oktet `c` u _Promise_ objekt i ponovo pokreće korutinu.
Oktet će u korutinu vratiti specijalni _Awaiter_ objekt koji generira `await_transform(std::byte)` metoda.
Nakon što je oktet učitan u korutinu treba ga _poništiti_.
Stoga se umjesto tipa `std::byte` u _Promise_ objektu može držati objekt tipa `std::optional<std::byte>`
u kojem se učitana vrijednost može poništiti s metodom `reset()`. 

- **parser_classic.cpp** je već gotov kod kojeg treba proučiti i pogledati izlaz koji daje na primjeru unutar datoteke. Isti izlaz treba dati i korutina.

- **parser_coro.h** sadrži implementaciju korutine. Tu ne treba ništa mijenjati već treba osigurati da korutina obavi zadatak. 

- **parser.h** Ovdje dolazi vaš kod. Treba napraviti klasu `Parser` sa svim potrebnim dijelovima. Kostur vam je zadan. 

- **main.cpp** je glavni program analogan glavnom programu u `parser_classic.cpp`. Oba programa trebaju dati isti ispis. Tu ne treba ništa mijenjati.

U rješavanju zadatka ne treba težiti općenotosti već samo učiniti da korutina korektno radi. 
