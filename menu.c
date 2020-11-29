#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MOV 0.3
#define tamCobra 10
#define alturatela 660
#define larguratela 1240
typedef struct 
{
	int x;
	int y;
}Mouse;
Mouse m;
int contx = 0;
int conty = 0;
int tamx = 0,tamy = 0;
SDL_Texture *fundo;
SDL_Texture *btn_comecar;
SDL_Texture *btn_comecar_ver;
SDL_Texture *btn_comecar_click;
SDL_Texture *btn_sair;
SDL_Texture *btn_sair_ver;
SDL_Texture *btn_sair_click;
void call_render(SDL_Renderer*);
int call_events(SDL_Rect,SDL_Rect);
SDL_Rect criaRect(int x,int y,int w,int h){
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;	
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
int main(int argc, char **argv){   
	int flag;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect rbtn_comecar,rbtn_sair;
    rbtn_comecar = criaRect(larguratela/4,200,500,50);
    rbtn_sair = criaRect(larguratela/4,300,500,50);
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
	TTF_Init();
	SDL_Event event;
	flag = 1;
	int fechamenu = 0;
	//FONTE----------------------------------------------
    	TTF_Font *fonte; 
    	fonte = TTF_OpenFont("FreeSans.ttf", 24);
    	if (fonte == NULL) {
          printf("Erro ao inicializar fonte: %s", TTF_GetError());
        exit(EXIT_FAILURE);
        }
	SDL_Color White = {255, 255, 255};
    SDL_Surface* texto = TTF_RenderText_Solid(fonte, "SnakeC", White); 
    SDL_Texture* titulo = SDL_CreateTextureFromSurface(renderer, texto); 
	SDL_Rect titulo_rect = criaRect(larguratela/4,50,500,100);
	//FONTE----------------------------------------------
	 while(flag){
	 	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	    SDL_RenderClear(renderer);	
		flag = call_events(rbtn_comecar,rbtn_sair);
		if(!fechamenu){
		menu(renderer,rbtn_comecar,rbtn_sair,flag);
	    SDL_RenderCopy(renderer, titulo, NULL, &titulo_rect);
	    }
		SDL_Delay(100);
		SDL_RenderPresent(renderer);		
		if(flag == 3 || flag == 0){
			liberagrafico();
			fechamenu = 1;
		}
		if(flag == 5)
			flag = 0;
	}
    TTF_Quit();
    SDL_FreeSurface(texto);
    SDL_DestroyTexture(titulo);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

/*void call_render(SDL_Renderer *renderer, Registro *reg){

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, reg->cor[0], reg->cor[1],reg->cor[2]);
    SDL_RenderFillRect(renderer,&reg->rect);
	
	
}*/

int call_events(SDL_Rect rbtn_comecar,SDL_Rect rbtn_sair){
	int flag = 1;
	
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	//printf("%ld ",sizeof(SDL_GetKeyboardState));
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


