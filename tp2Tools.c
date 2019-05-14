#include <stdio.h>

//Méthode qui inverse valeur d'une case d'une matrice de dimension n
void changeMatrice(int n, int k, int l, int mat[n][n]){
    if (k<n && l<n){
        if(mat[k][l] == 0){
            mat[k][l] = 1;
        }
        else{
            mat[k][l] = 0;
        }
    }
}

//Réinitialise une matrice de dimension n
void cleanMatrice(int n, int mat[n][n]){
    for(int i =0; i<n; i++){
        for(int j = 0; j<n; j++){
            mat[i][j] = 0;
        }
    }
}

//Lis la matrice 36x36 et associe la case cliquée à ses associations définies à l'initialisation et les change de couleur
void action(int k, int l, int matrice[6][6], int mat[36][36]){
    for(int i =0; i<36; i++){
        if(mat[i][k+l*6] == 1){
            changeMatrice(6, i%6, i/6, matrice);
        }
    }
}

//Remplis une matrice 36x36 de 1 sur sa diagonale
void initMat(int mat[36][36]){
    for(int i = 0; i<36; i++){
        mat[i][i] = 1;
    }
}

//Fais une rotation par symétrie centrale d'une matrice 36x36 à partir de 9 cases déjà associées par l'utilisateur
void rotation(int d, int mat[36][36]){
    for(int i = 0; i<9; i++){

        //Cases de la matrice 3x3
        int t = i / 3;        //Partie intière inférieure de la division de i par 3
        int k = t*6 + i%3;    //Cases de la première matrice 3x3

        for(int j = 0; j<36; j++){
            //On parcourt les lignes de ces cases pour voir leurs associations
            if(mat[j][k] == 1 && j != k){

                int r = d - 1;        //centre de la matrice : = 5 pour dimension 6

                //COLONNE : Correspond au colonne à affilier
                //on récupère ligne et colonne
                int lk = k / 6;
                int ck = k % 6;

                //On calcule ligne et colonne de rotation
                int rlk = r - lk;
                int rck = r - ck;

                //Trois autres coordonnées de rotation
                int r1k = rlk * 6 + rck;
                int r2k = ck * 6 + rlk;
                int r3k = rck * 6 + lk;

                //LIGNE : Cases affiliées
                //on récupère ligne et colonne
                int lj = j / 6;
                int cj = j % 6;

                //on effectue la rotation
                int rlj = r - lj;
                int rcj = r - cj;

                int r1j = rlj * 6 + rcj;
                int r2j = cj * 6 + rlj;
                int r3j = rcj * 6 + lj;

                //ROTATION
                mat[r1j][r1k] = 1;
                mat[r2j][r2k] = 1;
                mat[r3j][r3k] = 1;
            }
        }
    }
}

//Méthode du pivot de Gauss
int Gauss(int n, int matrice[36][36]){

    int i,j,k,val;

    for (i=0;i<n;i++){

        if (matrice[i][i] == 0){

            for (j = i+1; j<n; j++){
                if (matrice[j][i] != 0){
                    break;
                }
            }

            if (j >= n){
                return 0;
            }

            for (k = 0; k<n; k++){
                val = matrice[i][k];
                matrice[i][k] = matrice[j][k];
                matrice[j][k] = val;
            }
        }

        for (j = i+1; j<n; j++){

            if (matrice[j][i] != 0){
                for (k = i; k<n; k++){
                    matrice[j][k] = (matrice[i][k] != matrice[j][k])?1:0;
                }
            }
        }

    }
    return 1;
}

//Copie une matrice de dimension n
void copieMat(int n, int orig[n][n], int copie[n][n]){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            copie[i][j] = orig[i][j];
        }
    }
}

//Annule l'association rentrée par l'utilisateur
void repair(int n, int mat[n][n], int i){

    int t = i / 3;
    int j = t*6 + i%3;

    for(int i = 0; i < n; i++){
        if(i == j){
            mat[i][j] = 1;
        }
        else{
            mat[i][j] = 0;
        }
    }
}

//Crée un fichier texte avec la matrice 36x36 des associations
void fichierMatrice(int n, int mat[n][n]){
	FILE* fichier = NULL;
	fichier = fopen("Matrice Associations.txt", "w");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			fprintf(fichier, "%d", mat[i][j]);  //on print une case de la matrice
			fprintf(fichier, "  ");
		}
	fprintf(fichier, "\n");                             //retour à la ligne pour afficher ligne par ligne
	}
	fclose(fichier);
}
