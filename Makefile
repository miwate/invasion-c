# Nom de l'exécutable
TARGET = jeu

# Répertoires
SRC_DIR = src
PROT_DIR = prot

# Fichiers source et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Options de compilation
CC = gcc
CFLAGS = -I$(PROT_DIR) -Wall
LDFLAGS = 

# Règle par défaut
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJS)
\t$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compilation des fichiers .c en .o
%.o: %.c
\t$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
\trm -f $(SRC_DIR)/*.o $(TARGET)

# Réinitialisation complète
mrproper: clean
\trm -f $(TARGET)
