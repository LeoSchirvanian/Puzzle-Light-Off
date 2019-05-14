//Include
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void LightsOff(){
    
    //Initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);

    //Renvoie les erreurs si ils y en a
    if (SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL");
        exit(EXIT_FAILURE);
    }

    //Initialisation
    SDL_Surface *ecran = NULL;
    SDL_Event event;

    int jeu = 1;
    int init = 1;

    int matrice[6][6];
    cleanMatrice(6, matrice);

    int mat[36][36];
    cleanMatrice(36,mat);
    initMat(mat);

    int copie[36][36];

    //Légende de la fenêtre
    SDL_WM_SetCaption("Puzzle LightsOff", NULL);

    //Chargement des images
    SDL_Surface *Un = NULL;
    SDL_Surface *Zero = NULL;
    Un = SDL_LoadBMP("un.bmp");
    Zero = SDL_LoadBMP("zero.bmp");

    SDL_WM_SetIcon(Un, NULL);                                              //Définition icône de l'application
    ecran = SDL_SetVideoMode(390,390, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);  //Définition de la taille de l'écran

    //Remplissage écran de noir
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    //Initialisation des positions
    SDL_Rect position;  //Pour placer les cases sur l'écran
    position.x = 0;
    position.y = 0;
    SDL_Rect clic;      //Reprend position du clic de l'utilisateur
    clic.x = 0;
    clic.y = 0;

    reset(ecran, position, 6, matrice,Un,Zero); //Initialise les cases du Jeu sur l'écran

    SDL_WarpMouse(ecran->w / 2, ecran->h / 2);  //Place la souris au milieu de l'écran


    /// ****************************  FIRST LOOP : INITIALISATION **************************** ///
    int i = 0;
    for(i; i<9; i++){

        matrice[i%3][i/3] = 1;                      //On remplit à tour de role les cases de la premiere matrice 3x3
        reset(ecran, position, 6, matrice,Un,Zero);

        while (init){

            //A chacune des neufs cases, on attend la réponse de l'utilisateur
            SDL_WaitEvent(&event);
            switch(event.type){

                case SDL_QUIT:

                    //On quitte les trois boucles
                    i = 9;
                    init = 0;
                    jeu = 0;
                    break;

                case SDL_KEYDOWN :

                    if(event.key.keysym.sym == SDLK_SPACE){         //Détecte la touche Espace

                        cleanMatrice(6,matrice);                    //Clean la matrice
                        reset(ecran, position, 6, matrice,Un,Zero); //Reset l'écran
                        init = 0;                                   //On sort du deuxième while

                        //Test du rang de la matrice
                        //La matrice mat est de base de rang 36, donc si elle n'est pas inversible cela signifie que son rang a baissé
                        copieMat(36, mat, copie);    //On fait une copie de la matrice d'association mat
                        int inv = Gauss(36, copie);  //On trigonalise la copie de mat : 1 si inv, 0 sinon

                        if(inv == 0){
                            cleanMatrice(36, copie); //On clean la copie de mat
                            repair(36, mat, i);      //On réinitialise l'association incorrect rentrée par l'utilisateur
                            i--;                     //On reste au même niveau dans la boucle, l'utilisateur doit réessayer
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:

                    //On récupère poistion du clic de l'utilisateur
                    clic.x = event.motion.x;
                    clic.y = event.motion.y;

                    //On convertit en int
                    int x = clic.x;
                    int y = clic.y;

                    //On cherche son multiple de 65 pour savoir à quel case le clic correspond (65 à cause de la taille de la fenêtre et de la taille 			    des images "un.bmp" et "zero.bmp" de taille 64x64, 390 = 65*6, on laisse un pixel d'espace pour avoir la délimitation des cases)
		    int k = x / 65;
                    int l = y / 65;

                    int t = i / 3; //Partie entière inférieure de la division de i par 3

                    //On controle qu'on ne clique pas sur la case à initialiser
                    if( !(k == i%3 && l == i/3) ){

                        changeMatrice(6, k, l, matrice);         //On change la couleur de la case cliquée
                        changeMatrice(36, k+l*6, t*6+i%3, mat);  //On associe cette case à la case de la matrice 3x3 dont c'est le tour
                    }

                    reset(ecran, position, 6, matrice,Un,Zero);
            }
        }
        init = 1; //Permet de rerentrer dans la deuxième boucle à la prochaine itération de la première
    }

    //On applique la rotation
    rotation(6,mat);
    
    //On crée un fichier texte avec la matrice associations (pour pouvoir récupérer les associations du jeu crées par l'utilisateur)
    fichierMatrice(36, mat);

    /// ****************************  SECOND LOOP : PLAY **************************** ////
    while (jeu){

        //On attend l'évènement
        SDL_WaitEvent(&event);

        switch(event.type){

            case SDL_QUIT:

                jeu = 0;
                break;

            case SDL_MOUSEBUTTONUP:

                //On récupère poistion du clic de l'utilisateur
                clic.x = event.motion.x;
                clic.y = event.motion.y;

                //On convertit en int
                int x = clic.x;
                int y = clic.y;

                //On cherche son multiple de 65 pour savoir à quel case le clic correspond
                int k = x / 65;
                int l = y / 65;

                //Lis la matrice 36x36 et associe la case cliquée à ses associations définies à l'initialisation et les change de couleur
                action(k,l, matrice, mat);

                reset(ecran, position, 6, matrice,Un,Zero);
        }
    }

    // Libération de la surface
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(Zero);
    SDL_FreeSurface(Un);
    SDL_Quit();

    return EXIT_SUCCESS;
}

//Rafraîchissement de l'écran
void reset(SDL_Surface *ecran, SDL_Rect position, int n, int matrice[n][n],SDL_Surface *Un, SDL_Surface *Zero){
    for (int j =0; j<n; j++){
        for(int i =0; i<n; i++){

            int w = 390 / n;
            int h = 390 / n;

            //Parcours de l'écran
            position.x = i*w;
            position.y = j*h;

            if(matrice[i][j] == 0){
                SDL_BlitSurface(Zero, NULL, ecran, &position);
            }
            else{
                SDL_BlitSurface(Un, NULL, ecran, &position);
            }

            SDL_Flip(ecran); // Mise à jour de l'écran
        }
    }
}
