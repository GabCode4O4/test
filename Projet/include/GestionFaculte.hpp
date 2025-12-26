#ifndef GESTION_FACULTE_HPP
#define GESTION_FACULTE_HPP

#include <vector>
#include <map>
#include <string>
#include <functional>
#include <iostream>

#include "departement.hpp"
#include "ue.hpp"
#include "enseignement.hpp"
#include "enseignant_chercheur.hpp"
#include "autre_enseignant.hpp"
#include "intervention.hpp"
#include "diplome.hpp"
#include "semestre.hpp"

using namespace std;

class GestionFaculte {
private:
    vector<Departement*> departements;
    vector<Enseignant*> enseignants;
    vector<UE*> ues;
    vector<Diplome*> diplomes;

    // Maps pour la reconstruction des liens lors du chargement
    map<int, Departement*> mapDept;
    map<int, Enseignant*> mapEns;
    map<int, UE*> mapUE;

    // Utilitaires internes
    void viderBuffer();
    void afficherTitre(const string& titre);

    // Template d'affichage
    template<typename T>
    void afficherListeGenerique(const string& titre, const vector<T*>& liste, function<bool(const T*)> filtre = nullptr) {
        afficherTitre(titre);
        if(liste.empty()) {
            cout << "  (Liste vide)" << endl;
            return;
        }

        bool elementTrouve = false;
        for(const auto* elem : liste) {
            if(!filtre || filtre(elem)) {
                cout << *elem << endl;
                cout << "-----------------------------------" << endl;
                elementTrouve = true;
            }
        }

        if(!elementTrouve && filtre) {
            cout << "  (Aucun élément ne correspond au filtre)" << endl;
        }
    }

public:
    GestionFaculte();
    ~GestionFaculte();

    void sauvegarderTout();
    void chargerTout();

    void demo();
    void menuPrincipal();

    void menuRessources();
    void menuMaquette();
    void menuListes();
    void menuCalculs();

    void uiCreerDepartement();
    void uiCreerEnseignant();
    void uiCreerUEComplet();
    void uiCreerDiplome();
    void uiAjouterSemestre();
    void uiLierUEaSemestre();
    void uiAjouterIntervention();

    void uiCalculChargeEns();
    void uiCalculCoutDiplome();
    void uiCalculTauxEncadrement();
    void uiNommerResponsableDepartement();
};

#endif
