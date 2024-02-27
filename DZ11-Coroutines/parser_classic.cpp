// Parsiranje byte-stream-a. Verzija bez korutine.
// PROTOKOL:
// Ulazni tok podataka je podijeljen u pakete (okvire, frames). Početak
// poruke je označen s dva okteta: ESC i SOF (start of frame). Kraj poruke
// je također označen s oktetima ESC i SOF. ESC 
// je znak za izuzetak s kojim se označava da sljedeći znak nije dio 
// paketa već je to kontrolni znak. Koristimo ga ispred SOF koji je naš jedini 
// kontrolni znak, ali i ispred EOF znaka kada se nađe u paketu kako bi  
// signalizirao da sljedeći ESC znak nije znak za izuzetak već bio paketa.
// Na taj način je moguće slati ne samo oktete koji predstavljaju znakove veći 
// sve moguće oktete. 
//
// IMPLEMENTACIJA:
// Imamo dva stanja: možemo biti unutar poruke koju prenosi okvir (header) ili izvan nje.
// Ako smo izvan poruke onda ignoriramo oktete.  
// Unutar poruke sve znakove zapisujemo u string frame, osim eventualno znaka ESC.
// ESC može prethoditi ESC ili SOF. Zato imamo statičku varijablu wasESC_in koja pamti je li 
// prethodni znak bio ESC. Svaki znak samo ubacujemo u string frame osim ako prethodni
// znak nije ESC. Ako iza ESC nije došao SOF ili ESC ond imamo grešku u nizu i izbacujemo 
// izuzetak.
//   Kada nismo u poruci opet imam varijablu koja kaže je li prethodni znak bio ESC.
// Tu sada koristim drugu zastavicu wasESC_out neovisnu o onom što se dešava u unutrašnjosti 
// poruke. Izvan poruke nije greška ako ESC slijedi bilo što. Taj ESC jednostavno ignoriramo
// i čekamo da dođemo do niza ESC SOF koji označava početak poruke. 


#include <iostream>
#include <vector>
#include <string>
#include <cstddef>  // za std::byte

static const std::byte ESC{'H'};
static const std::byte SOF{0x10};

// User defined literals
std::byte operator""_B(char c){ return std::byte(c); }
std::byte operator""_B(unsigned long long int c){ return std::byte(c); }

// Procesiraj jedan oktet s ulaza. frameCompleted je callback koji se zove kad se
// pročita čitava poruka (čitav okvir).
template <typename T>
void processNextByte(std::byte b, T&& frameCompleted)
{
    // Statičke varijable osiguravaju da se stanje sačuva kroz niz poziva 
    // funkcije processNextByte().
    static std::string frame{};
    static bool inHeader  {false};
    static bool wasESC_in {false};         // Da li je prethodni znak ESC?
    static bool wasESC_out{false};

    if(inHeader){
        if(b == ESC && !wasESC_in){  // prvi ESC unutar poruke. Zabilježi to.
            wasESC_in = true;
            return;               // ne ubacujemo ništa u frame
        }
        else if(wasESC_in){         // prethodni znak je bio ESC
            wasESC_in = false;
            if(b == SOF){        // ovo je kraj ove poruke
                frameCompleted(frame); // zovi callback
                frame.clear();         // polazno stanje nove poruke
                inHeader = false;
                return;         // ne ubacujemo ništa u frame
            }
            else if(b != ESC){
                throw std::runtime_error(std::string("Parsing error: ") 
                      + static_cast<char>(b) + " after ESC.\n");    // ne ubacujemo ništa u frame
            }
        }   
        frame += static_cast<char>(b);
    }
    else if(b == ESC && !wasESC_out){
       wasESC_out = true;
    }
    else if(b == SOF && wasESC_out){
        inHeader = true;
        wasESC_out = false;
    }
    else{
        wasESC_out = false;
    }
}


int main(){
    std::vector<std::byte> fakeBytes{
                           0x70_B, ESC, SOF, ESC,
                           'H'_B, 'e'_B, 'l'_B, 'l'_B, 'o'_B, ESC, SOF,
                           0x7_B, ESC, SOF, 'W'_B, 'o'_B, 'r'_B, 'l'_B, 'd'_B, ESC, SOF,
                           0x67_B, 'g'_B,'g'_B, 
                           ESC, SOF, ESC, 'H'_B, 'e'_B, 'r'_B, 'e'_B, ','_B, ESC, SOF,
                           ESC, SOF, 'I'_B,  ESC, SOF,
                           ESC, SOF, 'a'_B, 'm'_B, ESC, SOF, 'g'_B, 'o'_B};

    auto callBack = [](std::string const & frame){
        std::cout << frame <<"\n";
    };
    
    for(auto b : fakeBytes){
        processNextByte(b, callBack);
    }
    return 0;
}

