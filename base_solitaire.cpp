#include <iostream>
#include <conio.h>
#include <windows.h>

#include "cvm_23.h"

using namespace std;

const char carte[3]{ 'O', ' ', '\xdb' };

const size_t LARGEUR = 7;
const size_t HAUTEUR = 7;
const size_t LIGNE_CONTROLES = HAUTEUR + 2;
const size_t LIGNE_ANNULER = LIGNE_CONTROLES + 1;
const size_t LIGNE_MESSAGE = LIGNE_ANNULER + 1;

enum Case { BILLE, VIDE, NO_DISPO };

enum class Configuration { ANGLAIS, EUROPEEN, PRATIQUE, INCONNU };

enum class Action
{
    INCONNU,
    HAUT = 119,
    GAUCHE = 97,
    DROITE = 100,
    BAS = 115,
    MANIPULER = 32,
    QUITTER = 27,
    REINITIALISER = 114,
    ANNULER = 117,
    REFAIRE = 121
};

using Cases = Case[HAUTEUR][LARGEUR];

struct Position
{
    int8_t	colonne;
    int8_t	ligne;
};

struct Main
{
    bool		enMain = false;
    Position	depart = { 3, 3 }, arrivee = { 3, 3 };
    Position	prise = { 0,0 };
};

struct InfoPileSolitaire
{
    Position depart;
    Position retiree;
    Position arrivee;
};

struct NoeudPileSolitaire
{
    InfoPileSolitaire info;
    NoeudPileSolitaire* sous = nullptr;
};

struct PileSolitaire
{
    size_t taille = 0;
    NoeudPileSolitaire* dessus = nullptr;
};

//*******************************
//   DÉCLARATION DES FONCTIONS  *
//*******************************

Configuration menuAccueil();
void initialiserPlateau(Cases plateau, Configuration conf);
void afficherPlateau(Cases plateau, Main m);
Action saisirAction();
void afficherMain(Main& m);
void reafficherCase(Main& m, Cases p);
void miseAJour(Action act, Cases plateau, Main& m);
bool positionValide(Position p, Cases plateau);
void manipulerBille(Cases plateau, Main& m);
void quitter();

//*******************************
//      LA FONCTION MAIN        *
//*******************************

int main()
{
Cases plateau;
Main m;
Action act = Action::INCONNU;
Configuration config;

config = menuAccueil();     //Mettre la configuration choisir par l'utilisateur au menu accueil dans un objet "Configuration"

initialiserPlateau(plateau, config);     //Initialiser le plateau en fonction de la configuration choisie

afficherPlateau(plateau, m);               //Afficher le plateau à la console

afficherMain(m);                        //Afficher la main au bon endroit

while (act != Action::QUITTER)
{
    act = saisirAction();                   //Saisir l'action que veut faire le joueur

    miseAJour(act, plateau, m);             //Mettre à jour la coordonnée position arrivée selon le déplacement effectuer par le joueur (implémenter manipuler et quitter plus tard)
}

return 0;

}

//*******************************
//    LES FONCTIONS DÉFINIES    *
//*******************************

Configuration menuAccueil()
{
        cout << "Faire le numero du mode que vous voulez." << endl;
        cout << "1 - Anglais" << endl;
        cout << "2 - Europeen" << endl;
        cout << "3 - Pratique" << endl;
        
        Configuration config = Configuration::INCONNU;

        do
        {
            char c;
            c = _getch();
            switch (c)
            {
            case '1': config = Configuration::ANGLAIS;          break;
            case '2': config = Configuration::EUROPEEN;         break;
            case '3': config = Configuration::PRATIQUE;         break;
            }
        } while (config == Configuration::INCONNU);

        clrscr();

        return config;
}

void initialiserPlateau(Cases plateau, Configuration conf)
{   
    if (conf == Configuration::ANGLAIS)
    {
        for (size_t lig = 0; lig < HAUTEUR; ++lig)
        {
            for (size_t col = 0; col < LARGEUR; ++col)
            {
                if ((lig == 0 || lig == 1 || lig == 5 || lig == 6) && (col == 0 || col == 1 || col == 5 || col == 6))
                {
                    plateau[lig][col] = NO_DISPO;
                }
                else
                {
                    plateau[lig][col] = BILLE;
                }
            }
        }
    }
    if (conf == Configuration::EUROPEEN)
    {
        for (size_t lig = 0; lig < HAUTEUR; ++lig)
        {
            for (size_t col = 0; col < LARGEUR; ++col)
            {
                if ((lig == 0 || lig == 6) && (col == 0 || col == 1 || col == 5 || col == 6))
                {
                    plateau[lig][col] = NO_DISPO;
                }
                else if ((lig == 1 || lig == 5) && (col == 0 || col == 6))
                {
                    plateau[lig][col] = NO_DISPO;
                }
                else
                {
                    plateau[lig][col] = BILLE;
                }
            }
        }
    }
    if (conf == Configuration::PRATIQUE)
    {
        for (size_t lig = 0; lig < HAUTEUR; ++lig)
        {
            for (size_t col = 0; col < LARGEUR; ++col)
            {
                if ((lig == 0 || lig == 1 || lig == 5 || lig == 6) && (col == 0 || col == 1 || col == 5 || col == 6))
                {
                    plateau[lig][col] = NO_DISPO;
                }
                else if (lig == 3 || col == 3)
                {
                    plateau[lig][col] = BILLE;
                }
                else
                {
                    plateau[lig][col] = VIDE;
                }
            }
        }
    }
}

void afficherPlateau(Cases plateau, Main m)
{
    for(size_t lig = 0; lig < HAUTEUR; ++lig)
    {
        for(size_t col = 0; col < LARGEUR; ++col)
        {
            cout << carte[plateau[lig][col]];
        }
        cout << endl;
    }

    gotoxy(0, LIGNE_CONTROLES);
    cout << "Gauche: a | Haut: w | Droite: d | Bas: s | Reinitialiser: r | Quitter: q | Annuler: u | Refaire: y";

    gotoxy(0, LIGNE_MESSAGE);
    cout << "Voulez-vous choisir le trou de depart (O/N) ?";
    char c;
    bool choixValide = false;
    bool choisirTrou;
    do
    {
        c = _getch();
        switch (c)
        {
        case 'o': choisirTrou = true; choixValide = true; gotoxy(0, LIGNE_MESSAGE); cout << "Deplacez-vous et selectionnez la bille a retirer";     break;
        case 'n': choisirTrou = false; choixValide = true;       break;
        }
    } while (choixValide = false);

    if (choisirTrou == true)
    {
    Action act = Action::INCONNU;

    while (act != Action::QUITTER)
    {
        act = saisirAction(); 
        miseAJour(act, plateau, m);      
    }
    }
}

Action saisirAction()
{
    char c;
    Action act;

    do
    {
        c = _getch();
        switch(c)
        {
            case 'w': act = Action::HAUT;       break;
            case 's': act = Action::BAS;        break;
            case 'a': act = Action::GAUCHE;     break;
            case 'd': act = Action::DROITE;     break;
            case ' ': act = Action::MANIPULER;  break;
            case 'q': act = Action::QUITTER;    break;
            default : act = Action::INCONNU;
        }
    } while(act == Action::INCONNU);

    return act;
}

void afficherMain(Main& m)                                   
{
    gotoxy(m.arrivee.colonne, m.arrivee.ligne);
    if(m.enMain)
    {
        setcolor(Color::red);
        cout << 'x';
    }
    else
    {
        setcolor(Color::grn);
        cout << 'X';
    }
    setcolor(Color::wht);
}

void reafficherCase(Main& m, Cases plateau)
{
    gotoxy(m.depart.colonne, m.depart.ligne);
    Case c = plateau[m.depart.colonne][m.depart.ligne];
    cout << carte[c];   
}

void miseAJour(Action act, Cases plateau, Main& m)           //Mettre à jour la position de la main à la console selon l'action effectué par leu joueur
{
    switch(act)
    {
        case Action::HAUT:      m.arrivee.ligne--;              break;
        case Action::BAS:       m.arrivee.ligne++;              break;
        case Action::DROITE:    m.arrivee.colonne++;            break;
        case Action::GAUCHE:    m.arrivee.colonne--;            break;
        case Action::MANIPULER: manipulerBille(plateau, m);     break;
        case Action::QUITTER:   quitter();                      break;
    }
    if(positionValide(m.arrivee, plateau))        //Si le déplacement est valide, on fait un cout de la main à la position d'arrivée et on réaffiche le plateau au complet la position de départ devient celle d'arrivée
    {
        reafficherCase(m, plateau);
        afficherMain(m);
        m.depart = m.arrivee;
    }
    else
    {
        m.arrivee = m.depart;           //Si l'action n'est pas vailde, on remet la position d'arrivée comme celle de départ
    }
}

bool positionValide(Position p, Cases plateau)
{
    if (p.colonne > LARGEUR - 1 || p.ligne > HAUTEUR - 1 || p.ligne < 0 || p.colonne < 0)
    {
        return false;
    }
    if (plateau[p.ligne][p.colonne] == NO_DISPO)
    {
        return false;
    }
    return true;
}

void manipulerBille(Cases plateau, Main& m)
{
    if (m.enMain)
    {
        m.enMain = false;
    }
    else
    {
        m.enMain = true;
    }
}

void quitter()
{
    clrscr();
    cout << "Au revoir!";
    _getch();
}