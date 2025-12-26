#pragma once
#ifndef __SEMESTRE_HPP__
#define __SEMESTRE_HPP__

#include <string>
#include <vector>
#include "ue.hpp" 

using namespace std;

class Diplome;

struct dansUE {
    UE* ue;
    int nbInscrits;
};

class Semestre
{
    friend class Diplome;
private:
    int id;
    string nom;
    Diplome * diplome;
    vector<dansUE> ue_inscrite; 

    // Pour que les constructeurs soient accessibles uniquement par Diplome
    explicit Semestre(Diplome * p_diplome, const string& p_nom); 
    // Pour que les constructeurs soient accessibles uniquement par Diplome
    explicit Semestre(Diplome* p_diplome);

public:
    ~Semestre() = default;

    void addUE(UE* ue, int nbInscrits);
    inline const vector<dansUE>& getUEs() const { return ue_inscrite; }
    inline const string& getNom() const { return nom; }
    inline int getId() const { return id; }

    float getCoutHoraires() const;
};

#endif