#pragma once
#ifndef __AUTRE_ENSEIGNANT_HPP__
#define __AUTRE_ENSEIGNANT_HPP__

#include "enseignant.hpp"
#include "define.hpp"

class AutreEnseignant : public Enseignant
{
public:
    AutreEnseignant(const string& p_nom, const string& p_prenom, const string& p_adresse): Enseignant(p_nom, p_prenom, p_adresse) {}
    ~AutreEnseignant() = default;
    float getETDMax() const override {return MAX_ETD_AUTRE_ENSEIGNANT;}
    string toString() const override {return "Autre (PRAG/PRCE): " + Enseignant::toString();}
};

#endif