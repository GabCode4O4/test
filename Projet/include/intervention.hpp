#pragma once
#ifndef __INTERVENTION_HPP__
#define __INTERVENTION_HPP__

#include <define.hpp>
class UE; // class forward
class Enseignant; // class forward

using namespace std;

/*
* Classe Intervention
* Représente une intervention d'un enseignant dans une UE
* Une intervention concerne 1 Enseignant et 1 UE
* Permet de garder une trace du nombre d'heure qu'a effectué l'enseignant pour cette UE
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
    ~Intervention();

    // Getters
    inline const Enseignant * getIntervenant() { return intervenant; }
    inline const UE * getUE() { return ue; }
    float getETD() const;
    inline float getHCours() const { return nb_heure_cours; }
    inline float getHTD() const { return nb_heure_td; }
    inline float getHTP() const { return nb_heure_tp; }

    // Méthodes statiques pour accéder aux interventions
    static vector<Intervention *> getInterventions(const Enseignant * e);
    static vector<Intervention *> getInterventions(const UE * ue);
    static const vector<Intervention*>& getAllInterventions() { return interventions; }
    
    // réinitialise la liste des interventions (utilisé lors du chargement)
    static void resetInterventions();
};


#endif