using namespace std;
#include<SDL.h> 
#include<SDL_ttf.h> 
#include<iostream>
#include "config_sdl.h"
#include <fstream>

const int TAILLE_MAX = 10;
const int y_premier_bambou = 570;

// Definition des bambou
struct Bambou {
    int croissance;
    int taille;
};

// Statistiques 

// Statistiques tailles max

int StatTaillleMax(Bambou tab[], int TailleChoisie) {
    int maximum = 0;
    for (int i = 0; i < TailleChoisie; i++) {
        if (tab[i].taille > maximum) {
            maximum = tab[i].taille;
        }
    }
    return maximum;
}

// Statistique de la taille moyenne de la bambouseraie
float TailleMoy(Bambou tab[], int taille) {
    float total = 0;
    for (int i = 0; i < taille; i++) {
        total += tab[i].taille;
    }
    return total / taille;
}

// Statistique de la taille minimum de la bambouseraie
int TailleMin(Bambou tab[], int taille) {
    int min = tab[0].taille;
    for (int i = 0; i < taille; i++) {
        if (tab[i].taille < min) {
            min = tab[i].taille;
        }
    }
    return min;
}

// (rand() % 9) + 1)

// Création d'un bambou
void CreateBambou(Bambou& bambou, int taille) {
    bambou.croissance = taille;
    bambou.taille = taille;
}

// Initialisation de bambouraie
void InitBamboueraie(Bambou bambouraie[], int taille, int tab[]) {
	for (int i = 0; i < taille; i++) {
		CreateBambou(bambouraie[i],tab[i]);
	}
}

// Croissance d'un bambou
int GrowBambou(Bambou& bambou) {
    bambou.taille += bambou.croissance;
    return bambou.taille;
}

// Coupe d'un bambou
void CutBambou(Bambou& bambou) {
    bambou.taille = bambou.croissance;
}

// Reperage du bambou le plus grand
int VerifMax(Bambou Bambou[], int TailleChoisie) {
    int TailleMax = 0;
    int indiceBambou = 0;

    for (int i = 0; i < TailleChoisie; i++) {
        if (Bambou[i].taille > TailleMax) {
            TailleMax = Bambou[i].taille;
            indiceBambou = i;
        }
    }
    return indiceBambou;
}

// Coupe le bambou le plus grand
int ReduceMax(Bambou bambou[], int TailleChoisie) {
    int taille;
    int indice = VerifMax(bambou, TailleChoisie);
    bambou[indice].taille = bambou[indice].croissance;
    return indice;
}

// Fonction Reduce-Fastest(x)
void ReduceFaster(int seuil, Bambou tab[], int TailleChoisie) {
    int maxCroissance = 0;
    for (int j = 0; j < TailleChoisie; j++) {
        if (tab[j].taille > seuil) {
            if (tab[j].croissance > maxCroissance) {
                maxCroissance = tab[j].croissance;
            }
        }
    }
    CutBambou(tab[maxCroissance]);
}

// Croissance simultané de la bambouraie
void GrowAll(Bambou bambouraie[], int taille) {
    for (int i = 0; i < taille; i++) {
        GrowBambou(bambouraie[i]);
    }
}

// --------------------------------------------------Fonctions SDL----------------------------------------------------------- //

// Fond
void fond(SDL_Renderer* rendu) {
    SDL_Rect rect1;
    rect1.x = 0;
    rect1.y = 0;
    rect1.w = 1080;
    rect1.h = 720;
    SDL_SetRenderDrawColor(rendu, 176, 224, 230, 255);	//pinceau bleu
    SDL_RenderFillRect(rendu, &rect1);

    // Fond 2
    SDL_Rect rect2;
    rect2.x = 0;
    rect2.y = 520;
    rect2.w = 1080;
    rect2.h = 200;
    SDL_SetRenderDrawColor(rendu, 0, 250, 154, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &rect2);

    // ligne pour le sol des bambou
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderDrawLine(rendu, 0, 600, 1080, 600);
}

// Bambou
void afficheBambou(SDL_Renderer* rendu, int positionX, int positionY, int nbBambou) {

    for (int i = 0; i < nbBambou; i++) {
        SDL_Rect bambou;
        bambou.x = positionX;
        bambou.y = positionY;
        bambou.w = 10;
        bambou.h = 30;
        SDL_SetRenderDrawColor(rendu, 173, 255, 47, 255);	//pinceau vert
        SDL_RenderFillRect(rendu, &bambou);
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
        SDL_RenderDrawRect(rendu, &bambou);



        SDL_Rect haut;
        haut.x = positionX - 2;
        haut.y = positionY + 31;
        haut.w = 15;
        haut.h = 5;
        SDL_SetRenderDrawColor(rendu, 173, 255, 47, 255);	//pinceau vert
        SDL_RenderFillRect(rendu, &haut);
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
        SDL_RenderDrawRect(rendu, &haut);



        SDL_Rect bas;
        bas.x = positionX - 2;
        bas.y = positionY;
        bas.w = 15;
        bas.h = 5;
        SDL_SetRenderDrawColor(rendu, 173, 255, 47, 255);	//pinceau vert
        SDL_RenderFillRect(rendu, &bas);
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
        SDL_RenderDrawRect(rendu, &bas);
        positionY -= 31;
        SDL_RenderPresent(rendu);//on rafraichit
    }
}

// Robot Panda
void afficheRobot(SDL_Renderer* rendu, int positionX, int positionY) {


    SDL_Rect RobotPanda;
    RobotPanda.x = positionX;
    RobotPanda.y = positionY;
    RobotPanda.w = 30;
    RobotPanda.h = 30;
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &RobotPanda);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &RobotPanda);
    SDL_RenderPresent(rendu);//on rafraichit

    //pieds
    SDL_Rect pied_gauche;
    pied_gauche.x = positionX;
    pied_gauche.y = positionY + 30;
    pied_gauche.w = 10;
    pied_gauche.h = 10;
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &pied_gauche);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &pied_gauche);
    SDL_RenderPresent(rendu);//on rafraichit

    SDL_Rect pied_droite;
    pied_droite.x = positionX + 20;
    pied_droite.y = positionY + 30;
    pied_droite.w = 10;
    pied_droite.h = 10;
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &pied_droite);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &pied_droite);
    SDL_RenderPresent(rendu);//on rafraichit

    //bras
    SDL_Rect bras_gauche;
    bras_gauche.x = positionX - 10;
    bras_gauche.y = positionY;
    bras_gauche.w = 10;
    bras_gauche.h = 10;
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &bras_gauche);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &bras_gauche);
    SDL_RenderPresent(rendu);//on rafraichit

    //bras
    SDL_Rect outil;
    outil.x = positionX - 70;
    outil.y = positionY + 5;
    outil.w = 80;
    outil.h = 2;
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &outil);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &outil);
    SDL_RenderPresent(rendu);//on rafraichit

    SDL_Rect bras_droite;
    bras_droite.x = positionX + 30;
    bras_droite.y = positionY;
    bras_droite.w = 10;
    bras_droite.h = 10;
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &bras_droite);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &bras_droite);
    SDL_RenderPresent(rendu);//on rafraichit

    //tête
    SDL_Rect tete;
    tete.x = positionX + 5;
    tete.y = positionY - 10;
    tete.w = 20;
    tete.h = 20;
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &tete);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &tete);
    SDL_RenderPresent(rendu);//on rafraichit

    SDL_Rect oeil_gauche;
    oeil_gauche.x = positionX + 8;
    oeil_gauche.y = positionY - 5;
    oeil_gauche.w = 5;
    oeil_gauche.h = 5;
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &oeil_gauche);
    SDL_SetRenderDrawColor(rendu, 254, 254, 254, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &oeil_gauche);
    SDL_RenderPresent(rendu);//on rafraichit

    SDL_Rect oeil_droit;
    oeil_droit.x = positionX + 15;
    oeil_droit.y = positionY - 5;
    oeil_droit.w = 5;
    oeil_droit.h = 5;
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &oeil_droit);
    SDL_SetRenderDrawColor(rendu, 254, 254, 254, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &oeil_droit);
    SDL_RenderPresent(rendu);//on rafraichit

    SDL_Rect oreille_droit;
    oreille_droit.x = positionX + 16;
    oreille_droit.y = positionY - 20;
    oreille_droit.w = 10;
    oreille_droit.h = 10;
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &oreille_droit);
    SDL_SetRenderDrawColor(rendu, 254, 254, 254, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &oreille_droit);
    SDL_RenderPresent(rendu);//on rafraichit

    SDL_Rect oreille_gauche;
    oreille_gauche.x = positionX + 2;
    oreille_gauche.y = positionY - 20;
    oreille_gauche.w = 10;
    oreille_gauche.h = 10;
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau vert
    SDL_RenderFillRect(rendu, &oreille_gauche);
    SDL_SetRenderDrawColor(rendu, 254, 254, 254, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &oreille_gauche);
    SDL_RenderPresent(rendu);//on rafraichit
}

// soleil

void soleil(SDL_Renderer* rendu, int positionX, int positionY) {

    //soleil
    SDL_Rect soleil;
    soleil.x = positionX;
    soleil.y = positionY;
    soleil.w = 150;
    soleil.h = 150;
    SDL_SetRenderDrawColor(rendu, 255, 255, 0, 255);	//pinceau jaune 
    SDL_RenderFillRect(rendu, &soleil);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir
    SDL_RenderDrawRect(rendu, &soleil);
    SDL_RenderPresent(rendu);//on rafraichit

}

// Lune

void lune(SDL_Renderer* rendu, int positionX, int positionY) {

    //lune
    SDL_Rect lune;
    lune.x = positionX;
    lune.y = positionY;
    lune.w = 100;
    lune.h = 150;
    SDL_SetRenderDrawColor(rendu, 0, 0, 205, 255);	//pinceau jaune 
    SDL_RenderFillRect(rendu, &lune);
    SDL_RenderPresent(rendu);//on rafraichit

        //lune
    SDL_Rect lune1;
    lune1.x = positionX;
    lune1.y = positionY + 100;
    lune1.w = 150;
    lune1.h = 100;
    SDL_SetRenderDrawColor(rendu, 0, 0, 205, 255);	//pinceau jaune 
    SDL_RenderFillRect(rendu, &lune1);
    SDL_RenderPresent(rendu);//on rafraichit

        //lune
    SDL_Rect lune2;
    lune2.x = positionX + 50;
    lune2.y = positionY + 50;
    lune2.w = 80;
    lune2.h = 80;
    SDL_SetRenderDrawColor(rendu, 0, 0, 205, 255);	//pinceau jaune 
    SDL_RenderFillRect(rendu, &lune2);
    SDL_RenderPresent(rendu);//on rafraichit

}

// ----------------------------------------------Fin fonctions SDL----------------------------------------------------------- //



int main(int argn, char* argv[]) {

    Bambou bambouseraie[TAILLE_MAX];

    // tests

    /*cout << " Debut " << endl << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
    }
    cout << endl;
    GrowAll(bambouseraie, 5);
    for (int i = 0; i < 5; i++) {
        cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
    }
    cout << endl;
    cout << "Premiere decoupe"<<endl;
    ReduceMax(bambouseraie, 5);
    GrowAll(bambouseraie, 5);
    for (int i = 0; i < 5; i++) {
        cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
    }
    cout << endl;
    cout << "deuxieme decoupe" << endl;
    ReduceMax(bambouseraie, 5);
    GrowAll(bambouseraie, 5);
    for (int i = 0; i < 5; i++) {
        cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
    }
    cout << endl;
    cout << "troisieme decoupe"<<endl;
    ReduceMax(bambouseraie, 5);
    GrowAll(bambouseraie, 5);
    for (int i = 0; i < 5; i++) {
        cout << "Bambou " << i + 1 <<" : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
    }
    cout << endl;
    cout << "fini" << endl;
    cout << " Le bambou le plus grand est le bambou numero : " << VerifMax(bambouseraie, 5) + 1 << " avec une taille de : "<< bambouseraie[VerifMax(bambouseraie, 5)].taille << endl;
    ReduceMax(bambouseraie, 5);
    ReduceFaster(6, bambouseraie, 5);*/

    // SDL (interface graphique)

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Echec à l’ouverture";
        return 1;
    }
    //on ouvre la police de caractères
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 25);

    //on crée la fenêtre
    SDL_Window* win = SDL_CreateWindow("PandaRobot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);

    if (win == NULL)
        cout << "erreur ouverture fenetre";

    SDL_Renderer* rendu = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    //fin parametres fenetres 

    // debut des dessin sur l'ecran
    fond(rendu);

    // variables
    int soleilx = 0;
    int soleily = 500;
    int parametres[6] = { 1,2,3,4,5,6 };

    // creation des bambous 
    InitBamboueraie(bambouseraie, 6, parametres);

    // boucle infinie
    bool boucle = true;
    for (int i = 0; boucle; i++) {

        // affichage des bambous 
        afficheBambou(rendu, 150, 570, bambouseraie[0].taille);
        afficheBambou(rendu, 250, 570, bambouseraie[1].taille);
        afficheBambou(rendu, 350, 570, bambouseraie[2].taille);
        afficheBambou(rendu, 450, 570, bambouseraie[3].taille);
        afficheBambou(rendu, 550, 570, bambouseraie[4].taille);
        afficheBambou(rendu, 550, 570, bambouseraie[0].taille);

        SDL_Delay(900);

        // test dans la console
        for (int i = 0; i < 5; i++) {
            cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
        }
        cout << endl;

        // croissance des bambous
        for (int i = 0; i < 5; i++) {
            GrowBambou(bambouseraie[i]);
        }

        int indice_a_couper = ReduceMax(bambouseraie, 5);
        bambouseraie[indice_a_couper].taille = bambouseraie[indice_a_couper].croissance;



        // mis à jour de l'ecran avec les bonne tailles des bambous en affichant le fond de la fenetre 
        fond(rendu);

        //system("pause");

        // les endroits ou le Robot spawn
        if (indice_a_couper == 0) {
            afficheRobot(rendu, 200, 570);
        }
        else if (indice_a_couper == 1) {
            afficheRobot(rendu, 300, 570);
        }
        else if (indice_a_couper == 2) {
            afficheRobot(rendu, 400, 570);
        }
        else if (indice_a_couper == 3) {
            afficheRobot(rendu, 500, 570);
        }
        else if (indice_a_couper == 4) {
            afficheRobot(rendu, 600, 570);
        }

        if (soleilx == 1000) {
            soleilx = 0;
            SDL_Rect rect1;
            rect1.x = 0;
            rect1.y = 0;
            rect1.w = 1080;
            rect1.h = 500;
            SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);	//pinceau bleu
            SDL_RenderFillRect(rendu, &rect1);
            lune(rendu, 0, 0);
            SDL_Delay(5000);
        }
        soleilx += 50;
        if (soleily > 0) {
            soleily -= 50;
        }
        if (soleily == 0) {
            soleily = +50;
        }

        soleil(rendu, soleilx, soleily);

    }

    SDL_RenderPresent(rendu);//on rafraichit

    /*************BOUCLE D'evenements**************/

    bool continuer = true;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:

            continuer = false;
            break;
        }
    }
    //destruction du renderer à la fin
    SDL_DestroyRenderer(rendu);
    //destruction à la fin
    SDL_DestroyWindow(win);

    TTF_CloseFont(font);
    TTF_Quit();

    //fermeture
    SDL_Quit();

    return 0;
}