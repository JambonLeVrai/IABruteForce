#pragma once
#include <unordered_set>
#include <vector>
#include <optional>
#include <algorithm>
#include <iterator>

using namespace std;

enum class Joueur {
	A, B
};

enum class Score {
	Victoire, Defaite, Egalite
};

template<class T>
class Noeud {
public:
	Noeud(T _id, Joueur _j);
	void ajoute_liaison(Noeud<T>* l);
	Score get_score();
	void set_score(Score _s);
	unordered_set<Noeud<T>*> get_feuilles();
	Noeud<T>* get_meilleur_noeud();
	size_t get_nombre_noeuds();
	T get_plateau();
	Joueur get_joueur();
	Noeud<T>* get_posterieur(T plateau);

private:
	bool is_feuille();
	T plateau;
	Joueur j; // Contient l'information sur le joueur qui vient de jouer
	vector<Noeud<T>*> noeuds_posterieurs;
	optional<Score> s;
};

template<class T>
Noeud<T>::Noeud(T _id, Joueur _j) : plateau(_id), j(_j) {}

template<class T>
void Noeud<T>::ajoute_liaison(Noeud<T>* l) {
	noeuds_posterieurs.push_back(l);
}

template<class T>
Score Noeud<T>::get_score() {
	if (is_feuille() && !s)
		throw; // Exception : toute feuille doit avoir un score
	else if (!s) {
		// Ce n'est pas une feuille, et on n'a pas calculé le score de ce noeud, il faut donc le calculer
		if (j == Joueur::A) {
			// Le joueur qui va jouer est donc le joueur B (adversaire), sur lequel on n'a aucun contrôle
			s = Score::Victoire; // On commence par supposer que ce noeud est victorieux
			for (Noeud<T> *it : noeuds_posterieurs) {
				// On itère sur tous les noeuds postérieurs
				if (it->get_score() == Score::Defaite) {
					// Si le joueur peut mener à un noeud de défaite, alors ce noeud est un noeud de défaite et on peut sortir de la boucle
					s = Score::Defaite;
					break;
				}
				else if (it->get_score() == Score::Egalite) {
					// Si c'est un noeud d'égalité, alors le noeud est au mieux un noeud d'égalité
					s = Score::Egalite;
				}
				// Sinon, c'est un noeud victorieux, alors on passe simplement au suivant
			}
		}
		else {
			// Le joueur qui va jouer est le joueur A (nous), donc le score de ce noeud est le meilleur score accessible
			s = Score::Defaite;
			for (Noeud<T> *it : noeuds_posterieurs) {
				// On itère sur tous les noeuds postérieurs
				if (it->get_score() == Score::Victoire) {
					// Si le joueur peut mener à un noeud victorieux, alors ce noeud est un noeud victorieux et on peut sortir de la boucle
					s = Score::Victoire;
					break;
				}
				else if (it->get_score() == Score::Egalite) {
					// Si c'est un noeud d'égalité, alors le noeud est au mieux un noeud d'égalité
					s = Score::Egalite;
				}
				// Sinon, c'est un noeud victorieux, alors on passe simplement au suivant
			}
		}
	}
	return *s;
}

template<class T>
void Noeud<T>::set_score(Score _s) {
	s = _s;
}

template<class T>
unordered_set<Noeud<T>*> Noeud<T>::get_feuilles() {
	unordered_set<Noeud<T>*> feuilles;
	if (is_feuille()) {
		// Ce noeud est une feuille, on renvoie donc simplement cette feuille
		feuilles.insert(this);
	}
	else {
		// Sinon, les feuilles sont l'ensemble des feuilles des noeuds enfants
		for (Noeud<T> *it : noeuds_posterieurs) {
			// Pour chaque noeud enfant, on ajoute les feuilles descendantes de ce noeud
			unordered_set<Noeud<T>*> feuilles_posterieures = it->get_feuilles();
			feuilles.insert(feuilles_posterieures.begin(), feuilles_posterieures.end());
		}
	}
	return feuilles;
}

template<class T>
Noeud<T>* Noeud<T>::get_meilleur_noeud() {
	if (noeuds_posterieurs.empty())
		throw;

	Noeud<T>* sortie = noeuds_posterieurs.front();
	for (Noeud<T>* it : noeuds_posterieurs) {
		if (it->get_score() == Score::Victoire)
			return it;
		else if (it->get_score() == Score::Egalite)
			sortie = it;
	}
	return sortie;
}

template<class T>
inline size_t Noeud<T>::get_nombre_noeuds() {
	if (noeuds_posterieurs.empty())
		return 1;
	else {
		size_t somme = 0;
		for (auto it : noeuds_posterieurs)
			somme += it->get_nombre_noeuds();
		return somme;
	}
}

template<class T>
T Noeud<T>::get_plateau() {
	return plateau;
}

template<class T>
Joueur Noeud<T>::get_joueur() {
	return j;
}

template<class T>
inline Noeud<T>* Noeud<T>::get_posterieur(T plateau) {
	for (auto* it : noeuds_posterieurs) {
		if (it->get_plateau() == plateau)
			return it;
	}
	throw;
}

template<class T>
bool Noeud<T>::is_feuille() {
	return noeuds_posterieurs.empty();
}
