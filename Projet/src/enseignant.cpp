#include <enseignant.hpp>
#include <departement.hpp>
#include <intervention.hpp>

using namespace std;

Enseignant::Enseignant(const string& p_nom, const string& p_prenom) : nom(p_nom), prenom(p_prenom)
{
    static int count_id = 0;
    id = count_id++;
}



float Enseignant::getETDRealise() const{
     float res = 0;
     for (const Intervention * i : Intervention::getInterventions(this)) {    
         res+= i->getETD();
     }
     return res;
}

const string& Enseignant::getNom() const {return nom;}

void Enseignant::setDepartement(const Departement *p_departement){departement = p_departement;}

string Enseignant::toString() const {
    return "Enseignant : " + to_string(id) +
           "\nPrenom -> " + prenom +
           "\nNom -> " + nom +
           "\nDepartement -> " + departement->getNom();
}

ostream& operator<<(ostream &strm, const Enseignant& e) 
{
    return strm << e.toString();
}