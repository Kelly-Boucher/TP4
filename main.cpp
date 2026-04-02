#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cmath>

#include "cvm_23.h"
#include "solitaire.h"

using namespace std;

int main()
{
Cases plateau;
Main m;
Action act = Action::INCONNU;
bool choisirTrouDepart;
InfoPileSolitaire tableauPositions[35] = { 0, 0, 0 };
size_t indiceTableau = 0;
size_t nbBillesRestantes = 0;
size_t nbBillesMAX;
size_t pastilleAnnuler = 0;
size_t pastilleRefaire = 0;

show(false);

demarrer(plateau, choisirTrouDepart, m, act, tableauPositions, indiceTableau, nbBillesRestantes, nbBillesMAX, pastilleRefaire, pastilleAnnuler);

while (act != Action::QUITTER)
{
    act = saisirAction();                   //Saisir l'action que veut faire le joueur

    miseAJour(act, plateau, m, choisirTrouDepart, tableauPositions, indiceTableau, nbBillesRestantes, nbBillesMAX, pastilleRefaire, pastilleAnnuler);             //Mettre à jour la coordonnée position arrivée selon le déplacement effectuer par le joueur
    if(!m.enMain)
    coupPossible(plateau);
}

quitter();

return 0;

}