#pragma once
#ifndef __UE_HPP__
#define __UE_HPP__

#include <define.hpp>

#include <enseignant.hpp>
#include <departement.hpp>
#include <enseignement.hpp>


using namespace std;

/*
* Classe UE
* Représente une Unité d'Enseignement (UE) universitaire
* Contient des enseignements et un responsable
* Appartient à un département

*/

class UE
{
private:
    int id;
    string nom;
    const Enseignant *responsable;
    vector<Enseignement *> enseignements;
    const Departement * departement;
    int nb_inscrits = 0;
public:

    UE() = default;
    UE(const string& p_nom, const Enseignant *p_responsable );
    UE(const string& p_nom, const Departement * p_departement, const Enseignant *p_responsable);
    ~UE() = default;

    // Getters
    const string& getNom() const;
    float getETD() const;
    int getNbTotalInscrits() const;
    const Departement *getDepartement() const ;
    inline const vector<Enseignement*>& getEnseignements() const { return enseignements; }
    inline const Enseignant* getResponsable() const { return responsable; }
    inline int getId() const { return id; }
    
    // Setters
    inline void setDepartement(const Departement * p_departement) { departement = p_departement; }
    
    // Ajoute un enseignement à l'UE
    void addEnseignement(Enseignement * p_enseignement);
    // Ajoute des inscrits à l'UE
    void ajouterInscrits(int nombre); 

    string toString() const;
    friend ostream& operator<<(ostream& os, const UE& ue); 
};

#endif