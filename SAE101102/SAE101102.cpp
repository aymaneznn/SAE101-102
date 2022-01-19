using namespace std;
#include<SDL.h> 
#include<SDL_ttf.h> 
#include<iostream>
#include "config_sdl.h"
#include <fstream>
#include <sstream>
#include "SDL_image.h"
#include<ctime>


SDL_Renderer* rendu;
TTF_Font* font;
SDL_TimerID timer;
const int TAILLE_MAX = 100;
const int seuil = 10;
int parametres[8] = { 0 };
int manuell;

int max1 = 0;
int max2 = 0;

int faster1 = 0;
int faster2 = 0;

const int taille = 8;
int indice_batterie = 0;
bool play = true;

// Definition des bambou
struct Bambou {
    int croissance;
    int taille;
};

Bambou bambouseraie[TAILLE_MAX];

// Statistique de la taille maximum de la bambouseraie
int TaillleMax(Bambou tab[], int TailleChoisie) {
    int maximum = 0;
    for (int i = 0; i < TailleChoisie; i++) {
        if (tab[i].taille > maximum) {
            maximum = tab[i].taille;
        }
    }
    return maximum;
}

// Statistique de la taille moyenne de la bambouseraie

int TailleMoy(Bambou tab[], int taille) {
    int total = 0;
    int nb = 0;
    for (int i = 0; i < taille; i++) {
        if (tab[i].taille != 0) {
            nb++;
        }
        total += tab[i].taille;
    }
    if (nb == 0) {
        nb = 1;
    }
    return total / nb;
}

// Statistique de la taille minimum de la bambouseraie
int TailleMin(Bambou tab[], int taille) {
    int min = TaillleMax(tab, 8);
    for (int i = 0; i < taille; i++) {
        if (tab[i].taille < min && tab[i].taille>0) {
            min = tab[i].taille;
        }
    }
    return min;
}

// Création d'un bambou
void CreateBambou(Bambou& bambou, int taille) {
    bambou.croissance = taille;
    bambou.taille = taille;
}

// Initialisation de bambouraie
void InitBamboueraie(Bambou bambouraie[], int taille, int tab[]) {
    srand(time(NULL));
    for (int i = 0; i < taille; i++) {
        CreateBambou(bambouraie[i], parametres[i]);
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

void VerifMax2BOT(Bambou bambouseraie[]) {
    int TailleMax1 = 0;
    int TailleMax2 = 0;
    for (int i = 0; i < taille; i++) {
        if (TailleMax1 < bambouseraie[i].taille) {
            TailleMax1 = bambouseraie[i].taille;
            max1 = i;
        }

    }
    for (int i = 0; i < taille; i++) {
        if (TailleMax2 < bambouseraie[i].taille && bambouseraie[i].taille != TailleMax1) {
            TailleMax2 = bambouseraie[i].taille;
            max2 = i;
        }
    }
}

// Coupe le bambou le plus grand
int ReduceMax(Bambou bambou[], int TailleChoisie) {
    int indice = VerifMax(bambou, TailleChoisie);
    bambou[indice].taille = bambou[indice].croissance;
    return indice;
}

// Fonction Reduce-Fastest(x)
int ReduceFaster(int seuil, Bambou tab[], int TailleChoisie) {
    int maxCroissance = 0;
    int indice = 0;
    for (int j = 0; j < TailleChoisie; j++) {
        if (tab[j].taille > seuil) {
            if (tab[j].croissance > maxCroissance) {
                maxCroissance = tab[j].croissance;
                indice = j;
            }
        }
    }
    return indice;
}

void ReduceFaster2BOT(Bambou tab[]) {
    
    int maxCroissance1 = 0;
    int maxCroissance2 = 0;
    for (int j = 0; j < 8; j++) {
        if (tab[j].taille > seuil) {
            if (tab[j].croissance > maxCroissance1) {
                maxCroissance1 = tab[j].croissance;
                faster1 = j;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        if (tab[i].taille > seuil ) {
            if (tab[i].croissance > maxCroissance2 && tab[i].croissance != maxCroissance1) {
                maxCroissance2 = tab[i].croissance;
                faster2 = i;
            }
        }
    }

}

// Croissance simultané de la bambouraie
void GrowAll(Bambou bambouraie[], int taille) {
    for (int i = 0; i < taille; i++) {
        GrowBambou(bambouraie[i]);
    }
}

void save() {
    ofstream sortie("save.txt", ios::trunc);
    for (int i = 0; i < taille; i++) {
        sortie << parametres[i] << " ";
    }
    sortie.close();
}

void load() {
    ifstream entre("save.txt", ios::in);
    for (int i = 0; i < taille; i++) {
        entre >> parametres[i];
        cout << parametres[i];
    }
    entre.close();
}

//--------------------------------------------------Fonctions SDL----------------------------------------------------------- //

// Fond
void fond(SDL_Renderer* rendu) {
    char name[] = "fond_pixelv2.bmp";


    SDL_Surface* image = SDL_LoadBMP(name);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);

    SDL_Rect dstrect = { 0,0,1080,700 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
}


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
    char name[] = "pandav2.bmp";
    SDL_Surface* image = SDL_LoadBMP(name);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);
    SDL_Rect dstrect = { positionX,positionY,100,100 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
}

// Robot Panda2
void afficheRobot2(SDL_Renderer* rendu, int positionX, int positionY) {
    char name[] = "panda2.bmp";
    SDL_Surface* image = SDL_LoadBMP(name);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);
    SDL_Rect dstrect = { positionX,positionY,100,100 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
}

//exit
void exit() {
    char name[] = "exit.bmp";
    SDL_Surface* image = SDL_LoadBMP(name);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);
    SDL_Rect dstrect = { 950,650,100,50 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    //SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
}

void exit_menu() {
    char name[] = "exit.bmp";
    SDL_Surface* image = SDL_LoadBMP(name);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);
    SDL_Rect dstrect = { 1150,590,100,50 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    //SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
}

// menu
void menu() {
    char name[] = "menuv2.bmp";
    SDL_Surface* image = SDL_LoadBMP(name);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);
    SDL_Rect dstrect = { 0,0,1360,720 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    //SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
}

// Echange deux nombres dans un tableau
void exch(int tab[], int i, int j) {
    int temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
}

// Ajout dynamique dans un tableau de taille 18
void ajout(int tab[], int truc) {
    for (int i = 1; i < 19; i++) {
        exch(tab, i, i - 1);
    }
    tab[18] = truc;
}

//image batterie
void batterie(char nom[]) {
    SDL_Surface* image = SDL_LoadBMP(nom);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);
    SDL_Rect dstrect = { 900,10,200 ,100 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    //SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);
}

// fond recharge
void fond_recharge() {
    SDL_Surface* image = SDL_LoadBMP("fond_recharge.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, image);
    SDL_Rect dstrect = { 0,0,1080 ,700 };
    SDL_RenderCopy(rendu, texture, NULL, &dstrect);
    //SDL_RenderPresent(rendu);
    SDL_DestroyTexture(texture);

    image = SDL_LoadBMP("eclair.bmp");
    SDL_Texture* eclair = SDL_CreateTextureFromSurface(rendu, image);
    dstrect = { 830,210,100,116 };
    SDL_RenderCopy(rendu, eclair, NULL, &dstrect);
    SDL_DestroyTexture(eclair);
    SDL_RenderPresent(rendu);
}

bool repos = false;
// affiche le bon level batterie 
void batterie_lv1() {
    char lv1[] = "batterie_lv1.bmp";
    char lv2[] = "batterie_lv2.bmp";
    char lv3[] = "batterie_lv3.bmp";
    if (indice_batterie <= 7) {
        batterie(lv3);
    }
    if (indice_batterie >= 7 && indice_batterie < 14) {
        batterie(lv2);
    }
    if (indice_batterie >= 14) {
        batterie(lv1);
    }
    if (indice_batterie >= 21) {
        fond_recharge();
        repos = true;
        if (indice_batterie == 25) {
            indice_batterie = 0;
            repos = false;
            fond(rendu);
        }
    }
    indice_batterie++;
    if(repos){
        SDL_Delay(800);
    }
}

// definition des tableaux fonction pour les stats
int number = 0;
int tabMax[19] = { 0 };
int tabMoy[19] = { 0 };
int tabMin[19] = { 0 };
int scale = 10;

int maxi(int tab[]) {
    int n = 0;
    for (int i = 0; i < 19; i++) {
        if (n < tab[i]) {
            n = tab[i];
        }
    }
    return n;
}

void graph() {
    ajout(tabMax, TaillleMax(bambouseraie, 8));
    ajout(tabMoy, TailleMoy(bambouseraie, 8));
    ajout(tabMin, TailleMin(bambouseraie, 8));
    while (125 + (maxi(tabMax) * scale) >= 205) {
        scale--;
    }
    while (125 + (maxi(tabMax) * scale) < 205) {
        scale++;
    }
    scale--;
    number++;

    SDL_Rect Noir = { 1080,0,280,720 };
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
    SDL_RenderFillRect(rendu, &Noir);

    string tmp = to_string(number);
    char text[200] = "Jours : ";
    char const* num_char = tmp.c_str();
    strcat_s(text, 200, num_char);
    SDL_Color blanc = { 255,255,255 }; //on définit une couleur de texte
    SDL_Rect positionTexte; //rectangle définissant le positionnement du texte, et sa taille

    //on place le texte au point (100,100)
    positionTexte.x = 1130;
    positionTexte.y = 25;

    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    SDL_Texture* texture = loadText(rendu, text, blanc, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //si on veut modifier le cadre du texte
    positionTexte.w *= 3;
    positionTexte.h *= 3;
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);

    //on détruit la texture
    SDL_DestroyTexture(texture);

    //on détruit la texture
    SDL_DestroyTexture(texture);

    SDL_Color rouge = { 255,0,0 }; //on définit une couleur de texte
    positionTexte.x = 1130;
    positionTexte.y = 100;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    texture = loadText(rendu, "Maximum", rouge, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on détruit la texture
    SDL_DestroyTexture(texture);
    SDL_Rect graphMax;
    graphMax.x = 1130;
    graphMax.y = 125;
    graphMax.w = 180;
    graphMax.h = 80;
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
    for (int i = 1; i < 19; i++) {
        SDL_RenderDrawLine(rendu, 1130 + ((i - 1) * 10), 205 - (tabMax[i - 1] * scale), 1130 + (i * 10), 205 - (tabMax[i] * scale));
    }
    SDL_RenderDrawRect(rendu, &graphMax);

    SDL_Color vert = { 0,255,0 }; //on définit une couleur de texte
    positionTexte.x = 1130;
    positionTexte.y = 255;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    texture = loadText(rendu, "Moyenne", vert, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on détruit la texture
    SDL_DestroyTexture(texture);
    graphMax.x = 1130;
    graphMax.y = 280;
    graphMax.w = 180;
    graphMax.h = 80;
    SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);
    for (int i = 1; i < 19; i++) {
        SDL_RenderDrawLine(rendu, 1130 + ((i - 1) * 10), 360 - (tabMoy[i - 1] * scale), 1130 + (i * 10), 360 - (tabMoy[i] * scale));
    }
    SDL_RenderDrawRect(rendu, &graphMax);

    SDL_Color bleu = { 100,100,255 }; //on définit une couleur de texte
    positionTexte.x = 1130;
    positionTexte.y = 410;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    texture = loadText(rendu, "Minimum", bleu, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on détruit la texture
    SDL_DestroyTexture(texture);
    graphMax.x = 1130;
    graphMax.y = 435;
    graphMax.w = 180;
    graphMax.h = 80;
    SDL_SetRenderDrawColor(rendu, 100, 100, 255, 255);
    for (int i = 1; i < 19; i++) {
        SDL_RenderDrawLine(rendu, 1130 + ((i - 1) * 10), 515 - (tabMin[i - 1] * scale), 1130 + (i * 10), 515 - (tabMin[i] * scale));
    }
    SDL_RenderDrawRect(rendu, &graphMax);

    blanc = { 255,255,255 }; //on définit une couleur de texte
    positionTexte.x = 1130;
    positionTexte.y = 565;
    //on crée une texture à partir du texte, de sa couleur, et de la fonte
    texture = loadText(rendu, "Overview", blanc, font);
    //on maj le rectangle couvrant cette texture
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    //on copie la texture dans le rendu
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    //on détruit la texture
    SDL_DestroyTexture(texture);
    graphMax.x = 1130;
    graphMax.y = 590;
    graphMax.w = 180;
    graphMax.h = 80;
    SDL_SetRenderDrawColor(rendu, 255, 0, 0, 255);
    for (int i = 1; i < 19; i++) {
        SDL_RenderDrawLine(rendu, 1130 + ((i - 1) * 10), 670 - (tabMax[i - 1] * scale), 1130 + (i * 10), 670 - (tabMax[i] * scale));
    }
    SDL_SetRenderDrawColor(rendu, 0, 255, 0, 255);
    for (int i = 1; i < 19; i++) {
        SDL_RenderDrawLine(rendu, 1130 + ((i - 1) * 10), 670 - (tabMoy[i - 1] * scale), 1130 + (i * 10), 670 - (tabMoy[i] * scale));
    }
    SDL_SetRenderDrawColor(rendu, 100, 100, 255, 255);
    for (int i = 1; i < 19; i++) {
        SDL_RenderDrawLine(rendu, 1130 + ((i - 1) * 10), 670 - (tabMin[i - 1] * scale), 1130 + (i * 10), 670 - (tabMin[i] * scale));
    }
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
    SDL_RenderDrawRect(rendu, &graphMax);
}

int interval = 10;
Uint32 event1(Uint32 interval, void* param) {
    // variables
    int number = 0;
    int xx = 0;
    int yy = -80;
    int posx = 150;
    int posx2 = 150;
    int h = 10;
    
    // creation des bambous 
    InitBamboueraie(bambouseraie, 8, parametres);
    for (int i = 0; i < 8; i++) {
        afficheBambou(rendu, posx, 600, bambouseraie[i].taille);
        posx += 100;
    }

    // boucle infinie
    bool boucle = true;
    for (int i = 0; boucle; i++) {
        if (play) {
        exit();
        batterie_lv1();
        if (repos == false) {

            // affichage des bambous 
            /*afficheBambou(rendu, 150, 600, bambouseraie[0].taille);
            afficheBambou(rendu, 250, 600, bambouseraie[1].taille);
            afficheBambou(rendu, 350, 600, bambouseraie[2].taille);
            afficheBambou(rendu, 450, 600, bambouseraie[3].taille);
            afficheBambou(rendu, 550, 600, bambouseraie[4].taille);
            afficheBambou(rendu, 650, 600, bambouseraie[5].taille);
            afficheBambou(rendu, 750, 600, bambouseraie[6].taille);
            afficheBambou(rendu, 850, 600, bambouseraie[7].taille);*/

            for (int i = 0; i < 8; i++) {
                afficheBambou(rendu, posx2, 600, bambouseraie[i].taille);
                posx2 += 100;
            }
            posx2 = 150;
        }

        // test dans la console
        for (int i = 0; i < 8; i++) {
            cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
        }
        cout << endl;

        // croissance des bambous
        for (int i = 0; i < 8; i++) {
            GrowBambou(bambouseraie[i]);
        }

        // Mode repos du Panda
        if (repos == false) {
            int indice_a_couper = ReduceMax(bambouseraie, 8);
            bambouseraie[indice_a_couper].taille = bambouseraie[indice_a_couper].croissance;

            SDL_Delay(800);
            if (play) {
                // mis à jour de l'ecran avec les bonne tailles des bambous en affichant le fond de la fenetre 
                fond(rendu);
            }
            //system("pause");
            if (play) {
                // les endroits ou le Robot spawn
                if (indice_a_couper == 0) {
                    afficheRobot(rendu, 150, 570);
                }
                else if (indice_a_couper == 1) {
                    afficheRobot(rendu, 250, 570);
                }
                else if (indice_a_couper == 2) {
                    afficheRobot(rendu, 350, 570);
                }
                else if (indice_a_couper == 3) {
                    afficheRobot(rendu, 450, 570);
                }
                else if (indice_a_couper == 4) {
                    afficheRobot(rendu, 550, 570);
                }
                else if (indice_a_couper == 5) {
                    afficheRobot(rendu, 650, 570);
                }
                else if (indice_a_couper == 6) {
                    afficheRobot(rendu, 750, 570);
                }
                else if (indice_a_couper == 7) {
                    afficheRobot(rendu, 850, 570);
                }
            }

            // Le soleil se lève à tout jamais
            char soleil[] = "soleiv2.bmp";
            SDL_Surface* image5 = SDL_LoadBMP(soleil);
            SDL_Texture* texture5 = SDL_CreateTextureFromSurface(rendu, image5);
            SDL_Rect dstrect5 = { xx,yy,300,300 };
            SDL_RenderCopy(rendu, texture5, NULL, &dstrect5);
            if (xx == 1000) {
                xx = 0;
            }
            xx += 50;
            SDL_DestroyTexture(texture5);
        }
        }
        if (play) {
            graph();
        }
    }
    
    //SDL_DestroyRenderer(rendu);
    SDL_RenderPresent(rendu);//on rafraichit
    return interval;

}

Uint32 event2(Uint32 interval, void* param) {

    // variables
    int number = 0;
    int xx = 0;
    int yy = -80;
    int posx2 = 150;

    // creation des bambous 
    InitBamboueraie(bambouseraie, 8, parametres);

    // boucle infinie
    bool boucle = true;
    for (int i = 0; boucle; i++) {
        if (play) {
            exit();
            batterie_lv1();
            if (repos == false) {
                // affichage des bambous 
                for (int i = 0; i < 8; i++) {
                    afficheBambou(rendu, posx2, 600, bambouseraie[i].taille);
                    posx2 += 100;
                }
                posx2 = 150;
            }
            // test dans la console
            for (int i = 0; i < 8; i++) {
                cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
            }
            cout << endl;

            // croissance des bambous
            for (int i = 0; i < 8; i++) {
                GrowBambou(bambouseraie[i]);
            }

            if (repos == false) {
                int indice_a_couper = ReduceFaster(seuil, bambouseraie, 8);
                bambouseraie[indice_a_couper].taille = bambouseraie[indice_a_couper].croissance;

                SDL_Delay(800);

                if (play) {
                    // mis à jour de l'ecran avec les bonne tailles des bambous en affichant le fond de la fenetre 
                    fond(rendu);
                }
                //system("pause");
                if (play) {
                    // les endroits ou le Robot spawn
                    if (indice_a_couper == 0) {
                        afficheRobot(rendu, 150, 570);
                    }
                    else if (indice_a_couper == 1) {
                        afficheRobot(rendu, 250, 570);
                    }
                    else if (indice_a_couper == 2) {
                        afficheRobot(rendu, 350, 570);
                    }
                    else if (indice_a_couper == 3) {
                        afficheRobot(rendu, 450, 570);
                    }
                    else if (indice_a_couper == 4) {
                        afficheRobot(rendu, 550, 570);
                    }
                    else if (indice_a_couper == 5) {
                        afficheRobot(rendu, 650, 570);
                    }
                    else if (indice_a_couper == 6) {
                        afficheRobot(rendu, 750, 570);
                    }
                    else if (indice_a_couper == 7) {
                        afficheRobot(rendu, 850, 570);
                    }
                }
                // Le soleil se lève à tout jamais
                char soleil[] = "soleiv2.bmp";
                SDL_Surface* image5 = SDL_LoadBMP(soleil);
                SDL_Texture* texture5 = SDL_CreateTextureFromSurface(rendu, image5);
                SDL_Rect dstrect5 = { xx,yy,300,300 };
                SDL_RenderCopy(rendu, texture5, NULL, &dstrect5);
                if (xx == 1000) {
                    xx = 0;
                }
                xx += 50;
                SDL_DestroyTexture(texture5);
            }
        }
        if (play) {
            graph();
        }

    }

    SDL_RenderPresent(rendu);//on rafraichit

    return interval;
}

Uint32 event3(Uint32 interval, void* param) {

    // variables
    int number = 0;
    int xx = 0;
    int yy = -80;
    int posx2 = 150;
    // creation des bambous 
    InitBamboueraie(bambouseraie, 8, parametres);

    // boucle infinie
    bool boucle = true;
    for (int i = 0; boucle; i++) {
        if (play) {
            exit();
            batterie_lv1();

            if (repos == false) {
                // affichage des bambous 
                for (int i = 0; i < 8; i++) {
                    afficheBambou(rendu, posx2, 600, bambouseraie[i].taille);
                    posx2 += 100;
                }
                posx2 = 150;
            }
            // test dans la console
            for (int i = 0; i < 8; i++) {
                cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
            }
            cout << endl;

            VerifMax2BOT(bambouseraie);

            // croissance des bambous
            for (int i = 0; i < 8; i++) {
                GrowBambou(bambouseraie[i]);
            }

            if (repos == false) {
                int indice_a_couper1 = max1;
                cout << "max1 " << max1 << endl;
                cout << "max2 " << max2 << endl;
                int indice_a_couper2 = max2;
                bambouseraie[indice_a_couper1].taille = bambouseraie[indice_a_couper1].croissance;
                bambouseraie[indice_a_couper2].taille = bambouseraie[indice_a_couper2].croissance;

                SDL_Delay(800);
                if (play) {
                    // mis à jour de l'ecran avec les bonne tailles des bambous en affichant le fond de la fenetre 
                    fond(rendu);
                }
                //system("pause");

                // les endroits ou le Robot spawn
                if (play) {
                    if (indice_a_couper1 == 0) {
                        afficheRobot(rendu, 150, 570);
                    }
                    else if (indice_a_couper1 == 1) {
                        afficheRobot(rendu, 250, 570);
                    }
                    else if (indice_a_couper1 == 2) {
                        afficheRobot(rendu, 350, 570);
                    }
                    else if (indice_a_couper1 == 3) {
                        afficheRobot(rendu, 450, 570);
                    }
                    else if (indice_a_couper1 == 4) {
                        afficheRobot(rendu, 550, 570);
                    }
                    else if (indice_a_couper1 == 5) {
                        afficheRobot(rendu, 650, 570);
                    }
                    else if (indice_a_couper1 == 6) {
                        afficheRobot(rendu, 750, 570);
                    }
                    else if (indice_a_couper1 == 7) {
                        afficheRobot(rendu, 850, 570);
                    }

                    // les endroits ou le Robot spawn
                    if (indice_a_couper2 == 0) {
                        afficheRobot2(rendu, 150, 570);
                    }
                    else if (indice_a_couper2 == 1) {
                        afficheRobot2(rendu, 250, 570);
                    }
                    else if (indice_a_couper2 == 2) {
                        afficheRobot2(rendu, 350, 570);
                    }
                    else if (indice_a_couper2 == 3) {
                        afficheRobot2(rendu, 450, 570);
                    }
                    else if (indice_a_couper2 == 4) {
                        afficheRobot2(rendu, 550, 570);
                    }
                    else if (indice_a_couper2 == 5) {
                        afficheRobot2(rendu, 650, 570);
                    }
                    else if (indice_a_couper2 == 6) {
                        afficheRobot2(rendu, 750, 570);
                    }
                    else if (indice_a_couper2 == 7) {
                        afficheRobot2(rendu, 850, 570);
                    }
                }
                // Le soleil se lève à tout jamais
                char soleil[] = "soleiv2.bmp";
                SDL_Surface* image5 = SDL_LoadBMP(soleil);
                SDL_Texture* texture5 = SDL_CreateTextureFromSurface(rendu, image5);
                SDL_Rect dstrect5 = { xx,yy,300,300 };
                SDL_RenderCopy(rendu, texture5, NULL, &dstrect5);
                if (xx == 1000) {
                    xx = 0;
                }
                xx += 50;
                SDL_DestroyTexture(texture5);
            }
        }
        if (play) {
            graph();
        }
    }

    SDL_RenderPresent(rendu);//on rafraichit

    return interval;
}

Uint32 event4(Uint32 interval, void* param) {

    // variables
    int number = 0;
    int xx = 0;
    int yy = -80;
    int posx2 = 150;
    // creation des bambous 
    InitBamboueraie(bambouseraie, 8, parametres);

    // boucle infinie
    bool boucle = true;
    for (int i = 0; boucle; i++) {
        if (play) {
            exit();
            batterie_lv1();

            if (repos == false) {
                // affichage des bambous 
                for (int i = 0; i < 8; i++) {
                    afficheBambou(rendu, posx2, 600, bambouseraie[i].taille);
                    posx2 += 100;
                }
                posx2 = 150;
            }
            // test dans la console
            for (int i = 0; i < 8; i++) {
                cout << "Bambou " << i + 1 << " : " << bambouseraie[i].taille << " | il croie de : " << bambouseraie[i].croissance << endl;
            }
            cout << endl;

            VerifMax2BOT(bambouseraie);

            // croissance des bambous
            for (int i = 0; i < 8; i++) {
                GrowBambou(bambouseraie[i]);
            }
            ReduceFaster2BOT(bambouseraie);
            if (repos == false) {
                int indice_a_couper1 = faster1;
                int indice_a_couper2 = faster2;
                bambouseraie[indice_a_couper1].taille = bambouseraie[indice_a_couper1].croissance;
                bambouseraie[indice_a_couper2].taille = bambouseraie[indice_a_couper2].croissance;

                SDL_Delay(800);
                if (play) {
                    // mis à jour de l'ecran avec les bonne tailles des bambous en affichant le fond de la fenetre 
                    fond(rendu);
                }
                //system("pause");

                // les endroits ou le Robot spawn
                if (play) {
                    if (indice_a_couper1 == 0) {
                        afficheRobot(rendu, 150, 570);
                    }
                    else if (indice_a_couper1 == 1) {
                        afficheRobot(rendu, 250, 570);
                    }
                    else if (indice_a_couper1 == 2) {
                        afficheRobot(rendu, 350, 570);
                    }
                    else if (indice_a_couper1 == 3) {
                        afficheRobot(rendu, 450, 570);
                    }
                    else if (indice_a_couper1 == 4) {
                        afficheRobot(rendu, 550, 570);
                    }
                    else if (indice_a_couper1 == 5) {
                        afficheRobot(rendu, 650, 570);
                    }
                    else if (indice_a_couper1 == 6) {
                        afficheRobot(rendu, 750, 570);
                    }
                    else if (indice_a_couper1 == 7) {
                        afficheRobot(rendu, 850, 570);
                    }

                    // les endroits ou le Robot spawn
                    if (indice_a_couper2 == 0) {
                        afficheRobot2(rendu, 150, 570);
                    }
                    else if (indice_a_couper2 == 1) {
                        afficheRobot2(rendu, 250, 570);
                    }
                    else if (indice_a_couper2 == 2) {
                        afficheRobot2(rendu, 350, 570);
                    }
                    else if (indice_a_couper2 == 3) {
                        afficheRobot2(rendu, 450, 570);
                    }
                    else if (indice_a_couper2 == 4) {
                        afficheRobot2(rendu, 550, 570);
                    }
                    else if (indice_a_couper2 == 5) {
                        afficheRobot2(rendu, 650, 570);
                    }
                    else if (indice_a_couper2 == 6) {
                        afficheRobot2(rendu, 750, 570);
                    }
                    else if (indice_a_couper2 == 7) {
                        afficheRobot2(rendu, 850, 570);
                    }
                }
                // Le soleil se lève à tout jamais
                char soleil[] = "soleiv2.bmp";
                SDL_Surface* image5 = SDL_LoadBMP(soleil);
                SDL_Texture* texture5 = SDL_CreateTextureFromSurface(rendu, image5);
                SDL_Rect dstrect5 = { xx,yy,300,300 };
                SDL_RenderCopy(rendu, texture5, NULL, &dstrect5);
                if (xx == 1000) {
                    xx = 0;
                }
                xx += 50;
                SDL_DestroyTexture(texture5);
            }
        }
        if (play) {
            graph();
        }
    }

    SDL_RenderPresent(rendu);//on rafraichit

    return interval;
}

Uint32 manual(Uint32 interval, void* param) {

    // variables
    int xx = 0;
    int yy = -80;
    int posx2 = 200;
    // creation des bambous 
    InitBamboueraie(bambouseraie, 6, parametres);

    // boucle infinie
    bool boucle = true;
    for (int i = 0; boucle; i++) {
        if (play) {
            batterie_lv1();
            exit();
        }
            if (repos == false) {
                if (play) {
                    // affichage des bambous 
                    for (int i = 0; i < 8; i++) {
                afficheBambou(rendu, posx2, 600, bambouseraie[i].taille);
                posx2 += 100;
            }
            posx2 = 200;
                    SDL_Delay(1000);
                }

                //system("pause");
                if (play) {
                    if (manuell == 1) {
                        CutBambou(bambouseraie[0]);
                        manuell = 0;
                    }
                    if (manuell == 2) {
                        CutBambou(bambouseraie[1]);
                        manuell = 0;
                    }
                    if (manuell == 3) {
                        CutBambou(bambouseraie[2]);
                        manuell = 0;
                    }
                    if (manuell == 4) {
                        CutBambou(bambouseraie[3]);
                        manuell = 0;
                    }
                    if (manuell == 5) {
                        CutBambou(bambouseraie[4]);
                        manuell = 0;
                    }
                }
            }
            if (play) {
                // croissance des bambous
                for (int i = 0; i < 5; i++) {


                    GrowBambou(bambouseraie[i]);

                }
            }

            if (repos == false) {

                if (play) {
                    // Le soleil se lève à tout jamais
                    char soleil[] = "soleiv2.bmp";
                    SDL_Surface* image5 = SDL_LoadBMP(soleil);
                    SDL_Texture* texture5 = SDL_CreateTextureFromSurface(rendu, image5);
                    SDL_Rect dstrect5 = { xx,yy,300,300 };
                    SDL_RenderCopy(rendu, texture5, NULL, &dstrect5);
                    if (xx == 1000) {
                        xx = 0;
                    }
                    xx += 50;
                    SDL_DestroyTexture(texture5);
                    fond(rendu);
                }
            }
        
        if (play) {
            graph();
        }
    }

    SDL_RenderPresent(rendu);//on rafraichit

    return interval;
}

// ----------------------------------------------Fin fonctions SDL----------------------------------------------------------- //

int main(int argn, char* argv[]) {

    // SDL (interface graphique)
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Echec à l’ouverture";
        return 1;
    }

    //on ouvre la police de caractères
    TTF_Init();
    font = TTF_OpenFont("C:\\Windows\\Fonts\\calibri.ttf", 15);

    //on crée la fenêtre
    SDL_Window* win = SDL_CreateWindow("PandaRobot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1360, 720, SDL_WINDOW_SHOWN);

    if (win == NULL)
        cout << "erreur ouverture fenetre";

    rendu = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    // Fin parametres fenetres 

    // Menu avec tous les boutons
    menu();

    SDL_Rect bb1;
    bb1.x = 612;
    bb1.y = 485;
    bb1.w = 62;
    bb1.h = 75;

    SDL_Rect bb2;
    bb2.x = 686;
    bb2.y = 485;
    bb2.w = 62;
    bb2.h = 75;

    SDL_Rect bb3;
    bb3.x = 760;
    bb3.y = 485;
    bb3.w = 62;
    bb3.h = 75;

    SDL_Rect bb4;
    bb4.x = 834;
    bb4.y = 485;
    bb4.w = 62;
    bb4.h = 75;

    SDL_Rect bb5;
    bb5.x = 908;
    bb5.y = 485;
    bb5.w = 62;
    bb5.h = 75;

    SDL_Rect bb6;
    bb6.x = 982;
    bb6.y = 485;
    bb6.w = 62;
    bb6.h = 75;

    SDL_Rect bb7;
    bb7.x = 1056;
    bb7.y = 485;
    bb7.w = 62;
    bb7.h = 75;

    SDL_Rect bb8;
    bb8.x = 1130;
    bb8.y = 485;
    bb8.w = 62;
    bb8.h = 75;

    SDL_Rect ss;
    ss.x = 650;
    ss.y = 54;
    ss.w = 90;
    ss.h = 86;


    SDL_Rect ss2;
    ss2.x = 1030;
    ss2.y = 54;
    ss2.w = 90;
    ss2.h = 86;


    SDL_Rect rect2;
    rect2.x = 884;
    rect2.y = 208;
    rect2.w = 260;
    rect2.h = 80;

    SDL_Rect rect3;
    rect3.x = 624;
    rect3.y = 208;
    rect3.w = 260;
    rect3.h = 80;

    SDL_Rect retour;
    retour.x = 1150;
    retour.y = 590;
    retour.w = 100;
    retour.h = 50;

    SDL_Rect exit;
    exit.x = 950;
    exit.y = 650;
    exit.w = 100;
    exit.h = 50;

    SDL_Rect manuel;
    manuel.x = 612;
    manuel.y = 346;
    manuel.w = 540;
    manuel.h = 100;
    exit_menu();

    // Bambou
    SDL_Rect bambou1;
    bambou1.x = 200;
    bambou1.y = 370;
    bambou1.w = 15;
    bambou1.h = 300;

    SDL_Rect bambou2;
    bambou2.x = 300;
    bambou2.y = 370;
    bambou2.w = 15;
    bambou2.h = 300;

    SDL_Rect bambou3;
    bambou3.x = 400;
    bambou3.y = 370;
    bambou3.w = 15;
    bambou3.h = 300;

    SDL_Rect bambou4;
    bambou4.x = 500;
    bambou4.y = 370;
    bambou4.w = 15;
    bambou4.h = 300;

    SDL_Rect bambou5;
    bambou5.x = 600;
    bambou5.y = 370;
    bambou5.w = 15;
    bambou5.h = 300;

    // Menu Save & Load
    SDL_Color noir = { 0,0,0 };
    SDL_Rect positionTexte;
    SDL_Rect Save;
    Save.x = 620;
    Save.y = 585;
    Save.w = 200;
    Save.h = 50;
    positionTexte.x = 660;
    positionTexte.y = 580;
    SDL_Texture* texture = loadText(rendu, "Save", noir, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 4;
    positionTexte.h *= 4;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_DestroyTexture(texture);

    SDL_Rect Load;
    Load.x = 870;
    Load.y = 585;
    Load.w = 200;
    Load.h = 50;
    positionTexte.x = 910;
    positionTexte.y = 580;
    texture = loadText(rendu, "Load", noir, font);
    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
    positionTexte.w *= 4;
    positionTexte.h *= 4;
    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
    SDL_DestroyTexture(texture);
    SDL_RenderPresent(rendu);

    SDL_Rect play2;
    play2.x = 45;
    play2.y = 5;
    play2.w = 100;
    play2.h = 40;
    //SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);    //pinceau vert remplissage
    //SDL_RenderFillRect(rendu, &play2);
    //SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir contour
    //SDL_RenderDrawRect(rendu, &play2);

    SDL_Rect pause;
    pause.x = 190;
    pause.y = 5;
    pause.w = 100;
    pause.h = 40;
    //SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);    //pinceau vert remplissage
    //SDL_RenderFillRect(rendu, &pause);
    //SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255); //pinceau noir contour
    //SDL_RenderDrawRect(rendu, &pause);

    /*************BOUCLE D'evenements**************/

    bool continuer = true;
    SDL_Event event;
    int champ = 0;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:

            continuer = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_a) { //touche a
                fond(rendu);
                timer = SDL_AddTimer(interval, manual, NULL);
            }
            if (event.key.keysym.sym == SDLK_p) { //touche a
                play = false;
            }
            if (event.key.keysym.sym == SDLK_o) { //touche a
                play = true;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE) { //touche a
                continuer = false;
            }
            if (event.key.keysym.sym == SDLK_b) { //touche b
                fond(rendu);
                timer = SDL_AddTimer(interval, event2, NULL);
            }

            if (champ != 0) {
                if (event.key.keysym.sym == SDLK_0) { //touche 0
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 0;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "0", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_1) { //touche 1
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 1;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "1", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_2) { //touche 2
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 2;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "2", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_3) { //touche 3
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 3;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "3", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_4) { //touche 4
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 4;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "4", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_5) { //touche 5
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 5;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "5", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_6) { //touche 6
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 6;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "6", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_7) { //touche 7
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 7;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "7", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_8) { //touche 8
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 8;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "8", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                if (event.key.keysym.sym == SDLK_9) { //touche 9
                    SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                    SDL_Rect dstrect = { 607 + (74 * (champ - 1)),488,72,70 };
                    SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                    SDL_DestroyTexture(texture2);

                    parametres[champ - 1] = 9;
                    positionTexte.x = 625 + (74 * (champ - 1));
                    champ = 0;
                    positionTexte.y = 484;
                    SDL_Texture* texture = loadText(rendu, "9", noir, font);
                    SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                    positionTexte.w *= 5;
                    positionTexte.h *= 5;
                    SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                    SDL_DestroyTexture(texture);
                }
                SDL_RenderPresent(rendu);
            }

            break;
            SDL_RemoveTimer(timer);

        case SDL_MOUSEBUTTONUP://appui souris
            if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
                if (event.button.x > rect2.x && event.button.x<rect2.x + rect2.w && event.button.y>rect2.y && event.button.y < rect2.y + rect2.h) { //dans 	le rectangle
                    cout << "faster" << endl;
                    fond(rendu);
                    timer = SDL_AddTimer(interval, event2, NULL);
                }
                
                if (event.button.x > rect3.x && event.button.x<rect3.x + rect3.w && event.button.y>rect3.y && event.button.y < rect3.y + rect3.h) { //dans 	le rectangle
                    cout << "max" << endl;
                    fond(rendu);
                    timer = SDL_AddTimer(interval, event1, NULL);
                }
                if (event.button.x > manuel.x && event.button.x<manuel.x + manuel.w && event.button.y>manuel.y && event.button.y < manuel.y + manuel.h) { //dans 	le rectangle
                    cout << "Manuel" << endl;
                    fond(rendu);
                    timer = SDL_AddTimer(interval, manual, NULL);
                }
                if (event.button.x > retour.x && event.button.x<retour.x + retour.w && event.button.y>retour.y && event.button.y < retour.y + retour.h) { //dans 	le rectangle
                    cout << "retour" << endl;
                    continuer = false;
                    break;
                }
                if (event.button.x > bambou1.x && event.button.x<bambou1.x + bambou1.w && event.button.y>bambou1.y && event.button.y < bambou1.y + bambou1.h) { //dans 	le rectangle
                    cout << "bambou1 " << endl;
                    manuell = 1;
                }
                if (event.button.x > bambou2.x && event.button.x<bambou2.x + bambou2.w && event.button.y>bambou2.y && event.button.y < bambou2.y + bambou2.h) { //dans 	le rectangle
                    cout << "bambou2" << endl;
                    manuell = 2;
                }
                if (event.button.x > bambou3.x && event.button.x<bambou3.x + bambou3.w && event.button.y>bambou3.y && event.button.y < bambou3.y + bambou3.h) { //dans 	le rectangle
                    cout << "bambou3" << endl;
                    manuell = 3;
                }
                if (event.button.x > bambou4.x && event.button.x<bambou4.x + bambou4.w && event.button.y>bambou4.y && event.button.y < bambou4.y + bambou4.h) { //dans 	le rectangle
                    cout << "bambou4" << endl;
                    manuell = 4;
                }
                if (event.button.x > bambou5.x && event.button.x<bambou5.x + bambou5.w && event.button.y>bambou5.y && event.button.y < bambou5.y + bambou5.h) { //dans 	le rectangle
                    cout << "bambou5" << endl;
                    manuell = 5;
                }
                if (event.button.x > retour.x && event.button.x<retour.x + retour.w && event.button.y>retour.y && event.button.y < retour.y + retour.h || event.button.x > exit.x && event.button.x<exit.x + exit.w && event.button.y>exit.y && event.button.y < exit.y + exit.h) {
                    cout << "retour" << endl;
                    continuer = false;
                    break;
                }
                if (event.button.x > bb1.x && event.button.x<bb1.x + bb1.w && event.button.y>bb1.y && event.button.y < bb1.y + bb1.h) { //dans 	le rectangle
                    cout << "bouton1" << endl;
                    champ = 1;
                    break;
                }
                if (event.button.x > bb2.x && event.button.x<bb2.x + bb2.w && event.button.y>bb2.y && event.button.y < bb2.y + bb2.h) { //dans 	le rectangle
                    cout << "bouton2" << endl;
                    champ = 2;
                    break;
                }
                if (event.button.x > bb3.x && event.button.x<bb3.x + bb3.w && event.button.y>bb3.y && event.button.y < bb3.y + bb3.h) { //dans 	le rectangle
                    cout << "bouton3" << endl;
                    champ = 3;
                    break;
                }
                if (event.button.x > bb4.x && event.button.x<bb4.x + bb4.w && event.button.y>bb4.y && event.button.y < bb4.y + bb4.h) { //dans 	le rectangle
                    cout << "bouton4" << endl;
                    champ = 4;
                    break;
                }
                if (event.button.x > bb5.x && event.button.x<bb5.x + bb5.w && event.button.y>bb5.y && event.button.y < bb5.y + bb5.h) { //dans 	le rectangle
                    cout << "bouton5" << endl;
                    champ = 5;
                    break;
                }
                if (event.button.x > bb6.x && event.button.x<bb6.x + bb6.w && event.button.y>bb6.y && event.button.y < bb6.y + bb6.h) { //dans 	le rectangle
                    cout << "bouton6" << endl;
                    champ = 6;
                    break;
                }
                if (event.button.x > bb7.x && event.button.x<bb7.x + bb7.w && event.button.y>bb7.y && event.button.y < bb7.y + bb7.h) { //dans 	le rectangle
                    cout << "bouton7" << endl;
                    champ = 7;
                    break;
                }
                if (event.button.x > bb8.x && event.button.x<bb8.x + bb8.w && event.button.y>bb8.y && event.button.y < bb8.y + bb8.h) { //dans 	le rectangle
                    cout << "bouton8" << endl;
                    champ = 8;
                    break;
                }
                if (event.button.x > Save.x && event.button.x<Save.x + Save.w && event.button.y>Save.y && event.button.y < Save.y + Save.h) { //dans 	le rectangle
                    cout << "sauvegarde" << endl;
                    save();
                    break;
                }
                if (event.button.x > play2.x && event.button.x<play2.x + play2.w && event.button.y>play2.y && event.button.y < play2.y + play2.h) { //dans 	le rectangle
                    cout << "play2" << endl;
                    play = true;
                    
                }
                if (event.button.x > pause.x && event.button.x<pause.x + pause.w && event.button.y>pause.y && event.button.y < pause.y + pause.h) { //dans 	le rectangle
                    cout << "pause" << endl;
                    play = false;
                }

                if (event.button.x > ss.x && event.button.x<ss.x + ss.w && event.button.y>ss.y && event.button.y < ss.y + ss.h) { //dans 	le rectangle
                    cout << "ss" << endl;
                    cout << "2bot" << endl;
                    fond(rendu);
                    timer = SDL_AddTimer(interval, event3, NULL);

                }
                if (event.button.x > ss2.x && event.button.x<ss2.x + ss2.w && event.button.y>ss2.y && event.button.y < ss2.y + ss2.h) { //dans 	le rectangle
                    cout << "ss2" << endl;
                    cout << "2bot" << endl;
                    fond(rendu);
                    timer = SDL_AddTimer(interval, event4, NULL);
                    
                }
                if (event.button.x > Load.x && event.button.x<Load.x + Load.w && event.button.y>Load.y && event.button.y < Load.y + Load.h) { //dans 	le rectangle
                    cout << "charge" << endl;
                    load();
                    for (int i = 0; i < taille; i++) {
                        SDL_Surface* image = SDL_LoadBMP("Capture.bmp");
                        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(rendu, image);
                        SDL_Rect dstrect = { 607 + (74 * i),488,72,70 };
                        SDL_RenderCopy(rendu, texture2, NULL, &dstrect);
                        SDL_DestroyTexture(texture2);

                        string tmp = to_string(parametres[i]);
                        char const* num_char = tmp.c_str();
                        positionTexte.x = 625 + (74 * i);
                        positionTexte.y = 484;
                        SDL_Texture* texture = loadText(rendu, num_char, noir, font);
                        SDL_QueryTexture(texture, NULL, NULL, &positionTexte.w, &positionTexte.h);
                        positionTexte.w *= 5;
                        positionTexte.h *= 5;
                        SDL_RenderCopy(rendu, texture, NULL, &positionTexte);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_RenderPresent(rendu);
                }
            }
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
