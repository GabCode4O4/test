#include "departement.hpp"
#include "enseignant.hpp" // Nécessaire pour appeler les méthodes de Enseignant
#include "ue.hpp"
#include <iostream>

using namespace std;

Departement::Departement(const string& p_nom) : nom(p_nom){
    static int count_id = 0;
    id = count_id++;
}

Departement::~Departement() {
    for(auto* e : enseignants) delete e;}

void Departement::addEnseignant(Enseignant* p_enseignant){
    if (p_enseignant->getDepartement() != nullptr) {
        cerr << "ERREUR :: L'enseignant à dejà un département " << endl;
        return ;
    }
    p_enseignant->setDepartement(this);
    enseignants.push_back(p_enseignant);
}

void Departement::addUE(UE * p_ue) {
    ue_departement.push_back(p_ue);
}

Enseignant * Departement::getEnseignant(int id){
    for (auto* e : enseignants) {
        if (e->getId() == id)
            return e;
    }
    return nullptr;
}

const vector<UE *>& Departement::getUEs() const {
    return ue_departement;
}

float Departement::getHeuresDispo() const {
    float total = 0.0;
    for (const auto* e : enseignants) {
        total += e->getETDMax();
    }
    return total;
}
float Departement::getHeuresBesoin() const {
    float total = 0.0;
    for (const auto* u : ue_departement) {
        total += u->getETD();
    }
    return total;
}

float Departement::getTauxEncadrement() const {
    float h_besoin = getHeuresBesoin();
    if (h_besoin == 0) return 0.0;
    return getHeuresDispo() / h_besoin;
}

string Departement::toString() const {
    return "Departement: " + nom + 
    "\nResponsable: " + (responsable ? responsable->getNom() : "Aucun") +
    "\nNombre enseignants: " + to_string(enseignants.size());
}

ostream& operator<<(ostream& os, const Departement& d) {
    return os << d.toString();
}