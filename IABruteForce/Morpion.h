#pragma once
#include <set>
#include <iostream>
#include <string>
#include <array>
#include <map>
#include "Arbre.h"

using namespace std;

namespace Morpion {
	enum class Case {
		Vide, Croix, Rond
	};

	enum class MiroirDirection {
		Horizontal, Vertical, DiagonalBD, DiagonalBG
	};

	enum class EtatPartie {
		RondGagne, CroixGagne, Egalite, PasFini
	};

	class Plateau {
	public:
		Plateau();
		array<array<Case, 3>, 3> tab;

		void rotation(size_t quarts_de_tours);
		void miroir(MiroirDirection dir);

		EtatPartie gagne();

		void trace();

		set<Plateau> get_equiv();
		Plateau get_representant();

		// Comparaison par ordre lexicographique
		friend bool operator<(const Plateau& a, const Plateau& b) {
			for (size_t i = 0; i < 3; i++) {
				for (size_t j = 0; j < 3; j++) {
					if (a.tab[i][j] < b.tab[i][j])
						return true;
					else if (a.tab[i][j] > b.tab[i][j])
						return false;
				}
			}
			return false;
		}

		friend bool operator==(const Plateau& a, const Plateau& b) {
			return a.tab == b.tab;
		}
	};

	EtatPartie correspondance_symbole_etat(Case c);
	void genere_enfants(Noeud<Plateau>* racine, map<Plateau, Noeud<Plateau>*>* ensemble_plateaux = nullptr);
	string case_str(Case c);
	void jeu(Noeud<Plateau>* racine);
	Noeud<Plateau>* coup_IA(Noeud<Plateau>* racine);
	Noeud<Plateau>* coup_joueur(Noeud<Plateau>* racine, Plateau p);

	void test();
}