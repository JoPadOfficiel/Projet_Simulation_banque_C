//
//  main.c
//  Projet_Simulation_banque
//
//  Created by JoPad on 11/04/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compte.h"
#include "transaction.h"

#define MAX_COMPTES 1000
extern int nombre_comptes;

void effectuer_paiement_carte_credit(Compte *compte, double montant);

void afficher_menu(void) {
    printf("Menu :\n");
    printf("\t 1. Vérifier le solde d'un compte\n");
    printf("\t 2. Calculer les intérêts d'un compte d'épargne\n");
    printf("\t 3. Transférer de l'argent entre comptes\n");
    printf("\t 4. Payer une facture\n");
    printf("\t 5. Effectuer un paiement par carte de crédit\n");
    printf("\t 6. Afficher l'historique des transactions\n");
    printf("\t 7. Rechercher des transactions\n");
    printf("\t 8. Afficher le relevé d'un compte\n");
    printf("\t 9. Modifier un compte\n");
    printf("\t10. Fermer un compte\n");
    printf("\t 0. Quitter\n");
}

// Fonction pour la connexion à un compte sans chiffrement ni déchiffrement
Compte* connexion(Compte *comptes, int nombre_comptes) {
    char email[100];
    char mot_de_passe[100];
    Compte *compte = NULL;

    printf("Entrez votre e-mail : ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = 0; // Supprime le caractère de nouvelle ligne

    for (int i = 0; i < nombre_comptes; i++) {
        if (strcmp(comptes[i].email, email) == 0) {
            printf("Entrez votre mot de passe : ");
            fgets(mot_de_passe, sizeof(mot_de_passe), stdin);
            mot_de_passe[strcspn(mot_de_passe, "\n")] = 0; // Supprime le caractère de nouvelle ligne

            if (strcmp(comptes[i].mot_de_passe, mot_de_passe) == 0) { // Remplacer "mot_de_passe_connexion" par "mot_de_passe"
                compte = &comptes[i];
                break;
            } else {
                printf("Mot de passe incorrect.\n");
                return NULL;
            }
        }
    }

    if (compte == NULL) {
        printf("Compte non trouvé.\n");
        return NULL;
    }

    return compte;
}

int main(void) {
    // Debogage - test chiffrement et dechiffrement de César
    test_cesar();

    initialiser_transactions();
    
    char choixComptes_txt;
    int saisie_valide;

    // Demande si je souhaite charger des nouvelles données dans le fichier "comptes.txt". (Oui ou Non)
    do {
        printf("Voulez-vous écraser les anciennes données du fichier comptes.txt par de nouvelles données aléatoires? (O/N): ");
        scanf(" %c", &choixComptes_txt);
        getchar(); // Pour avaler le caractère de nouvelle ligne

        if (choixComptes_txt == 'o' || choixComptes_txt == 'O') {
            initialiser_comptes();
            generer_fichier_comptes();
            saisie_valide = 1;
        } else if (choixComptes_txt == 'n' || choixComptes_txt == 'N') {
            saisie_valide = 1;
        } else {
            printf("Réponse invalide. Veuillez répondre par 'O' ou 'N'.\n");
            saisie_valide = 0;
        }
    } while (!saisie_valide);

    charger_comptes();
    charger_historique_transactions();
    
    // Menu de connexion utilisateur
    int choix_connexion;
    Compte *compte_connecte = NULL;
    do {
        printf("\n\t1. Se connecter\n");
        printf("\t2. Créer un nouveau compte\n");
        printf("\tChoisissez une option (1-2) : ");
        scanf("%d", &choix_connexion);
        getchar(); // Pour avaler le caractère de nouvelle ligne

        if (choix_connexion == 1) {
            compte_connecte = connexion(comptes, nombre_comptes);
            if (compte_connecte == NULL) {
                printf("Identifiants incorrects. Veuillez réessayer.\n");
            }
        } else if (choix_connexion == 2) {
            ouvrir_compte();
            printf("Compte ajouté !\n");
        } else {
            printf("Option invalide. Veuillez réessayer.\n");
        }
    } while (!compte_connecte);

    int choix = 0;

    do {
        afficher_menu();
        printf("Choisissez une option : ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                verifier_solde(compte_connecte);
                break;
            case 2:
                if (strcmp(compte_connecte->type_compte, "epargne") == 0) {
                    calculer_interet(compte_connecte);
                } else {
                    printf("Compte d'épargne non trouvé.\n");
                }
                break;
            case 3:
                {
                    int compte_destination;
                    double montant;
                    printf("Entrez le numéro du compte destination : ");
                    scanf("%d", &compte_destination);
                    printf("Entrez le montant à transférer : ");
                    scanf("%lf", &montant);
                    Compte *destination = trouver_compte(compte_destination);
                    if (destination != NULL) {
                        transferer_argent(compte_connecte, destination, montant);
                    } else {
                        printf("Le compte destination est introuvable.\n");
                    }
                }
                break;
            case 4:
                {
                    double montant;
                    printf("Entrez le montant de la facture : ");
                    scanf("%lf", &montant);
                    payer_facture(compte_connecte, montant);
                }
                break;
            case 5:
                effectuer_transaction_carte_credit(compte_connecte);
                break;
            case 6:
                afficher_historique_transactions(compte_connecte);
                break;
            case 7: {
            /*
             Dans ce case j'ai rajouté des accolades ({}) pour créer une portée locale pour la variable "choixTransaction".
             Mon compilateur refusait la déclaration après un case.
             */
                       int choixTransaction;
                       printf("Veuillez choisir un critère de recherche:\n");
                       printf("1. Recherche par type (Facture ou Transfert)\n");
                       printf("2. Recherche par catégorie (Alimentation, Transports, Loisirs, Shopping, Autre)\n");
                       printf("3. Recherche par date\n");
                       scanf("%d", &choixTransaction);

                       switch (choixTransaction) {
                           case 1: {
                               char type[32];
                               printf("Entrez le type de transaction (Facture ou Transfert) : ");
                               scanf("%s", type);
                               rechercher_transactions_par_type(compte_connecte, type);
                               break;
                           }
                           case 2: {
                               char categorie[32];
                               printf("Entrez la catégorie de transaction (Alimentation, Transports, Loisirs, Shopping, Autre) : ");
                               scanf("%s", categorie);
                               rechercher_transactions_par_categorie(compte_connecte, categorie);
                               break;
                           }
                           case 3: {
                               char date_debut[11];
                               char date_fin[11];
                               printf("Entrez la date de début (JJ-MM-AAAA) : ");
                               scanf("%s", date_debut);
                               printf("Entrez la date de fin (JJ-MM-AAAA) : ");
                               scanf("%s", date_fin);
                               rechercher_transactions_par_date(compte_connecte, date_debut, date_fin);
                               break;
                           }
                           default:
                               printf("Choix invalide.\n");
                               break;
                       }
                       break;
                   }
            case 8:
                afficher_releve(compte_connecte);
                break;
            case 9:
                modifier_compte(compte_connecte);
                break;
            case 10:
                {
                    char confirmation;
                    printf("Êtes-vous sûr de vouloir fermer le compte de manière définitive ? Cette action est irréversible (O/N) : ");
                    scanf(" %c", &confirmation);
                    getchar();

                    if (confirmation == 'o' || confirmation == 'O') {
                        fermer_compte(compte_connecte);
                    } else {
                        printf("Suppression du compte annulée.\n");

                    }
                }
                break;
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Option invalide.\n");
                
                // Nettoyer le tampon d'entrée (buffer), pour éviter de créer une boucle infini
                while ((getchar()) != '\n');
        }
    } while (choix != 0);

    sauvegarder_comptes();
    return 0;
}
