#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cmath>

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
    bool        mettreTrouDepart = false;
    bool		enMain = false;
    Position	depart = { 3, 3 }, arrivee = { 3, 3 };
    Position	prise = { 0, 0 };
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
bool afficherPlateau(Cases plateau, Main m);
Action saisirAction();
void afficherMain(Main& m);
void mettreTrouDepart(Action act, Cases plateau, Main& m, bool& quitte);
void reafficherCase(Main& m, Cases p);
void miseAJour(Action act, Cases plateau, Main& m, bool& quitte);
bool positionValide(Position p, Cases plateau);
void manipulerBille(Cases plateau, Main& m);
void prendreBille(Cases plateau, Main& m);
void deposerBille(Cases plateau, Main& m);
void redemarrer();
void confirmerChoix(bool& quitte);
void quitter();

//*******************************
//      LA FONCTION MAIN        *
//*******************************

int main()
{
bool redemarre = false;

do {

Cases plateau;
Main m;
Action act = Action::INCONNU;
Configuration config;
bool choisirTrouDepart;
bool quitte = false;

show(false);

config = menuAccueil();     //Mettre la configuration choisir par l'utilisateur au menu accueil dans un objet "Configuration"

initialiserPlateau(plateau, config);     //Initialiser le plateau en fonction de la configuration choisie

choisirTrouDepart = afficherPlateau(plateau, m);               //Afficher le plateau à la console

afficherMain(m);                        //Afficher la main au bon endroit

if (choisirTrouDepart)
{
    gotoxy(0, LIGNE_MESSAGE);
    clreol();
    cout << "Deplacez-vous et choisissez la bille a retirer";
    m.mettreTrouDepart = true;
    mettreTrouDepart(act, plateau, m, quitte);
}
else
{
    plateau[3][3] = VIDE;
    //cout << carte[plateau[3][3]];
}

do 
{
    act = saisirAction();                   //Saisir l'action que veut faire le joueur

    miseAJour(act, plateau, m, quitte);             //Mettre à jour la coordonnée position arrivée selon le déplacement effectuer par le joueur (implémenter manipuler et quitter plus tard)
} while (!quitte);

} while (!redemarre);

quitter();


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

bool afficherPlateau(Cases plateau, Main m)
{
    for (size_t lig = 0; lig < HAUTEUR; ++lig)
    {
        for (size_t col = 0; col < LARGEUR; ++col)
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
    bool choisirTrou = false;
    bool choixValide = false;

    do {
        c = _getch();

        switch(c)
        {
        case 'o': choisirTrou = true; choixValide = true;   break;
        case 'n': choisirTrou = false; choixValide = true;  break;
        }

    } while (choixValide == false);

    return choisirTrou;
}

void mettreTrouDepart(Action act, Cases plateau, Main& m, bool& quitte)
{
    while (act != Action::QUITTER && m.mettreTrouDepart)
    {
        act = saisirAction();                   //Saisir l'action que veut faire le joueur

        miseAJour(act, plateau, m, quitte);             //Mettre à jour la coordonnée position arrivée selon le déplacement effectuer par le joueur (implémenter manipuler et quitter plus tard)
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
    gotoxy(m.arrivee.ligne, m.arrivee.colonne);
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
    gotoxy(m.depart.ligne, m.depart.colonne);
    Case c = plateau[m.depart.ligne][m.depart.colonne];
    cout << carte[c];   
}

void miseAJour(Action act, Cases plateau, Main& m, bool& quitte)           //Mettre à jour la position de la main à la console selon l'action effectué par leu joueur
{
    switch(act)
    {
        case Action::HAUT:      m.arrivee.colonne--;            break;
        case Action::BAS:       m.arrivee.colonne++;            break;
        case Action::DROITE:    m.arrivee.ligne++;              break;
        case Action::GAUCHE:    m.arrivee.ligne--;              break;
        case Action::MANIPULER: manipulerBille(plateau, m);     break;
        case Action::QUITTER:   confirmerChoix(quitte);               break;
    }
    if(positionValide(m.arrivee, plateau))        //Si le déplacement est valide, on fait un cout de la main à la position d'arrivée et on réaffiche la case sur laquelle la position de départ devient celle d'arrivée
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
    if (p.colonne > LARGEUR - 1 || p.ligne > HAUTEUR - 1 || p.ligne < 0 || p.colonne < 0)   //Ne pas sortir du plateau 7x7
    {
        return false;
    }
    if (plateau[p.ligne][p.colonne] == NO_DISPO)        //Ne pas aller sur une case NO_DISPO
    {
        return false;
    }
    return true;
}

void manipulerBille(Cases plateau, Main& m)
{
    if (m.mettreTrouDepart)
    {
        plateau[m.arrivee.ligne][m.arrivee.colonne] = VIDE;
        m.mettreTrouDepart = false;
    }
    else
    {
        if (m.enMain)
        {
        m.enMain = false;               //On vient déposer la bille
        deposerBille(plateau, m);
        }
        else
        {
            if (plateau[m.arrivee.ligne][m.arrivee.colonne] == BILLE)
            {
            m.enMain = true;                //On vient de prendre une bille -> petit x rouge
            prendreBille(plateau, m);
            }
        }
    }
}

void prendreBille(Cases plateau, Main& m)
{
    m.prise = m.arrivee;        //Prendre en note la position de départ (la bille qui a été prise)
    plateau[m.prise.ligne][m.prise.colonne] = VIDE;
}

void deposerBille(Cases plateau, Main& m)
{
    if(plateau[m.arrivee.ligne][m.arrivee.colonne] == VIDE && ((abs(m.prise.ligne - m.arrivee.ligne) == 2 && abs(m.prise.colonne - m.arrivee.colonne) == 0) || abs(m.prise.colonne - m.arrivee.colonne) == 2 && abs(m.prise.ligne - m.arrivee.ligne) == 0))
    {
        Position intermediaire;

        //Calculer la position de la bille intermédiaire
        if (m.arrivee.ligne == m.prise.ligne)                                    //Si la ligne a la même valeur, alors le déplacement a été effectué sur une colonne et vice-versa
        {
            if (m.arrivee.colonne > m.prise.colonne)
            {
                intermediaire.colonne = m.arrivee.colonne - 1;
            }
            else
            {
                intermediaire.colonne = m.prise.colonne - 1;
            }
            intermediaire.ligne = m.arrivee.ligne;
        }
        if (m.arrivee.colonne == m.prise.colonne)
        {
            if (m.arrivee.ligne > m.prise.ligne)
            {
                intermediaire.ligne = m.arrivee.ligne - 1;
            }
            else
            {
                intermediaire.ligne = m.prise.ligne - 1;
            }
            intermediaire.colonne = m.arrivee.colonne;
        }

        plateau[m.arrivee.ligne][m.arrivee.colonne] = BILLE;
        plateau[intermediaire.ligne][intermediaire.colonne] = VIDE;

    //Pour afficher tout de suite une case vide à l'endroit de la bille intermédiaire, sinon il faut attendre que le X passe par-dessus vu qu'on ne réaffiche pas le plateau au complet a chaque mouvement
    gotoxy(intermediaire.ligne, intermediaire.colonne);                             
    Case Intermed = plateau[intermediaire.ligne][intermediaire.colonne];
    cout << carte[Intermed];
    }
    else {
        plateau[m.prise.ligne][m.prise.colonne] = BILLE;            //Si le joueur dépose la bille a un endroit non valide, on remet la bille prise au départ à sa place et enMain redevient faux
        m.enMain = false;
        gotoxy(m.prise.ligne, m.prise.colonne);
        Case caseVide = plateau[m.prise.ligne][m.prise.colonne];
        cout << carte[caseVide];
    }
}

void confirmerChoix(bool& quitte)
{
    char c;
    bool choixValide = false;
    gotoxy(0, LIGNE_MESSAGE);
    clreol();
    cout << "Etes-vous sur de vouloir quitter? (O/N)";

    do
    {
        c = _getch();
        switch (c)
        {
        case 'o': quitte = true;    choixValide = true; break;
        case 'n': quitte = false;   choixValide = true; break;
        }
    } while (!choixValide);
    gotoxy(0, LIGNE_MESSAGE);
    clreol();
}

void quitter()
{
    clrscr();
    cout << "Au revoir!";
    _getch();
}