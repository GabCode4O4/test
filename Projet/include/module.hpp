#pragma once
#ifndef __MODULE_HPP__
#define __MODULE_HPP__

#include "define.hpp"

class UE;

/*
* Classe Module
* Représente une UE inscrite dans un semestre avec le nombre d'inscrits
*/

class Module
{
private:
    UE * ue = nullptr;
    int nbInscrits = 0;
public:
    Module(UE *p_ue, int p_nbInscrits );
    ~Module() = default;
    
    inline UE* getUE() const { return ue; }
    inline int getNbInscrits() const { return nbInscrits; }
};

#endif