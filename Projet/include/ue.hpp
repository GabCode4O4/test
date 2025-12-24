#pragma once
#ifndef __UE_HPP__
#define __UE_HPP__

#include <define.hpp>

#include <enseignant.hpp>
#include <departement.hpp>
#include <enseignement.hpp>


using namespace std;

/*
* Possède un nom, un enseignant, des enseignements, un 
* l'enseignant est le responsable de l'ue
* les enseignement peuvent etre de 3 type et constituent les TP/TD/COURS qui composent l'ue
*/

class UE
{
private:
    int id;
    std::string nom;
    const Enseignant *responsable;
    std::vector<Enseignement *> enseignements;
    const Departement * departement;
    int nb_inscrits = 0;
public:
    UE() = default;
    // le département est automatiquement celui de l'enseignant responsable
    UE(const std::string& p_nom, const Enseignant *p_responsable );
    UE(const std::string& p_nom, const Departement * p_departement, const Enseignant *p_responsable);
    ~UE() = default;

    // rajoute un enseignement à l'ue
    void addEnseignement(Enseignement * p_enseignement);
    float getETD() const;

    // return le département qui gère l'ue
    const Departement *getDepartement() const ;
    const std::string& getNom() const;
    inline int getId() const { return id; }


    inline void setDepartement(const Departement * p_departement) { departement = p_departement; }
    void ajouterInscrits(int nombre); 
    int getNbTotalInscrits() const;

    string toString() const;
    friend ostream& operator<<(ostream& os, const UE& ue); 
};

#endif