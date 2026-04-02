#include "solitaire.h"
#include "cvm_23.h"
using namespace std;

void demarrer(Cases& plateau, bool& choisirTrouDepart, Main& m, Action& act, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler) {

    //La fonction menuAccueil retourne une Configuration utilisé comme paramètre dans par la fonction initialiserPlateau
    initialiserPlateau(plateau, menuAccueil(nbBillesRestantes, nbBillesMAX));

    choisirTrouDepart = afficherPlateau(plateau, m, nbBillesRestantes);               //Afficher le plateau à la console

    afficherMain(m);                        //Afficher la main au bon endroit

    if (choisirTrouDepart)
    {
        gotoxy(0, LIGNE_MESSAGE);
        clreol();
        cout << "Deplacez-vous et choisissez la bille a retirer";
        m.mettreTrouDepart = true;
        mettreTrouDepart(act, plateau, m, choisirTrouDepart, tableauPositions, indiceTableau, nbBillesRestantes, nbBillesMAX, pastilleRefaire, pastilleAnnuler);
    }
    else
    {
        plateau[3][3] = VIDE;
    }
}

Configuration menuAccueil(size_t& nbBillesRestantes, size_t& nbBillesMAX)
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

    if (config == Configuration::ANGLAIS)
    {
        nbBillesRestantes = 32;
    }
    if (config == Configuration::EUROPEEN)
    {
        nbBillesRestantes = 36;
    }
    if (config == Configuration::PRATIQUE)
    {
        nbBillesRestantes = 12;
    }
    nbBillesMAX = nbBillesRestantes;

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

bool afficherPlateau(Cases plateau, Main& m, size_t& nbBillesRestantes)
{
    for (size_t lig = 0; lig < HAUTEUR; ++lig)
    {
        for (size_t col = 0; col < LARGEUR; ++col)
        {
            cout << carte[plateau[lig][col]];
        }
        cout << endl;
    }

    m.depart = { 3, 3 };
    m.arrivee = { 3, 3 };

    gotoxy(0, LIGNE_CONTROLES);
    cout << "Gauche: a | Haut: w | Droite: d | Bas: s | Reinitialiser: r | Quitter: q | Annuler: u | Refaire: y";

    gotoxy(0, LIGNE_MESSAGE);
    cout << "Voulez-vous choisir le trou de depart (O/N) ?";
    char c;
    bool choisirTrou = false;
    bool choixValide = false;

    do {
        c = _getch();

        switch (c)
        {
        case 'o': choisirTrou = true; choixValide = true;   break;
        case 'n': choisirTrou = false; choixValide = true;  break;
        }

    } while (choixValide == false);
    gotoxy(0, LIGNE_MESSAGE);
    clreol();
    gotoxy(15, 0);
    clreol();
    cout << "Nombre de billes restantes : " << nbBillesRestantes;

    return choisirTrou;
}

void mettreTrouDepart(Action act, Cases& plateau, Main& m, bool& choisirTrouDepart, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler)
{
    while (act != Action::QUITTER && m.mettreTrouDepart)
    {
        act = saisirAction();                   //Saisir l'action que veut faire le joueur

        miseAJour(act, plateau, m, choisirTrouDepart, tableauPositions, indiceTableau, nbBillesRestantes, nbBillesMAX, pastilleRefaire, pastilleAnnuler);             //Mettre à jour la coordonnée position arrivée selon le déplacement effectuer par le joueur
    }
}

Action saisirAction()
{
    char c;
    Action act;

    do
    {
        c = _getch();
        switch (c)
        {
        case 'w': act = Action::HAUT;           break;
        case 's': act = Action::BAS;            break;
        case 'a': act = Action::GAUCHE;         break;
        case 'd': act = Action::DROITE;         break;
        case ' ': act = Action::MANIPULER;      break;
        case 'q': act = Action::QUITTER;        break;
        case 'r': act = Action::REINITIALISER;  break;
        case 'u': act = Action::ANNULER;        break;
        case 'y': act = Action::REFAIRE;        break;
        default: act = Action::INCONNU;
        }
    } while (act == Action::INCONNU);

    return act;
}

void afficherMain(Main& m)
{
    gotoxy(m.arrivee.ligne, m.arrivee.colonne);
    if (m.enMain)
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

void miseAJour(Action& act, Cases& plateau, Main& m, bool& choisirTrouDepart, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler)          
{
    switch (act)
    {
    case Action::HAUT:          m.arrivee.colonne--;                                                                                                                                break;
    case Action::BAS:           m.arrivee.colonne++;                                                                                                                                break;
    case Action::DROITE:        m.arrivee.ligne++;                                                                                                                                  break;
    case Action::GAUCHE:        m.arrivee.ligne--;                                                                                                                                  break;
    case Action::MANIPULER:     manipulerBille(plateau, m, tableauPositions, indiceTableau, nbBillesRestantes, pastilleRefaire, pastilleAnnuler);                                   break;
    case Action::QUITTER:       confirmerChoix(act);                                                                                                                                break;
    case Action::REINITIALISER: redemarrer(plateau, choisirTrouDepart, m, act, tableauPositions, indiceTableau, nbBillesRestantes, nbBillesMAX, pastilleRefaire, pastilleAnnuler);  break;
    case Action::ANNULER:       annuler(plateau, tableauPositions, indiceTableau, nbBillesRestantes, nbBillesMAX, pastilleRefaire, pastilleAnnuler);                                break;
    case Action::REFAIRE:       refaire(plateau, tableauPositions, indiceTableau, nbBillesRestantes, pastilleRefaire, pastilleAnnuler);                                             break;
    }
    if (positionValide(m.arrivee, plateau))        //Si le déplacement est valide, on fait un cout de la main à la position d'arrivée et on réaffiche la case sur laquelle la position de départ devient celle d'arrivée
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

void manipulerBille(Cases plateau, Main& m, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& pastilleRefaire, size_t& pastilleAnnuler)
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
            deposerBille(plateau, m, tableauPositions, indiceTableau, nbBillesRestantes, pastilleRefaire, pastilleAnnuler);
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

void deposerBille(Cases plateau, Main& m, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& pastilleRefaire, size_t& pastilleAnnuler)
{
    if (plateau[m.arrivee.ligne][m.arrivee.colonne] == VIDE && ((abs(m.prise.ligne - m.arrivee.ligne) == 2 && abs(m.prise.colonne - m.arrivee.colonne) == 0) || abs(m.prise.colonne - m.arrivee.colonne) == 2 && abs(m.prise.ligne - m.arrivee.ligne) == 0))
    {
        Position intermediaire;

        //Calculer la position de la bille intermédiaire
        if (m.arrivee.ligne == m.prise.ligne)                                    //Si les lignes ont la même valeur, alors le déplacement a été effectué sur une colonne et vice-versa
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

        //Pour afficher tout de suite une case vide à l'endroit de la bille intermédiaire, sinon il faut attendre que le X passe par-dessus vu qu'on ne réaffiche pas le plateau au complet à chaque mouvement
        gotoxy(intermediaire.ligne, intermediaire.colonne);
        Case Intermed = plateau[intermediaire.ligne][intermediaire.colonne];
        cout << carte[Intermed];

        //Imprimer une pastille mauve
        gotoxy(pastilleAnnuler, LIGNE_ANNULER);
        setcolor(Color::pur);
        cout << 'O';
        setcolor(Color::wht);
        pastilleAnnuler++;

        //Prendre en note la position de départ et d'arrivée de la bille manipulée et la position de la bille intermédiaire. Mettre ces positions dans le tableau de positions
        tableauPositions[indiceTableau] = { m.prise, intermediaire, m.arrivee};
        indiceTableau++;

        //Décrémenter le nombre de billes restantes
        nbBillesRestantes--;
        gotoxy(15, 0);
        clreol();
        cout << "Nombre de billes restantes : " << nbBillesRestantes;

        //Réinitialiser les valeurs lignes de départ dans le tableau de position à -1 pour avoir une condition qui empêche de faire un Redo sur ces éléments
        for (size_t i = indiceTableau; i < 35; i++)
        {
            tableauPositions[indiceTableau].depart.ligne = -1;
        }

        //Les Redo sont effacés
        gotoxy(0, LIGNE_ANNULER + 1);
        clreol();
        pastilleRefaire = 0;

        //Afficher un message si le joueur gagne
        if (nbBillesRestantes == 0)
        {
            gotoxy(0, LIGNE_MESSAGE);
            clreol();
            cout << "Victoire!";
        }
    }
    else {
        plateau[m.prise.ligne][m.prise.colonne] = BILLE;            //Si le joueur dépose la bille a un endroit non valide, on remet la bille prise au départ à sa place et enMain redevient faux
        m.enMain = false;
        gotoxy(m.prise.ligne, m.prise.colonne);
        Case caseVide = plateau[m.prise.ligne][m.prise.colonne];
        cout << carte[caseVide];
    }
}


void annuler(Cases& plateau, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler)
{
    if (indiceTableau > 0)
    {
    ////Décrémenter l'indice du tableau de 1 pour accéder au coup avant
    indiceTableau--;

    ////Changer les cases du tableau pour qu'elles aient la configuration au coup précédent
    plateau[tableauPositions[indiceTableau].depart.ligne][tableauPositions[indiceTableau].depart.colonne] = BILLE;
    plateau[tableauPositions[indiceTableau].retiree.ligne][tableauPositions[indiceTableau].retiree.colonne] = BILLE;
    plateau[tableauPositions[indiceTableau].arrivee.ligne][tableauPositions[indiceTableau].arrivee.colonne] = VIDE;

    ////Afficher les nouvelles cases correctement
    gotoxy(tableauPositions[indiceTableau].depart.ligne, tableauPositions[indiceTableau].depart.colonne);
    Case depart = plateau[tableauPositions[indiceTableau].depart.ligne][tableauPositions[indiceTableau].depart.colonne];
    cout << carte[depart];

    gotoxy(tableauPositions[indiceTableau].retiree.ligne, tableauPositions[indiceTableau].retiree.colonne);
    Case retiree = plateau[tableauPositions[indiceTableau].retiree.ligne][tableauPositions[indiceTableau].retiree.colonne];
    cout << carte[retiree];

    gotoxy(tableauPositions[indiceTableau].arrivee.ligne, tableauPositions[indiceTableau].arrivee.colonne);
    Case arrivee = plateau[tableauPositions[indiceTableau].arrivee.ligne][tableauPositions[indiceTableau].arrivee.colonne];
    cout << carte[arrivee];

    //Imprimer une case jaune
    gotoxy(pastilleRefaire, LIGNE_ANNULER + 1);
    setcolor(Color::yel);
    cout << 'O';
    setcolor(Color::wht);
    pastilleRefaire++;

    //Imprimer une case noir à l'endroit de la dernière pastille mauve
    pastilleAnnuler--;
    gotoxy(pastilleAnnuler, LIGNE_ANNULER);
    setcolor(Color::blk);
    cout << 'O';
    setcolor(Color::wht);

    if (nbBillesRestantes < nbBillesMAX)
    {
        nbBillesRestantes++;
        gotoxy(15, 0);
        clreol();
        cout << "Nombre de billes restantes : " << nbBillesRestantes;
    }
    }
}

void refaire(Cases& plateau, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& pastilleRefaire, size_t& pastilleAnnuler)
{
    if(tableauPositions[indiceTableau].depart.ligne != -1 && indiceTableau < 35)
    {
    //Changer les cases du tableau pour qu'elles aient la configuration du coup suivant
    plateau[tableauPositions[indiceTableau].depart.ligne][tableauPositions[indiceTableau].depart.colonne] = VIDE;
    plateau[tableauPositions[indiceTableau].retiree.ligne][tableauPositions[indiceTableau].retiree.colonne] = VIDE;
    plateau[tableauPositions[indiceTableau].arrivee.ligne][tableauPositions[indiceTableau].arrivee.colonne] = BILLE;

    //Afficher les nouvelles cases correctement
    gotoxy(tableauPositions[indiceTableau].depart.ligne, tableauPositions[indiceTableau].depart.colonne);
    Case depart = plateau[tableauPositions[indiceTableau].depart.ligne][tableauPositions[indiceTableau].depart.colonne];
    cout << carte[depart];

    gotoxy(tableauPositions[indiceTableau].retiree.ligne, tableauPositions[indiceTableau].retiree.colonne);
    Case retiree = plateau[tableauPositions[indiceTableau].retiree.ligne][tableauPositions[indiceTableau].retiree.colonne];
    cout << carte[retiree];

    gotoxy(tableauPositions[indiceTableau].arrivee.ligne, tableauPositions[indiceTableau].arrivee.colonne);
    Case arrivee = plateau[tableauPositions[indiceTableau].arrivee.ligne][tableauPositions[indiceTableau].arrivee.colonne];
    cout << carte[arrivee];

    //Imprimer une case mauve
    gotoxy(pastilleAnnuler, LIGNE_ANNULER);
    setcolor(Color::pur);
    cout << 'O';
    setcolor(Color::wht);
    pastilleAnnuler++;

    //Effacer la dernière pastille refaire
    if (pastilleRefaire > 0)
    {
    pastilleRefaire--;
    gotoxy(pastilleRefaire, LIGNE_ANNULER + 1);
    setcolor(Color::blk);
    cout << 'O';
    setcolor(Color::wht);
    }

    //Incrémenter l'indice du tableau de 1 pour accéder au coup après
    indiceTableau++;
    if(nbBillesRestantes)
    nbBillesRestantes--;
    gotoxy(15, 0);
    clreol();
    cout << "Nombre de billes restantes : " << nbBillesRestantes;
    }
}


void redemarrer(Cases& plateau, bool& choisirTrouDepart, Main& m, Action& act, InfoPileSolitaire tableauPositions[35], size_t& indiceTableau, size_t& nbBillesRestantes, size_t& nbBillesMAX, size_t& pastilleRefaire, size_t& pastilleAnnuler)
{
    char c;
    bool choixValide = false;
    bool redemarrer;
    gotoxy(0, LIGNE_MESSAGE);
    clreol();
    cout << "Voulez-vous redemarrer une partie ? (O/N)";
    do
    {
        c = _getch();
        switch (c)
        {
        case 'o': redemarrer = true;    choixValide = true; break;
        case 'n': redemarrer = false;   choixValide = true; break;
        }
    } while (!choixValide);

    if (redemarrer)
    {
        clrscr();
        demarrer(plateau, choisirTrouDepart, m, act, tableauPositions, indiceTableau, nbBillesRestantes, nbBillesMAX, pastilleRefaire, pastilleAnnuler);
    }
    else
    {
        gotoxy(0, LIGNE_MESSAGE);
        clreol();
    }
    pastilleRefaire = 0;
    pastilleAnnuler = 0;
}

void confirmerChoix(Action& act)
{
    char c;
    bool choixValide = false;
    bool quitte;
    gotoxy(0, LIGNE_MESSAGE);
    clreol();
    cout << "Etes-vous sur de vouloir quitter ? (O/N)";

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
    if (!quitte)
    {
        act = Action::INCONNU;
    }
}

void quitter()
{
    clrscr();
    cout << "Au revoir!";
    _getch();
}

void coupPossible(Cases& plateau)
{
    int coupPossible = 0;

    //Itérer chaque case pour voir s'il reste un coup possible
    for (size_t lig = 0; lig < HAUTEUR; ++lig)
    {
        for (size_t col = 0; col < LARGEUR; ++col)
        {
            if (plateau[lig][col] == BILLE)
            {
                if (plateau[lig - 1][col] == BILLE && plateau[lig - 2][col] == VIDE)
                {
                    coupPossible++;
                }
                else if (plateau[lig][col - 1] == BILLE && plateau[lig][col - 2] == VIDE)
                {
                    coupPossible++;
                }
                else if (plateau[lig + 1][col] == BILLE && plateau[lig + 2][col] == VIDE)
                {
                    coupPossible++;
                }
                else if (plateau[lig][col + 1] == BILLE && plateau[lig][col + 2] == VIDE)
                {
                    coupPossible++;
                }
            }
        }
    }
    if (coupPossible == 0)
    {
        gotoxy(0, LIGNE_MESSAGE);
        clreol();
        cout << "Il n'y a plus de coup possible!";
    }
}