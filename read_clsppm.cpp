#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include<stdbool.h>

using namespace std;

//LargeSizeProblem: M = 15, I = 100, T = 24
//J30T24M12-instance: M = 12, I = 30, T = 24
//J35T20M15-instance: M = 15, I = 35, T = 20

#define M 12 //machine
#define I 30 //item
#define T 24 //period

struct info_mac {
  int machine;
  float value;
};

struct item_mac {
  int item;
  int machine;
  float value;
};

struct demand {
  int item;
  int period;
  int value;
};

struct prod_time {
  int item;
  int machine;
  double value;
};

struct prod_cost {
  int item;
  int machine;
  double value;
};

struct prod_emi {
  int item;
  int machine;
  float value;
};

struct inv_cost {
  int item;
  float value;
};

struct setup_time {
  int item;
  int machine;
  double value;
};

struct setup_emi {
  int item;
  int machine;
  double value;
};

struct fixed_cost {
  int machine;
  int value;
};

struct cap_mac {
  int machine;
  int period;
  double value;
};

struct lim_carb {
  int period;
  double value;
};

int main (int argc, char** argv)
{
  FILE* fpin = NULL;
  FILE* fpout = NULL;

  int m, i, t, k;
  char ch;
  char linha[200];
  bool status;
  int n;
  int passo;
  
  typedef struct info_mac *info;
  typedef struct item_mac *imac;
  typedef struct demand *dem;
  typedef struct prod_time *pt;
  typedef struct prod_cost *pc;
  typedef struct prod_emi *pe;
  typedef struct inv_cost *ic;
  typedef struct setup_time *st;
  typedef struct setup_emi *se;
  typedef struct fixed_cost *fc;
  typedef struct cap_mac *cm;
  typedef struct lim_carb *em;

  info INFO[M];
  imac IMAC[I*M+M];
  dem D[I*T+T];
  pt PT[I*M+M];
  pc PC[I*M+M];
  pe PE[I*M+M];
  ic IC[I];
  st ST[I*M+M];
  se SE[I*M+M];
  fc FC[M];
  cm CM[M*T+T];
  em EM[T];

  passo = 1;
  
  for (m=0; m<M; m++) {
    INFO[m] = NULL;
  }

  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M + m;
      IMAC[k] = NULL;
    }
  }

  for (i=0; i<I; i++) {
    for (t=0; t<T; t++) {
      k = i*T + t;
      D[k] = NULL;
    }
  }

  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M + m;
      PT[k] = NULL;
    }
  }
 
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      PC[k] = NULL;
    }
  }

  
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      PE[k] = NULL;
    }
  }

  for (i=0; i<I; i++) {
    IC[i] = NULL;
  }

  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      ST[k] = NULL;
    }
  }

  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      SE[k] = NULL;
    }
  }

  for (m=0; m<M; m++) {
    FC[m] = NULL;
  }

  for (m=0; m<M; m++) {
    for (t=0; t<T; t++) {
      k = m*T+t;
      CM[k] = NULL;
    }
  }

  for (t=0; t<T; t++) {
    EM[t] = NULL;
  }

    
  fpin = fopen(argv[1], "r");
  if (fpin == NULL) {
    fprintf(stderr, "nao pode abrir %s \n", argv[1]);
    return 0;
  }

  while (fgets(linha, 200, fpin) != NULL) {
    n = sscanf(linha," %c ",&ch);

    if (ch == 'I' && passo == 1) {
      //printf("informacoes \n");
      for (m = 0; m < M; m++) {
	if (INFO[m] == NULL) INFO[m] = (info)malloc(sizeof(info));
	fscanf(fpin, "%d %f", &INFO[m]->machine, &INFO[m]->value);
	//printf("%d %.2f \n", INFO[m]->machine, INFO[m]->value);
      }
      passo = 2;
      ch = '\n';
    }
    
    if (ch == 'i' && passo == 2) {
      //printf("item maquina \n");
      for (i = 0; i < I; i++) {
	for (m = 0; m < M; m++) {
	  k = i*M+m;
	  if (IMAC[k] == NULL) IMAC[k] = (imac)malloc(sizeof(imac));
	  fscanf(fpin, "%d %d %f", &IMAC[k]->item, &IMAC[k]->machine, &IMAC[k]->value);
	  //printf("%d %d %.2f\n", IMAC[k]->item, IMAC[k]->machine, IMAC[k]->value);
	}
      }
      passo = 3;
      ch = '\n';
    }

    if (ch == 'd' && passo == 3) {
      //printf("demanda \n");
      for (i = 0; i < I; i++) {
	for (t = 0; t < T; t++) {
	  k = i*T+t;
	  if (D[k] == NULL) D[k] = (dem)malloc(sizeof(dem));
	  fscanf(fpin, "%d %d %d", &D[k]->item, &D[k]->period, &D[k]->value);
	  //printf("%d %d %d\n", D[k]->item, D[k]->period, D[k]->value);
	}
      }
      passo = 4;
      ch = '\n';
    }
    
    status = true;
    if (ch == 'p' && passo == 4) {
      //printf("tempo producao \n");
      for (i=0; i<I; i++) {
	for (m=0; m<M; m++) {
	  k = i*M+m;
	  if (PT[k] == NULL) PT[k] = (pt)malloc(sizeof(pt));
	  fscanf(fpin, "%d %d %lf", &PT[k]->item, &PT[k]->machine, &PT[k]->value);
	  //printf("%d %d %f\n", PT[k]->item, PT[k]->machine, PT[k]->value);
          if (PT[k]->item < 1 || PT[k]->item > I) {
	    status = false;
	    break;
	  }
	}
	if (status == false) break;
      }
      PT[k] = NULL;
      passo = 5;
      ch = '\n';
    }
    
    status = true;
    if (ch == 'p' && passo == 5) {
      //printf("custo producao \n");
      for (i=0; i<I; i++) {
	for (m=0; m<M; m++) {
	  k = i*M+m;
	  if (PC[k] == NULL) PC[k] = (pc)malloc(sizeof(pc));
	  fscanf(fpin, "%d %d %lf", &PC[k]->item, &PC[k]->machine, &PC[k]->value);
	  //printf("%d %d %f\n", PC[k]->item, PC[k]->machine, PC[k]->value);
          if (PC[k]->item < 1 || PC[k]->item > I) {
	    status = false;
	    break;
	  }
	}
	if (status == false) break;
      }
      PC[k] = NULL;
      passo = 6;
      ch = '\n';
    }


    status = true;
    if (ch == 'p' && passo == 6) {
      //printf("emissao producao \n");
      for (i = 0; i < I; i++) {
	for (m = 0; m < M; m++) {
	  k = i*M+m;
	  if (PE[k] == NULL) PE[k] = (pe)malloc(sizeof(pe));
	  fscanf(fpin, "%d %d %f", &PE[k]->item, &PE[k]->machine, &PE[k]->value);
	  //printf("%d %d %f\n", PE[k]->item, PE[k]->machine, PE[k]->value);
          if (PE[k]->item < 1 || PE[k]->item > I) {
	    status = false;
	    break;
	  }
	}
	if (status == false) break;
      }
      PE[k] = NULL;
      passo = 7;
      ch = '\n';
    }

    if (ch == 'h' && passo == 7) {
      //printf("inventary producao \n");
      for (i=0; i<I; i++) {
	if (IC[i] == NULL) IC[i] = (ic)malloc(sizeof(ic));
	fscanf(fpin, "%d %f", &IC[i]->item, &IC[i]->value);
	//printf("%d %.2f \n", IC[i]->item, IC[i]->value);
      }
      passo = 8;
      ch = '\n';
    }

    status =true;
    if (ch == 's' && passo == 8) {
      //printf("setup time \n");
      for (i=0; i<I; i++) {
	for (m=0; m<M; m++) {
	  k = i*M+m;
	  if (ST[k] == NULL) ST[k] = (st)malloc(sizeof(st));
	  fscanf(fpin, "%d %d %lf", &ST[k]->item, &ST[k]->machine, &ST[k]->value);
	  //printf("%d %d %f\n", ST[k]->item, ST[k]->machine, ST[k]->value);
          if (ST[k]->item < 1 || ST[k]->item > I) {
	    status = false;
	    break;
	  }
	}
	if (status == false) break;
      }
      ST[k] = NULL;
      passo = 9;
      ch = '\n';
    }
    
    status = true;
    if (ch == 's' && passo == 9) {
      //printf("setup emissao \n");
      for (i=0; i<I; i++) {
	for (m=0; m<M; m++) {
	  k = i*M+m;
	  if (SE[k] == NULL) SE[k] = (se)malloc(sizeof(se));
	  fscanf(fpin, "%d %d %lf", &SE[k]->item, &SE[k]->machine, &SE[k]->value);
	  //printf("%d %d %f\n", SE[k]->item, SE[k]->machine, SE[k]->value);
          if (SE[k]->item < 1 || SE[k]->item > I) {
	    status = false;
	    break;
	  }
	}
	if (status == false) break;
      }
      SE[k] = NULL;
      passo = 10;
      ch = '\n';
    }
    
    if (ch == 'b' && passo == 10) {
      //printf("fixed cost \n");
      for (m=0; m<M; m++) {
	if (FC[m] == NULL) FC[m] = (fc)malloc(sizeof(fc));
	fscanf(fpin, "%d %d", &FC[m]->machine, &FC[m]->value);
	//printf("%d %d \n", FC[m]->machine, FC[m]->value);
	if (FC[m]->machine < 1 || FC[m]->machine > M) break;
      }
      FC[m] = NULL;
      passo = 11;
      ch = '\n';
    }

    status = true;
    if (ch == 'c' && passo == 11) {
      //printf("capacidade machine \n");
      for (m= 0; m<M; m++) {
	for (t=0; t<T; t++) {
	  k = m*T+t;
	  if (CM[k] == NULL) CM[k] = (cm)malloc(sizeof(cm));
	  fscanf(fpin, "%d %d %lf", &CM[k]->machine, &CM[k]->period, &CM[k]->value);
	  //printf("%d %d %f\n", CM[k]->machine, CM[k]->period, CM[k]->value);
          if (CM[k]->machine < 1 || CM[k]->machine > M) {
	    status = false;
	    break;
	  }
	}
	if (status == false) break;
      }
      CM[k] = NULL;
      passo = 12;
      ch = '\n';
    }

    if (ch == 'e' && passo == 12) {
      //printf("emissao de carbono \n");
      for (t=0; t<T; t++) {
	if (EM[t] == NULL) EM[t] = (em)malloc(sizeof(em));
	fscanf(fpin, "%d %lf", &EM[t]->period, &EM[t]->value);
	//printf("%d %f \n", EM[t]->period, EM[t]->value);
      }
      passo = 13;
      ch = '\n';
    }
    /*end while*/
  }

  /*print instances*/

  printf("********* information **************\n");
  for (m = 0; m < M; m++) {
    if (INFO[m] != NULL) {
      printf("%d %.2f \n", INFO[m]->machine, INFO[m]->value);
    }
  }

  printf("******** item machine ***************\n");
  for (i = 0; i < I; i++) {
    for (m = 0; m < M; m++) {
      k = i*M+m;
      if (IMAC[k] != NULL) {
	printf("%d %d %.2f\n", IMAC[k]->item, IMAC[k]->machine, IMAC[k]->value);
      }
    }
  }

  printf("******** demand ***************\n");
  for (i = 0; i < I; i++) {
    for (t = 0; t < T; t++) {
      k = i*T+t;
      if (D[k] != NULL) {
	printf("%d %d %d\n", D[k]->item, D[k]->period, D[k]->value);
      }
    }
  }
  
  //CONTROLE
  printf("********** time produce *************\n");
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (PT[k] != NULL) {
	printf("%d %d %f\n", PT[k]->item, PT[k]->machine, PT[k]->value);
      }
    }
  }
  
  //CONTROLE
  printf("********* costs of producing **************\n");
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (PC[k] != NULL) {
	printf("%d %d %f\n", PC[k]->item, PC[k]->machine, PC[k]->value);
      }
    }
  }

  //controle
  printf("********* production emission **************\n");
  for (i = 0; i < I; i++) {
    for (m = 0; m < M; m++) {
      k = i*M+m;
      if (PE[k] != NULL) {
	printf("%d %d %f\n", PE[k]->item, PE[k]->machine, PE[k]->value);
      }
    }
  }

  printf("******** inventory costs ***************\n");
  for (i=0; i<I; i++) {
    if (IC[i] != NULL) {
      printf("%d %.2f \n", IC[i]->item, IC[i]->value);
    }
  }
  
  //controle
  printf("********  setup time ***************\n");
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (ST[k] != NULL) {
	printf("%d %d %f\n", ST[k]->item, ST[k]->machine, ST[k]->value);
      }
    }
  }
  
  //CONTROLE
  printf("******** setup emission ***************\n");
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (SE[k] != NULL) {
	printf("%d %d %f\n", SE[k]->item, SE[k]->machine, SE[k]->value);
      }
    }
  }
  
  //controle
  printf("******* fixed cost ****************\n");
  for (m=0; m<M; m++) {
    if (FC[m] != NULL) {
      printf("%d %d \n", FC[m]->machine, FC[m]->value);
    }
  }
  
  //controle
  printf("******* capacity machine ****************\n");
  for (m= 0; m<M; m++) {
    for (t=0; t<T; t++) {
      k = m*T+t;
      if (CM[k] != NULL) {
	printf("%d %d %f\n", CM[k]->machine, CM[k]->period, CM[k]->value);
      }
    }
  }
  
  //controle
  printf("******* ccarbono emissions  ****************\n");
  for (t=0; t<T; t++) {
    if (EM[t] != NULL) {
      printf("%d %f \n", EM[t]->period, EM[t]->value);
    }
  }

  /*end print*/
  
  
  for (m=0; m<M; m++) {
    if (INFO[m] != NULL) {
      free(INFO[m]);
      INFO[m] = NULL;
    }
  }
  
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (IMAC[k] != NULL) {
	free(IMAC[k]);
	IMAC[k] = NULL;
      }
    }
  }
  
  for (i=0; i<I; i++) {
    for (t=0; t<T; t++) {
      k = i*T+t;
      if (D[k] != NULL) {
	free(D[k]);
	D[k] = NULL;
      }
    }
  }
  
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (PT[k] != NULL) {
	free(PT[k]);
	PT[k] = NULL;
      }
    }
  }
  
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (PC[k] != NULL) {
	free(PC[k]);
	PC[k] = NULL;
      }
    }
  }
  
  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (PE[k] != NULL) {
	free(PE[k]);
	PE[k] = NULL;
      }
    }
  }

  for (i=0; i<I; i++) {
    if (IC[i] != NULL) {
      free(IC[i]);
      IC[i] = NULL;
    }
  }

  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (ST[k] != NULL) {
	free(ST[k]);
	ST[k] = NULL;
      }
    }
  }

  for (i=0; i<I; i++) {
    for (m=0; m<M; m++) {
      k = i*M+m;
      if (SE[k] != NULL) {
	free(SE[k]);
	SE[k] = NULL;
      }
    }
  }

  for (m=0; m<M; m++) {
    if (FC[m] != NULL) {
      free(FC[m]);
      FC[m] = NULL;
    }
  }

  for (m=0; m<M; m++) {
    for (t=0; t<T; t++) {
      k = m*T+t;
      if (CM[k] != NULL) {
	free(CM[k]);
	CM[k] = NULL;
      }
    }
  }

  for (t=0; t<T; t++) {
    if (EM[t] != NULL) {
      free(EM[t]);
      EM[t] = NULL;
    }
  }
 
  if (fpin) { fclose(fpin); fpin = NULL;}
  if (fpout) { fclose(fpout); fpout = NULL;}
  
  return 0;
  
}
