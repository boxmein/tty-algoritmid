#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_TEKST 10000
#define MAX_PUU   511

// vastik binaarkoodi dekooder
// https://stackoverflow.com/a/3208376
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


typedef struct node {
  char symbol;
  int weight;
  struct node* left;
  struct node* right;
  struct node* parent;
  char tipp;
} node;

// Kirjuta baidijadasse bitt vastavale kohale
void kirjuta_bitt_baiti(char *tekst, int bait, int bitikoht, char bitt) {
  printf("muudan baidi %d enne=%x bitt=%d to=%d ", bait, tekst[bait], bitikoht, bitt);
  tekst[bait] = (tekst[bait] & (~(1 << bitikoht))) | (bitt << bitikoht);
  printf("%x\n", tekst[bait] & 0xff);
}

// Kodeeri tekstijada Huffman-koodiks
void kodeeri(char **kooditabel, char *tekst, char *kodeeritud) {
  int tekst_len = strlen(tekst);
  int hetke_bait = 0;
  int hetke_bitt = 0;

  for (int i = 0; i < tekst_len; i++) {
    char *kood = kooditabel[tekst[i]];
    printf(" kodeeri: kodeerin tähe %c kui %s\n", tekst[i], kood);

    for (int j = 0, kood_len = strlen(kood); j < kood_len; j++) {
      // väljundis kohal hetke_bait biti hetke_bitt seadmine
      char bitt = kood[j] == '1' ? 1 : 0;

      kirjuta_bitt_baiti(kodeeritud, hetke_bait, hetke_bitt, bitt);
      hetke_bitt++;

      if (hetke_bitt == 8) {
        hetke_bait++;
        hetke_bitt = 0;
      }

    }
  }
}

void dekodeeri(char **kooditaber, char *tekst, char *dekodeeritud) {

}

// Koosta Huffman-kooditabel
void koosta_kooditabel(node *tipp, char *kooditabel[MAX_PUU], char *praegune_kood, int idx) {
  // Rekurseeru vasakule
  if (tipp->left) {
    praegune_kood[idx] = '0';
    koosta_kooditabel(tipp->left, kooditabel, praegune_kood, idx + 1);
  }

  // Rekurseeru paremale
  if (tipp->right) {
    praegune_kood[idx] = '1';
    koosta_kooditabel(tipp->right, kooditabel, praegune_kood, idx + 1);
  }

  // Leides lehe...
  if (!tipp->left && !tipp->right) {
    // Salvesta kogutud kood kooditabelisse kohale <täht>
    memcpy(kooditabel[tipp->symbol], praegune_kood, idx);
    kooditabel[tipp->symbol][idx] = 0;
    // Väljasta lehe <täht> kood
    printf("leht: %c kood ", tipp->symbol);
    printf("%s", kooditabel[tipp->symbol]);
    printf("\n");
  }
}


int loe_puud(node *symbolid, int len) {
  int loendur = 0;
  for (int i = 0; i < len; i++) {
    loendur += symbolid[i].tipp; // + 1 kui tipp, + 0 kui ei ole tipp
  }

  printf("loe_puud: puude arv hetkel on %d\n", loendur);
  return loendur;
}

// leia minimaalse kaaluga tippnode
node *find_min(node *symbolid, int len, node *not_node) {

  int min_weight = INT_MAX;
  node *min_symbol = NULL;

  for (int i = 0; i < len; i++) {
    if ( (symbolid+i) != not_node &&
	 symbolid[i].weight < min_weight &&
	 symbolid[i].tipp) {
      min_symbol = symbolid + i;
      min_weight = symbolid[i].weight;
    }
  }

  printf("find_min: leidsin minimaalse symboli %p %c %d\n", min_symbol, min_symbol->symbol, min_symbol->weight);
  return min_symbol;
}

// Tabuleeri N korda
void tabuleeri(int tabulaatoreid) {
  for (int i = 0; i < tabulaatoreid; i++) {
    printf("   |");
  }
}

// Väljasta puu formaadis:
// puu            ::= "L(", puu_voi_symbol, "),R(", puu_voi_symbol, ")"
// puu_voi_symbol ::= puu | symbol
// symbol         ::= ASCII sümbol
// L<kaal>(vasak puu või sümbol), R<kaal>(parem puu või sümbol)

void valjasta_puu(node *symbol, int tasand) {
  if (!symbol->left && !symbol->right) {
    tabuleeri(tasand);
    printf("%c", symbol->symbol);
    return;
  }

  if (symbol->left) {
    tabuleeri(tasand);
    printf("L%d(\n", symbol->left->weight);
    valjasta_puu(symbol->left, tasand + 1);
    printf("\n");
    tabuleeri(tasand);
    printf(")");
  }

  if (symbol->left && symbol->right) {
    printf(",\n");
  }

  if (symbol->right) {
    tabuleeri(tasand);
    printf("R%d(\n", symbol->right->weight);
    valjasta_puu(symbol->right, tasand + 1);
    printf(")\n");
  }

}

// loo Huffman-kodeeringu puu
void loo_huffman_puu(char *tekst, node *symbolid, int *viimane) {
  int len = strlen(tekst);
  *viimane = 0;

  // leiame unikaalsed sümbolid ja loendame nende esinemissagedust
  for (int i = 0; i < len; i++) {
    char unikaalne = 1;
    int leitud_idx = -1;

    printf("loo_huff: Taht %c: ", tekst[i]);

    // iga teksti sümboli korral leiame selle unikaalsete sümbolite listist
    for (int j = 0; j < len; j++) {
      if (symbolid[j].symbol == tekst[i]) {
        unikaalne = 0;
        leitud_idx = j;
      }
    }

    // loome iga unikaalse sümboli jaoks uue node ja täidame ära
    if (unikaalne) {
      leitud_idx = *viimane;
      *viimane += 1;
      printf("unikaalne ");
    } else {
      printf("olemas ");
    }

    // kui ei leitud, lisame juurde
    symbolid[leitud_idx].symbol = tekst[i];
    symbolid[leitud_idx].weight += 1;
    symbolid[leitud_idx].tipp = 1;

    printf("kaal: %d\n", symbolid[leitud_idx].weight);
  }


  // leiame minimaalsed "puud" ja liidame
  while(loe_puud(symbolid, *viimane) > 1){

    node *first = find_min(symbolid, *viimane, NULL);
    node *second = find_min(symbolid, *viimane, first);

    symbolid[*viimane].symbol = 0;
    symbolid[*viimane].weight = first->weight + second->weight;

    symbolid[*viimane].left = first;
    symbolid[*viimane].right = second;

    first->parent = second->parent = symbolid + *viimane;

    first->tipp = second->tipp = 0;
    symbolid[*viimane].tipp = 1;

    printf("loo_huff: loon uue symboli liitmiseks @ %d weight %d left %d right %d\n",
      *viimane, symbolid[*viimane].weight, symbolid[*viimane].left, symbolid[*viimane].right);

    *viimane += 1;
  }

}

int main() {
  char tekst[MAX_TEKST];
  char kodeeritud[MAX_TEKST];
  char praegune_kood[MAX_PUU];
  int puu_pikkus = 0;
  char *kooditabel[255];
  char vastus;

  printf("Huffmani kodeerija\n");

  printf("Sisesta tekstilõik > ");
  fgets(tekst, MAX_TEKST, stdin);

  printf("Tekstilõik on: %s\n", tekst);

  // Kustuta teksti lõpust \n
  int sl = strlen(tekst);

  if (sl == 0) {
    printf("Sisesta teksti\n");
    return 1;
  }

  tekst[sl - 1] = 0;

  node *symbolid = malloc(MAX_PUU * sizeof(node));
  node *puu_tipp = NULL;

  printf("Loon Huffmani puu.\n");

  loo_huffman_puu(tekst, symbolid, &puu_pikkus);

  for (int i = 0; i < MAX_PUU; i++) {
    if (symbolid[i].tipp) {
      puu_tipp = symbolid + i;
      break;
    }
  }

  for (int i = 0; i < 255; i++) {
    kooditabel[i] = malloc(MAX_PUU * sizeof(char));
  }

  printf("Väljastan ja loon kooditabeli.\n");
  koosta_kooditabel(puu_tipp, kooditabel, praegune_kood, 0);

  printf("Kodeerin sõnumi.\n");
  kodeeri(kooditabel, tekst, kodeeritud);

  // Valikulised väljastamised

  printf("Väljasta Huffmani puu? (y) ");

  vastus = getchar();
  getchar(); // söö \n ära

  if (vastus == 'y') {
    printf("Huffmani puu: \n");
    valjasta_puu(puu_tipp, 0);
  }


  printf("Väljasta kodeeritud sõnum? (y) ");

  vastus = getchar();
  getchar(); // söö \n ära

  if (vastus == 'y') {
    printf("Kodeeritud sõnum: \n");
    for (int i = 0, l = hetke_bait; i <= l; i++) {
      printf("%x ", kodeeritud[i] & 0xff);
    }
    printf("\n");
  }

  // vabaaaa
  for (int i = 0; i < 255; i++) {
    free(kooditabel[i]);
  }
  free(symbolid);

  return 0;
}
