# Projet C

Ce projet est réalisé dans le cadre du cours de Programmation Avancée Impérative de l'ENSTA Bretagne.
Ce projet est réalisé en binôme, les auteurs de ce projet sont Corentin Goetghebeur et Chris Arridi.

## Objectif
Ce projet a pour objectif la mise en place de structures et fonctions permettant la simulation d'un système bancaire.
Pour plus de détails sur ce projet, consulter Projet_C_2020_2021.pdf.

## Téléchargement / Installation / Utilisation
Ce projet est disponible sur GitHub à l'adresse: https://github.com/CorentinGoet/C_Project.git. L'exécutable compilé **main** devrait déjà être utilisable dans l'état ou vous l'avez téléchargé.

### Compilation
Si vous effectuez des modifications sur le projet, vous pouvez compiler le projet en utilisant le makefile.
```bash
make
```
En exécutant cette commande, vous produirez deux exécutables, **main** et **test**.

### Utilisation
Une fois le projet compilé, vous pouvez l'exécuter avec **main**.
```bash
./main
```

Le deuxième exécutable généré par la compilation est un pseudo test unitaire permettant de vérifier que les fonctions se comportent comme prévu.

<p align=center>ATTENTION !<p>

L'exécution de ce test créée des fichiers de comptes et les ajoute au registre (voir paragraphe structure).
```bash
./test
```

## Structure
Le dossier du projet est organisé de la manière suivante:

