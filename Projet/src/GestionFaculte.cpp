#include "GestionFaculte.hpp"
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

void GestionFaculte::viderBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void GestionFaculte::afficherTitre(const string& titre) {
    cout << "\n==========================================" << endl;
    cout << "   " << titre << endl;
    cout << "==========================================" << endl;
}

GestionFaculte::GestionFaculte() {}

GestionFaculte::~GestionFaculte() {
    for (auto* u : ues) {
        delete u;
    }
    ues.clear();
    for (auto* d : diplomes) {
        delete d;
    }
    diplomes.clear();
    for (auto* e : enseignants) {
        if (e->getDepartement() == nullptr) {
            delete e;
        }
    }
    enseignants.clear();

    // 4. Suppression des Départements
    for (auto* d : departements) {
        delete d;
    }
    departements.clear();
}

// Helper pour découper les strings
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void GestionFaculte::toutEffacer() {
    for(auto* u : ues) delete u; ues.clear();
    for(auto* d : diplomes) delete d; diplomes.clear();
    {
       Intervention::resetInterventions();
    }

    for(auto* e : enseignants) if(e->getDepartement() == nullptr) delete e; 
    enseignants.clear();
    for(auto* d : departements) delete d; departements.clear();
    mapDept.clear();
    mapEns.clear();
    mapUE.clear();
}

void GestionFaculte::sauvegarder() {
    ofstream file("sauvegarde_faculte.txt");
    if (!file.is_open()) {
        cerr << "Erreur : Impossible de créer le fichier de sauvegarde." << endl;
        return;
    }

    for (const auto* d : departements) {
        int idResp = (d->getResponsable()) ? d->getResponsable()->getId() : -1;
        file << "DEPT;" << d->getId() << ";" << d->getNom() << ";" << idResp << endl;
    }

    for (const auto* e : enseignants) {
        int type = (dynamic_cast<const EnseignantChercheur*>(e)) ? 1 : 2;
        int idDept = (e->getDepartement()) ? e->getDepartement()->getId() : -1;
        
        file << "ENS;" << e->getId() << ";" << type << ";" 
             << e->getNom() << ";" << e->getPrenom() << ";" 
             << e->getAdresse() << ";" << idDept << endl;
    }

    for (const auto* u : ues) {
        int idDept = (u->getDepartement()) ? u->getDepartement()->getId() : -1;
        int idResp = (u->getResponsable()) ? u->getResponsable()->getId() : -1;

        file << "UE;" << u->getId() << ";" << u->getNom() << ";" 
             << idDept << ";" << idResp << endl;

        for (const auto* ens : u->getEnseignements()) {
            file << "COURS_UE;" << u->getId() << ";" 
                 << ens->getNbGroupe() << ";" << ens->getNbHeure() << ";" 
                 << (int)ens->getType() << endl;
        }
    }
    for (const auto* d : diplomes) {
        file << "DIPLOME;" << d->getNom() << endl;
        
        for (int i = 1; i <= d->getNbSemestres(); ++i) {
            Semestre* s = d->getSemestre(i);
            file << "SEMESTRE;" << d->getNom() << ";" << s->getNom() << endl;

            for (const auto& lien : s->getUEs()) {
                file << "LIEN_UE;" << d->getNom() << ";" << s->getNom() << ";" 
                     << lien.ue->getId() << ";" << lien.nbInscrits << endl;
            }
        }
    }
    for (auto* i : Intervention::getAllInterventions()) {
        file << "INTERVENTION;" << i->getIntervenant()->getId() << ";" 
             << i->getUE()->getId() << ";" 
             << i->getHTP() << ";" << i->getHTD() << ";" << i->getHCours() << endl;
    }

    cout << "Sauvegarde terminee avec succes !" << endl;
    file.close();
}

void GestionFaculte::charger() {
    ifstream file("sauvegarde_faculte.txt");
    if (!file.is_open()) {
        cout << "Aucun fichier de sauvegarde trouve." << endl;
        return;
    }
    toutEffacer(); 
    cout << "Chargement en cours..." << endl;

    string line;
    
    map<int, int> liensDeptResp; 

    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> data = split(line, ';');
        string type = data[0];

        if (type == "DEPT") {
            int oldId = stoi(data[1]);
            string nom = data[2];
            int idResp = stoi(data[3]);

            Departement* d = new Departement(nom);
            departements.push_back(d);
            mapDept[oldId] = d; 
            
            if (idResp != -1) liensDeptResp[oldId] = idResp;
        }
        else if (type == "ENS") {
            int oldId = stoi(data[1]);
            int typeEns = stoi(data[2]);
            string nom = data[3];
            string prenom = data[4];
            string adresse = data[5];
            int idDept = stoi(data[6]);

            Enseignant* e = nullptr;
            if (typeEns == 1) e = new EnseignantChercheur(nom, prenom, adresse);
            else e = new AutreEnseignant(nom, prenom, adresse);
            
            enseignants.push_back(e);
            mapEns[oldId] = e;
            if (mapDept.find(idDept) != mapDept.end()) {
                mapDept[idDept]->addEnseignant(e);
            }
        }
        else if (type == "UE") {
            int oldId = stoi(data[1]);
            string nom = data[2];
            int idDept = stoi(data[3]);
            int idResp = stoi(data[4]);

            // On récupère les pointeurs via les maps
            Departement* d = mapDept[idDept];
            Enseignant* r = mapEns[idResp];

            if (d && r) {
                UE* u = new UE(nom, d, r);
                const_cast<Departement*>(d)->addUE(u); 
                ues.push_back(u);
                mapUE[oldId] = u;
            }
        }
        else if (type == "COURS_UE") {
            int idUE = stoi(data[1]);
            int grp = stoi(data[2]);
            float hrs = stof(data[3]);
            int t = stoi(data[4]);

            if (mapUE.find(idUE) != mapUE.end()) {
                mapUE[idUE]->addEnseignement(new Enseignement(grp, hrs, (enseignement_t)t));
            }
        }
        else if (type == "DIPLOME") {
            diplomes.push_back(new Diplome(data[1]));
        }
        else if (type == "SEMESTRE") {
            string nomDip = data[1];
            for (auto* d : diplomes) {
                if (d->getNom() == nomDip) {
                    d->addSemestre(data[2]);
                    break;
                }
            }
        }
        else if (type == "LIEN_UE") {
            string nomDip = data[1];
            string nomSem = data[2];
            int idUE = stoi(data[3]);
            int nb = stoi(data[4]);

            if (mapUE.find(idUE) != mapUE.end()) {
                for (auto* d : diplomes) {
                    if (d->getNom() == nomDip) {
                         for(int i=1; i<=d->getNbSemestres(); ++i) {
                             if(d->getSemestre(i)->getNom() == nomSem) {
                                 d->getSemestre(i)->addUE(mapUE[idUE], nb);
                                 break;
                             }
                         }
                         break;
                    }
                }
            }
        }
        else if (type == "INTERVENTION") {
            int idEns = stoi(data[1]);
            int idUE = stoi(data[2]);
            float htp = stof(data[3]);
            float htd = stof(data[4]);
            float hc = stof(data[5]);

            if (mapEns.find(idEns) != mapEns.end() && mapUE.find(idUE) != mapUE.end()) {
                new Intervention(mapEns[idEns], mapUE[idUE], htp, htd, hc);
            }
        }
    }
    for (auto const& pair : liensDeptResp) {
        int oldDeptId = pair.first;
        int oldRespId = pair.second;
        if (mapDept[oldDeptId] && mapEns[oldRespId]) {
            mapDept[oldDeptId]->setResponsable(mapEns[oldRespId]);
        }
    }

    cout << "Chargement termine !" << endl;
    file.close();
}

void GestionFaculte::demo() {
    afficherTitre("INITIALISATION DE LA DEMONSTRATION");

    // 1. Création des Départements
    Departement* deptInfo = new Departement("Informatique");
    Departement* deptMath = new Departement("Mathematiques");
    Departement* deptPhys = new Departement("Physique");
    departements.push_back(deptInfo);
    departements.push_back(deptMath);
    departements.push_back(deptPhys);
    cout << "-> 3 Departements crees." << endl;

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
    cout << "-> 7 Enseignants crees et affectes." << endl;

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

    UE* ueBDD = new UE("Bases de Donnees", profLovelace);
    ueBDD->setDepartement(deptInfo);
    deptInfo->addUE(ueBDD);
    // 1 CM, 6 TP
    ueBDD->addEnseignement(new Enseignement(1, 15, COURS));
    ueBDD->addEnseignement(new Enseignement(6, 30, TP));
    ues.push_back(ueBDD);

    UE* ueWeb = new UE("Developpement Web", profHopper);
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

    UE* ueAlgebra = new UE("Algebre", profGauss);
    ueAlgebra->setDepartement(deptMath);
    deptMath->addUE(ueAlgebra);
    // 1 CM, 5 TD
    ueAlgebra->addEnseignement(new Enseignement(1, 25, COURS));
    ueAlgebra->addEnseignement(new Enseignement(5, 35, TD));
    ues.push_back(ueAlgebra);

    // Physique
    UE* ueMeca = new UE("Mecanique", profEinstein);
    ueMeca->setDepartement(deptPhys);
    deptPhys->addUE(ueMeca);
    // 1 CM, 4 TD, 4 TP
    ueMeca->addEnseignement(new Enseignement(1, 20, COURS));
    ueMeca->addEnseignement(new Enseignement(4, 20, TD));
    ueMeca->addEnseignement(new Enseignement(4, 10, TP));
    ues.push_back(ueMeca);

    cout << "-> 6 UEs creees avec leurs enseignements." << endl;

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
    cout << "-> 2 Diplomes crees avec semestres et UEs." << endl;

    // 5. Création des Interventions (Services)
    // Turing fait du CM et TD en Algo
    new Intervention(profTuring, ueAlgo, 0, 30, 20); 
    // Lovelace fait du TP en Algo et tout BDD
    new Intervention(profLovelace, ueAlgo, 15, 0, 0);
    new Intervention(profLovelace, ueBDD, 30, 0, 15);
    // Hopper fait du Web
    new Intervention(profHopper, ueWeb, 30, 20, 10);
    
    cout << "-> Interventions generees." << endl;
    cout << "Demonstration chargee avec succes !" << endl;
}

void GestionFaculte::menuPrincipal() {
    int choix = -1;
    while (choix != 0) {
        cout << "\n==========================================" << endl;
        cout << "      GESTION FACULTE - PROJET C++        " << endl;
        cout << "==========================================" << endl;
        cout << "1. Gestion RESSOURCES (Depts, Profs, UEs)" << endl;
        cout << "2. Gestion MAQUETTE (Diplomes, Semestres)" << endl;
        cout << "3. Gestion SERVICES (Interventions)" << endl;
        cout << "4. LISTES & AFFICHAGE (Visualiser)" << endl;
        cout << "5. CALCULS & STATISTIQUES" << endl;
        cout << "------------------------------------------" << endl;
        cout << "7. Mode DEMONSTRATION (Charger donnees test)" << endl;
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
            case 8: sauvegarder(); break;
            case 9: charger(); break;
            case 0: cout << "Au revoir." << endl; break;
            default: cout << "Choix invalide." << endl;
        }
    }
}

void GestionFaculte::menuRessources() {
    int choix = -1;
    while(choix != 0) {
        afficherTitre("GESTION RESSOURCES");
        cout << "1. Creer un Departement" << endl;
        cout << "2. Creer un Enseignant" << endl;
        cout << "3. Creer une UE (et ses enseignements)" << endl;
        cout << "4. Nommer un Responsable de Departement" << endl;
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

void GestionFaculte::menuMaquette() {
    int choix = -1;
    while(choix != 0) {
        afficherTitre("GESTION MAQUETTE");
        cout << "1. Creer un Diplome" << endl;
        cout << "2. Ajouter un Semestre a un Diplome" << endl;
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

void GestionFaculte::menuListes() {
    int choix = -1;
    while(choix != 0) {
        afficherTitre("FILTRES D'AFFICHAGE");
        cout << "1. Departements" << endl;
        cout << "2. Tous les Enseignants" << endl;
        cout << "3. Enseignants Chercheurs uniquement" << endl;
        cout << "4. Autre Enseignants uniquement" << endl;
        cout << "5. UEs et volumes horaires" << endl;
        cout << "6. Diplomes (Structure complete)" << endl;
        cout << "0. Retour" << endl;
        cout << "Choix : "; cin >> choix; viderBuffer();

        switch(choix) {
            case 1: afficherListeGenerique("DEPARTEMENTS", departements); break;
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
            case 5: afficherListeGenerique("UNITES D'ENSEIGNEMENT", ues); break;
            case 6: afficherListeGenerique("DIPLOMES", diplomes); break;
            case 0: break;
        }
    }
}

void GestionFaculte::menuCalculs() {
    int choix = -1;
    while(choix != 0) {
        afficherTitre("CALCULS & STATS");
        cout << "1. Charges Enseignants (Service vs Obligations)" << endl;
        cout << "2. Couts des Diplomes" << endl;
        cout << "3. Charges horaires et Taux d'encadrement" << endl;
        cout << "0. Retour" << endl;
        cout << "Choix : "; cin >> choix; viderBuffer();
        
        if(choix == 1) uiCalculChargeEns();
        else if(choix == 2) uiCalculCoutDiplome();
        else if(choix == 3) uiCalculTauxEncadrement();
    }
}

void GestionFaculte::uiCreerDepartement() {
    string nom;
    cout << "Nom du departement : "; getline(cin, nom);
    departements.push_back(new Departement(nom));
    cout << "Departement cree." << endl;
}

void GestionFaculte::uiCreerEnseignant() {
    if(departements.empty()) { cout << "Erreur: Creez un departement d'abord." << endl; return; }
    
    string nom, prenom, adresse;
    cout << "Nom : "; getline(cin, nom);
    cout << "Prenom : "; getline(cin, prenom);
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
        cout << "Enseignant ajoute." << endl;
    }
}

void GestionFaculte::uiCreerUEComplet() {
    if(enseignants.empty() || departements.empty()) { 
        cout << "Erreur: Il faut des enseignants et des departements." << endl; return; 
    }

    string nom;
    cout << "Nom de l'UE : "; getline(cin, nom);
    
    // 1. Choix du Responsable (n'importe quel enseignant)
    cout << "Enseignant Responsable de l'UE : " << endl;
    for(size_t i=0; i<enseignants.size(); ++i) cout << i << ". " << enseignants[i]->getNom() << endl;
    int idxEns; cin >> idxEns; viderBuffer();
    if(idxEns < 0 || idxEns >= enseignants.size()) return;

    // 2. Choix du Département de rattachement (Indépendant du prof)
    cout << "Departement porteur de l'UE : " << endl;
    for(size_t i=0; i<departements.size(); ++i) cout << i << ". " << departements[i]->getNom() << endl;
    int idxDept; cin >> idxDept; viderBuffer();
    if(idxDept < 0 || idxDept >= departements.size()) return;

    // Utilisation du constructeur spécifique (Nom, Dept, Responsable)
    UE* ue = new UE(nom, departements[idxDept], enseignants[idxEns]);
    
    // Ajout des volumes horaires (Reste inchangé)
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

    // Important : Ajouter l'UE au département choisi manuellement
    const_cast<Departement*>(ue->getDepartement())->addUE(ue);
    ues.push_back(ue);
    cout << "UE créée et affectée au département " << ue->getDepartement()->getNom() << "." << endl;
}

void GestionFaculte::uiCreerDiplome() {
    string nom;
    cout << "Nom du diplome : "; getline(cin, nom);
    diplomes.push_back(new Diplome(nom));
    cout << "Diplome cree." << endl;
}

void GestionFaculte::uiAjouterSemestre() {
    if(diplomes.empty()) { cout << "Aucun diplome disponible." << endl; return; }
    
    cout << "Choisir le diplome :" << endl;
    for(size_t i=0; i<diplomes.size(); ++i) cout << i << ". " << diplomes[i]->getNom() << endl;
    int idx; cin >> idx; viderBuffer();
    
    if(idx >= 0 && idx < diplomes.size()) {
        string nomSem;
        cout << "Nom du semestre : "; getline(cin, nomSem);
        diplomes[idx]->addSemestre(nomSem);
        cout << "Semestre ajoute." << endl;
    }
}

void GestionFaculte::uiLierUEaSemestre() {
    // Logique UML : Diplome -> Semestre -> UE
    if(diplomes.empty() || ues.empty()) { cout << "Manque de Diplomes ou d'UEs." << endl; return; }

    cout << "--- 1. Choisir Diplome ---" << endl;
    for(size_t i=0; i<diplomes.size(); ++i) cout << i << ". " << diplomes[i]->getNom() << endl;
    int idxDip; cin >> idxDip; 
    if(idxDip < 0 || idxDip >= diplomes.size()) return;
    
    Diplome* d = diplomes[idxDip];
    if(d->getNbSemestres() == 0) { cout << "Ce diplome n'a pas de semestre." << endl; return; }

    cout << "--- 2. Choisir Semestre ---" << endl;
    // Parcours manuel car getSemestre prend un ID 1-based
    for(int i=1; i<=d->getNbSemestres(); ++i) {
        cout << i << ". " << d->getSemestre(i)->getNom() << " (" << d->getSemestre(i)->getCoutHoraires() << "h ETD)" << endl; 
    }
    int numSem; cin >> numSem;
    Semestre* s = d->getSemestre(numSem);

    cout << "--- 3. Choisir l'UE a inscrire ---" << endl;
    for(size_t i=0; i<ues.size(); ++i) cout << i << ". " << ues[i]->getNom() << endl;
    int idxUE; cin >> idxUE; 
    
    int nbEtudiants;
    cout << "Nombre d'etudiants inscrits : "; cin >> nbEtudiants;

    s->addUE(ues[idxUE], nbEtudiants);
    cout << "Lien cree !" << endl;
}

void GestionFaculte::uiAjouterIntervention() {
    if(ues.empty() || enseignants.empty()) { cout << "Donnees manquantes." << endl; return; }
    
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
    cout << "Service ajoute." << endl;
}

void GestionFaculte::uiCalculChargeEns() {
    for(const auto* e : enseignants) {
        cout << e->getNom() << ": " << e->getETDRealise() << "h / " << e->getETDMax() << "h" << endl;
    }
}

void GestionFaculte::uiCalculCoutDiplome() {
    for(const auto* d : diplomes) {
        cout << d->getNom() << " : " << d->getCoutTotal() << "h ETD Total." << endl;
    }
}

void GestionFaculte::uiCalculTauxEncadrement() {
    afficherTitre("CHARGES ET TAUX D'ENCADREMENT");
    for(const auto* d : departements) {
        float besoin = d->getHeuresBesoin();
        float dispo = d->getHeuresDispo();
        float taux = (besoin > 0) ? (dispo / besoin * 100) : 0;
        
        cout << "DEPT: " << d->getNom() << endl;
        cout << "  > Charge (Besoin) : " << fixed << setprecision(1) << besoin << " h ETD" << endl;
        cout << "  > Ressources (Dispo) : " << dispo << " h ETD" << endl;
        cout << "  > Taux couverture : " << setprecision(2) << taux << " %" << endl;
        cout << "-----------------------------------" << endl;
    }
}
    
void GestionFaculte::uiNommerResponsableDepartement() {
    if(departements.empty()) { cout << "Aucun département." << endl; return; }

        cout << "Choisir le departement :" << endl;
        for(size_t i=0; i<departements.size(); ++i) 
            cout << i << ". " << departements[i]->getNom() << " (Resp: " 
                << (departements[i]->getResponsable() ? departements[i]->getResponsable()->getNom() : "Aucun") << ")" << endl;
        
        int idx; cin >> idx; viderBuffer();
        if(idx < 0 || idx >= departements.size()) return;
        Departement* d = departements[idx];

        // On suppose que le responsable doit faire partie du département (logique métier standard)
        const auto& profs = d->getEnseignants();
        if(profs.empty()) { cout << "Ce département n'a pas d'enseignants." << endl; return; }

        cout << "Choisir le nouveau responsable parmi les membres :" << endl;
        for(size_t i=0; i<profs.size(); ++i) 
            cout << i << ". " << profs[i]->getNom() << endl;

        int idxProf; cin >> idxProf; viderBuffer();
        if(idxProf >= 0 && idxProf < profs.size()) {
            d->setResponsable(profs[idxProf]);
            cout << "Responsable mis a jour." << endl;
        }
    }

