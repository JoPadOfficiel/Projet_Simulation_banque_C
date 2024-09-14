//
//  compte.c
//  Projet_Simulation_banque_DUTEUIL_GERMANIC
//
//  Created by Jocelin DUTEUIL on 11/04/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <time.h>
#include <string.h>

#include <stdint.h>
#include <inttypes.h>

#include "compte.h"
#include "transaction.h"

Compte comptes[MAX_COMPTES];
int nombre_comptes = 0;

const char caracteres_speciaux[] = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
const int Key_cesar = 3; // Clé de chiffrement pour le chiffrement César

// Fonction pour chiffrer les données sensibles à l'aide du chiffrement de César
void chiffrer_cesar(char *chaine, int decalage) {
    int longueur = strlen(chaine);
    int longueur_caracteres_speciaux = strlen(caracteres_speciaux);
    
    for (int i = 0; i < longueur; ++i) {
        char c = chaine[i];

        if (c >= 'a' && c <= 'z') {
            chaine[i] = (c - 'a' + decalage) % 26 + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            chaine[i] = (c - 'A' + decalage) % 26 + 'A';
        } else if (c >= '0' && c <= '9') {
            chaine[i] = (c - '0' + decalage) % 10 + '0';
        } else {
            for (int j = 0; j < longueur_caracteres_speciaux; ++j) {
                if (c == caracteres_speciaux[j]) {
                    chaine[i] = caracteres_speciaux[(j + decalage) % longueur_caracteres_speciaux];
                    break;
                }
            }
        }
    }
}

// Fonction pour déchiffrer les données sensibles à l'aide du chiffrement de César
void dechiffrer_cesar(char *chaine, int decalage) {
    int longueur = strlen(chaine);
    int longueur_caracteres_speciaux = strlen(caracteres_speciaux);

    for (int i = 0; i < longueur; ++i) {
        char c = chaine[i];

        if (c >= 'a' && c <= 'z') {
            chaine[i] = (c - 'a' + 26 - decalage % 26) % 26 + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            chaine[i] = (c - 'A' + 26 - decalage % 26) % 26 + 'A';
        } else if (c >= '0' && c <= '9') {
            chaine[i] = (c - '0' + 10 - decalage % 10) % 10 + '0';
        } else {
            for (int j = 0; j < longueur_caracteres_speciaux; ++j) {
                if (c == caracteres_speciaux[j]) {
                    chaine[i] = caracteres_speciaux[(j + longueur_caracteres_speciaux - decalage) % longueur_caracteres_speciaux];
                    break;
                }
            }
        }
    }
}


//Fonction juste pour tester si le chiffrement et déchiffrement fonctionne correctement
void test_cesar(void) {
    char original[] = "1-2-3-4-5-6-7-8-9";
    char chiffre[sizeof(original)];
    char dechiffre[sizeof(original)];

    strcpy(chiffre, original);
    chiffrer_cesar(chiffre, Key_cesar);
    printf("Original: %s\n", original);
    printf("chiffre: %s\n", chiffre);

    strcpy(dechiffre, chiffre);
    dechiffrer_cesar(dechiffre, Key_cesar);
    printf("dechiffre: %s\n", dechiffre);

    if (strcmp(original, dechiffre) == 0) {
        printf("Test réussi.\n");
    } else {
        printf("Test échoué.\n");
    }
}

void generer_fichier_comptes(void) {
    printf("Génération du fichier comptes.txt...\n");
    const char *chemin_fichier_chiffre = "./Projet_Simulation_banque_/comptes_chiffre.txt";
    const char *chemin_fichier = "./Projet_Simulation_banque_/comptes.txt";
    int nombre_comptes_a_generer = 0;
    
    // Demander le nombre de comptes à générer à l'utilisateur
    printf("Combien de comptes voulez-vous générer ? ");
    scanf("%d", &nombre_comptes_a_generer);

    // Ouvrir les fichiers en mode écriture
    FILE *file_chiffre = fopen(chemin_fichier_chiffre, "w");
    FILE *file = fopen(chemin_fichier, "w");

    if (file_chiffre == NULL || file == NULL) {
        printf("Impossible d'ouvrir les fichiers pour la sauvegarde.\n");
        return;
    }

    // Initialiser le générateur de nombres aléatoires avec le temps actuel
    srand(time(NULL));

    // Générer et écrire les comptes aléatoires dans les fichiers
    for (int i = 0; i < nombre_comptes_a_generer; i++) {
        Compte compte;
        
        // Générer des informations utilisateur aléatoires
        compte.numero_compte = rand() % 1000000;
        if (rand() % 2 == 0) {
            strcpy(compte.type_compte, "epargne");
        } else {
            strcpy(compte.type_compte, "courant");
        }
        compte.solde = rand() % 10000;
        generer_IBAN(compte.iban);

        // Générer le numéro de carte de crédit si nécessaire
        const char *type_carte = rand() % 4 == 0 ? "debit" : (rand() % 4 == 1 ? "credit" : "both");
        if (strcmp(type_carte, "credit") == 0 || strcmp(type_carte, "both") == 0) {
            generer_numero_carte_credit(compte.carte_credit);
        } else {
            strcpy(compte.carte_credit, "none");
        }
    
        // Générer le numéro de carte de débit si nécessaire
        if (strcmp(type_carte, "debit") == 0 || strcmp(type_carte, "both") == 0) {
            generer_numero_carte_debit(compte.carte_debit);
        } else {
            strcpy(compte.carte_debit, "none");
        }

        // Générer les informations de l'identité des utilisateurs
        generer_prenom(compte.prenom);
        generer_nom_utilisateur(compte.nom_utilisateur);
        generer_email(compte.email);
        generer_mot_de_passe(compte.mot_de_passe);

        // Chiffrer toutes les données
        char mot_de_passe_chiffre[100], prenom_chiffre[50], nom_utilisateur_chiffre[50], email_chiffre[100], iban_chiffre[100];
        char type_compte_chiffre[20], type_carte_chiffre[20], carte_debit_chiffre[20], carte_credit_chiffre[20];

        strcpy(mot_de_passe_chiffre, compte.mot_de_passe);
        strcpy(prenom_chiffre, compte.prenom);
        strcpy(nom_utilisateur_chiffre, compte.nom_utilisateur);
        strcpy(email_chiffre, compte.email);
        strcpy(iban_chiffre, compte.iban);
        strcpy(type_compte_chiffre, compte.type_compte);
        strcpy(type_carte_chiffre, type_carte);
        strcpy(carte_debit_chiffre, compte.carte_debit);
        strcpy(carte_credit_chiffre, compte.carte_credit);

        chiffrer_cesar(mot_de_passe_chiffre, Key_cesar);
        chiffrer_cesar(prenom_chiffre, Key_cesar);
        chiffrer_cesar(nom_utilisateur_chiffre, Key_cesar);
        chiffrer_cesar(email_chiffre, Key_cesar);
        chiffrer_cesar(iban_chiffre, Key_cesar);
        chiffrer_cesar(type_compte_chiffre, Key_cesar);
        chiffrer_cesar(type_carte_chiffre, Key_cesar);
        chiffrer_cesar(carte_debit_chiffre, Key_cesar);
        chiffrer_cesar(carte_credit_chiffre, Key_cesar);

        // Écrire les informations chiffrees dans le fichier comptes_chiffre.txt
        fprintf(file_chiffre, "%d %s %.2lf %s %s %s %s %s %s %s %s\n", compte.numero_compte, type_compte_chiffre, compte.solde, iban_chiffre, type_carte_chiffre, carte_debit_chiffre, carte_credit_chiffre, prenom_chiffre, nom_utilisateur_chiffre, email_chiffre, mot_de_passe_chiffre);

        // Écrire les informations non chiffrees dans le fichier comptes.txt
        fprintf(file, "%d %s %.2lf %s %s %s %s %s %s %s %s\n", compte.numero_compte, compte.type_compte, compte.solde, compte.iban, type_carte, compte.carte_debit, compte.carte_credit, compte.prenom, compte.nom_utilisateur, compte.email, compte.mot_de_passe);
    }

    // Fermer les fichiers et informer l'utilisateur
    fclose(file_chiffre);
    fclose(file);
    printf("Fichiers comptes.txt et comptes_chiffre.txt générés avec succès.\n");
}


void generer_numero_carte_credit(char *carte_credit) {
    snprintf(carte_credit, 17, "%016" PRIu64, ((uint64_t)rand() << 32) | rand());
}

void generer_numero_carte_debit(char *carte_debit) {
    snprintf(carte_debit, 17, "%016" PRIu64, ((uint64_t)rand() << 32) | rand());
}

void generer_prenom(char *prenom) {
    // Liste de prénoms aléatoires
    const char *prenoms[] = {"Emma", "Gabriel", "Jade", "Raphaël", "Louise", "Léo", "Alice", "Louis", "Chloé", "Hugo",
                             "Lina", "Adam", "Mila", "Lucas", "Ambre", "Maël", "Inès", "Arthur", "Léa", "Jules",
                             "Léna", "Gabin", "Lou", "Nathan", "Eva"};

    strcpy(prenom, prenoms[rand() % 25]);
}


void generer_nom_utilisateur(char *nom_utilisateur) {
    sprintf(nom_utilisateur, "user%d", rand() % 10000);
}

void generer_email(char *email) {
    sprintf(email, "email%d@example.com", rand() % 10000);
}

void generer_mot_de_passe(char *mot_de_passe) {
    const char caract_mdp[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=<>?";
    int long_mdp = 8 + rand() % 5; // Génère un mot de passe de longueur entre 8 et 12 caractères

    for (int i = 0; i < long_mdp; i++) {
        mot_de_passe[i] = caract_mdp[rand() % (sizeof(caract_mdp) - 1)];
    }
    mot_de_passe[long_mdp] = '\0'; // Ajoute le caractère de fin de chaîne
}

void generer_IBAN(char *iban) {
    const char *pays = "FR";
    const char *cle_controle = "76";
    int code_banque = rand() % 10000;
    int code_guichet = rand() % 10000;
    long long int numero_compte = rand() % 10000000000LL;
    int cle_rib = rand() % 100;

    snprintf(iban, 28, "%s%s%04d%04d%02d%010lld%02d", pays, cle_controle, code_banque, code_guichet, cle_rib, numero_compte, cle_rib);
}


void initialiser_comptes(void) {
    nombre_comptes = 0;
}

void ajouter_compte(Compte compte) {
    comptes[nombre_comptes] = compte;
    nombre_comptes++;
}

void charger_comptes(void) {
    initialiser_comptes();
    const char *chemin_fichier = "./Projet_Simulation_banque_/comptes_chiffre.txt";

    FILE *file = fopen(chemin_fichier, "r");
    if (file == NULL) {
        printf("Aucun fichier de comptes existant trouvé.\n");
        return;
    }

    Compte compte_temp;
    // Boucle jusqu'à ce qu'il n'y ait plus de comptes à lire dans le fichier
    while (fscanf(file, "%d %19s %lf %49s %19s %33s %33s %99s %99s %199s %199s\n",
                  &compte_temp.numero_compte, compte_temp.type_compte, &compte_temp.solde, compte_temp.iban, compte_temp.type_carte, compte_temp.carte_debit, compte_temp.carte_credit,
                  compte_temp.prenom, compte_temp.nom_utilisateur, compte_temp.email, compte_temp.mot_de_passe) == 11)
    {
        // Pour debug
        printf("Avant déchiffrement - Numero: %d, Email: %s, Mot de passe: %s\n", compte_temp.numero_compte, compte_temp.email, compte_temp.mot_de_passe);

        // Supprimez les caractères de fin de ligne de l'e-mail et du mot de passe
        compte_temp.email[strcspn(compte_temp.email, "\n\r")] = '\0';
        compte_temp.mot_de_passe[strcspn(compte_temp.mot_de_passe, "\n\r")] = '\0';

        // Déchiffrement des données
        dechiffrer_cesar(compte_temp.type_compte, Key_cesar);
        dechiffrer_cesar(compte_temp.iban, Key_cesar);
        dechiffrer_cesar(compte_temp.type_carte, Key_cesar);
        dechiffrer_cesar(compte_temp.prenom, Key_cesar);
        dechiffrer_cesar(compte_temp.nom_utilisateur, Key_cesar);
        dechiffrer_cesar(compte_temp.email, Key_cesar);
        dechiffrer_cesar(compte_temp.mot_de_passe, Key_cesar);
        dechiffrer_cesar(compte_temp.carte_credit, Key_cesar);
        dechiffrer_cesar(compte_temp.carte_debit, Key_cesar);

        // Pour debug
        printf("Après déchiffrement - Numero: %d, Email: %s, Mot de passe: %s\n", compte_temp.numero_compte, compte_temp.email, compte_temp.mot_de_passe);

        // Ajoute le compte temporaire à la liste des comptes
        ajouter_compte(compte_temp);
    }

    // Ferme le fichier et affiche un message de succès
    fclose(file);
    printf("Les comptes ont été chargés avec succès.\n");
}


void sauvegarder_comptes(void) {
    // Chemin relatif
    //const char *chemin_fichier_chiffre = "./Projet_Simulation_banque_/comptes_chiffre.txt";
    //const char *chemin_fichier = "./Projet_Simulation_banque_/comptes.txt";

    // Chemin absolu
    const char *chemin_fichier_chiffre = "./Projet_Simulation_banque_/comptes_chiffre.txt";
    const char *chemin_fichier = "./Projet_Simulation_banque_/comptes.txt";
    
    FILE *file_chiffre = fopen(chemin_fichier_chiffre, "w");
    FILE *file = fopen(chemin_fichier, "w");

    if (file_chiffre == NULL || file == NULL) {
        printf("Impossible d'ouvrir les fichiers pour la sauvegarde.\n");
        return;
    }

    for (int i = 0; i < nombre_comptes; i++) {
        Compte compte = comptes[i];

        //Pour debogage
        printf("Avant chiffrement - Numero: %d, Email: %s, Mot de passe: %s\n", compte.numero_compte, compte.email, compte.mot_de_passe);

        // Copie des données pour chiffrement
        char type_compte_chiffre[20];
        char iban_chiffre[50];
        char type_carte_chiffre[20];
        char prenom_chiffre[100];
        char nom_utilisateur_chiffre[100];
        char email_chiffre[200];
        char mot_de_passe_chiffre[200];
        char carte_credit_chiffre[34];
        char carte_debit_chiffre[34];

        strcpy(type_compte_chiffre, compte.type_compte);
        strcpy(iban_chiffre, compte.iban);
        strcpy(type_carte_chiffre, compte.type_carte);
        strcpy(prenom_chiffre, compte.prenom);
        strcpy(nom_utilisateur_chiffre, compte.nom_utilisateur);
        strcpy(email_chiffre, compte.email);
        strcpy(mot_de_passe_chiffre, compte.mot_de_passe);
        strcpy(carte_credit_chiffre, compte.carte_credit);
        strcpy(carte_debit_chiffre, compte.carte_debit);

        // Chiffrement des données
        chiffrer_cesar(type_compte_chiffre, Key_cesar);
        chiffrer_cesar(iban_chiffre, Key_cesar);
        chiffrer_cesar(type_carte_chiffre, Key_cesar);
        chiffrer_cesar(prenom_chiffre, Key_cesar);
        chiffrer_cesar(nom_utilisateur_chiffre, Key_cesar);
        chiffrer_cesar(email_chiffre, Key_cesar);
        chiffrer_cesar(mot_de_passe_chiffre, Key_cesar);
        chiffrer_cesar(carte_credit_chiffre, Key_cesar);
        chiffrer_cesar(carte_debit_chiffre, Key_cesar);

        //Pour debogage
        printf("Après chiffrement - Numero: %d, Email: %s, Mot de passe: %s\n", compte.numero_compte, email_chiffre, mot_de_passe_chiffre);

        // Écrit les données chiffrees dans le fichier chiffre
        fprintf(file_chiffre, "%d %s %.2lf %s %s %s %s %s %s %s %s\n", compte.numero_compte, type_compte_chiffre, compte.solde, iban_chiffre, type_carte_chiffre, carte_debit_chiffre, carte_credit_chiffre, prenom_chiffre, nom_utilisateur_chiffre, email_chiffre, mot_de_passe_chiffre);

        // Écrit les données non chiffrees dans le fichier non chiffre
        fprintf(file, "%d %s %.2lf %s %s %s %s %s %s %s %s\n", compte.numero_compte, compte.type_compte, compte.solde, compte.iban, compte.type_carte, compte.carte_debit, compte.carte_credit, compte.prenom, compte.nom_utilisateur, compte.email, compte.mot_de_passe);
    }
    fclose(file_chiffre);
    fclose(file);
    printf("Les comptes ont été sauvegardés avec succès.\n");
    
}

Compte* trouver_compte(int numero_compte) {
    for (int i = 0; i < nombre_comptes; i++) {
        if (comptes[i].numero_compte == numero_compte) {
            return &comptes[i];
        }
    }
    return NULL;
}

//Permet de génerer numéro carte si utilisateur le demande lros de la création compte
void generer_carte(Compte *compte, const char *type_carte) {
    if (strcmp(type_carte, "credit") == 0) {
        generer_numero_carte_credit(compte->carte_credit);
    } else if (strcmp(type_carte, "debit") == 0) {
        generer_numero_carte_debit(compte->carte_debit);
    } else {
        printf("Type de carte invalide.\n");
    }
}

void ouvrir_compte(void) {
    Compte nouveau_compte;
    char choix_carte[10];
    char deuxieme_choix_carte[10];
    char choix_generation[10];
    char mot_de_passe[20];
    
    printf("Entrez le numéro du compte : ");
    scanf("%d", &nouveau_compte.numero_compte);
    getchar(); // Pour avaler le caractère de nouvelle ligne
    printf("Entrez le nom d'utilisateur : ");
    fgets(nouveau_compte.nom_utilisateur, sizeof(nouveau_compte.nom_utilisateur), stdin);
    nouveau_compte.nom_utilisateur[strcspn(nouveau_compte.nom_utilisateur, "\n")] = 0;
    printf("Entrez le prénom : ");
    fgets(nouveau_compte.prenom, sizeof(nouveau_compte.prenom), stdin);
    nouveau_compte.prenom[strcspn(nouveau_compte.prenom, "\n")] = 0;
    printf("Entrez l'email : ");
    fgets(nouveau_compte.email, sizeof(nouveau_compte.email), stdin);
    nouveau_compte.email[strcspn(nouveau_compte.email, "\n")] = 0;
    printf("Entrez le mot de passe : ");
    fgets(mot_de_passe, sizeof(mot_de_passe), stdin);
    mot_de_passe[strcspn(mot_de_passe, "\n")] = 0;
    printf("Entrez le type de compte (courant / epargne): ");
    fgets(nouveau_compte.type_compte, sizeof(nouveau_compte.type_compte), stdin);
    nouveau_compte.type_compte[strcspn(nouveau_compte.type_compte, "\n")] = 0;
    printf("Entrez le solde initial du compte : ");
    scanf("%lf", &nouveau_compte.solde);
    getchar();
    
    // Initialise le champ type_carte avec une chaîne vide
    strcpy(nouveau_compte.type_carte, "");
    
    // Utilise le mot de passe saisi par l'utilisateur
    strcpy(nouveau_compte.mot_de_passe, mot_de_passe);
    
    // Génère un IBAN aléatoire pour le nouveau compte
    generer_IBAN(nouveau_compte.iban);
    ajouter_compte(nouveau_compte);
    mettre_a_jour_compte(&nouveau_compte);
    
    printf("Souhaitez-vous associer une carte de crédit ou de débit? (credit/debit/none): ");
    fgets(choix_carte, sizeof(choix_carte), stdin);
    choix_carte[strcspn(choix_carte, "\n")] = 0;

    if (strcmp(choix_carte, "credit") == 0 || strcmp(choix_carte, "debit") == 0) {
        printf("Souhaitez-vous saisir le numéro de la carte manuellement ou le générer aléatoirement? (manuel/auto): ");
        fgets(choix_generation, sizeof(choix_generation), stdin);
        choix_generation[strcspn(choix_generation, "\n")] = 0;

        if (strcmp(choix_generation, "manuel") == 0) {
            do {
                associer_carte(&nouveau_compte, choix_carte);
            } while (strlen(nouveau_compte.carte_debit) != 16 && strlen(nouveau_compte.carte_credit) != 16);
        } else if (strcmp(choix_generation, "auto") == 0) {
            generer_carte(&nouveau_compte, choix_carte);
        } else {
            printf("Choix invalide. Aucune carte n'a été associée.\n");
            return;
        }

        strcpy(nouveau_compte.type_carte, choix_carte);
        mettre_a_jour_compte(&nouveau_compte);

        // Demande à l'utilisateur s'il souhaite associer une deuxième carte
        if (strcmp(choix_carte, "credit") == 0) {
            printf("Souhaitez-vous associer une carte de débit? (debit/none): ");
            fgets(deuxieme_choix_carte, sizeof(deuxieme_choix_carte), stdin);
            deuxieme_choix_carte[strcspn(deuxieme_choix_carte, "\n")] = 0;
        } else {
            printf("Souhaitez-vous associer une carte de crédit? (credit/none): ");
            fgets(deuxieme_choix_carte, sizeof(deuxieme_choix_carte), stdin);
            deuxieme_choix_carte[strcspn(deuxieme_choix_carte, "\n")] = 0;
        }

        if (strcmp(deuxieme_choix_carte, "credit") == 0 || strcmp(deuxieme_choix_carte, "debit") == 0) {
            printf("Souhaitez-vous saisir le numéro de la deuxième carte manuellement ou le générer aléatoirement? (manuel/auto): ");
            fgets(choix_generation, sizeof(choix_generation), stdin);
            choix_generation[strcspn(choix_generation, "\n")] = 0;

            if (strcmp(choix_generation, "manuel") == 0) {
                do {
                    associer_carte(&nouveau_compte, deuxieme_choix_carte);
                } while (strlen(nouveau_compte.carte_debit) != 16 && strlen(nouveau_compte.carte_credit) != 16);
            } else if (strcmp(choix_generation, "auto") == 0) {
                generer_carte(&nouveau_compte, deuxieme_choix_carte);
            } else {
                printf("Choix invalide. Aucune deuxième carte n'a été associée.\n");
                return;
            }

            strcpy(nouveau_compte.type_carte, "both");
            mettre_a_jour_compte(&nouveau_compte);
        } else if (strcmp(deuxieme_choix_carte, "none") != 0) {
            printf("Choix invalide. Aucune deuxième carte n'a été associée.\n");
        }
    } else if (strcmp(choix_carte, "none") != 0) {
        printf("Choix invalide. Aucune carte n'a été associée.\n");
    }
    // Sauvegarde les comptes après la création d'un nouveau compte
    sauvegarder_comptes();
}

void mettre_a_jour_compte(Compte* compte) {
    int index = -1;

    // Trouver l'index du compte dans le tableau des comptes
    for (int i = 0; i < nombre_comptes; i++) {
        if (comptes[i].numero_compte == compte->numero_compte) {
            index = i;
            break;
        }
    }

    // Vérifier si le numéro de compte est déjà utilisé
    if (index != -1) {
        comptes[index] = *compte;
    } else {
        printf("Erreur: le numéro de compte n'existe pas.\n");
    }
}

// Fermer un compte en le supprimant de la liste des comptes
void fermer_compte(Compte* compte) {
    int index = -1;

    // Trouver l'index du compte dans le tableau des comptes
    for (int i = 0; i < nombre_comptes; i++) {
        if (comptes[i].numero_compte == compte->numero_compte) {
            index = i;
            break;
        }
    }

    // Supprimer le compte du tableau en décalant les éléments à droite
    if (index != -1) {
        for (int i = index; i < nombre_comptes - 1; i++) {
            comptes[i] = comptes[i + 1];
        }
        nombre_comptes--;
        printf("Compte %d fermé avec succès.\n", compte->numero_compte);
    } else {
        printf("Compte introuvable.\n");
    }
}

// Modifie les informations d'un compte
void modifier_compte(Compte* compte) {
    int choix;
    do {
        printf("\nMenu de modification du compte:\n");
        printf("\t1. Modifier le numéro de compte\n");
        printf("\t2. Modifier le nom d'utilisateur\n");
        printf("\t3. Modifier le prénom\n");
        printf("\t4. Modifier l'email\n");
        printf("\t5. Modifier le mot de passe\n");
        printf("\t6. Modifier le type de compte (courant / epargne)\n");
        printf("\t7. Modifier le solde du compte\n");
        printf("\t8. Quitter le menu de modification\n");
        printf("\tChoisissez une option: ");
        scanf("%d", &choix);
        getchar(); // Pour avaler le caractère de nouvelle ligne

        switch (choix) {
            case 1:
                printf("Entrez le nouveau numéro de compte : ");
                scanf("%d", &compte->numero_compte);
                getchar();
                printf("Modification effectuée.\n");
                sauvegarder_comptes();
                break;
            case 2:
                printf("Entrez le nouveau nom d'utilisateur : ");
                fgets(compte->nom_utilisateur, sizeof(compte->nom_utilisateur), stdin);
                compte->nom_utilisateur[strcspn(compte->nom_utilisateur, "\n")] = 0;
                printf("Modification effectuée.\n");
                sauvegarder_comptes();
                break;
            case 3:
                printf("Entrez le nouveau prénom : ");
                fgets(compte->prenom, sizeof(compte->prenom), stdin);
                compte->prenom[strcspn(compte->prenom, "\n")] = 0;
                printf("Modification effectuée.\n");
                sauvegarder_comptes();
                break;
            case 4:
                printf("Entrez le nouvel email : ");
                fgets(compte->email, sizeof(compte->email), stdin);
                compte->email[strcspn(compte->email, "\n")] = 0;
                printf("Modification effectuée.\n");
                sauvegarder_comptes();
                break;
            case 5:
                printf("Entrez le nouveau mot de passe : ");
                fgets(compte->mot_de_passe, sizeof(compte->mot_de_passe), stdin);
                compte->mot_de_passe[strcspn(compte->mot_de_passe, "\n")] = 0;
                printf("Modification effectuée.\n");
                sauvegarder_comptes();
                break;
            case 6:
                printf("Entrez le nouveau type de compte (courant / epargne) : ");
                fgets(compte->type_compte, sizeof(compte->type_compte), stdin);
                compte->type_compte[strcspn(compte->type_compte, "\n")] = 0;
                printf("Modification effectuée.\n");
                sauvegarder_comptes();
                break;
            case 7:
                printf("Entrez le nouveau solde du compte : ");
                scanf("%lf", &compte->solde);
                getchar();
                printf("Modification effectuée.\n");
                sauvegarder_comptes();
                break;
            case 8:
                printf("Sauvegarde en cours...\n");
                mettre_a_jour_compte(compte);
                printf("Sauvegarde effectuée. Retour au menu principal.\n");
                break;
            default:
                printf("Option invalide, veuillez choisir une option valide.\n");
                break;
        }
    } while (choix != 8);
}


void associer_carte(Compte* compte, const char* type_carte) {
    char temp_carte[20];

    if (strcmp(type_carte, "credit") == 0) {
        printf("Entrez le numéro de la carte de crédit (16 chiffres) : ");
        scanf("%s", temp_carte);

        if (strlen(temp_carte) == 16) {
            strcpy(compte->carte_credit, temp_carte);
            printf("Carte de crédit associée avec succès.\n");
        } else {
            printf("Erreur : la carte de crédit doit contenir 16 chiffres.\n");
        }
    } else if (strcmp(type_carte, "debit") == 0) {
        printf("Entrez le numéro de la carte de débit (16 chiffres) : ");
        scanf("%s", temp_carte);

        if (strlen(temp_carte) == 16) {
            strcpy(compte->carte_debit, temp_carte);
            strcpy(compte->type_carte, "debit");
            printf("Carte de débit associée avec succès.\n");
        } else {
            printf("Erreur : la carte de débit doit contenir 16 chiffres.\n");
        }
    } else {
        printf("Type de carte invalide. Aucune carte n'a été associée.\n");
    }
}

void afficher_releve(Compte* compte) {
    // Afficher les informations générales du compte
    printf("\tEmail: %s\n", compte->email);
    printf("\tNom utilisateur: %s\n", compte->nom_utilisateur);
    printf("\tPrénom: %s\n", compte->prenom);
    printf("\tNuméro de compte: %d\n", compte->numero_compte);
    printf("\tIBAN: %s\n", compte->iban);
    printf("\tType de compte: %s\n", compte->type_compte);
    printf("\tSolde: %.2lf\n", compte->solde);

    // Vérifiez si les numéros de carte sont "none"
    int carte_credit_none = strcmp(compte->carte_credit, "none") == 0;
    int carte_debit_none = strcmp(compte->carte_debit, "none") == 0;

    // Afficher les numéros de carte s'ils ne sont pas "none"
    if (!carte_credit_none) {
        printf("\tNuméro de carte de crédit: %s\n", compte->carte_credit);
    }

    if (!carte_debit_none) {
        printf("\tNuméro de carte de débit: %s\n", compte->carte_debit);
    }

    // Afficher un message indiquant qu'aucune carte n'est associée si les deux cartes sont "none"
    if (carte_credit_none && carte_debit_none) {
        printf("\tAucune carte associée\n");
    }
}


// Calcule les intérêts d'un compte d'épargne et ajoute les intérêts au solde
void calculer_interet(Compte* compte) {
    double taux_interet = 0.02; // Taux d'intérêt annuel, par exemple 2%

    if (strcmp(compte->type_compte, "epargne") == 0) {
        double interet = compte->solde * taux_interet;
        compte->solde += interet;
        printf("Intérêts calculés pour le compte %d: %.2lf\n", compte->numero_compte, interet);
    } else {
        printf("Le calcul des intérêts n'est applicable qu'aux comptes d'épargne.\n");
    }
}
