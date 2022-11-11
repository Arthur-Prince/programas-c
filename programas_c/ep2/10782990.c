#include "listadeprodutos.h"

PLISTA criarLista(){
  PLISTA res = (PLISTA) malloc(sizeof(LISTADEPRODUTOS));
  int x;
  for (x=0;x<NUMTIPOS;x++){
    res->LISTADELISTAS[x]=(PONT) malloc(sizeof(REGISTRO));
    res->LISTADELISTAS[x]->id=-1;
    res->LISTADELISTAS[x]->quantidade=0;
    res->LISTADELISTAS[x]->valorUnitario=0;
    res->LISTADELISTAS[x]->proxProd=NULL;
  }
  return res;
}


int tamanho(PLISTA l){
  int tam = 0;
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      atual = atual->proxProd;
      tam++;
    }
  }
  return tam;
}

PONT buscarID(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return atual;
      atual = atual->proxProd;
    }
  }
  return NULL;
}

PONT buscarIDTipo(PLISTA l, int id, int tipo){
  if (tipo<0 || tipo>=NUMTIPOS) return NULL;
  PONT atual = l->LISTADELISTAS[tipo]->proxProd;
  while (atual) {
    if (atual->id == id) return atual;
    atual = atual->proxProd;
  }
  return NULL;
}

void exibirLog(PLISTA f){
  int numElementos = tamanho(f);
  printf("Log lista [elementos: %i]\n", numElementos);
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    printf("  #TIPO: %i -> ", x);
    atual = f->LISTADELISTAS[x]->proxProd;
    while (atual){
      printf(" [%i;%i;%i;$%i]", atual->id, atual->quantidade, atual->valorUnitario, atual->quantidade*atual->valorUnitario);
      atual = atual->proxProd;
    }
    printf("\n");
  }
  printf("\n");
}

int consultarValorUnitario(PLISTA l, int id){
  int x;
  PONT atual;
  for (x=0;x<NUMTIPOS;x++){
    atual = l->LISTADELISTAS[x]->proxProd;
    while (atual) {
      if (atual->id == id) return atual->valorUnitario;
      atual = atual->proxProd;
    }
  }
  return 0;
}







//-------------------------------------------------------------------------------------------------------------------------------------------------------\\










bool insereP(PLISTA l, PONT p, int tipo){
	int pos = (p->valorUnitario)*(p->quantidade);
	PONT ant = l->LISTADELISTAS[tipo];
	PONT atual = ant->proxProd;
	if(!atual){
		p->proxProd = atual;
		ant->proxProd = p;
		return true;
	}
	int posDoProx=(atual->valorUnitario)*(atual->quantidade);
	while(atual && posDoProx<pos){
		ant=atual;
		atual=atual->proxProd;
		posDoProx = (atual->valorUnitario)*(atual->quantidade);

		
	}
	p->proxProd = atual;
	ant->proxProd = p;
	return true;
}

PONT fazPont(int id, int quantidade, int valor){
	PONT p = (PONT)malloc(sizeof(REGISTRO));
	p->id=id;
	p->quantidade= quantidade;
	p->valorUnitario=valor;
	return p;
}

bool inserirNovoProduto(PLISTA l, int id, int tipo, int quantidade, int valor){
	if(id<=0||valor<=0||quantidade<=0) return false;
	if(tipo<0||tipo>=NUMTIPOS) return false;
	if(buscarID(l,id)) return false;
	PONT p = fazPont(id,quantidade,valor);
	return insereP(l,p,tipo);
}



PONT buscarIDTipoComAnt(PLISTA l, int id, int tipo, PONT* ant){
	if (tipo<0 || tipo>=NUMTIPOS) return NULL;
	PONT anterior = l->LISTADELISTAS[tipo];
	PONT atual = anterior->proxProd;
	while (atual) {
		if (atual->id == id){
			*ant=anterior;
			return atual;
		}
		anterior = atual;
		atual = atual->proxProd;
	}
	ant= NULL;
	return NULL;
}
void ordenaCasoMaior(PONT p){
	int pos = (p->valorUnitario)*(p->quantidade);
	PONT atual = p->proxProd;
	int posDoProx;
	while(atual->proxProd){
		posDoProx = (atual->proxProd->valorUnitario)*(atual->proxProd->quantidade);
		if(pos<posDoProx)break;
		atual=atual->proxProd;
	}
	p->proxProd=atual->proxProd;
	atual->proxProd=p;
}

bool removerItensDeUmProduto(PLISTA l, int id, int quantidade){
	int tipo;
	PONT p,ant;
	for(tipo=0;tipo<NUMTIPOS;tipo++){
		p = buscarIDTipoComAnt(l,id,tipo,&ant);
		if(p) break;
	}
	if(!p) return false;
	if(quantidade<=0|| quantidade>p->quantidade)return false;
	p->quantidade = p->quantidade - quantidade;
	if(p->quantidade==0){
		ant->proxProd=p->proxProd;
		free(p);
	}else{
		int pos = (p->valorUnitario)*(p->quantidade);
		int posAnt = (ant->valorUnitario)*(ant->quantidade);
		
		if(pos<posAnt){
			ant->proxProd=p->proxProd;
			insereP(l,p,tipo);
		}
		if(p->proxProd){
			int posDoProx = (p->proxProd->valorUnitario)*(p->proxProd->quantidade);
			if(posDoProx<pos) ordenaCasoMaior(p);
		}
	}
	return true;
}


bool atualizarValorDoProduto(PLISTA l, int id, int valor){
	if(valor<=0)return false;
	int tipo;
	PONT p,ant;
	for(tipo=0;tipo<NUMTIPOS;tipo++){
		p = buscarIDTipoComAnt(l,id,tipo,&ant);
		if(p) break;
	}
	if(!p) return false;
	p->valorUnitario = valor;
	int pos = (p->valorUnitario)*(p->quantidade);
	int posAnt = (ant->valorUnitario)*(ant->quantidade);
	if(pos<posAnt){
		ant->proxProd=p->proxProd;
		insereP(l,p,tipo);
	}
	if(p->proxProd){
		int posDoProx = (p->proxProd->valorUnitario)*(p->proxProd->quantidade);
		if(posDoProx<pos){
			ant->proxProd=p->proxProd;
			ordenaCasoMaior(p);
		}
	}

  return true;
}
