/*
Mini-projet fin S1L1
MERLET Raphaël
*/

#include <iostream>
#include <fstream>
#include "liste.hpp"
using namespace std;

struct Horodatage
{
    int mois, jour, annee;
};


struct Tache
{
    string entree, sortie;
    int duree;
    float cout;
    Horodatage debut, fin;
    liste<string> regions;
};

struct Sortie
{
    string region;
    Horodatage debut, fin;
    float cout;
};



