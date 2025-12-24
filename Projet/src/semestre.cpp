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
    dansUE lien;
    lien.ue = ue;
    lien.nbInscrits = nbInscrits;
    ue_inscrite.push_back(lien);
    ue->ajouterInscrits(nbInscrits);
}

float Semestre::getCoutHoraires() const {
    float cout_total = 0.0f;
    
    for (const auto& lien : ue_inscrite) {
        float coutUE = lien.ue->getETD();
        int totalInscritsUE = lien.ue->getNbTotalInscrits();
        
        if (totalInscritsUE > 0) {
            float ratio = (float)lien.nbInscrits / (float)totalInscritsUE;
            cout_total += coutUE * ratio;
        }
    }
    return cout_total;
}