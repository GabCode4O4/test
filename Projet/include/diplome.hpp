#pragma once
#ifndef __DIPLOME_HPP__
#define __DIPLOME_HPP__

#include "define.hpp"
#include "semestre.hpp"

/*
* Classe Diplome
* Représente un diplôme universitaire
* Contient plusieurs semestres
* Les semestres sont créés et gérés par la classe Diplome
*/

using namespace std;
class Diplome
{
private:
    string nom;
    vector<Semestre*> semestres;
public:
    Diplome(const string& p_nom);
    ~Diplome();

    // Getters
    inline const string& getNom() const { return nom; }
    inline const std::vector<Semestre*>& getSemestres() const { return semestres; }
    inline int getNbSemestres() const { return semestres.size(); }
    inline Semestre *getSemestre(int numero) const { return semestres[numero-1]; }
    float getCoutTotal() const;
    
    // ajoute un semestre sans nom
    void addSemestre();
    // ajoute un semestre avec un nom
    void addSemestre(const string& p_nom);


    friend std::ostream& operator<<(std::ostream& os, const Diplome& diplome);
};



#endif