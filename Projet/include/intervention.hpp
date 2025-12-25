#pragma once
#ifndef __INTERVENTION_HPP__
#define __INTERVENTION_HPP__

#include <define.hpp>
class UE; // class forward
class Enseignant; // class forward

using namespace std;
/*
* Une intervention concerne 1 Enseignant et 1 UE
* Permet de garder une trace du nombre d'heure qu'a effectué le prof
*/

class Intervention
{
private:
    static vector<Intervention *> interventions; // stock toutes les interventions déjà créées
    const Enseignant * intervenant; 
    const UE * ue;
    float nb_heure_tp, nb_heure_cours, nb_heure_td;

public:
    Intervention(const Enseignant * p_intervenant, const UE * ue, float p_tp, float p_td, float p_cours);
    ~Intervention() = default;

    inline const Enseignant * getIntervenant() { return intervenant; }
    inline const UE * getUE() { return ue; }
    float getETD() const;
    // return un vector contenant les interventions faites par e
    static vector<Intervention *> getInterventions(const Enseignant * e);
    // return un vector contenant les intervention faites dans l'ue
    static vector<Intervention *> getInterventions(const UE * ue);
};


#endif