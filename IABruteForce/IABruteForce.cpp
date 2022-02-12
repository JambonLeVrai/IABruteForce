// IABruteForce.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "Batonnets.h"
#include "Morpion.h"
#include "Arbre.h"

int main(int argc, char** argv)
{
	Noeud<Morpion::Plateau> *p = new Noeud<Morpion::Plateau>(*new Morpion::Plateau(), Joueur::B);
	Morpion::genere_enfants(p);
	Morpion::jeu(p);
}