/*
Mini-projet fin S1L1, Base en info
MERLET Raphaël, Bilal RAHALI

Permet la planification d'une tâche décrite dans un fichier, avec un fichier de coûts et de productions fournis
Les solutions trouvées sont enregistrées dans un fichier de sortie (au nom indiqué dans le fichier de tâche)

Lien github : https://github.com/Ilade-s/Mini_projet_L1 
    (branche master par défaut, contient aussi la branche test_lecture_rapide)
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

// couts en g eqCO2/KWh de chaque type de production d'énergie
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
// Rôle : affichage de la structure de donnée Production (sur une ligne, sans passage de ligne).
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
// Rôle : enregistre la liste l_res des structures de données Solution à l'emplacement <filename>.
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
// Rôle : revoie vrai si temps est situé avant ou au même moment que borne, faux sinon.
// Précondition : les structures sont complètes
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

//-------------------------.
// Rôle : revoie vrai si temps est situé entre min et max (inclus).
// Précondition : les structures sont complètes
bool periode_valide(Horodatage temps, Horodatage min, Horodatage max){
    if (temps_inferieur(min, temps)) {
        if (temps_inferieur(temps, max)) {
            return true;
        }
    }
    return false;
}

//-------------------------.
// Rôle : revoie le cout moyen de la production de cette heure (moyenne pondérée des coûts par la production).
// Précondition : les structures sont complètes
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

//-------------------------.
// Rôle : revoie la liste prods ne contenant que les lignes indiquant la production de la region donnée.
// Précondition : les structures sont complètes, region est un numéro de région valide (sinon la liste retournée sera vide)
liste<Production> filtre_regions(string region, liste<Production> prods){
    liste<Production> prod_filtree;
    for (Production prod : prods) {
        if (prod.region == region) {
            inserer(prod, prod_filtree, taille(prod_filtree) + 1);
        }
    }

    return prod_filtree;
};

//-------------------------.
// Rôle : sépare l_f en deux listes l1 et l2 de même taille à l'unité près (récursif).
// Précondition : les structures de la liste l_s sont bien formées et l1 et l2 sont vides
void separation_l_s(liste<Solution> l_s, liste<Solution> & l1, liste<Solution> & l2){
    if (taille(l_s) > 0) {
        if (taille(l1) < taille(l2)) {
            inserer(tete(l_s), l1, taille(l1) + 1);
        }
        else {
            inserer(tete(l_s), l2, taille(l2) + 1);
        }
        separation_l_s(reste(l_s), l1, l2);
    }
}

//-------------------------.
// Rôle : revoie vrai si l2 est vide ou que la 1er elt de l1 est inférieur au 1er elt de l2, faux sinon.
// Précondition : les structures des listes sont bien formées
bool tete_inferieure(liste<Solution> l1, liste<Solution> l2){
    if (taille(l1) == 0) {
        return false;
    }
    if (taille(l2) == 0) {
        return true;
    }
    if (l1[1].cout < l2[1].cout) {
        return true;
    }
    return false;
}

//-------------------------.
// Rôle : fusionne les deux listes triées dans l'ordre croissant l1 et l2 dans l_f (récursif).
// Précondition : les structures des listes sont bien formées et l_f est vide
void fusion_liste_triees(liste<Solution> l1, liste<Solution> l2, liste<Solution> & l_f){
    if (tete_inferieure(l1, l2)) {
        inserer(tete(l1), l_f, taille(l_f) + 1);
        fusion_liste_triees(reste(l1), l2, l_f);
    }
    if (tete_inferieure(l2, l1)) {
        inserer(tete(l2), l_f, taille(l_f) + 1);
        fusion_liste_triees(l1, reste(l2), l_f);
    }
}

//-------------------------.
// Rôle : revoie la liste l_s triée en ordre croissant du coût des solutions (récursif : tri fusion).
// Précondition : les structures de la liste sont bien formées
liste<Solution> trier_solutions(liste<Solution> l_s){
    if (taille(l_s) <= 1) {
        return l_s;
    }

    liste<Solution> l1, l2, l_triee;

    separation_l_s(l_s, l1, l2);

    l1 = trier_solutions(l1);
    l2 = trier_solutions(l2);

    fusion_liste_triees(l1, l2, l_triee);
    return l_triee;
    
}

//-------------------------.
// Rôle : revoie vrai si l'heure de production donnée respecte les contraintes afin de créer une solution, faux sinon (incrémente la somme des couts).
// Précondition : les structures sont complètes, index inclus dans [1; taille(l_region)]
bool est_valide(Production prod, Tache task, Couts cost, float & somme_couts){
    float cout_moy = cout_moyen(cost, prod); // coût moyen de la production de l'heure
    somme_couts += cout_moy; // ajout de l'heure de prod au coût total
    if (!periode_valide(prod.temps, task.debut, task.fin)) { // vérif heure dans la période
        return false;
    }
    if (prod.solde > 0) { // vérif région productrice
        return false;
    }
    if (cout_moy > task.cout) { // vérif cout moyen inférieur à cout max
        return false;
    }
    return true;
}

//-------------------------.
// Rôle : crée la structure de donnée Solution et l'ajoute à la fin de la liste l_s.
// Précondition : les structures sont complètes, index inclus dans [1; taille(l_region) - duree + 1]
void ajouter_solution(liste<Solution> & l_s, liste<Production> l_region, string region, int index, int duree, float somme_couts){
    Solution planification;
    planification.region = region;
    planification.debut = l_region[index].temps; // première heure de production
    planification.fin = l_region[index+duree-1].temps; // dernière heure de production
    planification.cout = somme_couts;
    inserer(planification, l_s, taille(l_s) + 1);
}

//-------------------------.
// Rôle : revoie la liste des planifications possibles de la Tache task, selon les coûts et la liste des productions horiaires.
// Précondition : les structures sont complètes
liste<Solution> trouver_solutions(Tache task, Couts cost, liste<Production> prods){
    bool valide;
    liste<Solution> solutions;
    int j;
    for (string region: task.regions){ // solutions par régions
        liste<Production> prods_region = filtre_regions(region, prods); // liste des productions de cette region
        for (int i = 1; i <= (int) taille(prods_region) - task.duree; i++) { // parcours index 1ere heure tâche (selon durée)
            float somme_couts = 0; // coût total de la tâche (somme de couts moyen/h)
            valide = true;
            j = 0;
            while (j < task.duree && valide) { // parcours des indexs des heures de chaque tâche (ou jusqu'à ce que la solution soit invalide)
                valide = est_valide(prods_region[i+j], task, cost, somme_couts);
                j++;
            }
            if (valide) { // ajout éventuel de la solution dans la liste
                ajouter_solution(solutions, prods_region, region, i, task.duree, somme_couts);
            }
        }
    }
    
    return trier_solutions(solutions);
}

int main(){
    Tache task = lire_tache("tache1.txt");
    afficher_tache(task);
    Couts cost = lire_couts("couts.txt");
    afficher_couts(cost);
    liste<Production> prods = lire_donnees("energieFrance2021.txt", -1);
    //afficher_donnees(prods);
    liste<Solution> resultats = trouver_solutions(task, cost, prods);
    exporter_resultats(task.sortie, resultats);
    return 0;
}
