#pragma once
#ifndef __ENSEIGNEMENT_HPP__
#define __ENSEIGNEMENT_HPP__

#include "define.hpp"

using namespace std;



class UE; // class forward

class Enseignement
{
private:
    float coef;
    int id;
    int nb_groupe;
    enseignement_t type;
    float nb_heure;
    const UE *ue;
public:
    Enseignement() = default;
    Enseignement(int p_nb_groupe, float p_nb_heure, enseignement_t p_type);

    ~Enseignement() = default;
    float getCoef() const;
    float getNbHeure() const;
    int getNbGroupe() const;
    int getId() const;
    inline enseignement_t getType() const { return type; }
    void setUE(const UE *p_ue);

    string toString() const;
    friend ostream& operator<<(ostream& os, const Enseignement& e); 

};


#endif