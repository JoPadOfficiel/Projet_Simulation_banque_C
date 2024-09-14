//
//  transaction.c
//  Projet_Simulation_banque_DUTEUIL_GERMANIC
//
//  Created by Jocelin DUTEUIL on 11/04/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>
#include <time.h>

#include "transaction.h"
#include "compte.h"

Transaction transactions[MAX_TRANSACTIONS];
int nombre_transactions = 0;

// Initialise le tableau des transactions
void initialiser_transactions(void) {
    nombre_transactions = 1;
}

void ajouter_transaction(Transaction transaction) {
    transactions[nombre_transactions] = transaction;
    nombre_transactions++;
}

void verifier_solde(Compte* compte) {
    printf("Le solde du compte %d est de: %.2f\n", compte->numero_compte, compte->solde);
}

void transferer_argent(Compte* compte_source, Compte* compte_destination, double montant) {
    if (compte_source->solde >= montant) {
        compte_source->solde -= montant;
        compte_destination->solde += montant;
        printf("Transfert de %.2f du compte %d vers le compte %d effectué avec succès.\n",
                montant, compte_source->numero_compte, compte_destination->numero_compte);

        ajouter_transaction_par_details(compte_source->numero_compte, compte_destination->numero_compte, montant);

        // Copie la chaîne "Transfert" dans le champ type_transaction de la dernière transaction ajoutée,
        // en s'assurant de ne pas dépasser la taille maximale du champ en soustrayant 1 à la taille.
        strncpy(transactions[nombre_transactions - 1].type_transaction, "Transfert", sizeof(transactions[nombre_transactions - 1].type_transaction) - 1);

        // Ajout d'un caractère NULL à la fin de la chaîne
        transactions[nombre_transactions - 1].type_transaction[sizeof(transactions[nombre_transactions - 1].type_transaction) - 1] = '\0';
        
        // Sauvegarde l'historique des transactions dans le fichier .txt
        sauvegarder_historique_transactions();
        
    } else {
        printf("Solde insuffisant pour effectuer le transfert.\n");
    }
}

void payer_facture(Compte* compte, double montant) {
    if (compte->solde >= montant) {
        compte->solde -= montant;
        printf("Facture de %.2f payée avec succès depuis le compte %d.\n", montant, compte->numero_compte);
        
        ajouter_transaction_par_details(compte->numero_compte, 0, montant);
        
        // Copie la chaîne "Facture" dans le champ type_transaction de la dernière transaction ajoutée,
        // en s'assurant de ne pas dépasser la taille maximale du champ en soustrayant 1 à la taille.
        strncpy(transactions[nombre_transactions - 1].type_transaction, "Facture", sizeof(transactions[nombre_transactions - 1].type_transaction) - 1);

        // Ajout d'un caractère NULL à la fin de la chaîne
        transactions[nombre_transactions - 1].type_transaction[sizeof(transactions[nombre_transactions - 1].type_transaction) - 1] = '\0';
        
        // Sauvegarde l'historique des transactions dans le fichier .txt
        sauvegarder_historique_transactions();
    }else {
        printf("Solde insuffisant pour payer la facture.\n");
    }
}

void ajouter_transaction_par_details(int numero_compte_source, int numero_compte_destination, double montant) {
    Transaction transaction;
    transaction.id_transaction = nombre_transactions;
    transaction.numero_compte_source = numero_compte_source;
    transaction.numero_compte_destination = numero_compte_destination;
    transaction.montant = montant;

    // Ajoutez la date de la transaction au format JJ-MM-AAAA
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(transaction.date, sizeof(transaction.date), "%d-%m-%Y", tm_info);
    transactions[nombre_transactions] = transaction;
    nombre_transactions++;
}


// Cette fonction affiche l'historique des transactions pour un compte donné
void afficher_historique_transactions(Compte* compte) {
    // Afficher l'en-tête de l'historique des transactions
    printf("Historique des transactions pour le compte %d :\n", compte->numero_compte);

    // Afficher l'historique des paiements de factures
    printf("\nHistorique des paiements de factures :\n");
    for (int i = 0; i < nombre_transactions; i++) {
        // Vérifier si la transaction concerne le compte et si le type de transaction est "Facture"
        if (transactions[i].numero_compte_source == compte->numero_compte && strcmp(transactions[i].type_transaction, "Facture") == 0) {
            // Afficher les détails de la transaction
            printf("Transaction %d: Compte source: %d, Compte destination: %d, Montant: %.2lf, Type: %s, Date: %s\n",
                    transactions[i].id_transaction, transactions[i].numero_compte_source, transactions[i].numero_compte_destination, transactions[i].montant, transactions[i].type_transaction, transactions[i].date);
        }
    }

    // Afficher l'historique des transferts d'argent
    printf("\nHistorique des transferts d'argent :\n");
    for (int i = 0; i < nombre_transactions; i++) {
        // Vérifier si la transaction concerne le compte et si le type de transaction est "Transfert"
        if ((transactions[i].numero_compte_source == compte->numero_compte || transactions[i].numero_compte_destination == compte->numero_compte) && strcmp(transactions[i].type_transaction, "Transfert") == 0) {
            // Afficher les détails de la transaction
            printf("Transaction %d: Compte source: %d, Compte destination: %d, Montant: %.2lf, Type: %s, Date: %s\n",
                    transactions[i].id_transaction, transactions[i].numero_compte_source, transactions[i].numero_compte_destination, transactions[i].montant, transactions[i].type_transaction, transactions[i].date);
        }
    }

    // Afficher l'historique des paiements par carte de crédit
    printf("\nHistorique des paiements par carte de crédit :\n");
    for (int i = 0; i < nombre_transactions; i++) {
        // Vérifier si la transaction concerne le compte et si le type de transaction est l'une des catégories liées aux paiements par carte de crédit
        if (transactions[i].numero_compte_source == compte->numero_compte &&
            (strcmp(transactions[i].type_transaction, "Alimentation") == 0 ||
                strcmp(transactions[i].type_transaction, "Transports") == 0 ||
                strcmp(transactions[i].type_transaction, "Loisirs") == 0 ||
                strcmp(transactions[i].type_transaction, "Shopping") == 0 ||
                strcmp(transactions[i].type_transaction, "Autre") == 0)) {
            // Afficher les détails de la transaction
            printf("Transaction %d: Compte source: %d, Compte destination: %d, Montant: %.2lf, Type: %s, Date: %s\n",
                    transactions[i].id_transaction, transactions[i].numero_compte_source, transactions[i].numero_compte_destination, transactions[i].montant, transactions[i].type_transaction, transactions[i].date);
        }
    }
}

void effectuer_paiement_carte_credit(Compte *compte, double montant) {
    if (compte->solde >= montant) {
        compte->solde -= montant;
        printf("Paiement de %.2f effectué avec succès depuis le compte %d.\n", montant, compte->numero_compte);

        ajouter_transaction_par_details(compte->numero_compte, 0, montant); // Ajouter la transaction
    } else {
        printf("Solde insuffisant pour effectuer le paiement.\n");
    }
}

void effectuer_transaction_carte_credit(Compte *compte) {
    // Vérifier si le compte a une carte de crédit
    if (compte->carte_credit[0] == '\0') {
        printf("Erreur: Aucune carte de crédit associée à ce compte.\n");
        return;
    }

    printf("Veuillez choisir une catégorie de transaction :\n");
    printf("1. Alimentation\n");
    printf("2. Transports\n");
    printf("3. Loisirs\n");
    printf("4. Shopping\n");
    printf("5. Autre\n");

    int categorie;
    scanf("%d", &categorie);

    double montant;
    printf("Veuillez entrer le montant de la transaction: ");
    scanf("%lf", &montant);

    effectuer_paiement_carte_credit(compte, montant);

    // Ajouter la catégorie de transaction à la dernière transaction effectuée
    switch (categorie) {
        case 1:
            strncpy(transactions[nombre_transactions - 1].type_transaction, "Alimentation", sizeof(transactions[nombre_transactions - 1].type_transaction) - 1);
            sauvegarder_historique_transactions();            break;
        case 2:
            strncpy(transactions[nombre_transactions - 1].type_transaction, "Transports", sizeof(transactions[nombre_transactions - 1].type_transaction) - 1);
            sauvegarder_historique_transactions();            break;
        case 3:
            strncpy(transactions[nombre_transactions - 1].type_transaction, "Loisirs", sizeof(transactions[nombre_transactions - 1].type_transaction) - 1);
            sauvegarder_historique_transactions();            break;
        case 4:
            strncpy(transactions[nombre_transactions - 1].type_transaction, "Shopping", sizeof(transactions[nombre_transactions - 1].type_transaction) - 1);
            sauvegarder_historique_transactions();            break;
        case 5:
            strncpy(transactions[nombre_transactions - 1].type_transaction, "Autre", sizeof(transactions[nombre_transactions - 1].type_transaction) - 1);
            sauvegarder_historique_transactions();            break;
    }
    transactions[nombre_transactions - 1].type_transaction[sizeof(transactions[nombre_transactions - 1].type_transaction) - 1] = '\0'; // Ajout d'un caractère NULL à la fin de la chaîne
}


//*** Rechercher les transactions par différentes méthode ***

void rechercher_transactions_par_type(Compte* compte, const char* type) {
    printf("Historique des transactions de type '%s' pour le compte %d :\n", type, compte->numero_compte);
    
    for (int i = 0; i < nombre_transactions; i++) {
        if ((transactions[i].numero_compte_source == compte->numero_compte || transactions[i].numero_compte_destination == compte->numero_compte) && strcmp(transactions[i].type_transaction, type) == 0) {
            printf("Transaction %d: Compte source: %d, Compte destination: %d, Montant: %.2lf, Type: %s, Date: %s\n",
                    transactions[i].id_transaction, transactions[i].numero_compte_source, transactions[i].numero_compte_destination, transactions[i].montant, transactions[i].type_transaction, transactions[i].date);
        }
    }
}

void rechercher_transactions_par_categorie(Compte* compte, const char* categorie) {
    printf("Historique des transactions de catégorie '%s' pour le compte %d :\n", categorie, compte->numero_compte);

    for (int i = 0; i < nombre_transactions; i++) {
        if (transactions[i].numero_compte_source == compte->numero_compte && strcmp(transactions[i].type_transaction, categorie) == 0) {
            printf("Transaction %d: Compte source: %d, Compte destination: %d, Montant: %.2lf, Type: %s, Date: %s\n",
                    transactions[i].id_transaction, transactions[i].numero_compte_source, transactions[i].numero_compte_destination, transactions[i].montant, transactions[i].type_transaction, transactions[i].date);
        }
    }
}

void rechercher_transactions_par_date(Compte* compte, const char* date_debut, const char* date_fin) {
    printf("Historique des transactions pour le compte %d du %s au %s :\n", compte->numero_compte, date_debut, date_fin);

    for (int i = 0; i < nombre_transactions; i++) {
        if (transactions[i].numero_compte_source == compte->numero_compte || transactions[i].numero_compte_destination == compte->numero_compte) {
            if (strncmp(transactions[i].date, date_debut, 10) >= 0 && strncmp(transactions[i].date, date_fin, 10) <= 0) {
                printf("Transaction %d: Compte source: %d, Compte destination: %d, Montant: %.2lf, Type: %s, Date: %s\n",
                        transactions[i].id_transaction, transactions[i].numero_compte_source, transactions[i].numero_compte_destination, transactions[i].montant, transactions[i].type_transaction, transactions[i].date);
            }
        }
    }
}

void sauvegarder_historique_transactions(void) {
    const char *transactions_historiques = "./Projet_Simulation_banque_/transactions_historiques.txt";
    // Ouvrir les fichiers en mode écriture

    FILE *fichier = fopen(transactions_historiques, "w");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier 'transactions_historiques.txt'\n");
        return;
    }
    for (int i = 0; i < nombre_transactions; i++) {
        fprintf(fichier, "%d;%d;%d;%.2lf;%s;%s\n", transactions[i].id_transaction, transactions[i].numero_compte_source, transactions[i].numero_compte_destination, transactions[i].montant, transactions[i].type_transaction, transactions[i].date);
    }
    fclose(fichier);
    printf("Transaction(s) sauvegardé dans 'transactions_historiques.txt'\n");
}

void charger_historique_transactions(void) {
    const char *transactions_historiques = "./Projet_Simulation_banque_/transactions_historiques.txt";
    FILE *fichier = fopen(transactions_historiques, "r");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier 'transactions_historiques.txt'\n");
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        Transaction transaction;
        sscanf(ligne, "%d;%d;%d;%lf;%[^;];%s", &transaction.id_transaction, &transaction.numero_compte_source, &transaction.numero_compte_destination, &transaction.montant, transaction.type_transaction, transaction.date);
        ajouter_transaction(transaction);
    }
    fclose(fichier);
    printf("Historique des transactions chargé depuis 'transactions_historiques.txt'\n");
}
