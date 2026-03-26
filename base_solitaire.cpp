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
    int8_t	ligne;
    int8_t	colonne;
};

struct Main
{
    bool		enMain = false;
    Position	depart = { 0,0 }, arrivee = { 0,0 };
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

//Déclaration des fonctions
Configuration menuAccueil();
void initialiserPlateau(Configuration conf);
void afficherPlateau(Cases plateau);
Action saisirAction();
void afficherMain(Main m);
void reafficherCase(Main m, Cases p);
void miseAJour(Action act, Cases plateau, Main m);
bool positionValide(Position p, Cases plateau);
void manipulerBille(Cases plateau, Main m);
void quitter();

int main()
{
Cases plateau;
Main m;
Action act = Action::INCONNU;
Configuration config;

while (act != Action::QUITTER)
{
    config = menuAccueil();
    
    initialiserPlateau(config);

    act = saisirAction();

    switch (act)
    {
        case Action::HAUT:       break;
    }

    miseAJour(act, plateau, m);
}

return 0;

}

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

void initialiserPlateau(Configuration conf)
{   
    if (conf == Configuration::ANGLAIS)
    {
        Cases plateau[HAUTEUR][LARGEUR] =
        {
            {NO_DISPO, NO_DISPO, BILLE, BILLE, BILLE, NO_DISPO, NO_DISPO},
            {NO_DISPO, NO_DISPO, BILLE, BILLE, BILLE, NO_DISPO, NO_DISPO},
            {BILLE, BILLE, BILLE, BILLE, BILLE, BILLE, BILLE},
            {BILLE, BILLE, BILLE, BILLE, BILLE, BILLE, BILLE},
            {BILLE, BILLE, BILLE, BILLE, BILLE, BILLE, BILLE},
            {NO_DISPO, NO_DISPO,BILLE, BILLE, BILLE, NO_DISPO, NO_DISPO},
            {NO_DISPO, NO_DISPO,BILLE, BILLE, BILLE, NO_DISPO, NO_DISPO},
        };
    }
    if (conf == Configuration::EUROPEEN)
    {
        Cases plateau[HAUTEUR][LARGEUR] =
        {
            {NO_DISPO, NO_DISPO, BILLE, BILLE, BILLE, NO_DISPO, NO_DISPO},
            {NO_DISPO, BILLE, BILLE, BILLE, BILLE, BILLE, NO_DISPO},
            {BILLE, BILLE, BILLE, BILLE, BILLE, BILLE, BILLE},
            {BILLE, BILLE, BILLE, BILLE, BILLE, BILLE, BILLE},
            {BILLE, BILLE, BILLE, BILLE, BILLE, BILLE, BILLE},
            {NO_DISPO, BILLE,BILLE, BILLE, BILLE, BILLE, NO_DISPO},
            {NO_DISPO, NO_DISPO,BILLE, BILLE, BILLE, NO_DISPO, NO_DISPO},
        };
    }
    if (conf == Configuration::PRATIQUE)
    {
        Cases plateau[HAUTEUR][LARGEUR] =
        {
            {NO_DISPO, NO_DISPO, VIDE, BILLE, VIDE, NO_DISPO, NO_DISPO},
            {NO_DISPO, NO_DISPO, VIDE, BILLE, VIDE, NO_DISPO, NO_DISPO},
            {VIDE, VIDE, VIDE, BILLE, VIDE, VIDE, VIDE},
            {BILLE, BILLE, BILLE, BILLE, BILLE, BILLE, BILLE},
            {VIDE, VIDE, VIDE, BILLE, VIDE, VIDE, VIDE},
            {NO_DISPO, NO_DISPO, VIDE, BILLE, VIDE, NO_DISPO, NO_DISPO},
            {NO_DISPO, NO_DISPO, VIDE, BILLE, VIDE, NO_DISPO, NO_DISPO},
        };
    }
}

void afficherPlateau(Cases plateau)
{
    for(size_t lig = 0; lig < HAUTEUR; ++lig)
    {
        for(size_t col = 0; col < LARGEUR; ++col)
        {
            cout << carte[plateau[lig][col]];
        }
        cout << endl;
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

void afficherMain(Main m)
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

void reafficherCase(Main m, Cases plateau)
{
    gotoxy(m.depart.colonne, m.depart.ligne);
    Case c = plateau[m.depart.ligne][m.depart.colonne];
    cout << carte[c];   
}

void miseAJour(Action act, Cases plateau, Main m)
{
    switch(act)
    {
        case Action::HAUT: m.arrivee.ligne--; break;
        case Action::BAS: m.arrivee.ligne++; break;
        case Action::DROITE: m.arrivee.colonne++; break;
        case Action::GAUCHE: m.arrivee.colonne--; break;
        case Action::MANIPULER: manipulerBille(plateau, m); break;
        case Action::QUITTER: quitter(); break;
    }
    if(positionValide(m.arrivee, plateau))
    {
        afficherMain(m);
        reafficherCase(m, plateau);
        m.depart = m.arrivee;
    }
    else
    {
        m.arrivee = m.depart;
    }
}

bool positionValide(Position p, Cases plateau)
{
    // D'autre(s) condition(s) à ajouter...
    if (p.ligne > HAUTEUR || p.colonne > LARGEUR || p.ligne < 0 || p.colonne < 0) 
    {
        return false;
    }
    return true;

}

void manipulerBille(Cases plateau, Main m)
{

}

void quitter()
{

}