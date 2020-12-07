#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Vaciaveis globais do Jogo----------------
#define tamCobra 10
#define alturatela 600
#define larguratela 1000
int contx = 0;
int conty = 0;
int tamx = 0,tamy = 0;
int pont = 0;
time_t t;
SDL_Texture *fundo;
SDL_Texture *btn_comecar;
SDL_Texture *btn_comecar_ver;
SDL_Texture *btn_comecar_click;
SDL_Texture *btn_sair;
SDL_Texture *btn_sair_ver;
SDL_Texture *btn_sair_click;
// Vaciaveis globais do Jogo----------------
//Structs------------------
typedef struct{
	SDL_Rect rect;
	int cor[3];
}Registro;
typedef struct aux{
   Registro reg;
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
typedef struct 
{
	int x;
	int y;
}Mouse;
Mouse m;
//Structs------------------
//funções de eventos do jogo
void call_render(SDL_Renderer*, Registro*);
int call_events(Fila*,SDL_Rect*,int);
void placar(SDL_Renderer*);
SDL_Rect criaRect(int x,int y,int w,int h);
int menu(SDL_Renderer *renderer,SDL_Rect rbtn_comecar,SDL_Rect rbtn_sair,int flag);
int eventoMenu(SDL_Rect rbtn_comecar,SDL_Rect rbtn_sair);
void liberagrafico();
void mostraPlacar(SDL_Renderer* renderer, SDL_Texture* pontuacao, SDL_Rect* pont_rect,SDL_Surface* num_pont,SDL_Rect* num_rect,SDL_Texture* num,TTF_Font* fonte_placar,SDL_Color white);
//funções da cobra
Fila* criaCobra();
void inicializaCobra(Fila*);
void liberaCobra(Fila*);
void aumentaCobra(Fila*);
void mostraCobra(Fila*);
int insereNaCobra(Fila*,Registro);
void exibeCobra(SDL_Renderer *renderer,Fila* cobra);
void atualizaCobra(Fila*,SDL_Rect);
int colisaoCobra(Fila *cobra);
void addTamCobra(Fila *cobra);
void destruirFila(Fila*);
//void atCobra(Fila*,SDL_Rect);
void limiteTelaAtraveca(Elemento*,SDL_Rect*);
int limiteTelaColisao(Elemento*,SDL_Rect*);
int verLimiteTela(Elemento*);
int contTamCobra(Fila*);
//Funções do alimento
Objeto* criaAlimento();
void inicializaAlimento(Objeto*);
int gerarAlimento(SDL_Renderer*,Objeto*,SDL_Rect*);
void liberaAlimento(Objeto*);
int colisaoAlimento(Objeto*,Fila*);
//Arquivo do Bqanco de Dados
 FILE* criaArquivo(char arq[],char acao[3]);
 void inserePontuacao();
 void destroiArquivo(FILE*);
int main(int argc, char **argv){ 
 //Criação das variaveis locais  
	int flag = 1,resultado,p;
	SDL_Event event;
	int fechamenu = 0;
    char nome[21],conv_num[6];
    int buttonid;	
	Fila *cobra;
	Objeto *alimento;

    SDL_Rect telaJogo = criaRect(larguratela * 0.05,alturatela * 0.1,larguratela * 0.9,alturatela * 0.8);
	SDL_Rect rbtn_comecar,rbtn_sair;
    rbtn_comecar = criaRect(larguratela/4,200,500,50);
    rbtn_sair = criaRect(larguratela/4,300,500,50);

 //Criação da janela
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
//------------------------------
//FONTE----------------------------------------------  
   		TTF_Init();
    	TTF_Font *fonte_placar; 
    	fonte_placar = TTF_OpenFont("FreeSans.ttf", 15);
    	if (fonte_placar == NULL) {
          printf("Erro ao inicializar fonte: %s", TTF_GetError());
        exit(EXIT_FAILURE);
        }
        TTF_Font *fonte; 
    	fonte = TTF_OpenFont("FreeSans.ttf", 24);
    	if (fonte == NULL) {
          printf("Erro ao inicializar fonte: %s", TTF_GetError());
        exit(EXIT_FAILURE);
        }
	    SDL_Color white = {255, 255, 255};
	    SDL_Color red = {255,0,0};
        
        SDL_Surface* txt_pont;
        txt_pont = TTF_RenderText_Solid(fonte_placar,"placar", white); 
	    SDL_Rect pont_rect = criaRect(larguratela * 0.1,10,100,40);
        SDL_Texture* pontuacao = SDL_CreateTextureFromSurface(renderer, txt_pont); 

        SDL_Surface* num_pont;
	    SDL_Rect num_rect = criaRect(larguratela * 0.25,10,50,40);
	    SDL_Texture* num;
        
        SDL_Surface* texto = TTF_RenderText_Solid(fonte, "SnakeC", white); 
        SDL_Texture* titulo = SDL_CreateTextureFromSurface(renderer, texto); 
	    SDL_Rect titulo_rect = criaRect(larguratela/4,50,500,100);

	    SDL_Surface* texto_placar = TTF_RenderText_Solid(fonte_placar, "Maior Placar:", white); 
        SDL_Texture* textura_placar = SDL_CreateTextureFromSurface(renderer, texto_placar); 
	    SDL_Rect placar_rect = criaRect(larguratela/4,400,200,50);

	    SDL_Surface* maior;
	    SDL_Rect maior_rect = criaRect((larguratela/4 + 250),400,50,50);
	    SDL_Texture* num_maior;
//FONTE----------------------------------------------
	
	 while(flag){
	    SDL_RenderClear(renderer);		
	    if(flag == 3){
	      liberagrafico();
		  fechamenu = 1;
		  cobra = criaCobra();
		  inicializaCobra(cobra);
		  alimento = criaAlimento();
		  inicializaAlimento(alimento);
		    SDL_MessageBoxButtonData button_data[3] = {0};
		    button_data[0].flags    = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
		    button_data[0].buttonid = 1;
		    button_data[0].text     = "Sim";

		    button_data[1].flags    = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
		    button_data[1].buttonid = 0;
		    button_data[1].text     = "Não";

		    SDL_MessageBoxData message_box_data = {0};
		    message_box_data.flags       = 0;
		    message_box_data.window      = NULL;
		    message_box_data.title       = "Deseja jogar com parede?";
		    message_box_data.message     = "Caso sim, se a cobra colidir no limite do caminho irá contar como fim de jogo";
		    message_box_data.numbuttons  = 2;
		    message_box_data.buttons     = button_data;
		    message_box_data.colorScheme = NULL;
		    if ( SDL_ShowMessageBox(&message_box_data, &buttonid) < 0 )
		        SDL_Log("%s", SDL_GetError());
		}
		if(fechamenu){
		    SDL_SetRenderDrawColor(renderer, 0,0,0,100);
	        SDL_RenderFillRect(renderer,&telaJogo);
	        mostraPlacar(renderer,pontuacao,&pont_rect,num_pont,&num_rect,num,fonte_placar,white);   
		    gerarAlimento(renderer,alimento,&telaJogo);
			flag = call_events(cobra,&telaJogo,buttonid);
			exibeCobra(renderer,cobra);
			colisaoAlimento(alimento,cobra);
             
			if(colisaoCobra(cobra)){
	           if (SDL_ShowSimpleMessageBox(0, "Perdeu o jogo", "A cobra se comeu", NULL) < 0 )
	              SDL_Log("%s", SDL_GetError());
	           else
	           	flag = 0;
			}
            if(flag == 10){
	           if (SDL_ShowSimpleMessageBox(0, "Perdeu o jogo", "A cobra bateu no limite da tela", NULL) < 0 )
	              SDL_Log("%s", SDL_GetError());
	           else
	           	flag = 0;
			}

			if(!flag){
				fechamenu = 0;
				flag = 1;
				if(pont > p){
	                FILE *escrever = fopen("banco.txt","wt");
	                if(escrever == NULL)
		    	        printf("Erro ao criar o arquivo");
	                fprintf(escrever,"%d",pont);
	                fclose(escrever);
                }  
				pont = 0;
				destruirFila(cobra);
				liberaAlimento(alimento);
			}
	   }
	   if(!fechamenu){
			flag = eventoMenu(rbtn_comecar,rbtn_sair);
			menu(renderer,rbtn_comecar,rbtn_sair,flag);
		    SDL_RenderCopy(renderer, titulo, NULL, &titulo_rect);
		    //Desenho da Maior pontuação no Menu
		    SDL_RenderCopy(renderer, textura_placar, NULL, &placar_rect);
		    FILE *ler = fopen("banco.txt","rt");
		    if(ler == NULL)
		    	printf("Erro ao criar o arquivo");
		    resultado = fscanf(ler,"%d",&p);
		    fclose(ler);
		    resultado = fscanf(ler,"%d",&p);
		    sprintf(conv_num, "%d", p);
		    maior = TTF_RenderText_Solid(fonte_placar,conv_num, red); 
	        num_maior = SDL_CreateTextureFromSurface(renderer, maior); 
	        SDL_RenderCopy(renderer,num_maior, NULL, &maior_rect);
	         //Desenho da Maior pontuação no Menu
		    if(flag == 5)
		    	flag = 0;
	    }
		SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);	
		SDL_Delay(100);
		SDL_RenderPresent(renderer);		
	
	}
	liberagrafico();
	SDL_FreeSurface(texto);
    SDL_DestroyTexture(titulo);
    SDL_FreeSurface(txt_pont);
    SDL_DestroyTexture(pontuacao);
     SDL_FreeSurface(texto_placar);
    SDL_DestroyTexture(textura_placar);
     SDL_FreeSurface(maior);
    SDL_DestroyTexture(num_maior);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
    EXIT_FAILURE;
	return 0;
}
void mostraPlacar(SDL_Renderer* renderer, SDL_Texture* pontuacao, SDL_Rect* pont_rect,SDL_Surface* num_pont,SDL_Rect* num_rect,SDL_Texture* num,TTF_Font* fonte_placar,SDL_Color white){
	    SDL_RenderCopy(renderer,pontuacao, NULL, pont_rect);
	    char pb[16];
	    sprintf(pb, "%d", pont);
	    num_pont = TTF_RenderText_Solid(fonte_placar, pb , white); 
        num = SDL_CreateTextureFromSurface(renderer, num_pont); 
        SDL_FreeSurface(num_pont);
        SDL_RenderCopy(renderer,num, NULL, num_rect);
        SDL_DestroyTexture(num);
}
int menu(SDL_Renderer *renderer,SDL_Rect rbtn_comecar,SDL_Rect rbtn_sair,int flag){
   
   fundo = IMG_LoadTexture(renderer,"graficoSnakeC/fundocobra.jpg");
   btn_comecar = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn.png");
   btn_comecar_ver = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn_ver.png");
   btn_comecar_click = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn_ver_click.png");
   btn_sair = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn_sair.png");
   btn_sair_ver = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn_sair_ver.png");
   btn_sair_click = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn_sair_ver_click.png");
   SDL_RenderCopy(renderer,fundo,NULL,NULL);
   if(flag == 2)
   SDL_RenderCopy(renderer,btn_comecar_ver,NULL,&rbtn_comecar);
   else if(flag == 3)
   SDL_RenderCopy(renderer,btn_comecar_click,NULL,&rbtn_comecar);
   else
   SDL_RenderCopy(renderer,btn_comecar,NULL,&rbtn_comecar);
   if(flag == 4)
   SDL_RenderCopy(renderer,btn_sair_ver,NULL,&rbtn_sair);
   else if(flag == 5)
   SDL_RenderCopy(renderer,btn_sair_click,NULL,&rbtn_sair);
   else
   SDL_RenderCopy(renderer,btn_sair,NULL,&rbtn_sair);
   return 1;
}
void liberagrafico(){
	SDL_DestroyTexture(fundo);
    SDL_DestroyTexture(btn_comecar);
    SDL_DestroyTexture(btn_comecar_ver);
    SDL_DestroyTexture(btn_comecar_click);
    SDL_DestroyTexture(btn_sair);
    SDL_DestroyTexture(btn_sair_ver);
    SDL_DestroyTexture(btn_sair_click);
} 
SDL_Rect criaRect(int x,int y,int w,int h){
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;	
}
void call_render(SDL_Renderer *renderer, Registro *reg){	
    SDL_SetRenderDrawColor(renderer, reg->cor[0],reg->cor[1],reg->cor[2],255);
    SDL_RenderFillRect(renderer,&reg->rect);
}
int eventoMenu(SDL_Rect rbtn_comecar,SDL_Rect rbtn_sair){
	int flag = 1;
	
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(((m.x >= rbtn_comecar.x) && (m.x <= rbtn_comecar.x + rbtn_comecar.w)) && ((m.y >= rbtn_comecar.y) && (m.y <= rbtn_comecar.y + rbtn_comecar.h))){
		  	 flag = 2; 
	}
	if(((m.x >= rbtn_sair.x) && (m.x <= rbtn_sair.x + rbtn_sair.w)) && ((m.y >= rbtn_sair.y) && (m.y <= rbtn_sair.y + rbtn_sair.h))){
		  	 flag = 4; 
	}
	while(SDL_PollEvent(&event)){

		if((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)){
			flag = 0;
		}
		if(event.type == SDL_MOUSEMOTION){
		  m.x = event.motion.x;
		  m.y = event.motion.y;
		}
		
		if(((event.button.x >= rbtn_comecar.x) && (event.button.x <= rbtn_comecar.x + rbtn_comecar.w)) && ((event.button.y >= rbtn_comecar.y) && (event.button.y <= rbtn_comecar.y + rbtn_comecar.h))){
		    if((event.type == SDL_MOUSEBUTTONDOWN))	
			 flag = 3;
		}
		if(((event.button.x >= rbtn_sair.x) && (event.button.x <= rbtn_sair.x + rbtn_sair.w)) && ((event.button.y >= rbtn_sair.y) && (event.button.y <= rbtn_sair.y + rbtn_sair.h))){
		    if((event.type == SDL_MOUSEBUTTONDOWN))	
			 flag = 5;
		}
	}

	return flag;
}
int call_events(Fila *cobra,SDL_Rect *telaJogo,int buttonid){
	int flag = 1;
	int colisaotela = 0;
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	while(SDL_PollEvent(&event)){

		if((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE)){
			flag = 0;
			contx = 0;
			conty = 0;
		}
	}
    SDL_Rect *rect = &cobra->inicio->reg.rect;
    SDL_Rect rant = cobra->inicio->reg.rect;     
	if((state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) && (contx < 0 || contx == 0) && (contx != 0 || conty != 0)){		
   		contx = -tamCobra;
   		conty = 0;

	}
	else if((state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) && (contx > 0 || contx == 0)){
   	  contx = tamCobra;
   	  conty = 0;
   		
	}
	else if((state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) && (conty < 0 || conty == 0)){
   		conty = -tamCobra;
   		contx = 0;   	
	}
	else if((state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) && (contx > 0 || conty == 0)){
   		conty = tamCobra;
   		contx = 0;
	}   
	  rect->x += contx;
	  rect->y += conty;
	  if(buttonid){
        colisaotela = limiteTelaColisao(cobra->inicio,telaJogo);
        if(colisaotela){
	  		flag = 10;
	  	    contx = 0;
			conty = 0;
        }
	  }else
	  	limiteTelaAtraveca(cobra->inicio,telaJogo);
	  if(contx != 0 || conty != 0)
        atualizaCobra(cobra,rant);
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
	addTamCobra(cobra);
	addTamCobra(cobra);
	
}
void addTamCobra(Fila *cobra){
  Registro reg;
	reg = cobra->fim->reg;
	reg.rect.x -= tamCobra;
	reg.cor[0] = 255;
	reg.cor[1] = 0;
	reg.cor[2] = 255;
	insereNaCobra(cobra,reg);
}
void aumentaCobra(Fila *cobra){
	Registro reg;
	reg = cobra->fim->reg;
	reg.cor[0] = 255;
	reg.cor[1] = 0;
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
void limiteTelaAtraveca(Elemento *e,SDL_Rect *telaJogo){
   if((e->reg.rect.x + e->reg.rect.w) > (telaJogo->w + telaJogo->x)){
     e->reg.rect.x = telaJogo->x;
   }
    if(e->reg.rect.x < telaJogo->x){
      e->reg.rect.x = telaJogo->w + telaJogo->x;
    }
    if((e->reg.rect.y + e->reg.rect.h) > (telaJogo->h + telaJogo->y)){
      e->reg.rect.y = telaJogo->y;
    }
    if(e->reg.rect.y < telaJogo->y){
      e->reg.rect.y = telaJogo->h + telaJogo->y;
    }
}
int limiteTelaColisao(Elemento *e,SDL_Rect *telaJogo){
   if((e->reg.rect.x + e->reg.rect.w) > (telaJogo->w + telaJogo->x)){
     return 1;
   }
    if(e->reg.rect.x < telaJogo->x){
      return 1;
    }
    if((e->reg.rect.y + e->reg.rect.h) > (telaJogo->h + telaJogo->y)){
      return 1;
    }
    if(e->reg.rect.y < telaJogo->y){
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
int colisaoCobra(Fila *cobra){
	SDL_Rect cabeca;
	Elemento *atual = cobra->inicio->prox;
	cabeca = cobra->inicio->reg.rect;
	while(atual->prox != NULL){
       if((cabeca.x >= atual->reg.rect.x) && (cabeca.x + cabeca.w <= atual->reg.rect.x + atual->reg.rect.w)){
         if((cabeca.y == atual->reg.rect.y) && (cabeca.y + cabeca.h <= atual->reg.rect.y + atual->reg.rect.h)){
     	    return 1;
     	 }
        }
       atual = atual->prox;	   
	}
	return 0;
}
void exibeCobra(SDL_Renderer *renderer,Fila* cobra){
	Elemento *end = cobra->inicio;
	Elemento *aux;
	while(end != NULL){
		call_render(renderer,&end->reg);
	    end = end->prox;
	}
}
void liberaCobra(Fila* cobra){
    free(cobra);
}
void destruirFila(Fila* cobra) {
  Elemento* end = cobra->inicio;
  while (end != NULL){
    Elemento* apagar = end;
    end = end->prox;
    free(apagar);
  }
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
int gerarAlimento(SDL_Renderer *renderer,Objeto *alimento,SDL_Rect *telaJogo){
    if(alimento->ativo == 0){
    srand((unsigned) time(&t));
    int x = (((rand() % ((telaJogo->w) / tamCobra))) * tamCobra) + telaJogo->x;
    int y = (((rand() % ((telaJogo->h)/ tamCobra))) * tamCobra) + telaJogo->y;
    alimento->reg.rect.x = x;
    alimento->reg.rect.y = y;
    alimento->ativo = 1;
    }
        call_render(renderer,&alimento->reg);
 }

  int colisaoAlimento(Objeto *alimento,Fila *cobra){
 	int CobraLar = cobra->inicio->reg.rect.x + (cobra->inicio->reg.rect.w), CobraAlt = cobra->inicio->reg.rect.y + (cobra->inicio->reg.rect.h);
 	int AlimMaxLar = alimento->reg.rect.x + alimento->reg.rect.w, AlimMaxAlt = alimento->reg.rect.y + alimento->reg.rect.h;
    if((cobra->inicio->reg.rect.x >= alimento->reg.rect.x) && (CobraLar <= AlimMaxLar)){
       if((cobra->inicio->reg.rect.y == alimento->reg.rect.y) && (CobraAlt <= AlimMaxAlt)){
    	aumentaCobra(cobra);
    	pont++;
    	inicializaAlimento(alimento);
        return 1;
       }
    }
   return 0;
 }
 
 

