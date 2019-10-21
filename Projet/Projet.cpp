#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable: 4996)

#define MaxTours 10 // Nombre maximum de tours
#define maxEpreuves 16 // Nombre maximal d’épreuves
#define lgMot 30 //taille maximale d’une chaîne de caractère lu
#define nbPatEqu 3 // Le nombre de patineurs par équipe
#define nbEquEpr 2 // Le nombre d’équipes par épreuve

typedef struct {
	char nom[lgMot + 1];
	int dossard;
} Patineur;

typedef struct {
	Patineur data[nbPatEqu];
	char pays[lgMot];
} Equipe;

typedef struct {
	Equipe list[nbPatEqu];
	int nbIns;
} Inscrits;

typedef struct {
	Equipe NbEqEp[nbEquEpr];
}Epreuve;

typedef struct {
	float temps[11];
	int tour;
	int dossard;
}temp;

typedef struct {
	temp doss[maxEpreuves * MaxTours * nbPatEqu * nbEquEpr];
	int nbMesures;
}Mesure;
typedef struct {
	int parcours;
}Parcours;

void inscription_equipe(Inscrits* insc, int* dossard, int nbEquInsc, Mesure* mesu) {
	char mot[lgMot + 1];
	int d = 0;
	scanf("%s", mot);
	strcpy(insc->list[nbEquInsc].pays, mot);
	for (int i = 0; i < nbPatEqu; i++) {
		scanf("%s", mot);
		strcpy(insc->list[nbEquInsc].data[i].nom, mot);
		insc->list[nbEquInsc].data[i].dossard = *dossard + i;
		printf("inscription dossard %i\n", *dossard + i);
		d = *dossard + i;
		for (int i = 1; i < 11; ++i) {
			mesu->doss[d].tour = i;
			mesu->doss[d].temps[mesu->doss[d].tour] = 0;
			//printf(" enregistrement temps tour:%i \n Dossard:%i \n temps: %i \n", mesu->doss[d].tour, d, mesu->doss[d].temps[mesu->doss[d].tour]);
		}
	}


}void detection_fin_poursuite(Mesure* mesu, Parcours* parc, int nbEquInsc, const Inscrits* ins, int tour);

void affichage_equipe(const Inscrits* ins, int nbEquInsc) {
	for (int j = 0; j < nbEquInsc; j++) {
		printf("%s ", ins->list[j].pays);
		for (int i = 0; i < nbPatEqu; i++) {
			printf("%s ", ins->list[j].data[i].nom); //affiche le nom du joueur
			printf("%i ", ins->list[j].data[i].dossard); //affiche le dossard du joueur
		}
		printf("\n"); //saut de ligne
	}
}

void enregistrer_temps(Mesure* mesu, int nbEqInsc, Parcours* p, Inscrits* ins) {
	int d = 0;
	int e = 0;
	float tmp[lgMot + 1];
	scanf("%i", &d);
	scanf("%i", &e);
	//printf("%i", e);
	mesu->doss[d].tour = e;
	scanf("%f", tmp);
	mesu->doss[d].temps[mesu->doss[d].tour] = *tmp;
	detection_fin_poursuite( mesu, p, nbEqInsc, ins, e);
}
void afficher_temps(const Mesure* mesu, const Inscrits* ins, int nbEquInsc) {
	int d = 0;
	scanf("%i", &d);
	int test;
	for (int j = 0; j < nbEquInsc + 1; j++) {
		for (int i = 0; i < nbPatEqu; i++) {
			if (ins->list[j].data[i].dossard == d) {
				for (int k = 1; k < 11; ++k) {
					test = mesu->doss[d].temps[k];
					// printf("%i", test);
					if (test != 0.0) {
						printf("%s ", ins->list[j].pays);
						printf("%i ", k);
						printf("%s ", ins->list[j].data[i].nom);
						printf("%3.1f \n", mesu->doss[d].temps[k]);
					}
				}
			}
		}
	}
}
void affichage_temps_equipes(Mesure* mesu, const Inscrits* ins, int* dossard, int nbEquInsc) {
	int t = 1;
	int d = 101;
	int dmax = nbEquInsc * 3 + 100;
	scanf("%i", &t);
	for (int j = 0; j < nbEquInsc; j++) {
		printf("%s ", ins->list[j].pays);
		if (mesu->doss[d].temps[t] == 0 || mesu->doss[d + 1].temps[t] == 0 || mesu->doss[d + 2].temps[t] == 0)
			printf("Impossible \n");
		else {
			if (mesu->doss[d + 1].temps[t] < mesu->doss[d].temps[t] && mesu->doss[d + 2].temps[t] < mesu->doss[d].temps[t]) {
				printf("%3.1f \n", mesu->doss[d].temps[t]);
			}
			if (mesu->doss[d].temps[t] < mesu->doss[d + 1].temps[t] && mesu->doss[d + 2].temps[t] < mesu->doss[d + 1].temps[t]) {
				printf("%3.1f \n", mesu->doss[d + 1].temps[t]);
			}
			if (mesu->doss[d + 1].temps[t] < mesu->doss[d + 2].temps[t] && mesu->doss[d].temps[t] < mesu->doss[d + 2].temps[t]) {
				printf("%3.1f \n", mesu->doss[d + 2].temps[t]);
			}
		}
		d = d + 3;
	}
}
void definition_parcours(Parcours* parc){
	int p = 0;
	scanf("%i", &parc->parcours);
	if (parc->parcours < 1 || parc->parcours > 11) 
		printf("Impossible \n"); 
	
}
void detection_fin_poursuite(Mesure* mesu, Parcours* parc, int nbEquInsc, const Inscrits* ins, int tour) {
	int p = parc->parcours;
	int dmax = nbEquInsc * 3 + 100;
	int dInt = 0;
	int EqInsc = nbEquInsc;
	for (int l = 1; l < (nbEquInsc / 2) +1; ++l) {
		int k = 0;
		int d = 101;
		dInt = d + l * 6;
		for (int i = 1; i < p + 1; ++i) {
			for (int j = dInt - 6; j < dInt; ++j) {
				if (mesu->doss[j].temps[i] != 0) {
					k++;
				}
			}
		}
		if (p == (k / 6)) {
			printf("dection_fin_poursuite \n");
		}
	}
}

int main() {
	int dossard = 101;
	int nbEqInsc = 0;
	Mesure m;
	Inscrits i;
	Parcours p;
	char mot[lgMot + 1];
	i.nbIns = 0;
	while (1) {
		// Lecture de la commande (mot) 
		scanf("%s", mot);
		// si la commande est "inscrire_equipe" 
		if (strcmp(mot, "inscrire_equipe") == 0) {
			inscription_equipe(&i, &dossard, nbEqInsc, &m);
			nbEqInsc++;
			dossard = dossard + 3;
			//    appeler la fonction inscription_equipes
		}
		if (strcmp(mot, "afficher_equipes") == 0) {
			affichage_equipe(&i, nbEqInsc);
			//    appeler la fonction afficher_equipes
		}
		if (strcmp(mot, "enregistrer_temps") == 0) {
			enregistrer_temps(&m, nbEqInsc, &p,&i);
			//    appeler la fonction enregistrement_temps
		}
		if (strcmp(mot, "afficher_temps") == 0) {
			afficher_temps(&m, &i, nbEqInsc);
			//    appeler la fonction afficher_equipes
		}
		if (strcmp(mot, "afficher_temps_equipes") == 0) {
			affichage_temps_equipes(&m, &i, &dossard, nbEqInsc);
			//    appeler la fonction afficher_temps_equipes
		}
		if (strcmp(mot, "definir_parcours") == 0) {
			definition_parcours(&p);
			//    appeler la fonction definition_parcours
		}
		// si la commande est "exit". 
		if (strcmp(mot, "exit") == 0) {
			printf("Sortie du programme principal...");
			exit(0);
			// sortie du programme principal 
		}
	}
	system("pause");
	return 0;
}