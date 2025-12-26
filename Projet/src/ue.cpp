#include "ue.hpp"
#include "enseignement.hpp"
#include "departement.hpp"
#include "enseignant.hpp"
#include <iostream>

using namespace std;

UE::UE(const string& p_nom, const Enseignant *p_responsable)
{
    static int count_id = 0;
    id = count_id++;
    nom = p_nom;
    responsable = p_responsable;   
    departement = p_responsable->getDepartement();
}

UE::UE(const string& p_nom, const Departement* p_departement, const Enseignant *p_responsable)
    :  UE(p_nom, p_responsable)
{
    departement = p_departement;
}

void UE::addEnseignement(Enseignement * p_enseignement) {
    enseignements.push_back(p_enseignement);
    p_enseignement->setUE(this); 
}


void UE::ajouterInscrits(int nombre) {
    nb_inscrits += nombre;
}

int UE::getNbTotalInscrits() const {
    return nb_inscrits;
}

float UE::getETD() const {
    float total = 0.0;
    for (const auto* e : enseignements) {
        total += e->getNbHeure() * e->getNbGroupe() * e->getCoef();
    }
    return total;
}

const string& UE::getNom() const { return nom; }
const Departement *UE::getDepartement() const { return departement; }

string UE::toString() const {
    return "UE :" + to_string(id) +
            "\nNom -> " + nom +
            "\nNombre d'inscrits -> " + to_string(nb_inscrits) +
            "\nResponsable -> " + responsable->getNom() +
            "\nDepartement gerant -> " + responsable->getDepartement()->getNom() +
            "\nETD -> " + to_string(this->getETD());
}

ostream& operator<<(ostream& os, const UE& ue) {
    return os << ue.toString();
}