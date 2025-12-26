#pragma once
#ifndef __DEPARTEMENT_HPP__
#define __DEPARTEMENT_HPP__

#include "define.hpp"

/*
* Classe Departement
* Représente un département universitaire
* Contient des enseignants et gère unités d'enseignement (UE)
*/

// Forward declaration pour éviter include circulaire avec ue.hpp
class UE; 
class Enseignant;

using namespace std;

class Departement
{
private:
    int id;
    string nom;
    Enseignant * responsable = nullptr;
    vector<Enseignant*> enseignants;
    vector<UE*> ue_departement;

public:
    Departement(const string& p_nom);
    ~Departement();

    // Getters
    inline int getId() const { return id; }
    inline const string& getNom() const { return nom; }
    inline const vector<Enseignant*>& getEnseignants() const { return enseignants; }
    inline const Enseignant* getResponsable() const { return responsable; }
    float getHeuresDispo() const;
    float getHeuresBesoin() const;
    float getTauxEncadrement() const;
    Enseignant * getEnseignant(int id);
    const vector<UE *>& getUEs() const;
    
    // Setters
    inline void setResponsable(Enseignant* p_responsable) { responsable = p_responsable; }

    // ajoutes un enseignant ou une UE au département
    void addEnseignant(Enseignant* p_enseignant);
    void addUE(UE * p_ue);
    
    string toString() const;
    friend ostream& operator<<(ostream& os, const Departement& d);
};

#endif