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
	float temps[13];
	int tour;
	int dossard;
}temp;

typedef struct {
	temp doss[maxEpreuves * MaxTours * nbPatEqu * nbEquEpr];
	int nbMesures;
}Mesure;
typedef struct {
	int parcours;
	int Poursuites[32]; // epreuve
	int fin[16];
	int var;
	float tempEqui[nbEquEpr * maxEpreuves];
	char Equi[nbEquEpr * maxEpreuves];
}Parcours;

void inscription_equipe(Inscrits* insc, int* dossard, int nbEquInsc, Mesure* mesu) {
	char mot[lgMot + 1];
	int d = 0;
	scanf("%s", mot);
	//printf("%s", mot);
	strcpy(insc->list[nbEquInsc].pays, mot);
	for (int i = 0; i < nbPatEqu; i++) {
		scanf("%s", mot);
		//printf("%s", mot);
		strcpy(insc->list[nbEquInsc].data[i].nom, mot);
		insc->list[nbEquInsc].data[i].dossard = *dossard + i;
		printf("inscription dossard %i\n", *dossard + i);
		//mesu->doss[*dossard + i].temps[12] = 0;
		//printf("Pour dossard: %i", *dossard + i);
		//printf(" temps initialise a: %i \n", mesu->doss[*dossard + i].temps[12]);
		d = *dossard + i;
		for (int i = 1; i < 13; ++i) {
			mesu->doss[d].tour = i;
			mesu->doss[d].temps[mesu->doss[d].tour] = 0;
			//printf(" enregistrement temps tour:%i \n Dossard:%i \n temps: %i \n", mesu->doss[d].tour, d, mesu->doss[d].temps[mesu->doss[d].tour]);
		}
	}


}
void detection_fin_poursuite(Mesure* mesu, Parcours* parc, int nbEquInsc, const Inscrits* ins, int tour);

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


	// enregistrement best temps 
	//printf(" temps a questioner: %3.1f \n", mesu->doss[d].temps[e]);
	//printf("%3.1f \n", mesu->doss[d].temps[12]);
	if (mesu->doss[d].temps[mesu->doss[d].tour] > mesu->doss[d].temps[12]) {
		//printf("Remplacement meilleur temps !!");
		mesu->doss[d].temps[12] = mesu->doss[d].temps[e];
		//printf(" temps apres prog: %3.1f \n", mesu->doss[d].temps[12]);

	}
	detection_fin_poursuite(mesu, p, nbEqInsc, ins, e);
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
void definition_parcours(Parcours* parc) {
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
	for (int l = 1; l < (nbEquInsc / 2) + 1; ++l) {
		parc->Poursuites[l] = 0;
		int d = 101;
		dInt = d + l * 6;
		//printf("L:%i \n", l);
		for (int i = 1; i < 7; ++i) {
			for (int j = dInt - 6; j < dInt; ++j) {
				if (mesu->doss[j].temps[i] > 0) {
					//printf("%3.1f \n", mesu->doss[j].temps[i]);
					//printf("%d \n", j);
					parc->Poursuites[l] = parc->Poursuites[l] + 1;
					//printf("%i \n", parc->Poursuites[l]);
				}
			}
		}
		if (p == (parc->Poursuites[l] / 6) && parc->fin[l] == 0) {
			printf("detection_fin_poursuite \n");
			parc->fin[l] = 1;
			int dossa = 6 * l + 95; //nbEquEpr * nbPatEqu = 6
			int t = 12;
			int j = parc->var;
			//printf("%i", parc->var);
			//for (j; j < nbEquInsc; j++) {
			int E1;
			int E2;
			float tmp1;
			float tmp2;

			if (mesu->doss[dossa + 1].temps[t] < mesu->doss[dossa].temps[t] && mesu->doss[dossa + 2].temps[t] < mesu->doss[dossa].temps[t]) {
				//printf("%s ", ins->list[j].pays);
				//parc->Equi[parc->var] = j;
				E1 = j;
				//("%3.1f \n", mesu->doss[dossa].temps[t]);
				//parc->tempEqui[parc->var] = mesu->doss[dossa].temps[t];
				tmp1 = mesu->doss[dossa].temps[t];
				parc->var = parc->var + 1;
			}
			if (mesu->doss[dossa].temps[t] < mesu->doss[dossa + 1].temps[t] && mesu->doss[dossa + 2].temps[t] < mesu->doss[dossa + 1].temps[t]) {
				//printf("%s ", ins->list[j].pays);
				//parc->Equi[parc->var] = j;
				E1 = j;
				//printf("%3.1f \n", mesu->doss[dossa + 1].temps[t]);
				//parc->tempEqui[parc->var] = mesu->doss[dossa + 1].temps[t];
				tmp1 = mesu->doss[dossa + 1].temps[t];
				parc->var = parc->var + 1;
			}
			if (mesu->doss[dossa + 1].temps[t] < mesu->doss[dossa + 2].temps[t] && mesu->doss[dossa].temps[t] < mesu->doss[dossa + 2].temps[t]) {
				//printf("%s ", ins->list[j].pays);
				E1 = j;
				//parc->Equi[parc->var] = j;
				//printf("%3.1f \n", mesu->doss[dossa + 2].temps[t]);
				//parc->tempEqui[parc->var] = mesu->doss[dossa + 2].temps[t];
				tmp1 = mesu->doss[dossa + 2].temps[t];
				parc->var = parc->var + 1;
			}
			dossa = dossa + 3;

			if (mesu->doss[dossa + 1].temps[t] < mesu->doss[dossa].temps[t] && mesu->doss[dossa + 2].temps[t] < mesu->doss[dossa].temps[t]) {
				//printf("%s ", ins->list[j + 1].pays);
				E2 = j + 1;
				//parc->Equi[parc->var] = j + 1;
				//printf("%3.1f \n", mesu->doss[dossa].temps[t]);
				//parc->tempEqui[parc->var] = mesu->doss[dossa].temps[t];
				tmp2 = mesu->doss[dossa].temps[t];
				parc->var = parc->var + 1;
			}
			if (mesu->doss[dossa].temps[t] < mesu->doss[dossa + 1].temps[t] && mesu->doss[dossa + 2].temps[t] < mesu->doss[dossa + 1].temps[t]) {
				//printf("%s ", ins->list[j+1].pays);
				//parc->Equi[parc->var] = j+1;
				E2 = j + 1;
				//printf("%3.1f \n", mesu->doss[dossa + 1].temps[t]);
				//parc->tempEqui[parc->var] = mesu->doss[dossa + 1 ].temps[t];
				tmp2 = mesu->doss[dossa + 1].temps[t];
				parc->var = parc->var + 1;
			}
			if (mesu->doss[dossa + 1].temps[t] < mesu->doss[dossa + 2].temps[t] && mesu->doss[dossa].temps[t] < mesu->doss[dossa + 2].temps[t]) {
				//printf("%s ", ins->list[j+1].pays);
				//parc->Equi[parc->var] = j+1;
				E2 = j + 1;
				//printf("%3.1f \n", mesu->doss[dossa + 2].temps[t]);
				//parc->tempEqui[parc->var] = mesu->doss[dossa + 2].temps[t];
				tmp2 = mesu->doss[dossa + 2].temps[t];
				parc->var = parc->var + 1;
			}
			dossa = dossa + 3;

			//printf("%i\n",dossa);
			if (tmp2 > tmp1) {

				printf("%s ", ins->list[E1].pays);
				printf("%3.1f \n", tmp1);
				printf("%s ", ins->list[E2].pays);
				printf("%3.1f \n", tmp2);
			}
			else
			{

				printf("%s ", ins->list[E2].pays);
				printf("%3.1f \n", tmp2);
				printf("%s ", ins->list[E1].pays);
				printf("%3.1f \n", tmp1);


			}
			//}
		//dossa = dossa + 3;
		}


	}
}

int main() {
	int dossard = 101;
	int nbEqInsc = 0;
	Mesure m;
	Inscrits i;
	Parcours p;
	p.var = 0;
	char mot[lgMot + 1];
	i.nbIns = 0;

	for (int i = 0; i < 10; ++i) {
		p.fin[i] = 0;
	}
	// -> remplace 8 lignes
	//p.fin[1] = 0;
	//p.fin[2] = 0;
	//p.fin[3] = 0;
	//p.fin[4] = 0;
	//p.fin[5] = 0;
	//p.fin[6] = 0;
	//p.fin[7] = 0;
	//p.fin[8] = 0;
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
			enregistrer_temps(&m, nbEqInsc, &p, &i);
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
			exit(0);
			// sortie du programme principal 
		}
	}
	system("pause");
	return 0;
}
