#pragma once
#ifndef __SEMESTRE_HPP__
#define __SEMESTRE_HPP__

#include <string>
#include <vector>
#include "ue.hpp" 
#include "module.hpp"

using namespace std;

class Diplome;


/*
* Classe Semestre
* Représente un semestre d'un diplôme universitaire
* Contient plusieurs UEs avec le nombre d'inscrits pour chaque UE
* Les semestres sont créés et gérés par la classe Diplome
*/

class Semestre
{
    friend class Diplome;
private:
    int id;
    string nom;
    Diplome * diplome;
    vector<Module> modules; 

    // Pour que les constructeurs soient accessibles uniquement par Diplome
    explicit Semestre(Diplome * p_diplome, const string& p_nom); 
    // Pour que les constructeurs soient accessibles uniquement par Diplome
    explicit Semestre(Diplome* p_diplome);

public:
    ~Semestre() = default;
    
    // Getters
    inline const vector<Module>& getModules() const { return modules; }
    inline const string& getNom() const { return nom; }
    inline int getId() const { return id; }
    float getCoutHoraires() const;

    // Ajoute une UE avec le nombre d'inscrits
    void addUE(UE* ue, int nbInscrits);
};

#endif