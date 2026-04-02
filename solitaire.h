#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cmath>

using namespace std;

const char carte[3]{ 'O', ' ', '\xdb' };

const size_t LARGEUR = 7;
const size_t HAUTEUR = 7;
const size_t LIGNE_CONTROLES = HAUTEUR + 2;
const size_t LIGNE_ANNULER = LIGNE_CONTROLES + 2;
const size_t LIGNE_MESSAGE = LIGNE_ANNULER + 2;

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
    Position depart = { 0, 0 };
    Position retiree = { 0, 0 };
    Position arrivee = { 0, 0 };
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

void demarrer(Cases& plateau, bool& choisirTrouDepart, Main& m, Action& act, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler);
Configuration menuAccueil(size_t& nbBillesRestantes , size_t& nbBillesMAX);
void initialiserPlateau(Cases plateau, Configuration conf);
bool afficherPlateau(Cases plateau, Main m, size_t& nbBillesRestantes);
Action saisirAction();
void afficherMain(Main& m);
void mettreTrouDepart(Action act, Cases& plateau, Main& m, bool& choisirTrouDepart, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler);
void reafficherCase(Main& m, Cases p);
void miseAJour(Action& act, Cases& plateau, Main& m, bool& choisirTrouDepart, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler);
bool positionValide(Position p, Cases plateau);
void manipulerBille(Cases plateau, Main& m, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& pastilleRefaire, size_t& pastilleAnnuler);
void prendreBille(Cases plateau, Main& m);
void deposerBille(Cases plateau, Main& m, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& pastilleRefaire, size_t& pastilleAnnuler);
void annuler(Cases& plateau, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler);
void refaire(Cases& plateau, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& pastilleRefaire, size_t& pastilleAnnuler);
void redemarrer(Cases& plateau, bool& choisirTrouDepart, Main& m, Action& act, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler);
void confirmerChoix(Action& act);
void quitter();