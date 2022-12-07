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

// pour les temps au format MM/JJ/HH
struct Horodatage 
{
    int mois, jour, heure;
};

// pour stockage du fichier de tâche en entrée
struct Tache 
{
    string nom, sortie;
    int duree;
    float cout;
    Horodatage debut, fin;
    liste<string> regions;
};

// ligne de solution possible
struct Solution 
{
    string region;
    Horodatage debut, fin;
    float cout;
};

// ligne qui représente la production éléctrique durant une heure, dans une region donnée, dans le fichier de données en entrée
struct Production 
{
    string region;
    Horodatage temps;
    int thermique, nucleaire, eolien, solaire, hydro, bio, solde;
};

// couts en kgCO2/KWh de chaque type de production d'énergie
struct Couts
{
    float thermique, nucleaire, eolien, solaire, hydro, bio;
};

// lectures et affichages

//-------------------------.
// Rôle : affichage de la structure de donnée Horodatage (sur une ligne, sans passage de ligne).
// Précondition : temps avec attributs complétés
void afficher_horodatage(Horodatage temps){
    cout << temps.mois << " " << temps.jour << " " << temps.heure;
}

//-------------------------.
// Rôle : affichage de la structure de donnée Tache (en suivant le format du fichier d'exemple tache1.txt).
// Précondition : task avec attributs complétés
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

//-------------------------.
// Rôle : affichage de la structure de donnée Couts (sur une ligne, avec passage de ligne).
// Précondition : c_ener avec attributs complétés
void afficher_couts(Couts c_ener){
    cout << c_ener.thermique << " " << c_ener.nucleaire << " "
         << c_ener.eolien << " " << c_ener.solaire << " "
         << c_ener.hydro << " " << c_ener.bio << endl;
}

//-------------------------.
// Rôle : affichage de la structure de donnée Horodatage (sur une ligne, sans passage de ligne).
// Précondition : prod avec attributs complétés
void afficher_production(Production prod){
    cout << prod.region << " ";
    afficher_horodatage(prod.temps);
    cout << " " << prod.thermique << " " << prod.nucleaire << " " 
         << prod.eolien << " " << prod.solaire << " " 
         << prod.hydro << " " << prod.bio << " " << prod.solde; 

}

//-------------------------.
// Rôle : affichage de la liste des structures de données Productions (telles que contenues dans energieFrance 2021.txt).
// Précondition : l_prod avec attributs complétés
void afficher_donnees(liste<Production> l_prod){
    for (Production prod : l_prod) {
        afficher_production(prod);
        cout << endl;
    }
}

//-------------------------.
// Rôle : retourne le résultat de la lecture de la liste des structures de données Productions suivant le format du fichier d'exemple energieFrance 2021.txt.
// Précondition : filename est un emplacement de fichier valide.
// Info limit : mettre limit à -1 pour lire jusqu'au bout
liste<Production> lire_donnees(string filename, int limit){
    Production prod;
    liste<Production> prods;
    fstream flux;
    int i = 0;
    flux.open(filename, ios::in);
    if (flux.is_open())
    {
        while (flux.good() && (i < limit || limit == -1)) {
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

//-------------------------.
// Rôle : retourne le résultat de la lecture de la structure de donnée Tache en suivant le format du fichier d'exemple tache1.txt.
// Précondition : filename est un emplacement de fichier valide
Tache lire_tache(string filename){
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

//-------------------------.
// Rôle : retourne le résultat de la lecture de la structure de donnée Tache suivant le format du fichier couts.txt.
// Précondition : filename est un emplacement de fichier valide
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

//-------------------------.
// Rôle : enregistre la liste l_res des structures de données Solution à l'emplacement <filename>
// Précondition : filename est un emplacement de fichier valide
void exporter_resultats(string filename, liste<Solution> l_res){
    fstream flux;
    int i = 1;
    flux.open(filename, ios::out);
    if (flux.is_open())
    {
        while (flux.good() && i <= taille(l_res)) {
            flux << l_res[i].region << " ";
            flux << l_res[i].debut.mois << " ";
            flux << l_res[i].debut.jour << " ";
            flux << l_res[i].debut.heure << " ";
            flux << l_res[i].fin.mois << " ";
            flux << l_res[i].fin.jour << " ";
            flux << l_res[i].fin.heure << " ";
            flux << l_res[i].cout << endl;
            i++;
        }
    }
    else
    {
        cout << "Erreur : impossible d’enregistrer resultats a l'emplacement "  << filename << endl;
    }
}

//-------------------------.
// Rôle : revoie vrai si temps est situé avant ou au même moment que borne, faux sinon
// Précondition : filename est un emplacement de fichier valide
bool temps_inferieur(Horodatage temps, Horodatage borne){
    if (temps.mois < borne.mois) {
        return true;
    }
    else {
        if (temps.mois == borne.mois) {
            if (temps.jour < borne.jour) {
                return true;
            }
            else {
                if (temps.jour == borne.jour) {
                    if (temps.heure <= borne.heure) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool periode_valide(Horodatage temps, Horodatage min, Horodatage max){
    if (temps_inferieur(min, temps)) {
        if (temps_inferieur(temps, max)) {
            return true;
        }
    }
    return false;
}

float cout_moyen(Couts cost, Production prod){
    float cout_fois_prod = prod.thermique * cost.thermique + 
        prod.nucleaire * cost.nucleaire + 
        prod.eolien * cost.eolien +
        prod.solaire * cost.solaire + 
        prod.hydro * cost.hydro + 
        prod.bio * cost.bio; 
    float somme_prods = prod.thermique + prod.nucleaire + prod.eolien +
        prod.solaire + prod.hydro + prod.bio;
    
    return cout_fois_prod / somme_prods;
}

liste<Production> filtre_regions(string region, liste<Production> prods){
    liste<Production> prod_filtree;
    for (Production prod : prods) {
        if (prod.region == region) {
            inserer(prod, prod_filtree, taille(prod_filtree) + 1);
        }
    }

    return prod_filtree;
};

liste<Solution> trouver_solutions(Tache task, Couts cost, liste<Production> prods){
    bool valide;
    liste<Solution> solutions;
    int j;
    for (string region: task.regions){
        liste<Production> prods_region = filtre_regions(region, prods);
        for (int i = 1; i <= (int) taille(prods_region) - (task.duree - 1); i++) {
            float cout_moy;
            float somme_couts = 0;
            valide = true;
            j = 0;
            while (j < task.duree - 1 && valide) {
                cout_moy = cout_moyen(cost, prods_region[i+j]);
                if (!periode_valide(prods_region[i+j].temps, task.debut, task.fin)) { // heure dans la période
                    valide = false;
                }
                if (prods_region[i+j].solde > 0) { // région productrice
                    valide = false;
                }
                if (cout_moy > task.cout) { // cout moyen inférieur à cout max
                    valide = false;
                }
                somme_couts += cout_moy;
                j++;
            }
            if (valide) {
                Solution planification;
                planification.region = region;
                planification.debut = prods_region[i].temps;
                planification.fin = prods_region[i+j].temps;
                planification.cout = somme_couts;
                inserer(planification, solutions, taille(solutions) + 1);
            }
        }
    }
    
    return solutions;
}

int main(){
    Tache task = lire_tache("tache1.txt");
    afficher_tache(task);
    Couts cost = lire_couts("couts.txt");
    afficher_couts(cost);
    liste<Production> prods = lire_donnees("energieFrance2021.txt", -1);
    afficher_donnees(prods);
    liste<Solution> resultats = trouver_solutions(task, cost, prods);
    exporter_resultats(task.sortie, resultats);
    return 0;
}
