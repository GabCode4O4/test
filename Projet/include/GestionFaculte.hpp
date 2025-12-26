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
    std::vector<Departement*> departements;
    std::vector<Enseignant*> enseignants;
    std::vector<UE*> ues;
    std::vector<Diplome*> diplomes;

    // Maps pour la reconstruction des liens lors du chargement
    std::map<int, Departement*> mapDept;
    std::map<int, Enseignant*> mapEns;
    std::map<int, UE*> mapUE;

    // Utilitaires internes
    void viderBuffer();
    void afficherTitre(const std::string& titre);

    // Template d'affichage
    template<typename T>
    void afficherListeGenerique(const std::string& titre, const std::vector<T*>& liste, std::function<bool(const T*)> filtre = nullptr) {
        afficherTitre(titre);
        if(liste.empty()) {
            std::cout << "  (Liste vide)" << std::endl;
            return;
        }

        bool elementTrouve = false;
        for(const auto* elem : liste) {
            if(!filtre || filtre(elem)) {
                std::cout << *elem << std::endl;
                std::cout << "-----------------------------------" << std::endl;
                elementTrouve = true;
            }
        }

        if(!elementTrouve && filtre) {
            std::cout << "  (Aucun élément ne correspond au filtre)" << std::endl;
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
};

#endif
