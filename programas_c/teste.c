#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0

typedef int bool;

typedef struct aux {
  int id;
  int tipo;
  int quantidade;
  int valorUnitario;
  struct aux* proxProd;
} REGISTRO, * PONT;

typedef struct {
  PONT cabeca;
} LISTADEPRODUTOS, * PLISTA;

PLISTA criarLista();

int tamanho(PLISTA l);

int consultarValorUnitario(PLISTA l, int id);

PONT buscarID(PLISTA l, int id);


bool inserirNovoProduto(PLISTA l, int id, int tipo, int quantidade, int valor);

bool atualizarValorDoProduto(PLISTA l, int id, int valor);

bool removerItensDeUmProduto(PLISTA l, int id, int quantidade);

PLISTA criarLista(){
  PLISTA res = (PLISTA) malloc(sizeof(LISTADEPRODUTOS));
  res->cabeca = (PONT) malloc(sizeof(REGISTRO));
  res->cabeca->id=-1;
  res->cabeca->tipo=-1;
  res->cabeca->quantidade=0;
  res->cabeca->valorUnitario=0;
  res->cabeca->proxProd=NULL;
  return res;
}


int tamanho(PLISTA l){
  PONT atual = l->cabeca->proxProd;
  int tam = 0;
  while (atual) {
    atual = atual->proxProd;
    tam++;
  }
  return tam;
}

PONT buscarID(PLISTA l, int id){
  PONT atual = l->cabeca->proxProd;
   while (atual) {
    if (atual->id == id) return atual;
    atual = atual->proxProd;
  }
  return NULL;
}

void exibirLog(PLISTA f){
  int numElementos = tamanho(f);
  printf("Log lista [elementos: %i]\n    ", numElementos);
  PONT atual = f->cabeca->proxProd;
  while (atual){
    printf(" [%i;%i;%i;%i;$%i]", atual->id, atual->tipo, atual->quantidade, atual->valorUnitario, atual->quantidade*atual->valorUnitario);
    atual = atual->proxProd;
  }
  printf("\n\n");
}

int consultarValorUnitario(PLISTA l, int id){
  PONT atual = l->cabeca->proxProd;
   while (atual) {
    if (atual->id == id) return atual->valorUnitario;
    atual = atual->proxProd;
  }
  return 0;
}





bool inserirNovoProduto(PLISTA l, int id, int tipo, int quantidade, int valor){
	if (id>0 && tipo>0 && quantidade>0 && valor>0 && buscarID(l,id)==NULL){
			PONT novoprod = (PONT)malloc(sizeof(REGISTRO));
			novoprod->id=id;
			novoprod->tipo=tipo;
			novoprod->quantidade=quantidade;
			novoprod->valorUnitario=valor;
			PONT atual = l->cabeca;
			while (atual->proxProd && (atual->proxProd->valorUnitario*atual->proxProd->quantidade)<(valor*quantidade )) {
				atual = atual->proxProd;
			}
			if (atual == l->cabeca){
				novoprod->proxProd = l->cabeca->proxProd;
				l->cabeca->proxProd = novoprod;
			}
			else {
				if (atual->proxProd) {
					novoprod->proxProd = atual->proxProd;
					atual->proxProd = novoprod;
				}
				else {
					atual->proxProd = novoprod;
					novoprod->proxProd = NULL;
				}
			}
			return true;
	}
				
	return false;
}



bool removerItensDeUmProduto(PLISTA l, int id, int quantidade){
	if (buscarID(l,id)!=NULL && quantidade>0 && ((buscarID(l,id)->quantidade) >= quantidade)){
		PONT prod = buscarID(l,id);
		prod->quantidade -= quantidade;
		if (prod->quantidade == 0){
			PONT atual = l->cabeca;
			while (atual->proxProd->id != id){
				atual = atual->proxProd;
			}
			atual->proxProd = prod->proxProd;
			free (prod);
			return true;
		}
		else {
			PONT novoant = l->cabeca;
			while (((novoant->proxProd->valorUnitario)*(novoant->proxProd->quantidade))<((prod->valorUnitario)*(prod->quantidade))) {
				novoant= novoant->proxProd;
			}
			if (novoant->proxProd ==prod)	return true;
			else {
				PONT antprod = l->cabeca;
				while (antprod->proxProd != prod) {
					antprod = antprod->proxProd;
				}
				PONT aux = novoant->proxProd;
				novoant->proxProd = prod;
				antprod->proxProd =prod->proxProd;
				prod->proxProd = aux;
				return true;
			}
		}
	}
			
	return false;
}

bool atualizarValorDoProduto(PLISTA l, int id, int valor){
	if (buscarID(l,id) && valor>0 ){
		PONT prod = buscarID(l,id);
		prod->valorUnitario = valor;
		printf("-------------------------\n\n\n\n");
		PONT novoant = l->cabeca;
		while ((novoant->proxProd !=NULL) &&(((novoant->proxProd->valorUnitario)*(novoant->proxProd->quantidade))<((prod->valorUnitario)*(prod->quantidade)) || (novoant->proxProd == prod))) {
			printf("%p\n", novoant);
			
			printf("%i\n", (((novoant->proxProd->valorUnitario)*(novoant->proxProd->quantidade))<((prod->valorUnitario)*(prod->quantidade))));
			printf("%i\n", (novoant->proxProd == prod));
			printf("%i\n", (novoant->proxProd !=NULL));
			novoant= novoant->proxProd;
		}
		if (novoant->proxProd == prod)	return true;
		PONT antprod = l->cabeca;
		while (antprod->proxProd != prod){
		antprod = antprod->proxProd;
		}
		if (((novoant->proxProd->valorUnitario)*(novoant->proxProd->quantidade)) < ((prod->proxProd->valorUnitario)*(prod->proxProd->quantidade))){ 
			PONT aux = novoant->proxProd;
			novoant->proxProd = prod;
			antprod->proxProd = prod->proxProd;
			prod->proxProd = aux;
			return true;
		}
		else {
			PONT aux = novoant->proxProd;
			novoant->proxProd = prod;
			antprod->proxProd = prod->proxProd;
			prod->proxProd = aux;
			return true;
		}
	}
			

	return false;
}






int main() {
  PLISTA f = criarLista();
  int id;
  int tipo;
  int quantidade;
  int valor;
  bool res;

  printf("################# INSERINDO #######################\n");

  exibirLog(f);
  res = inserirNovoProduto(f, 2, 21, 22, 23);
  if(res) printf("Insercao retornou true (1)\n");
  else printf("Insercao retornou false (1)\n");
  exibirLog(f);
  res = inserirNovoProduto(f, 4, 4, 4, 4);
  if(res) printf("Insercao retornou true (3)\n");
  else printf("Insercao retornou false (2)\n");
  exibirLog(f);
  res = inserirNovoProduto(f, 6, 7, 8, 9);
  if(res) printf("Insercao retornou true (4)\n");
  else printf("Insercao retornou false (3)\n");
  exibirLog(f);
  res = inserirNovoProduto(f, 3, 21, 22, 23);
  if(res) printf("Insercao retornou true (2)\n");
  else printf("Insercao retornou false (4)\n");
  exibirLog(f);

  res = inserirNovoProduto(f, -5, 6, 7, 8);
  if(res) printf("Insercao retornou true (5)\n");
  else printf("Insercao retornou false (5)\n");
  exibirLog(f);

  res = inserirNovoProduto(f, 5, -6, 7, 8);
  if(res) printf("Insercao retornou true (6)\n");
  else printf("Insercao retornou false (6)\n");
  exibirLog(f);

  res = inserirNovoProduto(f, 5, 6, -7, 8);
  if(res) printf("Insercao retornou true (7)\n");
  else printf("Insercao retornou false (7)\n");
  exibirLog(f);

  res = inserirNovoProduto(f, 5, 6, 7, -8);
  if(res) printf("Insercao retornou true (4)\n");
  else printf("Insercao retornou false (8)\n");
  exibirLog(f);
 
  res = inserirNovoProduto(f, 3, 21, 9, 9);
  if(res) printf("Insercao retornou true (2)\n");
  else printf("Insercao retornou false (9)\n");
  exibirLog(f);


  printf("################# REMOVENDO #######################\n");
  res = removerItensDeUmProduto(f, 4, 1);
  if(res) printf("Remocao retornou true (1)\n");
  else printf("Remocao retornou false (1)\n");
  exibirLog(f);

  res = removerItensDeUmProduto(f, 6, 7);
  if(res) printf("Remocao retornou true (2)\n");
  else printf("Remocao retornou false (2)\n");
  exibirLog(f);

  res = removerItensDeUmProduto(f, 3, 22);
  if(res) printf("Remocao retornou true (3)\n");
  else printf("Remocao retornou false (3)\n");
  exibirLog(f);


  res = removerItensDeUmProduto(f, 20, 1);
  if(res) printf("Remocao retornou true (4)\n");
  else printf("Remocao retornou false (4)\n");
  exibirLog(f);

  res = removerItensDeUmProduto(f, 5, 10);
  if(res) printf("Remocao retornou true (5)\n");
  else printf("Remocao retornou false (5)\n");
  exibirLog(f);

  res = removerItensDeUmProduto(f, 4, 0);
  if(res) printf("Remocao retornou true (6)\n");
  else printf("Remocao retornou false (6)\n");
  exibirLog(f);



  printf("################# ATUALIZANDO VALOR #######\n");

  res = atualizarValorDoProduto(f, 6, 1);
  if(res) printf("Atualizacao retornou true (1)\n");
  else printf("Atualizacao retornou false (1)\n");
  exibirLog(f);

  res = atualizarValorDoProduto(f, 6, 20);
  if(res) printf("Atualizacao retornou true (2)\n");
  else printf("Atualizacao retornou false (2)\n");
  exibirLog(f);

  res = atualizarValorDoProduto(f, 6, 600);
  if(res) printf("Atualizacao retornou true (3)\n");
  else printf("Atualizacao retornou false (3)\n");
  exibirLog(f);

  res = atualizarValorDoProduto(f, 6, 0);
  if(res) printf("Atualizacao retornou true (4)\n");
  else printf("Atualizacao retornou false (4)\n");
  exibirLog(f);

  res = atualizarValorDoProduto(f, 6, -100);
  if(res) printf("Atualizacao retornou true (5)\n");
  else printf("Atualizacao retornou false (5)\n");
  exibirLog(f);

  res = atualizarValorDoProduto(f, 61, 600);
  if(res) printf("Atualizacao retornou true (6)\n");
  else printf("Atualizacao retornou false (6)\n");
  exibirLog(f);

 

  res = atualizarValorDoProduto(f, 6, 3);
  if(res) printf("Atualizacao retornou true (7)\n");
  else printf("Atualizacao retornou false (7)\n");
  exibirLog(f);

  res = atualizarValorDoProduto(f, 4, 200);
  if(res) printf("Atualizacao retornou true (8)\n");
  else printf("Atualizacao retornou false (8)\n");
  exibirLog(f);

  res = atualizarValorDoProduto(f, 2, 20);
  if(res) printf("Atualizacao retornou true (9)\n");
  else printf("Atualizacao retornou false (9)\n");
  exibirLog(f);




  return 0;
}