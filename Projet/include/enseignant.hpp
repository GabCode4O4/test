#pragma once
#ifndef __ENSEIGNANT_HPP__
#define __ENSEIGNANT_HPP__

#include "define.hpp"

using namespace std;

class Departement; // class forwarding 

/*
* Classe Enseignant
* Représente un enseignant universitaire
* Contient les informations de base d'un enseignant
* Chaque enseignant appartient à un département
*/

class Enseignant
{
protected:
    int id;
    string nom;
    string prenom;
    string adresse;
    const Departement * departement = nullptr;

public:
    Enseignant(const string& p_nom, const string& p_prenom, const string& p_adresse);
    virtual ~Enseignant() = default;

    // Getters
    inline int getId() const { return id; }
    inline const Departement * getDepartement() const { return departement; } 
    const string& getNom() const;
    const string& getPrenom() const;
    const string& getAdresse() const;
    float getETDRealise() const;
    virtual float getETDMax() const = 0;
    
    // Setter
    void setDepartement(const Departement *p_departement);

    virtual string toString() const;
    friend ostream& operator<<(ostream& os, const Enseignant& e); 
};




#endif