#include "semestre.hpp"
#include <iostream>

using namespace std;

Semestre::Semestre(Diplome* p_diplome, const string& p_nom) : diplome(p_diplome), nom(p_nom)
{
    static int count_id = 0;
    id = count_id++;
}

Semestre::Semestre(Diplome * p_diplome) : diplome(p_diplome)
{
    static int count_id = 0;
    id = count_id++;
    nom = "Semestre_" + to_string(id);
}

void Semestre::addUE(UE* ue, int nbInscrits) {
    Module module(ue, nbInscrits);
    modules.push_back(module);
    ue->ajouterInscrits(nbInscrits);
}

float Semestre::getCoutHoraires() const {
    float cout_total = 0.0f;

    for (const auto& m : modules) {
        float coutUE = m.getUE()->getETD();
        int totalInscritsUE = m.getUE()->getNbTotalInscrits();

        if (totalInscritsUE > 0) {
            float ratio = (float)m.getNbInscrits() / (float)totalInscritsUE;
            cout_total += coutUE * ratio;
        }
    }
    return cout_total;
}