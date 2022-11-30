/*
Mini-projet fin S1L1
MERLET Raphaël
*/

#include <iostream>
#include <fstream>
#include <cassert>
#include "liste.hpp"
using namespace std;

struct Horodatage
{
    int mois, jour, heure;
};


struct Tache
{
    string nom, sortie;
    int duree;
    float cout;
    Horodatage debut, fin;
    liste<string> regions;
};

struct Solution
{
    string region;
    Horodatage debut, fin;
    float cout;
};

void afficher_horodatage(Horodatage temps){
    cout << temps.mois << " " << temps.jour << " " << temps.heure << endl;
}

void afficher_tache(Tache task){
    cout << task.nom << endl << task.sortie << endl << task.duree << endl;
    afficher_horodatage(task.debut);
    afficher_horodatage(task.fin);
    cout << task.cout << endl;
    for (int i = 1; i <= taille(task.regions); i++)
    {
        cout << task.regions[i] << " ";
    }
    cout << endl;
}


Tache lire_fichier_tache(string filename){
    Tache task;
    fstream flux;
    string region;
    flux.open(filename, ios::in);
    if (flux.is_open())
    {
        flux >> task.nom;
        flux >> task.sortie;
        flux >> task.duree;
        flux >> task.debut.mois;
        flux >> task.debut.jour;
        flux >> task.debut.heure;
        flux >> task.fin.mois;
        flux >> task.fin.jour;
        flux >> task.fin.heure;
        flux >> task.cout;
        while (flux.good())
        {
            flux >> region;
            inserer(region, task.regions, taille(task.regions) + 1);
        }
    }
    else
    {
        cout << "Erreur : impossible d’ouvrir "  << filename << endl;
    }
    
    
    return task;
}

int main(){
    Tache task = lire_fichier_tache("tache1.txt");
    afficher_tache(task);
    return 0;
}
