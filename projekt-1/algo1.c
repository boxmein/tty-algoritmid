// Programm sorteerimisalgoritmide võrdlemiseks
// Johannes Kadak - 2017
// 
// Kasutamiseks kompileeri ja jooksuta.
// Esmalt küsib programm arvude hulka (täisarvuna), 
// siis iga arvu tühikuga eraldatult.
// Siis väljastab programm iga sortimisalgoritmi tulemuse
// ja aja, mis kulus sorteerimiseks.


// Standardväljundi jaoks. 
#include <stdio.h>
#include <wchar.h>

// malloc() / free() funktsioonide jaoks
#include <stdlib.h>

// vigade graatsiliseks väljastamiseks
#include <errno.h>
// strerror() funktsiooni jaoks
#include <string.h>

// Aja mõõtmiseks programmi töö käigus - clock()
#include <time.h>

// usleep() käsu jaoks, et algoritme aeglustada
#include <unistd.h>

// msleep() on funktsioon, mis võimaldab mugavalt millisekundite kaupa
// oodata.
void msleep(int millis) {
  usleep(millis * 1000);
}

// swap() on funktsioon, mis vahetab massiivis kohal idx_a ja idx_b elemendid.
void swap(int *numbrid, int idx_a, int idx_b) {
  int temp = numbrid[idx_a];
  numbrid[idx_a] = numbrid[idx_b];
  numbrid[idx_b] = temp;
}

// valjasta() on funktsioon, mis väljastab terve massiivi ühel real.
void valjasta(int *numbrid, int pikkus) {
  for (int i = 0; i < pikkus; i++) {
    wprintf(L"%d ", numbrid[i]);
  }
  wprintf(L"\n");
}

// quicksort_sise on funktsioon, mis sorteerib alamvahemiku massiivist
// jagades massiivi omakorda alamvahemikeks ja rekursiooni rakendades.
// meetodit nimetatakse quicksort-iks.
// idx_vasak ja idx_parem on vasaku ja parema lõike serva indeksid
void quicksort_sise(int *numbrid, int idx_vasak, int idx_parem) {
  if (idx_vasak < idx_parem) {
    int keskkoht = numbrid[idx_vasak];
    int i = idx_vasak + 1;
    int j = idx_parem;

    while (i < j) {

      while (numbrid[j] <= keskkoht && i < idx_parem) {
	i++;
	msleep(10);
      }

      while (numbrid[j] > keskkoht && j > idx_vasak) {
	j--;
	msleep(10);
      }

      if (i < j) {
	swap(numbrid, i, j);
	msleep(10);
      } 
    }

    if (keskkoht > numbrid[j]) {
      numbrid[idx_vasak] = numbrid[j];
      numbrid[j] = keskkoht;
    }

    quicksort_sise(numbrid, idx_vasak, j - 1);
    quicksort_sise(numbrid, i, idx_parem);
    msleep(10);
  }
}

// quicksort() on funktsioon, mille signatuur on sama nagu teised sortimis
// funktsioonid, mistõttu on seda lihtsam kasutada. kasutab oma tööks ära
// quicksort_sise() funktsiooni.
void quicksort(int *numbrid, int pikkus) {
  quicksort_sise(numbrid, 0, pikkus - 1); 
}

// bubblesort() on funktsioon, mis sorteerib massiivi kasutades mullimeetodit
// mille käigus käiakse üle nimekirja ja vahetatakse massiivi elemendid, kuni
// enam vahetusi pole vaja.
void bubblesort(int *numbrid, int pikkus) {
  int vahetusi_see_tsykkel = 1;

  while (vahetusi_see_tsykkel > 0) {
    vahetusi_see_tsykkel = 0; 
    for (int i = 0; i < pikkus-1; i++) {
      if (numbrid[i] > numbrid[i + 1]) {
	swap(numbrid, i, i + 1);
	vahetusi_see_tsykkel++;
	msleep(10);
      }
    }
    msleep(10);
  }
}


// selectionsort() on funktsioon, mis sorteerib massiivi, kasutades valikumeetodit
// valikumeetodi käigus leitakse iga massiivi elemendi kohale kõige väiksem
// sorteerimata osast.
// funktsioonis on sleep() käsud, kuna jookseb muidu väga kiiresti.
void selectionsort(int *numbrid, int pikkus) {
  // leia minimaalne sorteerimata listist, vaheta sorteerimata listi vasakpoolsega ära

  for (int i = 0; i < pikkus; i++) {
    int idx_min = i;
    for (int j = i; j < pikkus; j++) {
      if (numbrid[j] < numbrid[idx_min]) {
	idx_min = j;
      }
      msleep(10);
    }
    msleep(10);
    swap(numbrid, i, idx_min);
  }
}

// main() - loeb sisendist massiivi ja sorteerib selle, kasutades kolme meetodit.
// väljastab meetodite kiirused.
int main() {
  int pikkus = 0;

  wprintf(L"--- Sorteerimisalgoritmide võrdlus ---\n"
	  L"Kolm sortimisalgoritmi, iga tsükli peal 10ms viitega.\n"
	  L"Anna ette sisend, et näha, kui kiiresti iga jookseb.\n");

  // Küsime sisendi pikkuse, et võtta mälu vastavalt vajadusele
  
  wprintf(L"Sisendi pikkus: ");
  scanf("%d", &pikkus);

  if (pikkus < 2) {
    wprintf(L"Sisendis on vaja vähemalt kaht numbrit.\n");
    return 1;
  }

  // Võtame mälu sisendi jagu ja ühe koopia jagu (et selle peal sorteerida ja
  // originaalandmed alles hoida)

  int *arvud_originaal = (int *) malloc(pikkus * sizeof(int));
  int *arvud_sort = (int *) malloc(pikkus * sizeof (int));

  if (arvud_originaal == NULL || arvud_sort == NULL) {
    wprintf(L"Ei saanud piisavalt mälu võtta, et mahutada arve.\n");
    wprintf(L"%s\n", strerror(errno));
    return 1;
  }

  // Loeme sisse sisendi, massiivi täisarve.
  
  wprintf(L"Sisesta arvud, eralda tühikuga.\n");

  for (int i = 0; i < pikkus; i++) {
    scanf("%d", arvud_originaal + i);
  }

  // Kopeerime arvud ajutisse massiivi, mida funktsioonid sorteerivad.
  
  memcpy(arvud_sort, arvud_originaal, pikkus * sizeof (int));

  //
  // Valiksorteerimine (selection sort)
  //
  
  wprintf(L"Valiksorteerimine:\n");

  // Kasuta algus, lopp muutujaid et arvutada tsüklites funktsiooni ajakulu
  clock_t algus = clock();

  // Kutsub välja sortimisfunktsiooni
  selectionsort(arvud_sort, pikkus);

  clock_t lopp = clock();

  // Väljasta sorteeritud massiiv
  valjasta(arvud_sort, pikkus); 

  // Väljasta ajakulu sekundites
  wprintf(L"Aega kulus: %f\n", (lopp - algus) / (float)CLOCKS_PER_SEC);

  // Tagasta originaalne arvude järjestus
  memcpy(arvud_sort, arvud_originaal, pikkus * sizeof (int));

  // Sama protsess jätkub järgneva kahe meetodiga.

  // 
  // Kiirsort (quicksort)
  //
  
  wprintf(L"Kiirsort:\n");
  
  algus = clock();
  quicksort(arvud_sort, pikkus);
  lopp = clock();

  valjasta(arvud_sort, pikkus); 
  wprintf(L"Aega kulus: %f\n", (lopp - algus) / (float)CLOCKS_PER_SEC);

  memcpy(arvud_sort, arvud_originaal, pikkus * sizeof (int));

  //
  // Mullimeetod (bubble sort)
  //
  
  wprintf(L"Mullimeetod:\n");
  
  algus = clock();
  bubblesort(arvud_sort, pikkus);
  lopp = clock();

  valjasta(arvud_sort, pikkus); 
  wprintf(L"Aega kulus: %f\n", (lopp - algus) / (float)CLOCKS_PER_SEC);

  return 0;
}


