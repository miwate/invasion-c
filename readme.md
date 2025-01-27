# INVASION !!
Projet de C

## Git (notes pour nous)
Avant de push, assure-toi de faire
``
git status
``
pour voir ta branche et les modifications que t'as apportées

Ensuite
``
git add .
``
ensuite, encore
``
git status
``
ensuite, pour ajouter tes modifications que tu pourras commit
``
git commit -a -m "description de tes modifs"
``
ensuite, encore
``
git status
``
et finalement
``
git push
``

## Dépendances
Installation des dépendances

```bash
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
```

## Compilation
### Compilation du projet

```bash
make
```
- Exemple : lancer le niveau expert
```bash
make expert
```
### Nettoyage
- Standard
```bash
make clean
```
- Complet
```bash
make cleanAll
```
