#  ImageLibrary-Qt-OpenCV

##  Contexte Académique
Projet réalisé dans le cadre du **Master 2 SIA2 – Signal, Image et Apprentissage Automatique**  
Université Toulouse III – Paul Sabatier

##  Objectif du Projet
Développer une application **C++ modulaire** permettant :
- La gestion d’une bibliothèque d’images
- Le contrôle d’accès par niveau utilisateur
- L’application d’algorithmes classiques de traitement d’image
- Une interaction via interface graphique Qt

##  Architecture Logicielle
Architecture modulaire avec séparation claire :
- Interface graphique (Qt)
- Backend métier (ImageLibrary)
- Gestion des utilisateurs
- Module traitement d’image (OpenCV)

##  Schéma de l’architecture
<p align="center"> <img src="software_architecture.png" width="400"> </p>

### Objectifs de l’architecture
- Modularité
- Évolutivité
- Maintenance simplifiée

##  Traitements Implémentés
- Affichage de l’image originale
- Filtrage convolution 3×3
- Détection de contours (gradient)
- Histogramme des niveaux de gris

##  Composants Principaux

###  ImageDescriptor
Représente les métadonnées d’une image :
- Numéro
- Titre
- Chemin source
- Mot-clé
- Coût
- Niveau d’accès

Cette classe constitue l’entité de base de la bibliothèque.

###  ImageLibrary
Cœur du système.

Fonctionnalités :
- Stockage via liste chaînée personnalisée
- Ajout / suppression d’image
- Filtrage par coût
- Filtrage par mot-clé
- Vérification des droits d’accès
- Sauvegarde / chargement depuis un fichier

Cette classe centralise toute la logique métier.

###  User
Représente un utilisateur authentifié.

Niveaux :
- NONE
- LEVEL1 (Consultation)
- LEVEL2 (Administrateur)

Gestion :
- Validation des permissions
- Contrôle des rôles

###  Module d’Authentification
Valide :
- Format du code utilisateur (8 caractères)
- Attribution du niveau d’accès

##  Interface Graphique (Qt)

### Diagramme UML
Architecture générale de l’application :
<p align="center"> <img src="architecture_general_de_app.png" width="600"> </p>

Diagramme UML généré avec PlantUML :
<p align="center"> <img src="diagramme.png" width="700"> </p>

##  Système d’Authentification
Trois niveaux d’accès :

| Niveau | Rôle            | Droits                  |
|-------:|-----------------|-------------------------|
| 0      | Aucun accès     | Accès refusé            |
| 1      | Utilisateur     | Consultation uniquement |
| 2      | Administrateur  | Accès complet           |

##  Illustration des niveaux

### Niveau 0 – Aucun accès
<p align="center"> <img src="level_0.png" width="250"> </p>

### Niveau 1 – Consultation
<p align="center"> <img src="level_1.phg.png" width="250"> </p>

### Niveau 2 – Administrateur
<p align="center"> <img src="level_2.png" width="250"> </p>

##  Intégration des interfaces selon les droits utilisateurs
Après authentification, l’interface s’adapte automatiquement au niveau de l’utilisateur.

##  Interface Consultation – Niveau 1
L’interface « Bibliothèque – Consultation » permet :
- Consultation des images
- Affichage des informations
- Aucun accès aux fonctions de gestion

<p align="center"> <img src="consultation_level_1.png" width="400"> </p>

##  Interface Administrateur – Niveau 2
L’interface « Bibliothèque – Admin » permet :
- Consultation
- Gestion de la bibliothèque
- Application des traitements

<p align="center"> <img src="consultation_level_1.png" width="400"> </p>

##  Gestion de la Bibliothèque d’Images

### Mode Administrateur
- Affichage complet des images
- Fiches techniques (numéro, titre, source, mot-clé, coût)
- Ajout de nouvelles images

<p align="center"> <img src="administrateur.png" width="400"> </p>

### Mode Utilisateur Standard
- Consultation uniquement
- Pas d’ajout ni de modification

<p align="center"> <img src="utilisateur.png" width="400"> </p>

##  Fonctionnalités Avancées

### Gestion Niveau 2
- Ajout d’image
- Suppression
- Sauvegarde / chargement

<p align="center"> <img src="gestion_level_2.png" width="400"> </p>

### Filtrage par Coût – Niveau 1
- Images gratuites
- Intervalle de prix
- Affichage complet

<p align="center"> <img src="filtrage_par_cout.png" width="400"> </p>

##  Interface Administrateur – Gestion & Affichage
Ajout / suppression / modification des images via formulaire dédié.  
Affichage complet d’une image sélectionnée.

<p align="center"> <img src="level_2_gestion.png" width="400"> </p>
<p align="center"> <img src="level_2_affichage.png" width="400"> </p>

##  Interface Consultation – Utilisateur Standard
Affichage des images accessibles selon les critères définis.

<p align="center"> <img src="level_1_standard.png" width="400"> </p>

##  Compilation (Linux)
- Ouvrir le fichier `.pro` dans Qt Creator
- Configurer le kit (Desktop Qt)
- Cliquer sur **Build**
- Exécuter

##  Améliorations Futures
- Intégration d’une base de données (SQLite)
- Recherche intelligente d’images
- Segmentation avancée
- Modernisation de l’interface
- Optimisation des performances
- Intégration de tests unitaires
