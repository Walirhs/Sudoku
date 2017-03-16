/**
 * @file sudoku.c
 * @brief SUDOKU
 * @author Mateus.S
 * @author Julie.A
 * @version 0.1
 * @date 17 octobre 2016
 *
 * Algo du sudoku
 *
 */
 
/*Biblioteque standard*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*Biblioteque graphique*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define MAX_S 1

/**
     * @struct Sudoku
     * @brief Objet Sudoku
     *
     * Génére une grille [9][9] 
     * le nombre de chiffre presente dans la grille
     * s'il y a une erreur dans le sudoku
     * s'il la grille du sudoku est pleine
 */
typedef struct Sudoku Sudoku;
struct Sudoku{
	int grille[9][9];
};

/**
     * @struct Arbre
     * @brief Objet Arbre_Sauvegarde
     *
     * Arbre est composé d'un sukodu
     * et de ses 9 fils 
     *     
 */
typedef struct Arbre_Sauvegarde Arbre;
struct Arbre_Sauvegarde
{
	Sudoku sudoku;
	Arbre *fils[9];
};
/**
     * @struct Valeur
     * @brief Objet Valeur contenant une ligne, une colonne et les valeurs possibles correspondantes � la
     * ligne et la colonne
     *
     * Valeur est un petit objet permet de converser la ligne, la colonne et
     * les valeurs possible correspondant à la ligne et à la colonne
     * on met dans le tableau 1 si l'indice+1 correspond àune valeur possible
     * du tableau sinon 0   
 */
typedef struct Valeur Valeur;
struct Valeur{
    int ligne;
    int colonne;
    int v_Possible[9];
};
/*Initialisation des structures*/

int difficulte;

/**
     * @brief affiche du texte a l ecran
     * @param ecran fenetre principale
     * @param x point coordonnee abscisse 
     * @param y point coordonnee ordonnee
     * @param nb caracteres a afficher
     */ 
void afficherTexte(SDL_Surface *ecran, int x, int y, char *nb, int taille)
{
	SDL_Surface *texte = NULL;
	SDL_Rect position;

	TTF_Font *police = NULL;
	police = TTF_OpenFont("DejaVuSansMono.ttf", taille);
	SDL_Color noir = {0, 0, 0};
	if(*nb=='0')*nb='-';
		texte = TTF_RenderText_Blended(police, nb, noir);

	position.x = x;
        position.y = y;
        SDL_BlitSurface(texte, NULL, ecran, &position); 

	SDL_Flip(ecran); //Mise a jour de l ecran

	//liberer memoire
	SDL_FreeSurface(texte);
	TTF_CloseFont(police);
}

/**
     * @brief affiche une ligne horizontale
     * @param x point coordonnee abscisse 
     * @param y point coordonnee ordonnee
     * @param w largeur de la ligne
     * @param ecran fenetre principale
     */ 
void ligneHorizontale(int x, int y, int w, SDL_Surface *ecran)
{
 	SDL_Rect r;
 
 	r.x = x;
 	r.y = y;
 	r.w = w;
 	r.h = 1;
 
	SDL_FillRect(ecran, &r, SDL_MapRGB(ecran->format, 0, 0, 0));
}

/**
     * @brief affiche une ligne verticale
     * @param x point coordonnee abscisse 
     * @param y point coordonnee ordonnee
     * @param h hauteur de la ligne
     * @param ecran fenetre principale
     */ 
void ligneVerticale(int x, int y, int h, SDL_Surface *ecran)
{
 	SDL_Rect r;
 
 	r.x = x;
 	r.y = y;
 	r.w = 1;
 	r.h = h;
 
 	SDL_FillRect(ecran, &r, SDL_MapRGB(ecran->format, 0, 0, 0));
}

/**
	* @brief affiche la grille de sudoku 
	* @param ecran fenetre principale
*/
void afficher_grille(SDL_Surface *ecran, Sudoku s){
	int i, j;
	char nb[81] = "";
	int x=180, y=50, c1=0, c2=0;

	//lignes contours de la grille
	ligneHorizontale(150, 40, 350, ecran);
  	ligneHorizontale(150, 390, 350, ecran);
 	ligneVerticale(150, 41, 348, ecran);
  	ligneVerticale(499, 41, 348, ecran);

	//ligne internes de la grille
	ligneHorizontale(150, 155, 350, ecran);
  	ligneHorizontale(150, 280, 350, ecran);
 	ligneVerticale(275, 41, 348, ecran);
  	ligneVerticale(385, 41, 348, ecran);


	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			sprintf(nb,"%d",s.grille[i][j]);
			afficherTexte(ecran, x+c1, y+c2, nb, 30);
			c1+=30;
			if((j+1)%3==0)
			{
				c1+=20;
			}
			if((j+1)%9==0)
			{
                		c2+=30;
				c1=0;
				if((i+1)%3==0)
					c2+=30;
			}
		}
	}

}

/**
     * @brief affiche la fenetre de resolution automatique
     * @param ecran fenetre principale
     */
void fenetre_resolution(SDL_Surface *ecran, Sudoku s)
{
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_Surface *re = NULL;
	SDL_Rect positionRe;

	positionRe.x = 0;
	positionRe.y = 0;

	re = IMG_Load("recommencer.png");
	SDL_BlitSurface(re, NULL, ecran, &positionRe);

	afficher_grille(ecran, s);
	
	SDL_Flip(ecran); //Mise a jour de l ecran

	//liberer memoire
	SDL_FreeSurface(re);
}

/**
	* @brief affiche la fenetre de creation de grille
	* @param ecran fenetre principale
*/
void fenetre_grille(SDL_Surface *ecran, Sudoku sudoku){
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255)); //nettoie l ecran
	SDL_Flip(ecran); //mise a jour ecran 
	SDL_Surface *crea = NULL;
	SDL_Rect positionCrea;

	positionCrea.x = 0;
	positionCrea.y = 0;

	crea = IMG_Load("resolution.png");
	SDL_BlitSurface(crea, NULL, ecran, &positionCrea);

	afficher_grille(ecran, sudoku);
	
	SDL_Flip(ecran); //Mise a jour de l ecran

	//liberer memoire
	SDL_FreeSurface(crea);
}

/**
     * @brief return un nombre entre 0 et a
     * @param a correspond à l'intervalle supérieur sur lequel on cherche une variable aléa
     * @return eturn un nombre entre 0 et a
     */
int fonction_aleatoire(int a)
{
	return rand()%(a+1);
}
/**
     * @brief affiche le sudoku mis en paramtre (pour les tests)
     * @param s correspond au sudoku qu'on souhaite afficher
     * @return void
     */
void affiche_sudoku(Sudoku s){
    int i,j;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            printf("%d ",s.grille[i][j]);
            if((j+1)%3==0)
                printf(" ");
            
        }            
        printf("\n");
        if((i+1)%3==0)
                printf("\n");
    }
    printf("\n");
}


/**
     * @brief return un tab de 9 case. Les cases valuent 1 si l'indice du tab n'est pas prèsente dans la ligne du sudoku sinon 0
     * @param ligne correspond à la ligne qu'on souhaite savoir les valeurs possible
     * @param s le sudoku où sur lequel on souhaite savoir les valeurs possibles
     * @return tab de 9 cases 1 dans la case si l'indice est une valeur possible sinon 0 (si la ligne contient deja l'indice=
     */
int *valeurLigne(int ligne, Sudoku s){
    int i,j,tmp[9];
    static int valeur[9];
    for(i=0;i<9;i++)
        tmp[i]=0;
    for(j=0;j<9;j++){
        for(i=0;i<9;i++){
            if(s.grille[ligne][i]==j+1)
                tmp[j]++;
        }
    }
    for(i=0;i<9;i++){
        if(tmp[i]==0)
            valeur[i]=1;
        else
            valeur[i]=0;
    }
    return valeur;
}
/**
     * @brief return un tab de 9 case. Les cases valuent 1 si l'indice du tab n'est pas prèsente dans la ligne du sudoku sinon 0
     * @param ligne correspond à la ligne qu'on souhaite savoir les valeurs possible
     * @param s le sudoku où sur lequel on souhaite savoir les valeurs possibles
     * @return tab de 9 cases 1 dans la case si l'indice est une valeur possible sinon 0 (si la ligne contient deja l'indice=
     */
int *valeurColonne(int colonne, Sudoku s){
    int i,j,tmp[9];
    static int valeur[9];
    for(i=0;i<9;i++)
        tmp[i]=0;
    for(j=0;j<9;j++){
        for(i=0;i<9;i++){
            if(s.grille[i][colonne]==j+1){
                tmp[j]++;
            }
                
        }
    }
    for(i=0;i<9;i++){
        if(tmp[i]==0)
            valeur[i]=1;
        else
            valeur[i]=0;
    }
    return valeur;
}
/**
     * @brief return un tab de 9 case. Les cases valuent 1 si l'indice du tab n'est pas prèsente dans la ligne du sudoku sinon 0
     * @param ligne correspond à la ligne qu'on souhaite savoir les valeurs possible
     * @param s le sudoku où sur lequel on souhaite savoir les valeurs possibles
     * @return tab de 9 cases 1 dans la case si l'indice est une valeur possible sinon 0 (si la ligne contient deja l'indice=
     */
int *valeurRegion(int ligne, int colonne, Sudoku s){
   
    int tmp[9],i,j,k,i_debut,j_debut;
    int *valeur=(int*)malloc(sizeof(int)*9);
    for(i=0;i<9;i++)
        tmp[i]=0;
    
    if(ligne<3 && colonne<3){
        i_debut=0;
        j_debut=0;
    }if(ligne<3 &&(colonne>=3 && colonne<6)){
        i_debut=0;
        j_debut=3;
    }if(ligne<3 && colonne>=6){
        i_debut=0;
        j_debut=6;
    }if((ligne>=3 && ligne<6) && colonne<3){
        i_debut=3;
        j_debut=0;
    }if(ligne>=6 && colonne<3){
        i_debut=6;
        j_debut=0;
    }if(ligne>=3 && ligne <6 && colonne>=3 && colonne < 6){
        i_debut=3;
        j_debut=3;
    }if(ligne>=3 && ligne <6 && colonne >= 6){
        i_debut=3;
        j_debut=6;
    }if(ligne>=6 && colonne>=3 && colonne < 6){
        i_debut=6;
        j_debut=3;
    }if(ligne>=6 && colonne>=6){
        i_debut=6;
        j_debut=6;
    }
        
    for(k=0;k<9;k++){
        for(j=j_debut;j<j_debut+3;j++){
            for(i=i_debut;i<i_debut+3;i++){
                if(s.grille[i][j]==k+1)
                    tmp[k]++;
            }
        }
    }

    for(i=0;i<9;i++){
        if(tmp[i]==0)
            valeur[i]=1;
        else
            valeur[i]=0;
    }
        
    return valeur;
}

/**
     * @brief melange les valeurs du tab
     * @param tab correspond au qu'on souhaite mélanger
     * @return void
     */
void melange_tab(int *tab)//echange chaque tab une par une dans le paquet
{
    int a,b, temps = 9;
    int tmp;

    while(temps!=0)
    {
            a=0, b=0;
            while(a==b)
            {
                    a = fonction_aleatoire(8);
                    b = fonction_aleatoire(8);
            }
            tmp = tab[b];
            tab[b]= tab[a];
            tab[a] = tmp;
            temps--;
    }

	
}


/**
     * @brief return un objet valeur qui contient les valeurs possible d'un case si on met melange à 1,c'est pour a creation d'une grille
     * @param ligne correspond à la ligne qu'on souhaite savoir les valeurs possible
     * @param colonne correspond à la colonne qu'on souhaite savoir les valeurs possible
     * @param s le sudoku où sur lequel on souhaite savoir les valeurs possibles
     * @param melange est un entier 1 si on souhaite melanger les valeurs possible sinon 0
     * @return une valeur qui est composé d'une ligne, d'une colonne et un tab
     * des valeurs possibles par rapport à la ligne et la colonne
     */
Valeur possible(int ligne, int colonne, Sudoku s,int melange){
    int i;
  
    Valeur v;
    v.ligne=ligne;
    v.colonne=colonne;
    for(i=0;i<9;i++)
        v.v_Possible[i]=0;

    for(i=0;i<9;i++)
        v.v_Possible[i]+=*(valeurLigne(ligne,s)+i);

    for(i=0;i<9;i++)
        v.v_Possible[i]+=*(valeurColonne(colonne,s)+i);

    for(i=0;i<9;i++)
        v.v_Possible[i]+=*(valeurRegion(ligne,colonne,s)+i);

    for(i=0;i<9;i++){
        if(v.v_Possible[i]==3)
             v.v_Possible[i]=i+1;
        else
             v.v_Possible[i]=0;
    }
    if(melange==1)
        melange_tab(v.v_Possible);
    
    return v;
}

/**
     * @brief affiche la valeur mis en paramtre (pour les tests)
     * @param v correspond à la valeur qu'on souhaite afficher
     * @return void
     */
void affiche_valeur(Valeur v){
    int i;
    printf("\nligne: %d\n",v.ligne);
    printf("colonne: %d\n",v.colonne);
    for(i=0;i<9;i++){
        printf("%d|",i+1);
    }
    printf("\n");
    for(i=0;i<9;i++){
        printf("%d|",v.v_Possible[i]);
    }
}
/* fonction de test*/

/**
     * @brief Regarde si la ligne n'a pas de doublon
     * @param ligne correspond à la ligne qu'on souhaite tester
     * @param s le sudoku où sur lequel on souhaite tester la ligne
     * @return 0 s'il y a un doublon, 1 sinon
     */
int ligneEstValide(int ligne, Sudoku s){
    int L[9],i,j;
    for(i=0;i<9;i++)
        L[i]=0;
    
    for(j=1;j<=9;j++){
        for(i=0;i<9;i++){
            if(s.grille[ligne][i]==j)
                L[j-1]++;
        }
    }
    
    for(i=0;i<9;i++){
        if(L[i]>1)
            return 0;
    }
    return 1;
}
/**
     * @brief Regarde si la colonne n'a pas de doublon
     * @param colonne correspond à la ligne qu'on souhaite tester
     * @param s le sudoku où sur lequel on souhaite tester la colonne
     * @return 0 s'il y a un doublon, 1 sinon
     */
int colonneEstValide(int colonne, Sudoku s){
    int C[9],i,j;
    for(i=0;i<9;i++)
        C[i]=0;
    
    for(j=1;j<=9;j++){
        for(i=0;i<9;i++){
            if(s.grille[i][colonne]==j)
                C[j-1]++;
        }
    }

    for(i=0;i<9;i++){
        if(C[i]>1)
            return 0;
    }
    return 1;
}
/**
     * @brief Regarde si la region où n'a pas de doublon
     * @param ligne correspond à la ligne qu'on souhaite tester
     * @param colonne correspond à la ligne qu'on souhaite tester
     * @param s le sudoku où sur lequel on souhaite tester la colonne
     * @return 0 s'il y a un doublon, 1 sinon
     */
int regionEstValide(int ligne,int colonne, Sudoku s){
    int R[9],i,j,k,i_debut,j_debut;
    for(i=0;i<9;i++)
        R[i]=0;
    
    if(ligne<3 && colonne<3){
        i_debut=0;
        j_debut=0;
    }if(ligne<3 &&(colonne>=3 && colonne<6)){
        i_debut=0;
        j_debut=3;
    }if(ligne<3 && colonne>=6){
        i_debut=0;
        j_debut=6;
    }if((ligne>=3 && ligne<6) && colonne<3){
        i_debut=3;
        j_debut=0;
    }if(ligne>=6 && colonne<3){
        i_debut=6;
        j_debut=0;
    }if(ligne>=3 && ligne <6 && colonne>=3 && colonne < 6){
        i_debut=3;
        j_debut=3;
    }if(ligne>=3 && ligne <6 && colonne >= 6){
        i_debut=3;
        j_debut=6;
    }if(ligne>=6 && colonne>=3 && colonne < 6){
        i_debut=6;
        j_debut=3;
    }if(ligne>=6 && colonne>=6){
        i_debut=6;
        j_debut=6;
    }
        
    for(k=0;k<9;k++){
        for(j=j_debut;j<j_debut+3;j++){
            for(i=i_debut;i<i_debut+3;i++){
                if(s.grille[i][j]==k+1)
                    R[k]++;
            }
        }
    }
   
    for(i=0;i<9;i++){
        if(R[i]>1)
            return 0;
    }
    return 1;
}

/**
     * @brief affiche le noeud mis en paramtre (pour les tests)
     * @parama abr correspond à l'arbre qu'on souhaite afficher
     * @return void
     */
void affiche_arbre(Arbre* abr){
    if(abr!=NULL){
        int i;
        affiche_sudoku(abr->sudoku);
        for(i=0;i<9;i++){
            if(abr->fils[i]==NULL)
                printf("fils[%d]=NULL",i);
            else
                printf("fils[%d] continue",i);
            printf("\n");
        }
    }
    else 
        printf("arbre null\n");
}

/**
     * @brief return le nombre de case vide du sudoku
     * @param s afin de compter les cases
     * @return nombre de cases vide du sudoku mis en paramètre
     */
int nb_case_vide(Sudoku s){
    
    int i,j,cmtp=0;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(s.grille[i][j]==0)
                cmtp++;
        }
    }
  
    return cmtp;
}
/**
     * @brief return 0 s'il y a une erreur dans le sudoku sinon 1
     * @param s le sudoku où sur lequel on souhaite savoir s'il y a une erreur
     * @return 0 si il y aucune erreur, 1 s'il n'y en aucune
     */
int aucune_erreur(Sudoku s){
    int i,j;
    for(i=0;i<9;i++){
        if(ligneEstValide(i,s)==0 || colonneEstValide(i,s)==0)
            return 0;
    }
    for(i=0;i<9;i=i+3){
          for(j=0;j<9;j=j+3){
              if(regionEstValide(i,j,s)==0)
                   return 0;
          }
    }
    
    return 1;
}
/**
     * @brief return le nombre de valeur possible d'une case
     * @param v afin de récuperer toutes les valeurs
     * @return nombre de valeur possible
     */
int nb_valeur(Valeur v){
    int i,cmpt=0;
    for(i=0;i<9;i++)
        cmpt+=v.v_Possible[i];
    return cmpt;
}
/**
     * @brief return un tab de 9 case. Les cases valuent 1 si l'indice du tab n'est pas prèsente dans la ligne du sudoku sinon 0
     * @param ligne correspond à la ligne qu'on souhaite savoir les valeurs possible
     * @param s le sudoku où sur lequel on souhaite savoir les valeurs possibles
     * @return tab de 9 cases 1 dans la case si l'indice est une valeur possible sinon 0 (si la ligne contient deja l'indice=
     */
int egale(Sudoku S1,Sudoku S2){
    int j,i,cmtp=0;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
           if(S1.grille[i][j]!=S2.grille[i][j])
               cmtp++;
        }
    }
    return cmtp;
}
/**
     * @brief return arbre avec le sudoku en parametre et initialise les fils à NULL
     * @param s le sudoku qu'on souhaite mettre dans l'arbre
     * @return return arbre avec le sudoku en parametre et initialise les fils à NULL
     */
Arbre *init_abr(Sudoku s){
    int i;
    Arbre *a=(Arbre*)malloc(sizeof(Arbre));
    a->sudoku=s;
    for(i=0;i<9;i++){
        a->fils[i]=NULL;
    }
    return a;
}
/**
     * @brief return arbre avec le sudoku avec que des cases vides et initialise les fils à NULL
     * @return return arbre avec le sudoku avec que des cases vides et initialise les fils à NULL
     */
Arbre *init_null(){
    int i,j;
    Arbre *a=(Arbre*)malloc(sizeof(Arbre));
    for(i=0;i<9;i++)
        for(j=0;j<9;j++)
            a->sudoku.grille[i][j]=0;
    for(i=0;i<9;i++){
        a->fils[i]=NULL;
    }
    return a;
}
/**
     * @brief resout le sudoku mais de facon a l'aléatoire -> utile pour la creation d'une grille
     * @param arbre dans lequel se trouve le sudoku qu'onsouhaire résoudre
     * @return resout le sudoku s'il y aucune solution toutes ls branches seront à null
     */
void creation(Arbre *abr,int *solution){
    if(*solution<MAX_S){
        int i,j,k;
        Valeur v;
        if(nb_case_vide(abr->sudoku)==0 && aucune_erreur(abr->sudoku)==1){
            (*solution)++;    
        }

        if(abr!=NULL && aucune_erreur(abr->sudoku)==1 && nb_case_vide(abr->sudoku)>0){
            int trouve=0;
            for(i=0;i<9 && trouve==0;i++){
                for(j=0;j<9 && trouve==0;j++){
                    if(abr->sudoku.grille[i][j]==0){
                        v=possible(i,j,abr->sudoku,1);
                        trouve=1;
                        for(k=0;k<9;k++){
                            if(v.v_Possible[k]!=0){
                               abr->fils[k]=init_abr(abr->sudoku);
                               abr->fils[k]->sudoku.grille[v.ligne][v.colonne]=v.v_Possible[k];

                               creation(abr->fils[k],solution);
                            }
                            else{
                               abr->fils[k]=NULL;
                            }

                        }
                    }
                }
            }
        }
    }
}
/**
     * @brief resout le sudoku et compte le nombre de solution
     * @param arbre dans lequel se trouve le sudoku qu'on souhaite r�soudre
     * @return resout le sudoku s'il y aucune solution toutes ls branches seront ànull et compte le nombre de solution
     */
void resolution(Arbre *abr,int *solution){
    if(*solution<2){
	    int i,j,k;
	    Valeur v;
	    if(nb_case_vide(abr->sudoku)==0 && aucune_erreur(abr->sudoku)==1){
		(*solution)++;    
	    }
	    if(abr!=NULL && aucune_erreur(abr->sudoku)==1 && nb_case_vide(abr->sudoku)>0)
	    {
		int trouve=0;
		for(i=0;i<9 && trouve==0;i++)
		{
		    for(j=0;j<9 && trouve==0;j++)
		    {
		        if(abr->sudoku.grille[i][j]==0)
			{
		            v=possible(i,j,abr->sudoku,0);
		            trouve=1;
		            for(k=0;k<9;k++)
			    {
		                if(v.v_Possible[k]!=0)
                                {
		                   abr->fils[k]=init_abr(abr->sudoku);
		                   abr->fils[k]->sudoku.grille[v.ligne][v.colonne]=k+1;

		                   resolution(abr->fils[k],solution);
		                }
		                else
				{
		                   abr->fils[k]=NULL;
		                }
		            }
		        }
		    }
		}
	    }
    }
}

/**
     * @brief recherche la solution du sudoku dans abr et le met dans l'arbre b, si il y en n'a pas alors b 
     * aura la meme valeur qu'avant l'entréz dans la fonction
     * @param tree contient la solution eu auparvant par la fonction résolution
     * @param b arbre qui va contenir le résultat
     * @return void
     */
void searchSudoku(Arbre *abr,Arbre *b)
{
    int i;
    
    if(aucune_erreur(abr->sudoku)==1 && nb_case_vide(abr->sudoku)==0) {
        affiche_arbre(abr);
        *b=*abr;
    }
    else{
        for(i=0;i<9;i++)
            if(abr->fils[i]!=NULL)
                searchSudoku(abr->fils[i],b);
    }
}

/**
     * @brief fonction qui desaloue l'arbre mis en paramtre
     * @param arb l'arbre qu'on souhaite désalouer
     * @return void
     */
void DesalouerArbre(Arbre *abr)
{

    int i,j;
    if (abr != NULL){
        for(i=0;i<9;i++)
            for(j=0;j<9;j++)
                abr->sudoku.grille[i][j]=0;
        for(i=0;i<9;i++){
            if (abr->fils[i] != NULL){
                DesalouerArbre(abr->fils[i]);
            }
        }
        for(j=0;j<9;j++)
            abr->fils[j]=NULL;
        free(abr);
    }
}
/**
     * @brief pour une grille de sudoku donnee, en fonction du niveau de difficulte on enleve des valeurs et on met des zeros a la place 
     * @param s correspond à la grille du sudoku
     * @param numNiveau correspond au niveau de difficulte choisi par l'utilisateur
     */
Sudoku nettoyer(Sudoku s, int numNiveau)
{
	Sudoku s2; 
	
	//affiche_sudoku(s2);
	int x, y, k;
	int i=1,j=0; 
	int nombre_solution;
	printf("\n*********niveau = %d************\n",numNiveau);
	do{
		s2=s;
		if(i%4==0)
			j++;
			
		nombre_solution=0;
		if(numNiveau ==1)
		{
			for(k=0;k<50-j;k++)
			{
				x = fonction_aleatoire(8);
				y = fonction_aleatoire(8);
				s2.grille[x][y]=0;
			}
		}
		if(numNiveau ==2)
		{
			for(k=0;k<75-j;k++)
			{
				x = fonction_aleatoire(8);
				y = fonction_aleatoire(8);
				s2.grille[x][y]=0;
			}
		}
		if(numNiveau ==3)
		{
			for(k=0;k<90-j;k++)
			{
				x = fonction_aleatoire(8);
				y = fonction_aleatoire(8);
				s2.grille[x][y]=0;
			}

		}

		Arbre *abr=init_abr(s2);
		s2=abr->sudoku;
		resolution(abr,&nombre_solution);
		printf("\nnombre de solution %d ou plus (on soustrait par %d si il cherche tr\n",nombre_solution,j);
		i++;
			
		DesalouerArbre(abr);
	}
	while(nombre_solution>1);
	
	//affiche_sudoku(s2);
	//printf("%d",numNiveau);
	return s2;
}

/**
	* @brief change la difficulte en fonction de l utilisateur
	* @param ecran fenetre principal
	* @param numNiveau indique le niveau de difficulte choisi
*/
void gerer_difficulte(SDL_Surface *ecran, int numNiveau){
	SDL_Surface *niveau = NULL;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;

	if(numNiveau == 1)
	{
		niveau = IMG_Load("Accueil_facile.png");
		difficulte=1;
	}
	if(numNiveau == 2)
	{
		niveau = IMG_Load("Accueil_moyen.png");
		difficulte=2;
	}
	if(numNiveau == 3)
	{
		niveau = IMG_Load("Accueil_difficile.png");
		difficulte=3;
	}
	//printf("%d",difficulte);
	SDL_BlitSurface(niveau, NULL, ecran, &position);
	SDL_Flip(ecran);
	SDL_FreeSurface(niveau);
}

/**
	* @brief ouvre et affiche la fenetre principale d accueil
	* @param ecran fenetre principale
*/
void ouverture_fenetre_accueil(SDL_Surface **ecran){ 
	SDL_Surface *accueil = NULL;
	SDL_Rect positionAccueil;

	//initialisation de SDL
	SDL_Init(SDL_INIT_VIDEO);

	//ouverture de la fenetre principale
	*ecran = SDL_SetVideoMode(700, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

	// Si l ouverture a echoue, on l affiche et on arrete
	if (*ecran == NULL) 
	{
        	fprintf(stderr, "Impossible de charger le mode video : %s\n", SDL_GetError());
       		exit(EXIT_FAILURE);
    	}

	SDL_WM_SetCaption("Sudoku - Accueil", NULL);

	SDL_FillRect(*ecran, NULL, SDL_MapRGB((**ecran).format, 255, 255, 255));

	positionAccueil.x = 0;
	positionAccueil.y = 0;

	accueil = IMG_Load("Accueil.png");
	SDL_BlitSurface(accueil, NULL, *ecran, &positionAccueil); 
	afficherTexte(*ecran, 180, 257, "choisissez un niveau avant de lancer", 15);

	//Mise a jour de l ecran
	SDL_Flip(*ecran); 

	//liberer memoire
	SDL_FreeSurface(accueil);
}


/**
	* @brief verifie les actions de l utilisateur et maintient la fenetre ouverte
	* @param ecran fenetre principale
*/
void pause(SDL_Surface *ecran){
	int run = 1;
	SDL_Event event;
	int lancer=0;
	int nombre = 0;
		Arbre *a=init_null();
		Arbre *resultat=init_null();
		printf("*---------------------Creation------------------------\n");
		creation(a,&nombre);
		affiche_arbre(a);
		printf("*---------------------Resultat------------------------\n");
		searchSudoku(a,resultat);
		affiche_arbre(resultat);
	while(run)
	{

	
		SDL_WaitEvent(&event); //attente d'un evt sur la fenetre
		switch(event.type)
		{
			case SDL_QUIT: //si evt quitter la fenetre
				run = 0; //on sort de la boucle
				break;
			case SDL_MOUSEBUTTONDOWN:
				if( (event.button.x >= 271) && (event.button.x<= 432)  
				&& (event.button.y >= 452) && (event.button.y <= 510) && lancer==0 )
				{
					//on appuie sur le bouton lancer
					lancer=1;
					fenetre_grille(ecran, nettoyer(resultat->sudoku, difficulte));
					break;
				}	
				else if( (event.button.x >= 55) && (event.button.x<= 240)  
				&& (event.button.y >= 282) && (event.button.y <= 405) && lancer==0)
				{
					//facile
					gerer_difficulte(ecran,1);
					//nettoyer(resultat->sudoku, 1),
					break;
				}
				else if( (event.button.x >= 241) && (event.button.x<= 437)  
				&& (event.button.y >= 282) && (event.button.y <= 405) && lancer==0)
				{
					//moyen
					gerer_difficulte(ecran,2);
					//nettoyer(resultat->sudoku, 2),
					break;
				}
				else if( (event.button.x >= 438) && (event.button.x<= 630)  
				&& (event.button.y >= 282) && (event.button.y <= 405) && lancer==0)
				{
					//difficile
					gerer_difficulte(ecran,3);
					//nettoyer(resultat->sudoku, 3),
					break;
				}	
				else if( (event.button.x >= 150) && (event.button.x<= 550)  
				&& (event.button.y >= 460) && (event.button.y <= 540) && lancer==1 )
				{
					
					//bouton resolution
					nombre = 0;					
					lancer=2;
					//Changement de grille pourla prochaine partie
					fenetre_resolution(ecran, resultat->sudoku);
					Arbre *b=init_null();
					printf("*---------------------Creation------------------------\n");
					creation(b,&nombre);
					affiche_arbre(b);
					printf("*---------------------Resultat------------------------\n");
					searchSudoku(b,resultat);
					affiche_arbre(resultat);
					DesalouerArbre(b);
					break;
				}	
				else if( (event.button.x >= 120) && (event.button.x<= 540)  
				&& (event.button.y >= 450) && (event.button.y <= 540) && lancer==2)
				{

					//bouton recommencer
					lancer=0;
					ouverture_fenetre_accueil(&ecran);
					break;
				}

		
		}
		
	}
	DesalouerArbre(a);
    DesalouerArbre(resultat);
	
}


/**
     * @brief initialise TTF afin d ecrire a l ecran
     */
void initialiser_TTF()
{
	TTF_Init();
	if(TTF_Init() == -1)
	{
    		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    		exit(EXIT_FAILURE);
	}

}

int main(int argc, char** argv){
	SDL_Surface *ecran = NULL;
	srand(time(NULL));
	
	initialiser_TTF();
	ouverture_fenetre_accueil(&ecran);



	//fenetre_resolution(ecran, resultat->sudoku);

	//garde la fenetre ouverte et reagit aux evenements
	pause(ecran);

	


	//quitter TTF
	TTF_Quit();
	//quitter SDL
	SDL_Quit(); 
	return 0;
}
