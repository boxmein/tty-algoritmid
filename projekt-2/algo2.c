#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_TEKST 10000
#define MAX_PUU   255

typedef struct node {
  char symbol;
  int weight;
  struct node* left;
  struct node* right;
  struct node* parent;
  char tipp;
} node;

// Kodeeri tekstijada Huffman-koodiks
void kodeeri(node *symbolid, int len, char *tekst) {
  int tekst_len = strlen(tekst);
  // TODO!
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
    // Väljasta lehe <täht> kood
    printf("leht: %c kood ", tipp->symbol);
    for (int i = 0; i < idx; i++) {
      printf("%s", kooditabel[tipp->symbol]);
    }
    printf("\n");
  }
}

void kirjuta_bitt(char bitt) {
  putchar(bitt);
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
  while(loe_puud(symbolid, len) > 1){

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

  printf("Huffmani kodeerija\n");
  printf("Sisesta tekstilõik > ");

  fgets(tekst, MAX_TEKST, stdin);

  printf("Tekstilõik on: %s\n", tekst);

  // Kustuta teksti lõpust \n
  int sl = strlen(tekst);
  tekst[sl - 1] = 0;

  int puu_pikkus = 0;

  node *symbolid = malloc(MAX_PUU * sizeof(node));
  node *puu_tipp = NULL;

  loo_huffman_puu(tekst, symbolid, &puu_pikkus);

  for (int i = 0; i < MAX_PUU; i++) {
    if (symbolid[i].tipp) {
      puu_tipp = symbolid + i;
      printf("valjastan puu:\n");
      valjasta_puu(symbolid + i, 0);
      printf("\n");
      break;
    }
  }

  char *kooditabel[255];

  for (int i = 0; i < 255; i++) {
    kooditabel[i] = malloc(MAX_PUU * sizeof(char));
  }

  char praegune_kood[255];

  koosta_kooditabel(puu_tipp, kooditabel, praegune_kood, 0);

  // kodeeri sõnum ja väljasta baidid
  // kodeeri(puu_tipp, tekst);

  // vabaaaa
  for (int i = 0; i < 255; i++) {
    free(kooditabel[i]);
  }
  free(symbolid);

  return 0;
}
