TARGET = jeu

SRC_DIR = src
PROT_DIR = prot
BUILD_DIR = build
LEVELS_DIR = levels

# Fichiers source et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Compilation
CC = gcc
CFLAGS = -I$(PROT_DIR) -Wall
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET) $(LDFLAGS)

# Compilation (encore)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

# Réinitialisation complète
cleanAll: clean
	rm -f $(BUILD_DIR)/$(TARGET)

# Exécution de la démo
demo : 
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/demo.txt

# Exécution avec un fichier spécifique
facile:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/facile.txt

normal:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/normal.txt

difficile:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/difficile.txt


expert:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/expert.txt

impossible:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/impossible.txt

colossal:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/colossal.txt

meatshield:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/meatshield.txt

survie:
	./$(BUILD_DIR)/$(TARGET) $(LEVELS_DIR)/survie.txt
