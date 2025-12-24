#pragma once
#ifndef __ENSEIGNANT_HPP__
#define __ENSEIGNANT_HPP__

#include "define.hpp"

using namespace std;

class Departement; // class forwarding 

class Enseignant
{
protected:
    int id;
    string nom;
    string prenom;
    const Departement * departement = nullptr;

public:
    Enseignant(const string& p_nom, const string& p_prenom);
    virtual ~Enseignant() = default;

    inline int getId() const { return id; }
    inline const Departement * getDepartement() const { return departement; } 
    const string& getNom() const;

    float getETDRealise() const;
    virtual float getETDMax() const = 0;
    
    void setDepartement(const Departement *p_departement);
    virtual string toString() const;
    friend ostream& operator<<(ostream& os, const Enseignant& e); 
};




#endif