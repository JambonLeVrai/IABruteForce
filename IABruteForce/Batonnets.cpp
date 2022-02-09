#include "Batonnets.h"

using namespace std;

void Dim::genere_enfants(Noeud<unsigned int> *racine, unsigned int max_par_coups, map<pair<unsigned int, Joueur>, Noeud<unsigned int>*>* ensemble_plateaux) {
	if (ensemble_plateaux == nullptr)
		ensemble_plateaux = new map<pair<unsigned int, Joueur>, Noeud<unsigned int>*>();

	unsigned int plateau = racine->get_plateau();
	if (plateau == 0) {
		// Le plateau est vide, c'est donc une feuille, on doit lui donner son score
		if (racine->get_joueur() == Joueur::A)
			racine->set_score(Score::Defaite); // On est le dernier joueur, qui a joué le coup perdant, c'est donc une défaite
		else
			racine->set_score(Score::Victoire); // L'adversaire est le dernier joueur, qui vient de jouer le coup perdant, c'est donc une victoire

		return; // Dans tous les cas, on quitte directement la fonction, aucun noeud enfant à générer
	}

	// Heuristique : on garde en mémoire chaque noeud correspondant à chaque plateau possible pour éliminer les duplicats

	// Il reste des bâtonnets à retirer du plateau, donc on ajoute jusqu'à 3 noeuds qui correspondent aux trois possibilités de jeu
	for (unsigned int i = 1; i <= min(max_par_coups, racine->get_plateau()); i++) {
		Joueur prochain;
		if (racine->get_joueur() == Joueur::A)
			prochain = Joueur::B;
		else
			prochain = Joueur::A;

		unsigned int prochain_plateau = racine->get_plateau() - i;

		auto enfant = ensemble_plateaux->find(pair(prochain_plateau, prochain));
		if (enfant == ensemble_plateaux->end()) {
			// Ce noeud n'existe pas, on le créé
			// On créé le nouveau noeud
			Noeud<unsigned int>* nouveau_noeud = new Noeud<unsigned int>(prochain_plateau, prochain);

			ensemble_plateaux->insert(pair(pair(prochain_plateau, prochain), nouveau_noeud));

			// Et on génère les enfants de ce nouveau noeud
			Dim::genere_enfants(nouveau_noeud, max_par_coups, ensemble_plateaux);
			// Enfin, on ajoute ce noeud à la racine actuelle
			racine->ajoute_liaison(nouveau_noeud);
		}
		else {
			// Ce noeud existe déjà, on n'a qu'à lier vers ce noeud
			racine->ajoute_liaison(enfant->second);
		}
	}
}

Noeud<unsigned int>* Dim::coup_IA(Noeud<unsigned int>* racine) {
	Noeud<unsigned int>* n = racine->get_meilleur_noeud();
	return n;
}

Noeud<unsigned int>* Dim::coup_joueur(Noeud<unsigned int>* racine, unsigned int coup) {
	unsigned int p = racine->get_plateau() - coup;
	return racine->get_posterieur(p);
}

void Dim::jeu(unsigned int nb, Noeud<unsigned int> *racine, unsigned int max_par_coups) {
	unsigned int batonnets_actuels = nb;

	bool fini = false;
	Joueur J = Joueur::A; // L'IA commence toujours
	while (!fini) {
		unsigned int coup;
		cout << "Il y a actuellement " << batonnets_actuels << " batonnets." << endl;
		if (J == Joueur::A) {
			// Au tour de l'IA
			racine = Dim::coup_IA(racine);
			unsigned int b = racine->get_plateau();
			coup = batonnets_actuels - b;
		}
		else {
			// Au tour du joueur humain
			bool tour_joueur_termine = false;
			while (!tour_joueur_termine) {
				cout << "C'est votre tour, il y a actuellement " << batonnets_actuels << " batonnets. Combien en retirer ? (1~" << max_par_coups << ")" << endl;
				cin >> coup;
				if (coup > max_par_coups || coup < 1)
					throw;
				if (coup > batonnets_actuels)
					cout << "Impossible de retirer plus de batonnets qu'il n'y en a actuellement !" << endl;
				else
					tour_joueur_termine = true;
			}
			racine = Dim::coup_joueur(racine, coup);
		}

		batonnets_actuels -= coup;
		cout << coup << " batonnets ont ete retires. Il reste " << batonnets_actuels << " batonnets." << endl;
		if (batonnets_actuels == 0) {
			if (J == Joueur::A)
				cout << "L'IA a pris le dernier batonnet, et perd la partie, bien joue !" << endl;
			else
				cout << "Vous avez pris le dernier batonnet, vous perdez donc la partie." << endl;
			fini = true;
		}

		// Finalement, on change de joueur
		if (J == Joueur::A)
			J = Joueur::B;
		else
			J = Joueur::A;
	}

}

void Dim::commence(unsigned int nb, unsigned int max_par_coups) {
	// On commence (donc le prochain joueur à jouer, c'est nous, le joueur A), la racine contient toujours nb batonnets
	Noeud<unsigned int> racine(nb, Joueur::B);
	Dim::genere_enfants(&racine, max_par_coups);

	Dim::jeu(nb, &racine, max_par_coups);
}