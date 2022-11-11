#include<stdio.h>
#include<malloc.h>
#define true 1
#define false 0

#define MAXNUMVERTICES 100
#define AN -1	//aresta invalida
#define VI -1 	//vertice invalido
 
typedef int bool;
typedef int Peso;
typedef struct{
	Peso mat[MAXNUMVERTICES+1][MAXNUMVERTICES+1];
	int nv;	//numero de vertives
	int na;	//numero de arestas
} Grafo;

typedef struct ponteiro{
	struct ponteiro* prox;
	int vert;
}vertice, *PV;

typedef struct{
	PV V;
	int numeroDeVertice;
}ListaDeVertice, *PONT;
//inicializa grafo com nv vertices
bool inicializaGrafo(Grafo* g, int nv){
	if(nv>MAXNUMVERTICES||nv<=0)
		return false;
	g->nv=nv;
	g->na=0;
	int i;
	for(i=0;i<nv;i++){
		int j;
		for(j=0;j<nv;j++){
			g->mat[i][j]=AN;
		}
	}
	return true;
}

//verifica se o vertice v esta no grafo
bool existeV(Grafo* g, int v){
	if(v<0||v>=g->nv)
		return false;
	return true;
}
//incere no grafo(nao direcionado) a aresta (v1,v2) com peso peso
bool insereAresta(Grafo* g, int v1,int v2, Peso peso){
	if(!existeV(g,v1) || !existeV(g,v2))
		return false;
	g->mat[v1][v2] = peso;
	g->mat[v2][v1] = peso;
	g->na++;
	return true;
}
//verifica se existe a aresta(v1,v2)
bool existeA(Grafo* g, int v1, int v2){
	if(!existeV(g,v1) || !existeV(g,v2))
		return false;
	if(g->mat[v1][v2]==AN)
		return false;
	return true;
}
//retorna o peso da aresta (v1,v2)
Peso getPAresta(Grafo* g, int v1, int v2){
	if(!existeV(g,v1) || !existeV(g,v2))
		return AN;
	return g->mat[v1][v2];
}
//remove a aresta (v2,v1) e retorna o peso
Peso removeA(Grafo* g,int v1,int v2){
	int aux = g->mat[v1][v2];
	g->mat[v1][v2]=AN;
	g->mat[v2][v1]=AN;
	g->na--;
	return aux;
}
//verifica se o vertice v tem adjacente
bool temAdj(Grafo* g, int v){
	if(!existeV(g,v))
		return false;
	int i;
	for(i=0;i<g->nv;i++){
		if(g->mat[v][i]!=AN)
			return true;
	}
	return false;
}
//retorna o primeiro vertice adjacente a v
int primAdj(Grafo* g, int v){
	if(!existeV(g,v))
		return AN;
	int i;
	for(i=0;i<g->nv;i++){
		if(g->mat[v][i]!=AN)
			return i;
	}
	return AN;
}
//retorna o proximo vertice adjacente a v dado o vertice atual
int proxAdj(Grafo* g,int v,int atual){
	if(!existeV(g,v))
		return AN;
	int i;
	for(i=atual+1;i<g->nv;i++){
		if(g->mat[v][i]!=AN)
			return i;
	}
	return AN;
}
//imprime os vertices do grafo g
void imprime(Grafo* g){
	int i;
	for(i=0;i<g->nv;i++){
		int j;
		for(j=i;j<g->nv;j++){
			if(g->mat[i][j]!=AN)
				printf("%i %i  ",i,j);
		}
		printf("\n");
	}
}
//inicializa a lista
PONT inicializaLista(){
	PONT rtn = (PONT)malloc(sizeof(ListaDeVertice));
	PV vertice = (PV)malloc(sizeof(vertice));
	vertice->prox = NULL;
	vertice->vert = VI;
	rtn->V = vertice;
	rtn->numeroDeVertice=0;
	return rtn;
}
//adiciona no inicio da lista o vertice v e incrementa o numero de vertices
bool addLista(PONT l , int v){
	PV vertice = (PV)malloc(sizeof(vertice));
	vertice->prox = l->V;
	vertice->vert = v;
	l->V = vertice;
	l->numeroDeVertice++;
	return true;
}
//verifica se v esta na lista l
bool estaNaLista(PONT l, int v){
	PV aux = l->V;
	while(aux){
		if(aux->vert == v)
			return true;
		aux=aux->prox;
	}
	return false;
}
//imprime os vertices da lista
void imprime2(PONT l){
	PV aux = l->V;
	while(aux){
		printf("%i    ",aux->vert);
		aux=aux->prox;
	}
	printf("\n");
}
//retorna a arvore geradora minima do grafo g considerando que g é conexo
Grafo* algPrim(Grafo* g){
	//inicializa um novo grafo
	Grafo* rtn = malloc(sizeof(Grafo));
	inicializaGrafo(rtn,g->nv);
	PONT l = inicializaLista();
	//e adiciona o vertice 0 na lista de vertice da arvore geradora minima
	addLista(l,0);
	while(l->numeroDeVertice<g->nv){//enquanto a lista nao tem o mesmo numero de vertices que g faca:
		PV aux = l->V;
		//pega um vertica que esta na lista que sera o pai
		int paiVMenorPeso = aux->vert;
		//pega um vertice ligado ao pai(nao necessariamente fora da lista)
		int filhoVMenorPeso = primAdj(g,aux->vert);
		//aqui garante que o vertice filho nao esta na lista e que tem uma aresta entre o pai que esta na lista e o filho 
		while((filhoVMenorPeso==AN||estaNaLista(l,filhoVMenorPeso))&&aux){
			if(filhoVMenorPeso==AN){
				aux=aux->prox;
				paiVMenorPeso = aux->vert;
				filhoVMenorPeso = primAdj(g,aux->vert);
			}
			if(estaNaLista(l,filhoVMenorPeso))
				filhoVMenorPeso=proxAdj(g,paiVMenorPeso,filhoVMenorPeso);
		}
		//assim q tem um vertice fora da lista busca-se o de menor peso
		while(aux->prox){
			int comparador = primAdj(g,aux->vert);
			while(comparador!=VI){
				//verifica se a aresta de peso de menor peso anterior tem menor peso que as outras e se a outra esta na lista
				if((g->mat[paiVMenorPeso][filhoVMenorPeso]>g->mat[aux->vert][comparador])&&!estaNaLista(l,comparador)){
					//se as condicoes forem satisfeita coloca essa outra aresta na de menor peso
					filhoVMenorPeso = comparador;
					paiVMenorPeso = aux->vert;
				}
				
				comparador = proxAdj(g,aux->vert,comparador);
			}
			aux=aux->prox;
		}
		//por fim apos conseguir a aresta de menor peso. adicione-a no grafo da arvore geradora minima e bota o vertice q estava fora da arvore na lista
		insereAresta(rtn,paiVMenorPeso,filhoVMenorPeso,g->mat[paiVMenorPeso][filhoVMenorPeso]);
		addLista(l,filhoVMenorPeso);
		
	}
	return rtn;
}
//
//codigo de lida de arquivo baseado em: https://www.cprogressivo.net/2013/11/Como-ler-arquivos-em-C-As-funcoes-fgetc-fscanf-fgets.html
int main( int argc, char *argv[ ] ){
    
	Grafo* g = malloc(sizeof(Grafo));
	
    int v1,v2,peso,nv,na;
	FILE* arqRead = fopen(argv[1], "r");
	//ve se existe arquivo
	if(arqRead == NULL)
		printf("Erro, nao foi possivel abrir o arquivo para lida\n");
	else{
		//adiciona dados no grafo
		if(fscanf(arqRead,"%i %i\n", &nv, &na)!=EOF)
			inicializaGrafo(g,nv);
		while( (fscanf(arqRead,"%i %i %i\n", &v1, &v2, &peso))!=EOF ){
			insereAresta(g,v1,v2,peso);
		}
	}
	fclose(arqRead);
	
	Grafo* prim=algPrim(g);
	
	FILE* write = fopen(argv[2],"w");
	//verifica se existe arquivo para escrever
	if(arqRead == NULL)
		printf("Erro, nao foi possivel abrir o arqivo para escrita\n");
	else{
		//faz a soma dos pesos das arestas no grafo de prim 
		int soma = 0;
		int i;
		for(i=0;i<prim->nv;i++){
			int j;
			for(j=i;j<prim->nv;j++){
				if(prim->mat[i][j]!=AN)
					soma+=prim->mat[i][j];
			}
		}
		//coloca a soma no arquivo
		fprintf(write,"%i\n",soma);
		//coloca as arestas no arquivo
		for(i=0;i<prim->nv;i++){
			int j;
			for(j=i;j<prim->nv;j++){
				if(prim->mat[i][j]!=AN){
					fprintf(write,"%i %i\n",i,j);
				}
			}
		}
		
	}
	fclose(write);
	
	
	return 0;
}
