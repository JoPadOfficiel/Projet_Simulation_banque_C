//
//  compte.h
//  Projet_Simulation_banque_DUTEUIL_GERMANIC
//
//  Created by Jocelin DUTEUIL on 11/04/2023.
//

#ifndef COMPTE_H
#define COMPTE_H

#include <stdio.h>

#define MAX_COMPTES 1000
extern int nombre_comptes;
extern int chiffrement_cesar; // Clé de chiffrement pour le chiffrement César

//Structure carte bancaire
typedef struct {
    int numero_carte;
    char date_expiration[8];
    int code_securite;
} Carte;

//Structure compte bancaire
typedef struct {
    //Gestion des utilisateurs
    char mot_de_passe[100];
    char mot_de_passe_connexion[100];
    char prenom[50];
    char nom_utilisateur[100];
    char email[100];
    
    //Gestion des données des comptes
    int numero_compte;
    char type_compte[50];
    double solde;
    char carte_credit[50];
    char carte_debit[50];
    char type_carte[100];
    char iban[36];
    
    char carte_debit_dechiffre[100];
    char carte_credit_dechiffre[100];
} Compte;

extern Compte comptes[MAX_COMPTES];

// Fonctions qui permetent de générer aléatoirement les informations fichier comptes.txt
void generer_prenom(char *prenom);
void generer_nom_utilisateur(char *nom_utilisateur);
void generer_email(char *email);
void generer_mot_de_passe(char *mot_de_passe);
void generer_IBAN(char *iban);
void generer_numero_carte_credit(char *carte_credit);
void generer_numero_carte_debit(char *carte_debit);

//Fonctions qui permettent de chiffrer et déchiffrer les données des comptes
void chiffrer_cesar(char *chaine, int decalage);
void dechiffrer_cesar(char *chaine, int decalage);
void test_cesar(void);

//Charge les comptes à partir d'un fichier texte
void generer_fichier_comptes(void);
void charger_comptes(void);

//Sauvegarde les comptes dans un fichier texte
void sauvegarder_comptes(void);

//Fonctions pour la gestion des comptes de l'utilisateur
void initialiser_comptes(void);
void ajouter_compte(Compte compte);
void generer_carte(Compte *compte, const char *type_carte);
void ouvrir_compte(void);
void modifier_compte(Compte* compte);
void fermer_compte(Compte* compte);

void mettre_a_jour_compte(Compte* compte);
Compte* trouver_compte(int numero_compte);
void associer_carte(Compte* compte, const char* type_carte);

void afficher_releve(Compte* compte);
void calculer_interet(Compte* compte);


#endif //COMPTE_H
