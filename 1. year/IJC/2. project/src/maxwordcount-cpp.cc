// maxwordcount-cpp.cc
// Použijte: g++ -O2
// Příklad použití STL kontejneru unordered_map<>
// Program zjistí četnost slov ve vstupním textu,
// a tiskne seznam nejčetnějších slov.
// Slovo je cokoli oddělené "bílým znakem"

#include <string>
#include <iostream>
#include <unordered_map>

int main() {
    using namespace std;
    unordered_map<string,unsigned> m;  // asociativní pole
                     // mapuje klíč (string) na hodnotu (unsigned)
                     // prvky jsou dvojice (klíč,hodnota)
    string word;
    while (cin >> word) // čtení slova (jako scanf "%s", ale bezpečné)
        m[word]++;      // počítání výskytů slova (zvýší hodnotu pro
                            // zadaný klíč/slovo pokud záznam existuje,
                            // jinak vytvoří nový záznam s hodnotou 0 a
                            // tu operace ++ zvýší na 1)
    unsigned max=0;
    for (auto &mi: m)   // pro všechny prvky kontejneru m
        if(mi.second>max)
            max=mi.second;  // zjistíme maximální četnost
    for (auto &mi: m)
        if(mi.second==max)  // tisk jen nejčetnějších slov
            cout << mi.first << "\t" << mi.second << "\n";
            //      klíč/slovo          hodnota/počet
    }
