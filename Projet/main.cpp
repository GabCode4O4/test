#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <functional> // Pour std::function

// Includes du projet
#include "departement.hpp"
#include "ue.hpp"
#include "enseignement.hpp"
#include "enseignant_chercheur.hpp"
#include "autre_enseignant.hpp"
#include "intervention.hpp"
#include "diplome.hpp"
#include "semestre.hpp"

using namespace std;

// --- UTILITAIRES ---
void viderBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void afficherTitre(const string& titre) {
    cout << "\n==========================================" << endl;
    cout << "   " << titre << endl;
    cout << "==========================================" << endl;
}

// --- FONCTION GÉNÉRIQUE (TEMPLATE) D'AFFICHAGE ---
// Affiche n'importe quel vecteur de pointeurs T*
// Accepte un filtre optionnel (lambda)
template<typename T>
void afficherListeGenerique(const string& titre, const vector<T*>& liste, function<bool(const T*)> filtre = nullptr) {
    afficherTitre(titre);
    if(liste.empty()) {
        cout << "  (Liste vide)" << endl;
        return;
    }

    bool elementTrouve = false;
    for(const auto* elem : liste) {
        // Si filtre est null OU si le filtre renvoie true
        if(!filtre || filtre(elem)) {
            cout << *elem << endl; // Utilise operator<< de l'objet
            cout << "-----------------------------------" << endl;
            elementTrouve = true;
        }
    }

    if(!elementTrouve && filtre) {
        cout << "  (Aucun élément ne correspond au filtre)" << endl;
    }
}

// --- CLASSE PRINCIPALE ---
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

public:
    GestionFaculte() {}
    
    ~GestionFaculte() {
        // Note: Dans un vrai projet, gérer la suppression mémoire ici
    }

    // =========================================================
    //              1. GESTION DE LA PERSISTANCE (ROBUSTE)
    // =========================================================
    
    void sauvegarderTout() {
        afficherTitre("SAUVEGARDE EN COURS");
        
        // 1. Départements
        ofstream fDept("departements.txt");
        if(fDept.is_open()) {
            for(auto* d : departements) fDept << d->getId() << ";" << d->getNom() << endl;
        }

        // 2. Enseignants
        ofstream fEns("enseignants.txt");
        if(fEns.is_open()) {
            for(auto* e : enseignants) {
                int type = (dynamic_cast<EnseignantChercheur*>(e)) ? 1 : 2;
                int deptId = (e->getDepartement()) ? e->getDepartement()->getId() : -1;
                fEns << e->getId() << ";" << type << ";" << e->getNom() << ";" << e->getPrenom() << ";" << e->getAdresse() << ";" << deptId << endl;
            }
        }

        // 3. UEs
        ofstream fUE("ues.txt");
        if(fUE.is_open()) {
            for(auto* u : ues) {
                int deptId = (u->getDepartement()) ? u->getDepartement()->getId() : -1;
                fUE << u->getId() << ";" << u->getNom() << ";" << deptId << endl;
            }
        }

        // 4. Diplômes
        ofstream fDip("diplomes.txt");
        if(fDip.is_open()) {
            for(auto* d : diplomes) {
                fDip << d->getNom() << endl;
            }
        }

        cout << "✓ Données sauvegardées." << endl;
    }

    void chargerTout() {
        // Réinitialisation
        departements.clear(); enseignants.clear(); ues.clear(); diplomes.clear();
        mapDept.clear(); mapEns.clear(); mapUE.clear();

        afficherTitre("CHARGEMENT DES DONNÉES");
        string ligne;

        // 1. Dept
        ifstream fDept("departements.txt");
        if(fDept.is_open()) {
            while(getline(fDept, ligne)) {
                if(ligne.empty() || ligne == "\r") continue; // Fix bug ligne vide
                try {
                    stringstream ss(ligne);
                    string sId, sNom;
                    getline(ss, sId, ';'); getline(ss, sNom, ';');
                    Departement* d = new Departement(sNom);
                    departements.push_back(d);
                    mapDept[stoi(sId)] = d;
                } catch(...) {}
            }
        }

        // 2. Enseignants
        ifstream fEns("enseignants.txt");
        if(fEns.is_open()) {
            while(getline(fEns, ligne)) {
                if(ligne.empty() || ligne == "\r") continue;
                try {
                    stringstream ss(ligne);
                    string sId, sType, sNom, sPrenom, sAdresse, sDeptId;
                    getline(ss, sId, ';'); getline(ss, sType, ';'); getline(ss, sNom, ';'); getline(ss, sPrenom, ';'); getline(ss, sAdresse, ';'); getline(ss, sDeptId, ';');
                    
                    Enseignant* e = (stoi(sType) == 1) ? (Enseignant*)new EnseignantChercheur(sNom, sPrenom, sAdresse) : (Enseignant*)new AutreEnseignant(sNom, sPrenom, sAdresse);
                    enseignants.push_back(e);
                    mapEns[stoi(sId)] = e;
                    
                    int dId = stoi(sDeptId);
                    if(mapDept.count(dId)) mapDept[dId]->addEnseignant(e);
                } catch(...) {}
            }
        }

        // 3. UEs
        ifstream fUE("ues.txt");
        if(fUE.is_open()) {
            while(getline(fUE, ligne)) {
                if(ligne.empty() || ligne == "\r") continue;
                try {
                    stringstream ss(ligne);
                    string sId, sNom, sDeptId;
                    getline(ss, sId, ';'); getline(ss, sNom, ';'); getline(ss, sDeptId, ';');
                    
                    // Responsable par défaut si non trouvé (simplification pour l'exercice)
                    Enseignant* resp = (!enseignants.empty()) ? enseignants[0] : nullptr;
                    if(resp) {
                        UE* ue = new UE(sNom, resp);
                        ues.push_back(ue);
                        mapUE[stoi(sId)] = ue;
                    }
                } catch(...) {}
            }
        }
        
        // 4. Diplômes
        ifstream fDip("diplomes.txt");
        if(fDip.is_open()) {
            while(getline(fDip, ligne)) {
                if(!ligne.empty() && ligne != "\r") diplomes.push_back(new Diplome(ligne));
            }
        }

        cout << "✓ Données chargées avec succès." << endl;
    }


    // =========================================================
    //              2. MENUS INTERACTIFS
    // =========================================================

    void demo() {
        afficherTitre("INITIALISATION DE LA DÉMONSTRATION");

        // 1. Création des Départements
        Departement* deptInfo = new Departement("Informatique");
        Departement* deptMath = new Departement("Mathématiques");
        Departement* deptPhys = new Departement("Physique");
        departements.push_back(deptInfo);
        departements.push_back(deptMath);
        departements.push_back(deptPhys);
        cout << "-> 3 Départements créés." << endl;

        // 2. Création des Enseignants
        // Informatique
        Enseignant* profTuring = new EnseignantChercheur("Turing", "Alan", "8 King's Parade, Cambridge");
        Enseignant* profLovelace = new EnseignantChercheur("Lovelace", "Ada", "16 St. James Square, London");
        Enseignant* profHopper = new AutreEnseignant("Hopper", "Grace", "55 Broadway, New York");
        deptInfo->addEnseignant(profTuring);
        deptInfo->addEnseignant(profLovelace);
        deptInfo->addEnseignant(profHopper);
        enseignants.push_back(profTuring);
        enseignants.push_back(profLovelace);
        enseignants.push_back(profHopper);

        // Mathématiques
        Enseignant* profEuler = new EnseignantChercheur("Euler", "Leonhard", "Bern, Switzerland");
        Enseignant* profGauss = new AutreEnseignant("Gauss", "Carl", " Braunschweig, Germany");
        deptMath->addEnseignant(profEuler);
        deptMath->addEnseignant(profGauss);
        enseignants.push_back(profEuler);
        enseignants.push_back(profGauss);

        // Physique
        Enseignant* profEinstein = new EnseignantChercheur("Einstein", "Albert", "Ulm, Germany");
        Enseignant* profCurie = new EnseignantChercheur("Curie", "Marie", "Paris, France");
        deptPhys->addEnseignant(profEinstein);
        deptPhys->addEnseignant(profCurie);
        enseignants.push_back(profEinstein);
        enseignants.push_back(profCurie);
        cout << "-> 7 Enseignants créés et affectés." << endl;

        // 3. Création des UEs
        // Info
        UE* ueAlgo = new UE("Algorithmique", profTuring);
        ueAlgo->setDepartement(deptInfo);
        deptInfo->addUE(ueAlgo);
        // 1 groupe de CM, 4 groupes de TD, 6 groupes de TP
        ueAlgo->addEnseignement(new Enseignement(1, 20, COURS));
        ueAlgo->addEnseignement(new Enseignement(4, 30, TD));
        ueAlgo->addEnseignement(new Enseignement(6, 15, TP));
        ues.push_back(ueAlgo);

        UE* ueBDD = new UE("Bases de Données", profLovelace);
        ueBDD->setDepartement(deptInfo);
        deptInfo->addUE(ueBDD);
        // 1 CM, 6 TP
        ueBDD->addEnseignement(new Enseignement(1, 15, COURS));
        ueBDD->addEnseignement(new Enseignement(6, 30, TP));
        ues.push_back(ueBDD);

        UE* ueWeb = new UE("Développement Web", profHopper);
        ueWeb->setDepartement(deptInfo);
        deptInfo->addUE(ueWeb);
        // 1 CM, 4 TD, 6 TP
        ueWeb->addEnseignement(new Enseignement(1, 10, COURS));
        ueWeb->addEnseignement(new Enseignement(4, 20, TD));
        ueWeb->addEnseignement(new Enseignement(6, 30, TP));
        ues.push_back(ueWeb);

        // Math
        UE* ueAnalyse = new UE("Analyse", profEuler);
        ueAnalyse->setDepartement(deptMath);
        deptMath->addUE(ueAnalyse);
        // 1 CM, 5 TD
        ueAnalyse->addEnseignement(new Enseignement(1, 30, COURS));
        ueAnalyse->addEnseignement(new Enseignement(5, 40, TD));
        ues.push_back(ueAnalyse);

        UE* ueAlgebra = new UE("Algèbre", profGauss);
        ueAlgebra->setDepartement(deptMath);
        deptMath->addUE(ueAlgebra);
        // 1 CM, 5 TD
        ueAlgebra->addEnseignement(new Enseignement(1, 25, COURS));
        ueAlgebra->addEnseignement(new Enseignement(5, 35, TD));
        ues.push_back(ueAlgebra);

        // Physique
        UE* ueMeca = new UE("Mécanique", profEinstein);
        ueMeca->setDepartement(deptPhys);
        deptPhys->addUE(ueMeca);
        // 1 CM, 4 TD, 4 TP
        ueMeca->addEnseignement(new Enseignement(1, 20, COURS));
        ueMeca->addEnseignement(new Enseignement(4, 20, TD));
        ueMeca->addEnseignement(new Enseignement(4, 10, TP));
        ues.push_back(ueMeca);

        cout << "-> 6 UEs créées avec leurs enseignements." << endl;

        // 4. Création des Diplômes et Semestres
        Diplome* licenceInfo = new Diplome("Licence Informatique");
        licenceInfo->addSemestre("S1");
        licenceInfo->addSemestre("S2");
        licenceInfo->addSemestre("S3");
        licenceInfo->addSemestre("S4");
        licenceInfo->addSemestre("S5");
        licenceInfo->addSemestre("S6");
        
        // Lier UEs aux semestres
        licenceInfo->getSemestre(1)->addUE(ueAlgo, 50);
        licenceInfo->getSemestre(1)->addUE(ueAnalyse, 50);
        licenceInfo->getSemestre(2)->addUE(ueBDD, 45);
        licenceInfo->getSemestre(2)->addUE(ueAlgebra, 45);
        licenceInfo->getSemestre(3)->addUE(ueWeb, 40);
        licenceInfo->getSemestre(3)->addUE(ueMeca, 40); // Un peu de physique en info pourquoi pas

        diplomes.push_back(licenceInfo);

        Diplome* masterData = new Diplome("Master Data Science");
        masterData->addSemestre("M1 S1");
        masterData->addSemestre("M1 S2");
        masterData->getSemestre(1)->addUE(ueAlgo, 30); // Réutilisation d'UE (possible ?)
        masterData->getSemestre(1)->addUE(ueBDD, 30);
        
        diplomes.push_back(masterData);
        cout << "-> 2 Diplômes créés avec semestres et UEs." << endl;

        // 5. Création des Interventions (Services)
        // Turing fait du CM et TD en Algo
        new Intervention(profTuring, ueAlgo, 0, 30, 20); 
        // Lovelace fait du TP en Algo et tout BDD
        new Intervention(profLovelace, ueAlgo, 15, 0, 0);
        new Intervention(profLovelace, ueBDD, 30, 0, 15);
        // Hopper fait du Web
        new Intervention(profHopper, ueWeb, 30, 20, 10);
        
        cout << "-> Interventions générées." << endl;
        cout << "Démonstration chargée avec succès !" << endl;
    }

    void menuPrincipal() {
        int choix = -1;
        while (choix != 0) {
            cout << "\n==========================================" << endl;
            cout << "      GESTION FACULTE - PROJET C++        " << endl;
            cout << "==========================================" << endl;
            cout << "1. Gestion RESSOURCES (Dépts, Profs, UEs)" << endl;
            cout << "2. Gestion MAQUETTE (Diplômes, Semestres)" << endl;
            cout << "3. Gestion SERVICES (Interventions)" << endl;
            cout << "4. LISTES & AFFICHAGE (Visualiser)" << endl;
            cout << "5. CALCULS & STATISTIQUES" << endl;
            cout << "------------------------------------------" << endl;
            cout << "7. Mode DÉMONSTRATION (Charger données test)" << endl;
            cout << "8. Sauvegarder" << endl;
            cout << "9. Charger" << endl;
            cout << "0. Quitter" << endl;
            cout << "Votre choix : "; 
            
            if(!(cin >> choix)) { cin.clear(); viderBuffer(); choix = -1; }
            viderBuffer();

            switch(choix) {
                case 1: menuRessources(); break;
                case 2: menuMaquette(); break;
                case 3: uiAjouterIntervention(); break;
                case 4: menuListes(); break;
                case 5: menuCalculs(); break;
                case 7: demo(); break;
                case 8: sauvegarderTout(); break;
                case 9: chargerTout(); break;
                case 0: cout << "Au revoir." << endl; break;
                default: cout << "Choix invalide." << endl;
            }
        }
    }

    void menuRessources() {
        int choix = -1;
        while(choix != 0) {
            afficherTitre("GESTION RESSOURCES");
            cout << "1. Créer un Département" << endl;
            cout << "2. Créer un Enseignant" << endl;
            cout << "3. Créer une UE (et ses enseignements)" << endl;
            cout << "0. Retour" << endl;
            cout << "Choix : "; cin >> choix; viderBuffer();
            
            switch(choix) {
                case 1: uiCreerDepartement(); break;
                case 2: uiCreerEnseignant(); break;
                case 3: uiCreerUEComplet(); break;
                case 0: break;
            }
        }
    }

    void menuMaquette() {
        int choix = -1;
        while(choix != 0) {
            afficherTitre("GESTION MAQUETTE");
            cout << "1. Créer un Diplôme" << endl;
            cout << "2. Ajouter un Semestre à un Diplôme" << endl;
            cout << "3. Inscrire une UE dans un Semestre" << endl;
            cout << "0. Retour" << endl;
            cout << "Choix : "; cin >> choix; viderBuffer();
            
            switch(choix) {
                case 1: uiCreerDiplome(); break;
                case 2: uiAjouterSemestre(); break;
                case 3: uiLierUEaSemestre(); break;
                case 0: break;
            }
        }
    }

    void menuListes() {
        int choix = -1;
        while(choix != 0) {
            afficherTitre("FILTRES D'AFFICHAGE");
            cout << "1. Départements" << endl;
            cout << "2. Tous les Enseignants" << endl;
            cout << "3. Enseignants Chercheurs uniquement" << endl;
            cout << "4. Autre Enseignants uniquement" << endl;
            cout << "5. UEs et volumes horaires" << endl;
            cout << "6. Diplômes (Structure complète)" << endl;
            cout << "0. Retour" << endl;
            cout << "Choix : "; cin >> choix; viderBuffer();

            switch(choix) {
                case 1: afficherListeGenerique("DÉPARTEMENTS", departements); break;
                case 2: afficherListeGenerique("TOUS LES ENSEIGNANTS", enseignants); break;
                case 3: 
                    // Utilisation d'une lambda pour filtrer
                    afficherListeGenerique<Enseignant>("CHERCHEURS UNIQUEMENT", enseignants, 
                        [](const Enseignant* e) { return dynamic_cast<const EnseignantChercheur*>(e) != nullptr; }); 
                    break;
                case 4: 
                    afficherListeGenerique<Enseignant>("AUTRES ENSEIGNANTS", enseignants, 
                        [](const Enseignant* e) { return dynamic_cast<const AutreEnseignant*>(e) != nullptr; }); 
                    break;
                case 5: afficherListeGenerique("UNITÉS D'ENSEIGNEMENT", ues); break;
                case 6: afficherListeGenerique("DIPLÔMES", diplomes); break;
                case 0: break;
            }
        }
    }

    void menuCalculs() {
        int choix = -1;
        while(choix != 0) {
            afficherTitre("CALCULS & STATS");
            cout << "1. Charges Enseignants (Service vs Obligations)" << endl;
            cout << "2. Coûts des Diplômes" << endl;
            cout << "3. Taux d'encadrement des Départements" << endl;
            cout << "0. Retour" << endl;
            cout << "Choix : "; cin >> choix; viderBuffer();
            
            if(choix == 1) uiCalculChargeEns();
            else if(choix == 2) uiCalculCoutDiplome();
            else if(choix == 3) uiCalculTauxEncadrement();
        }
    }

    // =========================================================
    //              3. MÉTHODES DE CRÉATION UI
    // =========================================================

    void uiCreerDepartement() {
        string nom;
        cout << "Nom du département : "; getline(cin, nom);
        departements.push_back(new Departement(nom));
        cout << "Département créé." << endl;
    }

    void uiCreerEnseignant() {
        if(departements.empty()) { cout << "Erreur: Créez un département d'abord." << endl; return; }
        
        string nom, prenom, adresse;
        cout << "Nom : "; getline(cin, nom);
        cout << "Prénom : "; getline(cin, prenom);
        cout << "Adresse : "; getline(cin, adresse);
        
        cout << "Type (1=Chercheur [192h], 2=Autre [384h]) : "; 
        int type; cin >> type; viderBuffer();
        
        cout << "Rattacher à quel département ?" << endl;
        for(size_t i=0; i<departements.size(); ++i) cout << i << ". " << departements[i]->getNom() << endl;
        int idx; cin >> idx; viderBuffer();

        if(idx >= 0 && idx < departements.size()) {
            Enseignant* e = (type == 1) ? (Enseignant*)new EnseignantChercheur(nom, prenom, adresse) : (Enseignant*)new AutreEnseignant(nom, prenom, adresse);
            departements[idx]->addEnseignant(e);
            enseignants.push_back(e);
            cout << "Enseignant ajouté." << endl;
        }
    }

    void uiCreerUEComplet() {
        if(enseignants.empty()) { cout << "Erreur: Il faut des enseignants (responsables)." << endl; return; }

        string nom;
        cout << "Nom de l'UE : "; getline(cin, nom);
        
        cout << "Responsable de l'UE : " << endl;
        for(size_t i=0; i<enseignants.size(); ++i) cout << i << ". " << enseignants[i]->getNom() << endl;
        int idx; cin >> idx; viderBuffer();
        
        if(idx < 0 || idx >= enseignants.size()) return;

        UE* ue = new UE(nom, enseignants[idx]);
        
        // Ajout des volumes horaires (Composition)
        char continuer = 'o';
        while(continuer == 'o') {
            cout << "Ajouter un enseignement (Cours/TD/TP) ? (o/n) : "; cin >> continuer;
            if(continuer == 'n') break;
            
            int type; float heures; int groupes;
            cout << "Type (0=COURS, 1=TP, 2=TD) : "; cin >> type;
            cout << "Volume horaire (heures) : "; cin >> heures;
            cout << "Nombre de groupes : "; cin >> groupes;
            
            enseignement_t typeEnum = (type==0) ? COURS : (type==1 ? TP : TD);
            ue->addEnseignement(new Enseignement(groupes, heures, typeEnum));
        }

        const_cast<Departement*>(ue->getDepartement())->addUE(ue);
        ues.push_back(ue);
        cout << "UE créée." << endl;
    }

    void uiCreerDiplome() {
        string nom;
        cout << "Nom du diplôme : "; getline(cin, nom);
        diplomes.push_back(new Diplome(nom));
        cout << "Diplôme créé." << endl;
    }

    void uiAjouterSemestre() {
        if(diplomes.empty()) { cout << "Aucun diplôme disponible." << endl; return; }
        
        cout << "Choisir le diplôme :" << endl;
        for(size_t i=0; i<diplomes.size(); ++i) cout << i << ". " << diplomes[i]->getNom() << endl;
        int idx; cin >> idx; viderBuffer();
        
        if(idx >= 0 && idx < diplomes.size()) {
            string nomSem;
            cout << "Nom du semestre : "; getline(cin, nomSem);
            diplomes[idx]->addSemestre(nomSem);
            cout << "Semestre ajouté." << endl;
        }
    }

    void uiLierUEaSemestre() {
        // Logique UML : Diplôme -> Semestre -> UE
        if(diplomes.empty() || ues.empty()) { cout << "Manque de Diplômes ou d'UEs." << endl; return; }

        cout << "--- 1. Choisir Diplôme ---" << endl;
        for(size_t i=0; i<diplomes.size(); ++i) cout << i << ". " << diplomes[i]->getNom() << endl;
        int idxDip; cin >> idxDip; 
        if(idxDip < 0 || idxDip >= diplomes.size()) return;
        
        Diplome* d = diplomes[idxDip];
        if(d->getNbSemestres() == 0) { cout << "Ce diplôme n'a pas de semestre." << endl; return; }

        cout << "--- 2. Choisir Semestre ---" << endl;
        // Parcours manuel car getSemestre prend un ID 1-based
        for(int i=1; i<=d->getNbSemestres(); ++i) {
            cout << i << ". " << d->getSemestre(i)->getCoutHoraires() << "h ETD (Info cout)" << endl; 
        }
        int numSem; cin >> numSem;
        Semestre* s = d->getSemestre(numSem);

        cout << "--- 3. Choisir l'UE à inscrire ---" << endl;
        for(size_t i=0; i<ues.size(); ++i) cout << i << ". " << ues[i]->getNom() << endl;
        int idxUE; cin >> idxUE; 
        
        int nbEtudiants;
        cout << "Nombre d'étudiants inscrits : "; cin >> nbEtudiants;

        s->addUE(ues[idxUE], nbEtudiants);
        cout << "Lien créé !" << endl;
    }

    void uiAjouterIntervention() {
        if(ues.empty() || enseignants.empty()) { cout << "Données manquantes." << endl; return; }
        
        cout << "Enseignant : ";
        for(size_t i=0; i<enseignants.size(); ++i) cout << i << ". " << enseignants[i]->getNom() << endl;
        int idxEns; cin >> idxEns;

        cout << "UE : ";
        for(size_t i=0; i<ues.size(); ++i) cout << i << ". " << ues[i]->getNom() << endl;
        int idxUE; cin >> idxUE;

        float c, td, tp;
        cout << "Heures Cours : "; cin >> c;
        cout << "Heures TD : "; cin >> td;
        cout << "Heures TP : "; cin >> tp;

        new Intervention(enseignants[idxEns], ues[idxUE], tp, td, c);
        cout << "Service ajouté." << endl;
    }

    // =========================================================
    //              4. MÉTHODES DE CALCUL
    // =========================================================
    
    void uiCalculChargeEns() {
        for(const auto* e : enseignants) {
            cout << e->getNom() << ": " << e->getETDRealise() << "h / " << e->getETDMax() << "h" << endl;
        }
    }

    void uiCalculCoutDiplome() {
        for(const auto* d : diplomes) {
            cout << d->getNom() << " : " << d->getCoutTotal() << "h ETD Total." << endl;
        }
    }

    void uiCalculTauxEncadrement() {
        for(const auto* d : departements) {
            cout << d->getNom() << " : " << fixed << setprecision(2) << (d->getTauxEncadrement()*100) << "%" << endl;
        }
    }
};

int main() {
    GestionFaculte app;
    app.menuPrincipal();
    return 0;
}