#pragma once
#ifndef __DIPLOME_HPP__
#define __DIPLOME_HPP__

#include "define.hpp"
#include "semestre.hpp"

using namespace std;
class Diplome
{
private:
    string nom;
    vector<Semestre*> semestres;
public:
    Diplome(const string& p_nom);
    ~Diplome();
    inline const string& getNom() const { return nom; }
    inline const std::vector<Semestre*>& getSemestres() const { return semestres; }
    inline int getNbSemestres() const { return semestres.size(); }
    inline Semestre *getSemestre(int numero) const { return semestres[numero-1]; }
    void addSemestre();
    void addSemestre(const string& p_nom);
    float getCoutTotal() const;

    friend std::ostream& operator<<(std::ostream& os, const Diplome& diplome);
};



#endif