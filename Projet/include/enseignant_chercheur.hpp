#pragma once
#ifndef __ENSEIGNANT_CHERCHEUR_HPP__
#define __ENSEIGNANT_CHERCHEUR_HPP__

#include "enseignant.hpp"
#include "define.hpp"

class EnseignantChercheur : public Enseignant
{
public:
    EnseignantChercheur(const string& p_nom, const string& p_prenom, const string& p_adresse): Enseignant(p_nom, p_prenom, p_adresse) {}
    ~EnseignantChercheur() = default;
    float getETDMax() const override {return MAX_ETD_ENSEIGNANT_CHERCHEUR;}
    string toString() const override {return "Chercheur: " + Enseignant::toString();}
};

#endif