#include "diplome.hpp"
#include <iostream>

using namespace std;

Diplome::Diplome(const std::string& p_nom) : nom(p_nom) {
}

Diplome::~Diplome() {
    semestres.clear(); // détruit les semestres associés
}

void Diplome::addSemestre() {
    semestres.push_back(new Semestre(this));
}

void Diplome::addSemestre(const string& p_nom) {
    semestres.push_back(new Semestre(this, p_nom));
}

float Diplome::getCoutTotal() const {
    float total = 0.0;
    for (const auto* s : semestres) {
        total += s->getCoutHoraires();
    }
    return total;
}

std::ostream& operator<<(std::ostream& os, const Diplome& diplome) {
    os << "Diplome: " << diplome.getNom();
    return os;
}