#ifndef TP2_H_INCLUDED
#define TP2_H_INCLUDED

//Include
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

//fonctions de tp2Tools.c
void changeMatrice(int n, int k, int l, int mat[n][n]);
void cleanMatrice(int n, int mat[n][n]);
void action(int k, int l, int matrice[6][6], int mat[36][36]);
void initMat(int mat[36][36]);
void rotation(int d, int mat[36][36]);
int Gauss(int n, int matrice[36][36]);
void copieMat(int n, int orig[n][n], int copie[n][n]);
void repair(int n, int mat[n][n], int i);
void fichierMatrice(int n, int mat[n][n]);

//fonctions de tp2SDL.c
void LightsOff();
void reset(SDL_Surface *ecran, SDL_Rect position, int n, int matrice[n][n],SDL_Surface *Un, SDL_Surface *Zero);

#endif // TP2_H_INCLUDED
