#ifndef POLICRUSH_H
#define POLICRUSH_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <fstream>
#include <string>

//-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-//

extern const short SIZE;
extern const short MAX_VALUE;

class Grid {
private:
    char *grid;  // puntatore alla griglia di gioco
public:
    Grid();
    void fill();                       // riempie la griglia
    bool check_cell(short, char);      // controlla se una cella non genera tris
    void insert(short, char);          // inserisce un valore nella griglia
    char get(short);                   // estrae un valore della griglia
    void print();                      // stampa la griglia
    void swap(short, short);           // inverte due celle
    unsigned int tris_and_fall(bool);  // controlla i tris, la gravità e il punteggio
    ~Grid();
};

//-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-//

void interactive_mode();
void sequential_mode();

//-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-//

struct Player {
    std::string name;     // nome giocatore
    unsigned int record;  // record giocatore
    Player *next;
};

Player *read_ranking();                                                // legge il file della classifica
void add_node(Player *&, Player *&, std::string, unsigned int);        // memorizza la classifica in una lista puntata
void print_ranking(Player *);                                          // stampa la lista
Player *find_user(Player *, std::string);                              // cerca l'utente corrente in classifica
bool refresh_ranking(Player *&, Player *, std::string, unsigned int);  // aggiorna la lista con l'utente corrente
void write_ranking(Player *);                                          // salva la lista su file
void delete_ranking(Player *);                                         // cancella la lista

#endif // POLICRUSH_H
