#include "policrush.h"

using namespace std;

const short SIZE = 10;
const short MAX_VALUE = 3;

/* Costruttore della classe. Inizializza la griglia con spazi bianchi. Le funzioni successive
 * riconosceranno come "vuote" le celle contenenti uno spazio bianco.
 */
Grid::Grid() {
    grid = new char[SIZE*SIZE];
    for (short i = 0; i < SIZE*SIZE; ++i)
        grid[i] = ' ';
}

/* Riempie la griglia di gioco al termine di ogni mossa. Per sapere quali sono le celle da
 * riempire, scorre la griglia e le memorizza in un array di booleani dove 1 indica "cella da
 * riempire" e 0 "cella già riempita". Dopo di che, per ogni cella da riempire, inserisce un valore
 * casuale e, se la funzione check_cell() ritorna TRUE, prosegue alla cella successiva, altrimenti
 * inserisce un altro valore. Nel caso in cui nessun valore sia valido, torna indietro di una riga
 * e inserisce nuovi valori casuali. Se anche così non è possibile trovare un valore valido per la
 * cella, significa che la griglia è in una configurazione che non lo permette proprio. Lascia la
 * cella con l'ultimo valore inserito e prosegue.
 */
void Grid::fill() {
    // identifica le celle da riempire
    bool is_empty[SIZE*SIZE] = {false};
    for (short i = 0; i < SIZE*SIZE; ++i)
        if (grid[i] == ' ')
            is_empty[i] = true;

    // riempie la griglia
    short i = SIZE*SIZE - 1;  // indice di partenza (ultimo elemento)
    short antiloop = 0;       // conta quanti inserimenti sono stati fatti nella stessa cella
    short loop = 0;           // conta quante volte il programma è tornato indietro a partire dalla stessa cella
    short cell = i;           // indice cella corrente
    short ex_cell = i;        // indice cella precedente
    while (i >= 0) {
        if (is_empty[i]) {  // se è una cella da riempire
            if (grid[i] != ' ') {  // se non è vuota (cioè ha un valore non valido)
                if (antiloop < MAX_VALUE) {  // se non ha ancora provato tutti i valori possibili
                    if (grid[i] == 'F')  // ne inserisce un altro
                        grid[i] = 'A';
                    else if (grid[i] == 'A')
                        grid[i] = 'B';
                    else if (grid[i] == 'B')
                        grid[i] = 'F';
                    ++antiloop;
                    //Grid::print();
                } else {  // se li ha provati tutti, torna indietro di una riga (svuotandola)
                    cell = i;  // memorizza qual è la cella che genera il loop
                    if (cell == ex_cell) {  // se è la stessa di prima, incrementa il contatore
                        ++loop;
                    } else {  // altrimenti è un'altra, resetta il contatore a 1
                        loop = 1;
                    }
                    ex_cell = cell;
                    if (loop <= 10) {  // se il loop si è verificato poche volte, torna indietro di una riga (svuotandola)
                        for (short j = i; j < i + SIZE && j < SIZE*SIZE; ++j)
                            if (is_empty[j])
                                grid[j] = ' ';
                        antiloop = 0;
                        i += SIZE;
                        if (i >= SIZE*SIZE)
                            i = SIZE*SIZE - 1;  // controllo anti buffer overflow
                    } else {  // se il loop si è verificato tante volte, allora è impossibile riempire quella cella, prosegue
                        loop = 0;
                        is_empty[i] = false;
                        --i;
                    }
                    continue;
                }
            } else {  // se è vuota, la riempie
                char c = rand() % 3;
                switch (c) {
                case 0: grid[i] = 'B'; break;
                case 1: grid[i] = 'A'; break;
                case 2: grid[i] = 'F'; break;
                }
                ++antiloop;
                //Grid::print();
            }
            if (Grid::check_cell(i, grid[i])) {  // se la cella è valida, prosegue
                --i;
                antiloop = 0;
            }
        } else {  // se non è una cella da riempire, prosegue
            --i;
        }
    }
}

/* Verifica se il contenuto di una cella non genera tris, controllando le celle circostanti e
 * ritorna TRUE o FALSE. In entrata prende l'indice della cella e il suo contenuto.
 */
bool Grid::check_cell(short index, char val) {
    // controllo orizzontale
    short col = index % SIZE;
    short row_counter = 0;
    for (short i = index; i >= index - col && grid[i] == val; --i)
        ++row_counter;
    for (short i = index + 1; i < index - col + SIZE && grid[i] == val; ++i)
        ++row_counter;
    if (row_counter >= MAX_VALUE)
        return false;

    // controllo verticale
    short col_counter = 0;
    for (short i = index; i >= 0 && grid[i] == val; i -= SIZE)
        ++col_counter;
    for (short i = index + SIZE; i < SIZE*SIZE && grid[i] == val; i += SIZE)
        ++col_counter;
    if (col_counter >= MAX_VALUE)
        return false;

    // controllo diagonali
    short diag_counter[2] = {0, 0};
    for (short i = index; (i % SIZE) >= 0 && (i / SIZE) >= 0 && grid[i] == val; i -= (SIZE+1))
        ++diag_counter[0];
    for (short i = index; (i % SIZE) < SIZE && (i / SIZE) < SIZE && grid[i] == val; i += (SIZE+1))
        ++diag_counter[0];
    if (diag_counter[0] - 1 >= MAX_VALUE)  // -1 perché conta due volte la cella di partenza
        return false;
    for (short i = index; (i % SIZE) < SIZE && (i / SIZE) >= 0 && grid[i] == val; i -= (SIZE-1))
        ++diag_counter[1];
    for (short i = index; (i % SIZE) >= 0 && (i / SIZE) < SIZE && grid[i] == val; i += (SIZE-1))
        ++diag_counter[1];
    if (diag_counter[1] - 1 >= MAX_VALUE)  // come sopra
        return false;

    // se la cella è sopravvissuta a tutti i controlli, è accettabile
    return true;
}

// Permette l'assegnamento alle funzioni esterne alla classe.
void Grid::insert(short index, char val) {
    grid[index] = val;
}

// Permette la lettura alle funzioni esterne alla classe.
char Grid::get(short index) {
    return grid[index];
}

// Stampa la griglia corrente.
void Grid::print() {
    cout << "   ";
    for (short i = 0; i < SIZE; ++i)
        cout << setw(3) << i + 1;
    cout << endl;
    for (short r = 0; r < SIZE*SIZE; r += SIZE) {
        cout << setw(3) << r/10 + 1;
        for (short c = 0; c < SIZE; ++c)
            cout << setw(3) << grid[r+c];
        cout << endl;
    }
    cout << endl;
}

// Inverte due celle della griglia. Prende in entrata gli indici.
void Grid::swap(short x, short y) {
    char temp = grid[x];
    grid[x] = grid[y];
    grid[y] = temp;
}

/* Controlla la presenza di simboli uguali adiacenti nella griglia, ritorna il punteggio ottenuto
 * nella mossa corrente e fa cadere verso il basso i simboli sospesi nella griglia. Se viene
 * identificata una serie di almeno tre simboli uguali in almeno una delle quattro direzioni, le
 * coordinate degli elementi appartenenti alla serie vengono salvate in un array di valori
 * booleani. Al termine della scansione, scorre l'array di booleani e, per ogni elemento "true",
 * viene incrementato il contatore relativo all'elemento corrispondente nella griglia di gioco, che
 * viene poi sostituito da uno spazio bianco. Finita la scansione, calcola il punteggio ottenuto e
 * fa cadere verso il basso gli elementi rimasti sospesi nella griglia. Non verifica se la caduta
 * degli elementi provoca la formazione di ulteriori serie di simboli uguali (tale verifica avviene
 * con un'iterazione in interactive_mode).
 */
unsigned int Grid::tris_and_fall(bool print) {
    bool is_tris[SIZE*SIZE] = {false};

    // ricerca dei tris
    for (short i = 0; i < SIZE*SIZE; ++i) {
        if (grid[i] == ' ')
            continue;
        if (i % SIZE < SIZE-2 && grid[i] == grid[i+1] && grid[i] == grid[i+2]) {  // controllo orizzontale
            is_tris[i] = true;
            is_tris[i+1] = true;
            is_tris[i+2] = true;
        }
        if (i < SIZE*SIZE - 2*SIZE && grid[i] == grid[i+SIZE] && grid[i] == grid[i + 2*SIZE]) {  // controllo verticale
            is_tris[i] = true;
            is_tris[i + SIZE] = true;
            is_tris[i + 2*SIZE] = true;
        }
        if (i % SIZE < SIZE-2 && i < SIZE*SIZE - 2*SIZE && grid[i] == grid[i+SIZE+1] && grid[i] == grid[i + 2*SIZE + 2]) {  // controllo diagonale 1
            is_tris[i] = true;
            is_tris[i + SIZE + 1] = true;
            is_tris[i + 2*SIZE + 2] = true;
        }
        if (i % SIZE > 1 && i < SIZE*SIZE - 2*SIZE && grid[i] == grid[i+SIZE-1] && grid[i] == grid[i + 2*SIZE - 2]) {  // controllo diagonale 2
            is_tris[i] = true;
            is_tris[i + SIZE - 1] = true;
            is_tris[i + 2*SIZE - 2] = true;
        }
    }

    // calcolo punteggio
    short b = 0, a = 0, f = 0;           // contatori di ogni valore
    const short WB = 3, WA = 2, WF = 1;  // pesi di ogni valore
    for (short i = 0; i < SIZE*SIZE; ++i) {
        if (is_tris[i]) {
            if (grid[i] == 'B')
                ++b;
            else if (grid[i] == 'A')
                ++a;
            else if (grid[i] == 'F')
                ++f;
            grid[i] = ' ';
        }
    }
    unsigned int p = b*WB + a*WA + f*WF;

    if (print)
        Grid::print();

    // caduta dei simboli
    for (short i = SIZE*SIZE - 1; i >= SIZE; --i) {
        if (grid[i] == ' ') {
            short ix = i;
            while (ix >= SIZE && grid[ix] == ' ')
                ix -= SIZE;
            Grid::swap(i, ix);
        }
    }

    if (print)
        Grid::print();

    return p;
}

Grid::~Grid() {
    delete[] grid;
}

//-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-//

void interactive_mode() {
    Player *ranking = read_ranking();
    Grid *grid = new Grid;
    grid->fill();

    cout << "\nMODALITA' INTERATTIVA" << endl;
    string user_name;
    cout << "Inserisci il tuo nome: ";
    cin >> user_name;
    while (user_name.size() > 15) {
        cout << "Nome troppo lungo! Riprova: ";
        cin >> user_name;
    }
    cout << "\nPer effettuare una mossa, digita le coordinate (riga-colonna) della cella da spostare e la direzione dello spostamento." << endl;
    cout << "Ogni direzione e' indicata da uno dei seguenti numeri, basati sulla loro posizione sul tastierino numerico:" << endl;
    cout << "7 = Alto sx\t8 = Alto\t9 = Alto dx\n"
            "4 = Sinistra\t\t\t6 = Destra\n"
            "1 = Basso sx\t2 = Basso\t3 = Basso dx\n" << endl;
    cout << "Esempio: \"2 5 4\" sposta a sinistra la quinta cella della seconda riga.\n" << endl;
    cout << "Il comando speciale \"0 0 0\" ti permette di leggere la classifica dei punteggi massimi ottenuti dai giocatori.\n" << endl;

    unsigned int pnt = 0;        // punteggio totale
    unsigned int delta_pnt = 0;  // punti guadagnati in una mossa
    short mosse = 10;            // numero mosse rimanenti
    short r, c, dir;             // riga, colonna, direzione
    grid->print();
    cout << "Mosse rimanenti: " << mosse << endl;
    do {
        do {
            cin >> r >> c >> dir;
            if (r == 0 && c == 0 && dir == 0) {
                print_ranking(ranking);
                continue;
            }
            if (r < 1 || r > SIZE || c < 1 || c > SIZE) {
                cout << "La coordinata scelta non esiste!" << endl;
            } else if (dir == 5 || dir < 1 || dir > 9) {
                cout << "La direzione indicata non esiste!" << endl;
            } else if (   c == 1    && (dir == 7 || dir == 4 || dir == 1)
                       || c == SIZE && (dir == 9 || dir == 6 || dir == 3)
                       || r == 1    && (dir == 7 || dir == 8 || dir == 9)
                       || r == SIZE && (dir == 1 || dir == 2 || dir == 3) ) {
                cout << "Non e' possibile effettuare questo spostamento!" << endl;
            } else {
                break;
            }
        } while (true);
        --r;  // l'input è 1-based, converte a 0-based
        --c;
        switch (dir) {
        case 1: grid->swap(r*SIZE + c, r*SIZE+SIZE + (c-1)); break;
        case 2: grid->swap(r*SIZE + c, r*SIZE+SIZE +   c  ); break;
        case 3: grid->swap(r*SIZE + c, r*SIZE+SIZE + (c+1)); break;
        case 4: grid->swap(r*SIZE + c, r*SIZE      + (c-1)); break;
        case 6: grid->swap(r*SIZE + c, r*SIZE      + (c+1)); break;
        case 7: grid->swap(r*SIZE + c, r*SIZE-SIZE + (c-1)); break;
        case 8: grid->swap(r*SIZE + c, r*SIZE-SIZE +   c  ); break;
        case 9: grid->swap(r*SIZE + c, r*SIZE-SIZE + (c+1)); break;
        }
        cout << endl;
        do {
            do {
                delta_pnt = grid->tris_and_fall(1);
                pnt += delta_pnt;
            } while (delta_pnt != 0);
            grid->fill();
            delta_pnt = grid->tris_and_fall(1);
            pnt += delta_pnt;
        } while (delta_pnt != 0);
        grid->print();
        --mosse;
        cout << "Mosse rimanenti: " << mosse << endl;
    } while (mosse > 0);
    cout << "\nPartita terminata! Il punteggio realizzato e' " << pnt << "." << endl;

    Player *user_ptr = find_user(ranking, user_name);
    if (refresh_ranking(ranking, user_ptr, user_name, pnt))
        write_ranking(ranking);
    delete grid;
    delete_ranking(ranking);
}

void sequential_mode() {
    Grid *grid = new Grid;
    cout << "\nMODALITA' SEQUENZIALE" << endl;

    // lettura e stampa della griglia di gioco
    ifstream seq_grid("seq_grid.txt");
    if (!seq_grid) {
        cout << "Il file seq_grid.txt non esiste."
                "\nNon e' possibile avviare la partita." << endl;
        delete grid;
        return;
    }
    char ch;      // carattere corrente
    short i = 0;  // indice griglia
    while (!seq_grid.eof()) {
        seq_grid.get(ch);
        if (isalnum(ch)) {
            if (ch != 'B' && ch != 'A' && ch != 'F') {
                cout << "La griglia di gioco contiene valori non validi."
                        "\nNon e' possibile avviare la partita." << endl;
                seq_grid.close();
                delete grid;
                return;
            }
            grid->insert(i, ch);
            ++i;
        }
        if (i > SIZE*SIZE) {
            cout << "La griglia di gioco e' troppo grande."
                    "\nAssicurati che sia delle dimensioni " << SIZE << "x" << SIZE << "." << endl;
            seq_grid.close();
            delete grid;
            return;
        }
    }
    if (i < SIZE*SIZE) {
        cout << "La griglia di gioco e' troppo piccola."
                "\nAssicurati che sia delle dimensioni " << SIZE << "x" << SIZE << "." << endl;
        seq_grid.close();
        delete grid;
        return;
    }
    seq_grid.close();
    cout << "La griglia di gioco estratta e' la seguente:" << endl;
    grid->print();

    // lettura ed esecuzione delle mosse
    ifstream seq_player("seq_player.txt");
    if (!seq_player) {
        cout << "Il file seq_player.txt non esiste."
                "\nNon e' possibile continuare la partita." << endl;
        delete grid;
        return;
    }
    unsigned int pnt = 0;        // punteggio totale
    unsigned int delta_pnt = 0;  // punti guadagnati in una mossa
    short r, c, dir;             // riga, colonna, direzione
    string mossa;                // mossa corrente
    short num = 0;               // numero mossa
    while (!seq_player.eof()) {
        getline(seq_player, mossa);
        // Possibili input validi e indici (deve gestire coordinate con due cifre):
        // 01234567
        // 01 01 1;
        // 10 01 1;
        // 01 10 1;
        // 10 10 1;
        if (!isdigit(mossa[1]) || !isdigit(mossa[4]) || !isdigit(mossa[6])) {
            cout << "La lista delle mosse contiene valori non validi."
                    "\nNon e' possibile proseguire la partita." << endl;
            seq_grid.close();
            delete grid;
            return;
        }
        r = mossa[1] - '0';
        if (mossa[0] >= '1' && mossa[0] <= '9')
            r = (mossa[0] - '0')*10 + r;
        c = mossa[4] - '0';
        if (mossa[3] >= '1' && mossa[3] <= '9')
            c = (mossa[3] - '0')*10 + c;
        dir = mossa[6] - '0';
        ++num;

        // da qui in poi come interactive_mode() con qualche adattamento
        if (r < 1 || r > SIZE || c < 1 || c > SIZE) {
            cout << "Mossa " << num << " non valida: la coordinata scelta non esiste!\n" << endl;
            continue;  // salta la mossa corrente e va alla successiva
        } else if (dir < 1 || dir > 8) {
            cout << "Mossa " << num << " non valida: la direzione indicata non esiste!\n" << endl;
            continue;
        } else if (   c == 1    && (dir == 6 || dir == 4 || dir == 8)
                   || c == SIZE && (dir == 5 || dir == 3 || dir == 7)
                   || r == 1    && (dir == 6 || dir == 1 || dir == 5)
                   || r == SIZE && (dir == 8 || dir == 2 || dir == 7) ) {
            cout << "Mossa " << num << " non valida: non e' possibile effettuare questo spostamento!\n" << endl;
            continue;
        }
        --r;
        --c;
        switch (dir) {
        case 1: grid->swap(r*SIZE + c, r*SIZE-SIZE +   c  ); break;
        case 2: grid->swap(r*SIZE + c, r*SIZE+SIZE +   c  ); break;
        case 3: grid->swap(r*SIZE + c, r*SIZE      + (c+1)); break;
        case 4: grid->swap(r*SIZE + c, r*SIZE      + (c-1)); break;
        case 5: grid->swap(r*SIZE + c, r*SIZE-SIZE + (c+1)); break;
        case 6: grid->swap(r*SIZE + c, r*SIZE-SIZE + (c-1)); break;
        case 7: grid->swap(r*SIZE + c, r*SIZE+SIZE + (c+1)); break;
        case 8: grid->swap(r*SIZE + c, r*SIZE+SIZE + (c-1)); break;
        }
        do {
            do {
                delta_pnt = grid->tris_and_fall(0);
                pnt += delta_pnt;
            } while (delta_pnt != 0);
            grid->fill();
            delta_pnt = grid->tris_and_fall(0);
            pnt += delta_pnt;
        } while (delta_pnt != 0);
    }
    seq_player.close();

    // creazione file output
    ofstream seq_result("seq_result.txt");
    if (seq_result) {
        cout << "Il file seq_result.txt e' stato creato con successo." << endl;
    } else {
        cout << "Errore nella creazione del file seq_result.txt." << endl;
        delete grid;
        return;
    }
    for (short i = 0; i < SIZE*SIZE; ++i) {
        seq_result << grid->get(i) << (i % SIZE < SIZE-1 ? "," : ";\n");
    }
    seq_result << "\nIl punteggio realizzato e' " << pnt << "." << endl;
    seq_result.close();
    delete grid;
}

//-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-//

// Legge la classifica corrente e la salva in una lista puntata.
Player *read_ranking() {
    // apre il file della classifica in append per assicurarsi che esista
    fstream ranking("ranking.txt", ios::app);
    ranking.close();
    ranking.open("ranking.txt", ios::in);

    // salva la classifica in una lista
    Player *head = nullptr;
    Player *tail = nullptr;
    string name;
    unsigned int record;
    while (true) {
        ranking >> name;
        ranking >> record;
        if (ranking.eof())
            break;
        add_node(head, tail, name, record);
    }

    // chiude il file e ritorna il puntatore alla lista
    ranking.close();
    return head;
}

// Aggiunge un nodo in coda con inserimento intelligente.
void add_node(Player *&head, Player *&tail, string name, unsigned int record) {
    Player *node = new Player;
    node->name = name;
    node->record = record;
    node->next = nullptr;

    if (tail == nullptr) {
        head = tail = node;
    } else {        
        tail->next = node;
        tail = node;
    }
}

// Stampa a video la classifica corrente
void print_ranking(Player *head) {
    if (head == nullptr) {
        cout << "La classifica e' vuota." << endl;
    } else {
        Player *walker = head;
        while (walker != nullptr) {
            cout << walker->name;
            for (short i = 0; i < 20 - walker->name.size(); ++i)
                cout << ' ';
            cout << walker->record << endl;
            walker = walker->next;
        }
    }
}

// Cerca nella lista l'utente corrente e ne ritorna il puntatore (se non esiste ritorna nullptr).
Player *find_user(Player *head, string user_name) {
    if (head == nullptr)
        return nullptr;

    Player *walker = head;
    while (walker != nullptr) {
        if (walker->name == user_name)
            return walker;
        walker = walker->next;
    }

    return nullptr;
}

/* Aggiorna la classifica al termine della partita, se necessario, inserendo l'utente corrente e il
 * suo punteggio record. Ritorna TRUE se la lista viene modificata, FALSE se viene lasciata
 * intatta. Nel caso in cui la lista venga modificata, un'altra funzione avrà il compito di
 * memorizzarla su file.
 */
bool refresh_ranking(Player *&head, Player *user, string name, unsigned int record) {
    // utente nuovo
    if (user == nullptr) {
        Player *node = new Player;
        node->name = name;
        node->record = record;
        node->next = nullptr;
        // classifica vuota
        if (head == nullptr) {
            head = node;
            return true;
        }
        // classifica esistente
        Player *previous = nullptr;
        Player *walker = head;
        while (walker != nullptr && walker->record >= record) {
            previous = walker;
            walker = walker->next;
        }
        node->next = walker;
        if (previous != nullptr) {
            previous->next = node;
        } else {
            head = node;
        }
        return true;
    }

    // utente già presente
    if (user->record < record) {
        user->record = record;
        // se l'utente era già primo, lo rimane
        if (user == head)
            return true;
        // altrimenti può essere spostato
        Player *previous = head;
        while (previous->next != user) {
            previous = previous->next;
        }
        previous->next = user->next;

        previous = nullptr;
        Player *walker = head;
        while (walker != nullptr && walker->record >= record) {
            previous = walker;
            walker = walker->next;
        }
        user->next = walker;
        if (previous != nullptr) {
            previous->next = user;
        } else {
            head = user;
        }
        return true;
    }

    // nessun nuovo record
    return false;
}

// Scrive la lista su file.
void write_ranking(Player *head) {
    ofstream ranking("ranking.txt");
    Player *walker = head;
    while (walker != nullptr) {
        ranking << walker->name << "     " << walker->record << endl;
        walker = walker->next;
    }
    ranking.close();
}

// Cancella la lista dall'heap.
void delete_ranking(Player *head) {
    if (head == nullptr) {
        return;
    } else {
        Player *walker = head;
        while (walker != nullptr) {
            walker = walker->next;
            delete head;
            head = walker;
        }
    }
}
