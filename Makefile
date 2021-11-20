CC = gcc
CFLAGS = -ansi -pedantic -Wall
LDFLAGS = -lMLV -lm -lrt
EXEC = i-war
OBJ = Main.o Bonus.o Sound.o Partie.o Menu.o Fichier.o Player.o Parametre.o Slider.o Tutoriel.o Terrain.o Projectile.o Enemy.o Affichage.o 

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

Main.o: ./src/Main.c ./include/../include/Tutoriel.h ./include/../include/Affichage.h ./include/../include/Parametre.h ./include/../include/Tutoriel.h ./include/../include/Fichier.h ./include/../include/Partie.h ./include/../include/Sound.h ./include/../include/Menu.h 

Bonus.o: ./src/Bonus.c ./include/../include/Affichage.h ./include/../include/Bonus.h 

Sound.o: ./src/Sound.c ./include/../include/Projectile.h ./include/../include/Player.h ./include/../include/Sound.h ./include/../include/Enemy.h ./include/../include/Parametre.h ./include/../include/Terrain.h 

Partie.o: ./src/Partie.c ./include/../include/Partie.h ./include/../include/Menu.h ./include/../include/Parametre.h ./include/../include/Affichage.h ./include/../include/Sound.h 

Menu.o: ./src/Menu.c ./include/../include/Menu.h ./include/../include/Bonus.h ./include/../include/Slider.h ./include/../include/Tutoriel.h ./include/../include/Affichage.h ./include/../include/Affichage.h ./include/../include/Parametre.h ./include/../include/Fichier.h ./include/../include/Terrain.h ./include/../include/Partie.h 

Fichier.o: ./src/Fichier.c ./include/../include/Fichier.h ./include/../include/Player.h ./include/../include/Terrain.h ./include/../include/Parametre.h 

Player.o: ./src/Player.c ./include/../include/Player.h ./include/../include/Parametre.h ./include/../include/Bonus.h 

Parametre.o: ./src/Parametre.c ./include/../include/Projectile.h ./include/../include/Parametre.h ./include/../include/Player.h ./include/../include/Enemy.h ./include/../include/Bonus.h 

Slider.o: ./src/Slider.c ./include/../include/Slider.h 

Tutoriel.o: ./src/Tutoriel.c ./include/../include/Affichage.h ./include/../include/Tutoriel.h ./include/../include/Terrain.h ./include/../include/Player.h ./include/../include/Partie.h ./include/../include/Enemy.h ./include/../include/Menu.h ./include/../include/Partie.h 

Terrain.o: ./src/Terrain.c ./include/../include/Menu.h ./include/../include/Terrain.h ./include/../include/Fichier.h ./include/../include/Parametre.h ./include/../include/Player.h ./include/../include/Enemy.h ./include/../include/Projectile.h ./include/../include/Bonus.h 

Projectile.o: ./src/Projectile.c ./include/../include/Projectile.h ./include/../include/Parametre.h 

Enemy.o: ./src/Enemy.c ./include/../include/Affichage.h ./include/../include/Enemy.h ./include/../include/Parametre.h 

Affichage.o: ./src/Affichage.c ./include/../include/Affichage.h ./include/../include/Parametre.h ./include/../include/Terrain.h ./include/../include/Player.h ./include/../include/Sound.h 

%.o: ./src/%.c
	$(CC) -c $< $(CFLAGS)
	
clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)

install: $(EXEC)
	mkdir ./bin
	mv $(EXEC) ./bin
	make clean

uninstall: mrproper
	rm -f ./bin/*
	rm -df ./bin