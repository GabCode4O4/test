#include <iostream>
#include <iomanip>

#include "departement.hpp"
#include "ue.hpp"
#include "enseignement.hpp"
#include "enseignant_chercheur.hpp"
#include "autre_enseignant.hpp"
#include "intervention.hpp"
#include "diplome.hpp"
#include "semestre.hpp"

using namespace std;

void afficherSeparateur(const string& titre) {
    cout << "\n" << string(60, '=') << endl;
    cout << "  " << titre << endl;
    cout << string(60, '=') << endl;
}

void testDepartement() {
    afficherSeparateur("TEST DEPARTEMENT");
    
    Departement* dept = new Departement("Informatique");
    cout << "Département créé: " << dept->getNom() << " (ID: " << dept->getId() << ")" << endl;
    
    cout << "\n--- Ajout d'enseignants ---" << endl;
    EnseignantChercheur* prof1 = new EnseignantChercheur("Dupont", "Jean");
    EnseignantChercheur* prof2 = new EnseignantChercheur("Martin", "Sophie");
    AutreEnseignant* prof3 = new AutreEnseignant("Bernard", "Luc");
    
    dept->addEnseignant(prof1);
    dept->addEnseignant(prof2);
    dept->addEnseignant(prof3);
    
    cout << "Nombre d'enseignants: " << dept->getEnseignants().size() << endl;
    cout << "\nListe des enseignants:" << endl;
    for (const auto& ens : dept->getEnseignants()) {
        cout << "  - " << ens->toString() << " (ETD Max: " << ens->getETDMax() << "h)" << endl;
    }
    
    cout << "\n" << *dept << endl;
    
    delete dept;
}

void testEnseignants() {
    afficherSeparateur("TEST ENSEIGNANTS");
    
    Departement* dept = new Departement("Mathématiques");
    
    EnseignantChercheur* ec = new EnseignantChercheur("Conchon", "Emmanuel");
    AutreEnseignant* ae = new AutreEnseignant("Maria", "Maxime");
    
    dept->addEnseignant(ec);
    dept->addEnseignant(ae);
    
    cout << "Enseignant Chercheur:" << endl;
    cout << "  " << *ec << endl;
    cout << "  ETD Max: " << ec->getETDMax() << "h" << endl;
    cout << "  ETD Réalisé: " << ec->getETDRealise() << "h" << endl;
    
    cout << "\nAutre Enseignant (PRAG/PRCE):" << endl;
    cout << "  " << *ae << endl;
    cout << "  ETD Max: " << ae->getETDMax() << "h" << endl;
    cout << "  ETD Réalisé: " << ae->getETDRealise() << "h" << endl;
    
    delete dept;
}

void testUEetEnseignements() {
    afficherSeparateur("TEST UE ET ENSEIGNEMENTS");
    
    Departement* dept = new Departement("Informatique");
    EnseignantChercheur* responsable = new EnseignantChercheur("Dupuis", "Marie");
    dept->addEnseignant(responsable);
    
    UE* ue = new UE("Bases de Données Relationnelles", dept, responsable);
    dept->addUE(ue);
    
    cout << "UE créée: " << ue->getNom() << " (ID: " << ue->getId() << ")" << endl;
    cout << "Responsable: " << responsable->getNom() << endl;
    
    cout << "\n--- Ajout d'enseignements ---" << endl;
    Enseignement* cours = new Enseignement(1, 20.0f, COURS);
    Enseignement* td = new Enseignement(2, 15.0f, TD);
    Enseignement* tp = new Enseignement(4, 12.0f, TP);
    
    ue->addEnseignement(cours);
    ue->addEnseignement(td);
    ue->addEnseignement(tp);
    
    cout << "Cours: " << *cours << endl;
    cout << "TD: " << *td << endl;
    cout << "TP: " << *tp << endl;
    
    cout << "\nETD total de l'UE: " << ue->getETD() << "h" << endl;
    
    cout << "\n--- Ajout d'inscrits ---" << endl;
    ue->ajouterInscrits(50);
    cout << "Nombre d'inscrits: " << ue->getNbTotalInscrits() << endl;
    
    cout << "\n" << *ue << endl;
    
    delete dept;
}

void testInterventions() {
    afficherSeparateur("TEST INTERVENTIONS");
    
    Departement* dept = new Departement("Informatique");
    EnseignantChercheur* responsable = new EnseignantChercheur("Leclerc", "Paul");
    EnseignantChercheur* intervenant1 = new EnseignantChercheur("Rousseau", "Claire");
    AutreEnseignant* intervenant2 = new AutreEnseignant("Moreau", "Pierre");
    
    dept->addEnseignant(responsable);
    dept->addEnseignant(intervenant1);
    dept->addEnseignant(intervenant2);
    
    UE* ue = new UE("Programmation Orientée Objet", dept, responsable);
    dept->addUE(ue);
    
    Enseignement* cours = new Enseignement(1, 24.0f, COURS);
    Enseignement* td = new Enseignement(3, 18.0f, TD);
    Enseignement* tp = new Enseignement(6, 20.0f, TP);
    
    ue->addEnseignement(cours);
    ue->addEnseignement(td);
    ue->addEnseignement(tp);
    
    cout << "--- Création d'interventions ---" << endl;
    Intervention* int1 = new Intervention(responsable, ue, 10.0f, 8.0f, 12.0f);
    Intervention* int2 = new Intervention(intervenant1, ue, 10.0f, 10.0f, 12.0f);
    Intervention* int3 = new Intervention(intervenant2, ue, 0.0f, 0.0f, 0.0f);
    
    
    cout << "Intervention 1 - ETD: " << int1->getETD() << "h" << endl;
    cout << "Intervention 2 - ETD: " << int2->getETD() << "h" << endl;
    cout << "Intervention 3 - ETD: " << int3->getETD() << "h" << endl;
    
    cout << "\n--- Charge de travail des enseignants ---" << endl;
    cout << responsable->getNom() << " - ETD réalisé: " << responsable->getETDRealise() 
         << "h / " << responsable->getETDMax() << "h" << endl;
    cout << intervenant1->getNom() << " - ETD réalisé: " << intervenant1->getETDRealise() 
         << "h / " << intervenant1->getETDMax() << "h" << endl;
    cout << intervenant2->getNom() << " - ETD réalisé: " << intervenant2->getETDRealise() 
         << "h / " << intervenant2->getETDMax() << "h" << endl;
    
    delete dept;
}

void testDiplomeEtSemestre() {
    afficherSeparateur("TEST DIPLOME ET SEMESTRE");
    
    Departement* dept = new Departement("Informatique");
    
    EnseignantChercheur* resp1 = new EnseignantChercheur("Durand", "Alice");
    EnseignantChercheur* resp2 = new EnseignantChercheur("Petit", "Bob");
    EnseignantChercheur* resp3 = new EnseignantChercheur("Roux", "Charlie");
    
    dept->addEnseignant(resp1);
    dept->addEnseignant(resp2);
    dept->addEnseignant(resp3);
    
    Diplome* licence = new Diplome("Licence Informatique");
    
    cout << "Diplôme créé: Licence Informatique" << endl;
    
    cout << "\n--- Création des semestres ---" << endl;
    
    licence->addSemestre("Semestre 1");
    licence->addSemestre("Semestre 2");

    Semestre* s1 = const_cast<Semestre*>(licence->getSemestre(1));
    Semestre* s2 = const_cast<Semestre*>(licence->getSemestre(2));


    cout << "Semestre 1 et 2 ajoutés au diplôme" << endl;
    
    cout << "\n--- Création des UE pour S1 ---" << endl;
    UE* ue1 = new UE("Algorithmique", dept, resp1);
    UE* ue2 = new UE("Mathématiques", dept, resp2);
    
    dept->addUE(ue1);
    dept->addUE(ue2);
    
    Enseignement* cours1 = new Enseignement(1, 30.0f, COURS);
    Enseignement* td1 = new Enseignement(2, 20.0f, TD);
    ue1->addEnseignement(cours1);
    ue1->addEnseignement(td1);
    
    Enseignement* cours2 = new Enseignement(1, 25.0f, COURS);
    Enseignement* tp2 = new Enseignement(3, 15.0f, TP);
    ue2->addEnseignement(cours2);
    ue2->addEnseignement(tp2);
    
    s1->addUE(ue1, 60);
    s1->addUE(ue2, 60);
    
    cout << "\n--- Création des UE pour S2 ---" << endl;
    UE* ue3 = new UE("POO", dept, resp3);
    dept->addUE(ue3);
    
    Enseignement* cours3 = new Enseignement(1, 28.0f, COURS);
    Enseignement* tp3 = new Enseignement(4, 24.0f, TP);
    ue3->addEnseignement(cours3);
    ue3->addEnseignement(tp3);
    
    s2->addUE(ue3, 58);
    
    cout << "\nCoût horaire S1: " << s1->getCoutHoraires() << "h" << endl;
    cout << "Coût horaire S2: " << s2->getCoutHoraires() << "h" << endl;
    cout << "Coût total du diplôme: " << licence->getCoutTotal() << "h" << endl;
    
    delete licence;
    delete dept;
}

void testTauxEncadrement() {
    afficherSeparateur("TEST TAUX D'ENCADREMENT");
    
    Departement* dept = new Departement("Sciences");
    
    EnseignantChercheur* prof1 = new EnseignantChercheur("Einstein", "Albert");
    EnseignantChercheur* prof2 = new EnseignantChercheur("Curie", "Marie");
    AutreEnseignant* prof3 = new AutreEnseignant("Newton", "Isaac");
    
    dept->addEnseignant(prof1);
    dept->addEnseignant(prof2);
    dept->addEnseignant(prof3);
    
    UE* ue1 = new UE("Physique Quantique", dept, prof1);
    UE* ue2 = new UE("Chimie Générale", dept, prof2);
    
    dept->addUE(ue1);
    dept->addUE(ue2);
    
    Enseignement* cours1 = new Enseignement(1, 30.0f, COURS);
    Enseignement* tp1 = new Enseignement(3, 20.0f, TP);
    ue1->addEnseignement(cours1);
    ue1->addEnseignement(tp1);
    
    Enseignement* cours2 = new Enseignement(1, 25.0f, COURS);
    Enseignement* td2 = new Enseignement(2, 18.0f, TD);
    ue2->addEnseignement(cours2);
    ue2->addEnseignement(td2);
    
    Intervention* int1 = new Intervention(prof1, ue1, 20.0f, 0.0f, 30.0f);
    Intervention* int2 = new Intervention(prof2, ue2, 0.0f, 18.0f, 25.0f);
    Intervention* int3 = new Intervention(prof3, ue1, 0.0f, 0.0f, 0.0f);
    
  
    cout << "Taux d'encadrement du département: " << fixed << setprecision(2) 
         << dept->getTauxEncadrement() * 100 << "%" << endl;
    
    cout << "\nDétail des enseignants:" << endl;
    for (const auto& ens : dept->getEnseignants()) {
        float taux = (ens->getETDMax() > 0) ? (ens->getETDRealise() / ens->getETDMax()) * 100 : 0;
        cout << "  " << ens->getNom() << ": " << ens->getETDRealise() << "h / " 
             << ens->getETDMax() << "h (" << fixed << setprecision(1) << taux << "%)" << endl;
    }
    
    delete dept;
}

void testComplet() {
    afficherSeparateur("TEST COMPLET DU SYSTÈME");
    
    cout << "Création d'un système de gestion universitaire complet..." << endl;
    
    // Création du département
    Departement* deptInfo = new Departement("Informatique");
    
    // Création des enseignants
    EnseignantChercheur* conchon = new EnseignantChercheur("Conchon", "Emmanuel");
    EnseignantChercheur* maria = new EnseignantChercheur("Maria", "Maxime");
    AutreEnseignant* dupont = new AutreEnseignant("Dupont", "Jean");
    
    deptInfo->addEnseignant(conchon);
    deptInfo->addEnseignant(maria);
    deptInfo->addEnseignant(dupont);
    
    // Création du diplôme
    Diplome* master = new Diplome("Master Informatique");
    
    // Création des semestres
    master->addSemestre("M1 - Semestre 1");
    master->addSemestre("M1 - Semestre 2");

    Semestre* s1 = master->getSemestre(1);
    Semestre* s2 = master->getSemestre(2);

    // Création des UE
    UE* bdr = new UE("Bases de Données Relationnelles", deptInfo, conchon);
    UE* poo = new UE("Programmation Orientée Objet", deptInfo, maria);
    UE* algo = new UE("Algorithmique Avancée", deptInfo, dupont);
    
    deptInfo->addUE(bdr);
    deptInfo->addUE(poo);
    deptInfo->addUE(algo);
    
    // Enseignements pour BDR
    Enseignement* bdr_cours = new Enseignement(1, 24.0f, COURS);
    Enseignement* bdr_td = new Enseignement(2, 18.0f, TD);
    Enseignement* bdr_tp = new Enseignement(4, 20.0f, TP);
    
    bdr->addEnseignement(bdr_cours);
    bdr->addEnseignement(bdr_td);
    bdr->addEnseignement(bdr_tp);
    
    // Enseignements pour POO
    Enseignement* poo_cours = new Enseignement(1, 20.0f, COURS);
    Enseignement* poo_tp = new Enseignement(3, 25.0f, TP);
    
    poo->addEnseignement(poo_cours);
    poo->addEnseignement(poo_tp);
    
    // Enseignements pour Algo
    Enseignement* algo_cours = new Enseignement(1, 22.0f, COURS);
    Enseignement* algo_td = new Enseignement(2, 16.0f, TD);
    
    algo->addEnseignement(algo_cours);
    algo->addEnseignement(algo_td);
    
    // Ajout des UE aux semestres
    s1->addUE(bdr, 45);
    s1->addUE(poo, 45);
    s2->addUE(algo, 42);
    
    // Création des interventions
    Intervention* int1 = new Intervention(conchon, bdr, 20.0f, 18.0f, 24.0f);
    Intervention* int2 = new Intervention(maria, poo, 25.0f, 0.0f, 20.0f);
    Intervention* int3 = new Intervention(dupont, algo, 0.0f, 16.0f, 22.0f);
    
    
    // Affichage des résultats
    cout << "\n" << *deptInfo << endl;
    
    cout << "\n--- Détail des UE ---" << endl;
    for (const auto& ue : deptInfo->getUEs()) {
        cout << *ue << endl;
    }
    
    cout << "\n--- Coûts horaires ---" << endl;
    cout << "Semestre 1: " << s1->getCoutHoraires() << "h" << endl;
    cout << "Semestre 2: " << s2->getCoutHoraires() << "h" << endl;
    cout << "Total diplôme: " << master->getCoutTotal() << "h" << endl;
    
    cout << "\n--- Taux d'encadrement ---" << endl;
    cout << "Département: " << fixed << setprecision(2) 
         << deptInfo->getTauxEncadrement() * 100 << "%" << endl;
    
    cout << "\n--- Charge de travail des enseignants ---" << endl;
    for (const auto& ens : deptInfo->getEnseignants()) {
        float taux = (ens->getETDMax() > 0) ? (ens->getETDRealise() / ens->getETDMax()) * 100 : 0;
        cout << ens->toString() << endl;
        cout << "  ETD: " << ens->getETDRealise() << "h / " << ens->getETDMax() 
             << "h (" << fixed << setprecision(1) << taux << "%)" << endl;
    }
    
    delete master;
    delete deptInfo;
}

int main() {
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════════╗" << endl;
    cout << "║   TESTS COMPLETS DU SYSTÈME DE GESTION UNIVERSITAIRE       ║" << endl;
    cout << "╚════════════════════════════════════════════════════════════╝" << endl;
    
    try {
        testDepartement();
        testEnseignants();
        testUEetEnseignements();
        testInterventions();
        testDiplomeEtSemestre();
        testTauxEncadrement();
        testComplet();
        
        afficherSeparateur("TOUS LES TESTS SONT TERMINÉS AVEC SUCCÈS");
        cout << "\n✓ Tous les tests ont été exécutés sans erreur!\n" << endl;
        
    } catch (const exception& e) {
        cerr << "\n❌ ERREUR: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}