#include <enseignement.hpp>

#include <ue.hpp>

using namespace std;


Enseignement::Enseignement(int p_nb_groupe, float p_nb_heure, enseignement_t p_type) {
    static int count_id = 0;
    id=count_id++;
    nb_heure = p_nb_heure;
    nb_groupe = p_nb_groupe;
    type = p_type;
    switch (type)
    {
    case COURS:
        coef = COEF_COURS;
        break;
    case TD:
        coef = COEF_TD;
        break;
    case TP:
        coef = COEF_TP;
        break;
    default:
        coef=-1.f;
        break;
    }
    
}

float Enseignement::getCoef() const {return coef;}

float Enseignement::getNbHeure() const{ return nb_heure;}

int Enseignement::getNbGroupe() const{ return nb_groupe;}

int Enseignement::getId() const{return id;}

void Enseignement::setUE(const UE *p_ue) {ue = p_ue;}

string Enseignement::toString() const {
    return "Enseignement :" + to_string(id) +
    "\nCoef -> " + to_string(coef) + 
    "\nNombre d'heure -> " + to_string(nb_heure) +
    "\nNombre de groupe -> " + to_string(nb_groupe) +
    "\nUE -> " + ue->getNom();
}


ostream& operator<<(ostream& os, const Enseignement& e) 
{
    return os << e.toString();
}
