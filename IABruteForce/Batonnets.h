#pragma once
#include <iostream>
#include <string>
#include <map>
#include "Arbre.h"

void genere_enfants(Noeud<unsigned int>* racine);
Noeud<unsigned int>* coup_IA(Noeud<unsigned int>* racine);
void jeu(unsigned int nb, Noeud<unsigned int>* racine);
void batonnets(unsigned int nb);