# Projet - Star

## Description

L’objectif de ce projet est de réaliser *dans une proportion simplifiée* l’implémentation d’un
générateur/extracteur d’archive .tar similaire à la commande éponyme tar. Ce dernier doit pouvoir manipuler une
archive de type .tar respectant la norme GNU POSIX 1003.1-1988 dite « ustar ».

## Fonctions métier

- **Lister les fichiers d'une archive**
- **Extraire l'intégralité d'une archive**
- **Générer une archive**

> Optionnelles
> - Prise en charge de la compression d'archives (librairie zlib)
> - Prise en charge de la décompression d'une archive tar.gz (gzip)
> - Réalisation d'une interface en mode console (tui) via librairie ncurses

Les paramètres disponibles devront être de la sorte :
```bash
-l, --list		ARCHIVE_FILE
-e, --extract	ARCHIVE_FILE
-c, --create	ARCHIVE_FILE
-d, --directory DIRECTORY_TO_PROCESS
-z, --compress
-v, --verbose	: enable *verbose* mode
-h, --help		: display this help
```

## Exigences techniques

- Compilation via un Makefile
- Définition des structures dans un fichier typedef.h
- Séparer les prototypes des fonctions dans un fichier *header*.h
- Le code produit doit être documenté grâce à doxygen
- Récupération des paramètres via la librairie getopt
- Gestion des erreurs via errno
- Le code est soumis à un contrôle de couverture via gcov
- Une page de manuel Linux doit être générée pour détailler l'exécution du binaire

## A propos

Ce projet a été réalisé dans le cadre du module Système d'exploitation avancé & virtualisation dispensée par le CNAM.

> Auteurs : Florian SPINDLER et Arthur VILLARD