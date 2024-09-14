# Projet : Simulation de banque en ligne
![Description du GIF](demo.gif)
## Description du programme
L'application de banque en ligne permet aux utilisateurs de réaliser des opérations bancaires courantes telles que :
- Connexion à leur compte
- Vérification du solde
- Paiement de factures

Les données des comptes et des transactions sont stockées dans des fichiers, et un chiffrement de César est utilisé pour sécuriser ces données.

---

## Organisation du code
Le programme est structuré en trois parties :
1. **main_bank.c** : contient les directives principales du programme.
2. **compte.c** : un module contenant les fonctions liées à la gestion des comptes bancaires, telles que `generer_numero_carte_credit` ou `generer_IBAN`.
3. **transaction.c** : un module dédié aux transactions, avec des fonctions comme `transferer_argent` et `payer_facture`.

---

## Fonctionnalités principales

### 1. Création de comptes
Les utilisateurs peuvent créer des comptes bancaires :
- **Automatiquement** : en utilisant des données générées aléatoirement via la fonction `generer_fichier_comptes`, qui crée plusieurs comptes et génère l'identité bancaire complète.
- **Manuellement** : en saisissant leurs propres informations personnelles.

Les comptes générés sont stockés dans un fichier chiffré à l'aide du chiffrement de César. La fonction `afficher_compte` permet d'afficher les comptes en les déchiffrant depuis le fichier `comptes_chiffre.txt`.

### 2. Sauvegarde des comptes
La fonction `sauvegarder_comptes` enregistre toutes les modifications apportées aux comptes (modification d'informations, virements, achats). Les fichiers `comptes.txt` et `comptes_chiffre.txt` sont mis à jour avec les nouvelles données.

### 3. Ouverture de compte
La fonction `ouvrir_compte` permet aux utilisateurs de créer un nouveau compte en saisissant des informations personnelles (email, mot de passe, type de compte, etc.). Les utilisateurs peuvent également ajouter une carte bancaire :
- Choix entre carte de crédit ou carte de débit
- Carte générée manuellement ou automatiquement

La fonction `generer_carte` vérifie le type de carte choisi et génère, si besoin, une carte d'un autre type (par exemple, une carte de débit si une carte de crédit a été choisie).

### 4. Gestion des transactions
Les utilisateurs peuvent réaliser différentes transactions via les fonctions `payer_facture` et `effectuer_transaction_carte_credit`. L'historique des transactions est accessible avec plusieurs options de tri :
- Par type de transaction : factures, virements bancaires.
- Par période : recherche des transactions selon une date de début et de fin.
- Par catégorie : les transactions par carte sont classées par type (alimentation, loisirs, transports, shopping...).

---

## Fonctionnalités manquantes
Certaines fonctionnalités n'ont pas été implémentées, notamment :
- **Système de sécurité** : authentification à deux facteurs et vérification de sécurité pour les opérations sensibles.
- **Ajout d'acteurs supplémentaires** : comme un administrateur pour gérer les comptes ou un caissier pour réaliser des transactions pour les clients.

---

## Remarques d'utilisation
Pour exécuter ce programme, il est nécessaire de modifier les variables `chemin_fichier_chiffre` et `chemin_fichier` dans le fichier `compte.c` en y incluant le chemin absolu vers les fichiers `.txt` sur votre machine. Utilisez la commande Unix suivante pour obtenir ces chemins :
'realpath comptes_chiffre.txt'
'realpath compte.txt'

**Note** : Cette commande ne fonctionne que sur les systèmes Unix, Linux et Mac OS.

---

## Conclusion
Cette application de banque en ligne permet d'effectuer des opérations bancaires courantes avec une gestion sécurisée des données via le chiffrement de César. Cependant, des fonctionnalités de sécurité avancées et la gestion d'acteurs supplémentaires restent à implémenter pour améliorer la fiabilité et l'expérience utilisateur.