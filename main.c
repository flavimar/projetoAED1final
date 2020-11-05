#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MOV 0.3
#define tamCobra 10
#define alturatela 720
#define larguratela 1280
int contx = 0;
int conty = 0;
int tamx = 0,tamy = 0;
//int cima = -2 ,baixo = 2,dir = 1,esq = -1;
time_t t;
typedef struct{
	SDL_Rect rect;
	int cor[3];
}Registro;
typedef struct aux{
   Registro reg;
   //int dir; //direcao esq = -1 dir = 1 cima = -2 baixo = 2 neutro = 0
   struct  aux *prox;	
   }Elemento;
typedef struct{
	Registro reg;
	int ativo;
}Objeto;
typedef struct{
	Elemento *inicio;
	Elemento *fim;
}Fila;
//funções de eventos do jogo
void call_render(SDL_Renderer*, Registro*);
int call_events(Fila*);

//funções da cobra
Fila* criaCobra();
void inicializaCobra(Fila*);
void liberaCobra(Fila*);
void aumentaCobra(Fila*);
void mostraCobra(Fila*);
int insereNaCobra(Fila*,Registro);
void exibeCobra(SDL_Renderer *renderer,Fila* cobra);
void atualizaCobra(Fila*,SDL_Rect);
//void atCobra(Fila*,SDL_Rect);
void limiteTela(Elemento*);
int verLimiteTela(Elemento*);
int contTamCobra(Fila*);
//Funções do alimento
Objeto* criaAlimento();
void inicializaAlimento(Objeto*);
int gerarAlimento(SDL_Renderer*,Objeto*);
void liberaAlimento(Objeto*);
int colisaoAlimento(Objeto*,Fila*);

int main(int argc, char **argv){   
	int flag;
 //Posicao inicial da cobra
	Fila *cobra = criaCobra();
	inicializaCobra(cobra);
	Objeto *alimento = criaAlimento();
	inicializaAlimento(alimento);
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("SnakeC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, larguratela, alturatela, SDL_WINDOW_OPENGL);
	if(!window){
		SDL_Log("Erro na inicialização: %s", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(!renderer){
		SDL_Log("Erro ao cirar renderizacao! %s", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	SDL_Event event;
    aumentaCobra(cobra);
    aumentaCobra(cobra);
    mostraCobra(cobra);
    printf("contador: %d\n",contTamCobra(cobra));
	flag = 0;
   
	 while(!flag){
	 	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	    SDL_RenderClear(renderer);	
	    gerarAlimento(renderer, alimento);
		flag = call_events(cobra);
		exibeCobra(renderer,cobra);
		colisaoAlimento(alimento,cobra);
		//call_render(renderer,&cobra->inicio->reg);
		
		SDL_Delay(100);
		SDL_RenderPresent(renderer);		
	}
	
	liberaCobra(cobra);
    liberaAlimento(alimento);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void call_render(SDL_Renderer *renderer, Registro *reg){

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, reg->cor[0], reg->cor[1],reg->cor[2]);
    SDL_RenderFillRect(renderer,&reg->rect);
	
	
}

int call_events(Fila *cobra){
	int flag = 0;
	
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	//printf("%ld ",sizeof(SDL_GetKeyboardState));

	while(SDL_PollEvent(&event)){

		if((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)){
			flag = 1;
		}
	}
    SDL_Rect *rect = &cobra->inicio->reg.rect;
    SDL_Rect rant = cobra->inicio->reg.rect;
    if(contx == 0 && conty == 0){
      SDL_Log("Começe o jogo:\n");
    }else{
	if(state[SDL_SCANCODE_LEFT] && (contx < 0 || contx == 0)){		
   		contx = -tamCobra;
   		conty = 0;

	}
    }
	if(state[SDL_SCANCODE_RIGHT] && (contx > 0 || contx == 0)){
   	  contx = tamCobra;
   	  conty = 0;
   		
	}

	else if(state[SDL_SCANCODE_UP] && (conty < 0 || conty == 0)){
   		conty = -tamCobra;
   		contx = 0;   	
	}
	else if(state[SDL_SCANCODE_DOWN] && (contx > 0 || conty == 0)){
   		conty = tamCobra;
   		contx = 0;
	}
	 //atualizaCobra(cobra);
	 //limiteTela(cobra->inicio);
     
     
	  rect->x += contx;
	  rect->y += conty;
	  limiteTela(cobra->inicio);
	  if(contx != 0 || conty != 0)
      atualizaCobra(cobra,rant);
    //SDL_Log("%d %d %d",model->x + model->largura,model->y + model->altura,larguratela);
    

	return flag;
}
// Funções da Cobra cobra
Fila* criaCobra(){
   Fila *cobra = (Fila*)malloc(sizeof(Fila));
   if(cobra == NULL){
      SDL_Log("Erro ao criar a cobra\n");
   return cobra;

   }
}
void inicializaCobra(Fila *cobra){
	Elemento *cabeca = (Elemento*)malloc(sizeof(Elemento));
	cabeca->prox = NULL;
	cabeca->reg.rect.x = 100;
	cabeca->reg.rect.y = 100;
	cabeca->reg.rect.w = tamCobra;
	cabeca->reg.rect.h= tamCobra;
	cabeca->reg.cor[0] = 255;
	cabeca->reg.cor[1] = 1;
	cabeca->reg.cor[2] = 1;
	cobra->inicio = cabeca;
	cobra->fim = cabeca;
	
}
void aumentaCobra(Fila *cobra){
	Registro reg;
	//printf("%d\n",cobra->fim->dir);
	reg = cobra->fim->reg;
	/*if(cobra->fim->dir == cima)
		reg.rect.y += reg.rect.h;
	else if(cobra->fim->dir == baixo)
		reg.rect.y -= reg.rect.h;
	else if(cobra->fim->dir == esq)
		reg.rect.x += reg.rect.w;
	else if(cobra->fim->dir == dir)
		reg.rect.x -= reg.rect.w;
	else
		reg.rect.x -= reg.rect.w;*/
	
	reg.cor[0] = 255;
	reg.cor[1] = 255;
	reg.cor[2] = 255;
	insereNaCobra(cobra,reg);
	
}
int insereNaCobra(Fila *cobra,Registro reg){
	Elemento *novo = (Elemento*) malloc(sizeof(Elemento));
	novo->reg  = reg;
	novo->prox = NULL;
	if(cobra->inicio==NULL) cobra->inicio =novo;
	else cobra->fim->prox = novo;
	cobra->fim = novo;
	return 1;
}	
int contTamCobra(Fila *cobra){
   Elemento *end = cobra->inicio;
   int cont = 0;
   while(end != NULL){
   	 end = end->prox;
   	 cont++;
   }
  return cont;
}
void limiteTela(Elemento *e){
   if((e->reg.rect.x + e->reg.rect.w) >= larguratela){
     e->reg.rect.x = 1;
   }
    if(e->reg.rect.x <= 0){
      e->reg.rect.x = larguratela - e->reg.rect.w;
    }
    if((e->reg.rect.y + e->reg.rect.h) >= alturatela){
      e->reg.rect.y = 1;
    }
    if(e->reg.rect.y <= 0){
      e->reg.rect.y = alturatela - e->reg.rect.h;
    }
}
int verLimiteTela(Elemento *e){
	if(e->prox != NULL)
	//printf("%d\n",e->prox->dir);
   if((e->reg.rect.x + e->reg.rect.w) >= larguratela){
     return 1;
   }
    if(e->reg.rect.x <= 0){
      return 1;
    }
    if((e->reg.rect.y + e->reg.rect.h) >= alturatela){
      return 1;
    }
    if(e->reg.rect.y <= 0){
      return 1;
    }

  return 0;
}

void atualizaCobra(Fila *cobra,SDL_Rect aux){
	SDL_Rect ant;
	Elemento *atual = cobra->inicio->prox;
	ant = atual->reg.rect;
	atual->reg.rect = aux;
	while(atual->prox != NULL){
     	    atual = atual->prox;	   
		    aux = atual->reg.rect;
		    atual->reg.rect = ant;
		    ant = aux;
	}
}
/*
void atCobra(Fila *cobra){
	Elemento *ant = cobra->inicio;
	Elemento *atual = ant->prox;
	//int limite = verLimiteTela(ant);
	//end->reg.rect.x = aux->reg.rect.x + tamx;
	//end->reg.rect.y = aux->reg.rect.y + tamy; 
	while(atual != NULL){
		    
		    
		    if(atual->reg.rect.x == ant->reg.rect.x){
		    	if((ant->reg.rect.y - atual->reg.rect.y) > 0){
		          atual->reg.rect.y += tamCobra;
		          atual->dir = baixo;
		        }
		        else if((ant->reg.rect.y - atual->reg.rect.y) < 0){
		          atual->reg.rect.y -= tamCobra;		
		          atual->dir = cima;
		        }
		    }
            else if(atual->reg.rect.y == ant->reg.rect.y){
		    	if((ant->reg.rect.x - atual->reg.rect.x) > 0){
		          atual->reg.rect.x += tamCobra;
		          atual->dir = dir;
		        }
		        else if((ant->reg.rect.x - atual->reg.rect.x) < 0){
		          atual->reg.rect.x -= tamCobra;		
		          atual->dir = esq;
		        }
		    }
		    else if(((atual->reg.rect.y != ant->reg.rect.y) && (atual->reg.rect.x != ant->reg.rect.x))){
		    	if(atual->dir == esq)
		    	  atual->reg.rect.x -= tamCobra;
		    	else if(atual->dir == dir)
                  atual->reg.rect.x += tamCobra;
		    	else if(atual->dir == cima)
                  atual->reg.rect.y -= tamCobra;
		    	else
		    	  atual->reg.rect.y += tamCobra;
		    }
		    
		    ant = atual;
		    atual = atual->prox;	   
	}
}*/
void exibeCobra(SDL_Renderer *renderer,Fila* cobra){
	Elemento *end = cobra->inicio;
	Elemento *aux;
	while(end != NULL){
		call_render(renderer,&end->reg);
	    end = end->prox;
	}
}
void mostraCobra(Fila* cobra){
	Elemento *end = cobra->inicio;
	Elemento *aux;
	while(end != NULL){
		//printf("%d %d\n",end->reg.rect.x,end->reg.rect.y);
		if(end->prox != NULL){
			aux = end;
			end->reg.rect.x = aux->reg.rect.x;
			end->reg.rect.y = aux->reg.rect.y;
	    }
	    end = end->prox;
	}
}
void liberaCobra(Fila* cobra){
    free(cobra);
}

// Funções do Objeto Alimento
Objeto* criaAlimento(){
	Objeto *alimento = (Objeto*)malloc(sizeof(Objeto));
	if(alimento == NULL)
	  printf("Erro\n");
	return alimento;
}
void inicializaAlimento(Objeto *alimento){
   alimento->reg.rect.w = tamCobra;
   alimento->reg.rect.h = tamCobra;
   alimento->reg.cor[0] = 0;
   alimento->reg.cor[1] = 255;
   alimento->reg.cor[2] = 255;
   alimento->ativo = 0;
}
void liberaAlimento(Objeto *alimento){
   free(alimento);
}
int gerarAlimento(SDL_Renderer *renderer,Objeto *alimento){
    if(alimento->ativo == 0){
    srand((unsigned) time(&t));
    int x = (((rand() % (larguratela / tamCobra))) * tamCobra);
    int y = (((rand() % (alturatela / tamCobra)))* tamCobra);
    printf("%d %d\n",x,y);
    alimento->reg.rect.x = x;
    alimento->reg.rect.y = y;
    alimento->ativo = 1;
    }
        call_render(renderer,&alimento->reg);
        //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        //SDL_RenderFillRect(renderer, &alimento->a);	
	  
 }

   int colisaoAlimento(Objeto *alimento,Fila *cobra){
 	int CobraLar = cobra->inicio->reg.rect.x + (cobra->inicio->reg.rect.w), CobraAlt = cobra->inicio->reg.rect.y + (cobra->inicio->reg.rect.h);
 	int AlimMaxLar = alimento->reg.rect.x + alimento->reg.rect.w, AlimMaxAlt = alimento->reg.rect.y + alimento->reg.rect.h;
 	printf("cobrax = (%d , %d) cobray = (%d , %d)\n",cobra->inicio->reg.rect.x,CobraLar,cobra->inicio->reg.rect.y,CobraAlt);
 	printf("x = (%d , %d) y = (%d , %d)\n",alimento->reg.rect.x,AlimMaxLar,alimento->reg.rect.y,AlimMaxAlt);
    if((cobra->inicio->reg.rect.x >= alimento->reg.rect.x) && (CobraLar <= AlimMaxLar)){
    	//printf("UM JA FOI!!!!!!!!!!!!!!!!!!!!!\n");
       if((cobra->inicio->reg.rect.y == alimento->reg.rect.y) && (CobraAlt <= AlimMaxAlt)){
    	//printf("OIIIIIIIIIIOIIIIIIII\n");
    	aumentaCobra(cobra);
    	printf("contador: %d\n",contTamCobra(cobra));
    	inicializaAlimento(alimento);
        return 1;
       }
    }
   return 0;
 }

