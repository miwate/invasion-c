Ce __readme.md__ permet de vous expliquer comment utiliser le projet. Bonne lecture !


Avant de se lancer dans l'utilisation du jeu, vous aurez besoin d'avoir un compilateur C et des dépendances (ici 2 librairies).

### Compilateur et Dépendances


- GCC (le compilateur C nécessaire)

- SDL2 et SDL2_image (les 2 libraires nécessaires pour faire fonctionner le projet) 


Et enfin le dossier contenant le code source organisé en sous dossiers (qui devrait déjà ^etre inclus dans le ZIP d'installation).


Afin d'installer le compilateur GCC sur les ordinateurs munis de Debian (dont Ubuntu), exécutez dans le terminal la commande suivante : **sudo apt-get install build-essential** .

*(on pourrait utiliser seulement **sudo apt-get install gcc** pour n'installer que GCC. Mais pour utiliser le __Makefile__, nous avons aussi besoin d'entrer une commande **sudo apt-get install make** , nous pouvons passer par cette seule commande pour avoir tout ce dont on a besoin).*


Vous trouverez les commandes à exécuter dans le terminal pour installer les bibliothèques `SDL2` et `SDL2_image` nécessaires pour exécuter le projet.

- Utilisez la commande suivante : **apt-get install libsdl2-** , pour les interactions entre utilisateur et machine

- Ainsi que : **sudo apt-get install libsdl2-image-dev** , pour les images


---


## Commandes (toutes à utiliser **dans le terminal**)


1. Pour compiler le projet et générer l'exécutable, utilisez la commande suivante dans le terminal : **make** .


2. Pour pouvoir lancer le jeu et accéder à un niveau, il faut utilser la commande suivante dans le terminal :

**make <niveau>** en remplaçant **<niveau>** avec un niveau disponible, à savoir : facile, meatshield, ... (les niveaux étant trouvables dans le sous-dossier __levels__).


3. Après avoir fermé le jeu, vous devez nettoyer le terminal en utilisant la commande dans le terminal : **make cleanAll**

Après cette commande, vous pourrez alors relancer le jeu dans un autre niveau en repartant de l'étape 1 !


Ceci conclut le readme.md, veuillez vous rediriger vers le fichier __dev.pdf__ expliquant nos choix pour le programme !
