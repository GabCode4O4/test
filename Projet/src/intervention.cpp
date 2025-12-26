#include <intervention.hpp>
#include <algorithm>

#include <enseignant.hpp>
#include <ue.hpp>

using namespace std;

vector<Intervention *> Intervention::interventions = vector<Intervention *>();

Intervention::Intervention(const Enseignant * p_intervenant, const UE * ue, float p_tp, float p_td, float p_cours) {
    intervenant = p_intervenant;
    this->ue = ue;
    nb_heure_tp = p_tp;
    nb_heure_td = p_td;
    nb_heure_cours = p_cours;
    interventions.push_back(this);
}
Intervention::~Intervention() {
    // On cherche l'objet courant (this) dans le vecteur statique
    auto it = find(interventions.begin(), interventions.end(), this);
    
    // Si on le trouve, on le retire de la liste pour ne pas garder de pointeur invalide
    if (it != interventions.end()) {
        interventions.erase(it);
    }
}

void Intervention::resetInterventions() {
    while (!interventions.empty()) {
        delete interventions.back();
    }
}

float Intervention::getETD() const {
    return nb_heure_cours * COEF_COURS + nb_heure_td * COEF_TD + nb_heure_tp * COEF_TP; // A MODIFIER
}

vector<Intervention *> Intervention::getInterventions(const Enseignant * e) {
    int id_e = e->getId();
    vector<Intervention *> result;
    for (Intervention * i: interventions) {
        if (i->getIntervenant()->getId() == id_e)
            result.push_back(i);
    }

    return result;
}

vector<Intervention *> Intervention::getInterventions(const UE * ue) {

    int id_ue = ue->getId();
    vector<Intervention *> result;
    for (Intervention * i: interventions) {
        if (i->getUE()->getId() == id_ue)
            result.push_back(i);
    }

    return result;
}