#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Arbre.h"

namespace Dim {
	void genere_enfants(Noeud<unsigned int>* racine, unsigned int max_par_coups, map<pair<unsigned int, Joueur>, Noeud<unsigned int>*>* ensemble_plateaux = nullptr);
	Noeud<unsigned int>* coup_IA(Noeud<unsigned int>* racine);
	void jeu(unsigned int nb, Noeud<unsigned int>* racine, unsigned int max_par_coups);
	void commence(unsigned int nb=12, unsigned int max_par_coups = 3);
	Noeud<unsigned int>* coup_joueur(Noeud<unsigned int>* racine, unsigned int coup);
}