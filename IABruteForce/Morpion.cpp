#include "Morpion.h"
#include "Arbre.h"

void Morpion::genere_enfants(Noeud<Plateau>* racine, map<Plateau, Noeud<Plateau>*>* ensemble_plateaux) {
	if (ensemble_plateaux == nullptr)
		ensemble_plateaux = new map<Plateau, Noeud<Plateau>*>();

	auto p = racine->get_plateau().tab;
	// On regarde si ce noeud est une feuille
	switch (racine->get_plateau().gagne()) {
	case EtatPartie::CroixGagne:
		racine->set_score(Score::Defaite);
		return;
		break;
	case EtatPartie::RondGagne:
		racine->set_score(Score::Victoire);
		return;
		break;
	case EtatPartie::Egalite:
		racine->set_score(Score::Egalite);
		return;
		break;
	}

	// Le noeud a une suite
	for (size_t i = 0; i < p.size(); i++) {
		for (size_t j = 0; j < p[i].size(); j++) {
			if (p[i][j] == Case::Vide) {
				Plateau nouveauPlateau = racine->get_plateau();
				Joueur prochain;
				if (racine->get_joueur() == Joueur::A) {
					nouveauPlateau.tab[i][j] = Case::Croix;
					prochain = Joueur::B;
				}
				else {
					nouveauPlateau.tab[i][j] = Case::Rond;
					prochain = Joueur::A;
				}

				/*
				* Version heuristique à retravailler
				// Le premier équivalent est le représentant de la classe d'équivalence
				auto noeud_representant = ensemble_plateaux->find(nouveauPlateau);
				if (noeud_representant == ensemble_plateaux->end()) {
					// Le noeud n'existe pas, il faut donc l'ajouter
					Noeud<Plateau>* nouveau_noeud = new Noeud<Plateau>(nouveauPlateau.get_representant(), prochain); // Le seul noeud est le représentant
				}
				else {
					// Un noeud équivalent existe déjà
					racine->ajoute_liaison(noeud_representant->second);
				}*/

				// Version pure brute force
				auto noeud = ensemble_plateaux->find(nouveauPlateau);
				if (noeud == ensemble_plateaux->end()) {
					Noeud<Plateau>* nouveau_noeud = new Noeud<Plateau>(nouveauPlateau, prochain);
					ensemble_plateaux->insert(pair(nouveauPlateau, nouveau_noeud));

					genere_enfants(nouveau_noeud, ensemble_plateaux);

					racine->ajoute_liaison(nouveau_noeud);
				}
				else {
					racine->ajoute_liaison(noeud->second);
				}
			}
		}
	}
}

string Morpion::case_str(Case c) {
	switch (c) {
	case Case::Croix:
		return "X";
		break;
	case Case::Rond:
		return "O";
		break;
	case Case::Vide:
		return " ";
		break;
	}
}

void Morpion::jeu(Noeud<Plateau>* racine) {
	Plateau p;

	Joueur joueur_actuel = Joueur::A;
	bool jeu_termine = false;

	// On affiche le plateau au début
	p.trace();

	while (!jeu_termine) {

		if (joueur_actuel == Joueur::B) {
			// Joueur humain
			unsigned int x;
			unsigned int y;
			bool trouve = false;
			while (!trouve) {
				cout << "Quelle case jouer ? X : " << endl;
				cin >> x;
				cout << "Y : " << endl;
				cin >> y;

				if (x > 3 || x < 1 || y > 3 || y < 1)
					cout << "Non." << endl;
				else {
					if (p.tab[x-1][y-1] != Case::Vide) {
						cout << "La case est deja utilisee !" << endl;
					}
					else {
						p.tab[x-1][y-1] = Case::Croix;
						trouve = true;
					}
				}
			}

			racine = Morpion::coup_joueur(racine, p);
		}
		else {
			// Tour de l'IA
			racine = racine->get_meilleur_noeud();
			p = racine->get_plateau();
		}

		// On affiche le plateau
		p.trace();

		// On check si l'un des joueurs a gagné
		EtatPartie gagne = p.gagne();

		switch (gagne) {
		case EtatPartie::RondGagne:
			cout << "L'IA a remporte la partie" << endl;
			jeu_termine = true;
			break;
		case EtatPartie::CroixGagne:
			cout << "Vous avez gagne la partie" << endl;
			jeu_termine = true;
			break;
		case EtatPartie::Egalite:
			cout << "La partie est une egalite" << endl;
			jeu_termine = true;
			break;
		case EtatPartie::PasFini:
			if (joueur_actuel == Joueur::A)
				joueur_actuel = Joueur::B;
			else
				joueur_actuel = Joueur::A;
			break;
		}
	}
}

Morpion::Plateau::Plateau() {
	tab = { Case::Vide };
}

void Morpion::Plateau::rotation(size_t quarts_de_tours) {
	array<array<Case, 3>, 3> nouveau_tab = { Case::Vide };
	switch (quarts_de_tours) {
	case 0: // ??
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[i][j] = tab[i][j];
		break;
	case 1:
		// quart de tour indirect
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[2 - j][i] = tab[i][j];
		break;
	case 2:
		// demi-tour
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[2 - i][2 - j] = tab[i][j];
		break;
	case 3:
		// quart de tour direct
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[j][2 - i] = tab[i][j];
		break;
	default:
		throw;
		break;
	}

	tab = nouveau_tab;
}

void Morpion::Plateau::miroir(MiroirDirection horizontal) {
	array<array<Case, 3>, 3> nouveau_tab = { Case::Vide };
	switch (horizontal) {
	case MiroirDirection::Horizontal:
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[2 - i][j] = tab[i][j];
		break;
	case MiroirDirection::Vertical:
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[i][2-j] = tab[i][j];
		break;
	case MiroirDirection::DiagonalBD:
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[j][i] = tab[i][j];
		break;
	case MiroirDirection::DiagonalBG:
		for (size_t i = 0; i < 3; i++)
			for (size_t j = 0; j < 3; j++)
				nouveau_tab[2 - j][2 - i] = tab[i][j];
		break;
	}

	tab = nouveau_tab;
}

Morpion::EtatPartie Morpion::correspondance_symbole_etat(Case c) {
	switch (c) {
	case Case::Croix:
		return EtatPartie::CroixGagne;
		break;
	case Case::Rond:
		return EtatPartie::RondGagne;
		break;
	default:
		throw;
		break;
	}
}

Noeud<Morpion::Plateau>* Morpion::coup_joueur(Noeud<Plateau>* racine, Plateau p) {
	return racine->get_posterieur(p);
}

Noeud<Morpion::Plateau>* Morpion::coup_IA(Noeud<Plateau>* racine) {
	Noeud<Plateau>* n = racine->get_meilleur_noeud();
	return n;
}

Morpion::EtatPartie Morpion::Plateau::gagne() {
	Case gagne = Case::Vide;

	// On commence par regarder les colonnes
	for (unsigned int i = 0; i < 3; i++) {
		Case check = tab[i][0];
		bool colonne = false;
		if (check != Case::Vide) {
			colonne = true;
			for (unsigned int j = 1; j < 3; j++) {
				if (tab[i][j] != check) {
					colonne = false;
					break;
				}
			}
		}
		if (colonne) {
			return correspondance_symbole_etat(check);
			break;
		}
	}
	// Puis les lignes
	if (gagne == Case::Vide) {
		for (unsigned int i = 0; i < 3; i++) {
			Case check = tab[0][i];
			bool ligne = false;
			if (check != Case::Vide) {
				ligne = true;
				for (unsigned int j = 1; j < 3; j++) {
					if (tab[j][i] != check) {
						ligne = false;
						break;
					}
				}
			}
			if (ligne) {
				return correspondance_symbole_etat(check);
				break;
			}
		}
	}
	// Puis les deux diagonales
	if (gagne == Case::Vide) {
		if (tab[0][0] != Case::Vide) {
			if (tab[0][0] == tab[1][1] && tab[1][1] == tab[2][2])
				return correspondance_symbole_etat(tab[1][1]);
		}
		if (tab[0][2] != Case::Vide) {
			if (tab[0][2] == tab[1][1] && tab[1][1] == tab[2][0])
				return correspondance_symbole_etat(tab[0][2]);
		}
	}

	// Enfin, si personne n'a gagné, on regarde si la partie est finie (égalité) ou s'il reste des places vides
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			if (tab[i][j] == Case::Vide)
				return EtatPartie::PasFini;

	return EtatPartie::Egalite;
}

void Morpion::Plateau::trace() {
	cout << "-------" << endl;
	for (unsigned int j = 0; j < 3; j++) {
		for (unsigned int i = 0; i < 3; i++) {
			cout << "|" << case_str(tab[i][j]);
		}
		cout << "|" << endl << "-------" << endl;
	}
}

set<Morpion::Plateau> Morpion::Plateau::get_equiv() {
	set<Morpion::Plateau> s;
	s.insert(*this);

	Plateau p(*this);
	p.rotation(1);
	s.insert(p);

	p = *this;
	p.rotation(2);
	s.insert(p);

	p = *this;
	p.rotation(3);
	s.insert(p);

	p = *this;
	p.miroir(MiroirDirection::Horizontal);
	s.insert(p);

	p = *this;
	p.miroir(MiroirDirection::Vertical);
	s.insert(p);

	p = *this;
	p.miroir(MiroirDirection::DiagonalBD);
	s.insert(p);

	p = *this;
	p.miroir(MiroirDirection::DiagonalBG);
	s.insert(p);

	return s;
}

Morpion::Plateau Morpion::Plateau::get_representant() {
	return (*get_equiv().begin());
}

void Morpion::test() {
	Plateau p;
	p.tab[0][0] = Case::Croix;
	p.tab[1][0] = Case::Vide;
	p.tab[2][0] = Case::Vide;
	p.tab[0][1] = Case::Vide;
	p.tab[1][1] = Case::Vide;
	p.tab[2][1] = Case::Vide;
	p.tab[0][2] = Case::Vide;
	p.tab[1][2] = Case::Vide;
	p.tab[2][2] = Case::Vide;

	set<Plateau> s = p.get_equiv();

	for (Plateau it : s) {
		it.trace();
	}
}