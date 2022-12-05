/*
Mini-projet fin S1L1
MERLET Raphaël
*/

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include "liste.hpp"
using namespace std;

// structures de donnees

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

struct Production
{
    string region;
    Horodatage temps;
    int thermique, nucleaire, eolien, solaire, hydro, bio, solde;
};

struct Couts
{
    float thermique, nucleaire, eolien, solaire, hydro, bio;
};

// lectures et affichages

void afficher_horodatage(Horodatage temps){
    cout << temps.mois << " " << temps.jour << " " << temps.heure;
}

void afficher_tache(Tache task){
    cout << task.nom << endl << task.sortie << endl << task.duree << endl;
    afficher_horodatage(task.debut);
    cout << endl;
    afficher_horodatage(task.fin);
    cout << endl;
    cout << task.cout << endl;
    for (int i = 1; i <= taille(task.regions); i++)
    {
        cout << task.regions[i] << " ";
    }
    cout << endl;
}

Couts lire_couts(string filename){
    Couts cost;
    fstream flux;
    flux.open(filename, ios::in);
    if (flux.is_open())
    {
        flux >> cost.thermique;
        flux >> cost.nucleaire;
        flux >> cost.eolien;
        flux >> cost.solaire;
        flux >> cost.hydro;
        flux >> cost.bio;
    }
    else
    {
        cout << "Erreur : impossible d’ouvrir "  << filename << endl;
    }

    return cost;
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

liste<Production> lire_donnees(string filename, int limit){
    Production prod;
    liste<Production> prods;
    fstream flux;
    int i = 0;
    flux.open(filename, ios::in);
    if (flux.is_open())
    {
        while (flux.good() and i < limit) {
            flux >> prod.region;
            flux >> prod.temps.mois;
            flux >> prod.temps.jour;
            flux >> prod.temps.heure;
            flux >> prod.thermique;
            flux >> prod.nucleaire;
            flux >> prod.eolien;
            flux >> prod.solaire;
            flux >> prod.hydro;
            flux >> prod.bio;
            flux >> prod.solde;
            inserer(prod, prods, taille(prods) + 1);
            i++;
        }
    }
    else
    {
        cout << "Erreur : impossible d’ouvrir "  << filename << endl;
    }

    return prods;
}

void afficher_couts(Couts c_ener){
    cout << c_ener.thermique << " " << c_ener.nucleaire << " "
         << c_ener.eolien << " " << c_ener.solaire << " "
         << c_ener.hydro << " " << c_ener.bio << endl;
}

void afficher_production(Production prod){
    cout << prod.region << " ";
    afficher_horodatage(prod.temps);
    cout << " " << prod.thermique << " " << prod.nucleaire << " " 
         << prod.eolien << " " << prod.solaire << " " 
         << prod.hydro << " " << prod.bio << " " << prod.solde; 

}

void afficher_donnees(liste<Production> l_prod){
    for (Production prod : l_prod) {
        afficher_production(prod);
        cout << endl;
    }
}

int main(){
    Tache task = lire_fichier_tache("tache1.txt");
    afficher_tache(task);
    Couts cost = lire_couts("couts.txt");
    afficher_couts(cost);
    liste<Production> prods = lire_donnees("energieFrance2021.txt", 10);
    afficher_donnees(prods);
    return 0;
}
