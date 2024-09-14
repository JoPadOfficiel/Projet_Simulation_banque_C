//
//  transaction.h
//  Projet_Simulation_banque_DUTEUIL_GERMANIC
//
//  Created by Jocelin DUTEUIL on 11/04/2023.
//

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>
#include "compte.h"

#define MAX_TRANSACTIONS 1000 // Ajout d'une limite de transactions

// Structure transaction
typedef struct {
    int id_transaction;
    int numero_compte_source;
    int numero_compte_destination;
    double montant;
    char type_transaction[32];
    char date[11]; // format JJ-MM-AAAA
} Transaction;

void charger_historique_transactions(void);

// Initialise le tableau des transactions
void initialiser_transactions(void);
void ajouter_transaction(Transaction transaction);

void verifier_solde(Compte* compte);
void transferer_argent(Compte* compte_source, Compte* compte_destination, double montant);
void payer_facture(Compte* compte, double montant);


void ajouter_transaction_par_details(int numero_compte_source, int numero_compte_destination, double montant);
void effectuer_paiement_carte_credit(Compte *compte, double montant);
void effectuer_transaction_carte_credit(Compte *compte);

void afficher_historique_transactions(Compte* compte);

void rechercher_transactions_par_type(Compte* compte, const char* type);
void rechercher_transactions_par_categorie(Compte* compte, const char* categorie);
void rechercher_transactions_par_date(Compte* compte, const char* date_debut, const char* date_fin);
    
void sauvegarder_historique_transactions(void);

#endif // TRANSACTION_H

