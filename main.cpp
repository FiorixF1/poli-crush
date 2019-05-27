/* Elaborato del Corso di Fondamenti di Informatica 2014-2015
 * Autore: Alessandro FIORILLO
 * Matricola: 828861
 * Data inizio: 13 dicembre 2014
 * Data completamento: 22 gennaio 2015
 */

#include "policrush.h"

using namespace std;

int main() {
    srand(time(0));

    short game;
    cout << "---------------Benvenuto in POLI CRUSH!---------------" << endl;
    cout << "\nDigita 1 per giocare in modalita' interattiva."
            "\nDigita 2 per giocare in modalita' sequenziale." << endl;
    do {
        cin >> game;
    } while (game != 1 && game != 2);

    switch (game) {
    case 1: interactive_mode(); break;
    case 2: sequential_mode(); break;
    }

    return 0;
}
