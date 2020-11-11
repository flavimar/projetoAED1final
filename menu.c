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
void call_render(SDL_Renderer*);
int call_events(SDL_Rect);
SDL_Rect criaRect(int x,int y,int w,int h){
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;	
}
int menu(SDL_Renderer *renderer,SDL_Rect rbtn_comecar,int flag){
   
   fundo = IMG_LoadTexture(renderer,"graficoSnakeC/fundocobra.jpg");
   btn_comecar = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn.png");
   btn_comecar_ver = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn_ver.png");
   btn_comecar_click = IMG_LoadTexture(renderer,"graficoSnakeC/btn/btn_ver_click.png");
   SDL_RenderCopy(renderer,fundo,NULL,NULL);
   printf("%d %d -- %d\n",m.x,m.y,flag);
   if(flag == 2)
   SDL_RenderCopy(renderer,btn_comecar_ver,NULL,&rbtn_comecar);
   else if(flag == 3)
   SDL_RenderCopy(renderer,btn_comecar_click,NULL,&rbtn_comecar);
   else
   SDL_RenderCopy(renderer,btn_comecar,NULL,&rbtn_comecar);
   return 1;
}
void liberagrafico(){
	SDL_DestroyTexture(fundo);
    SDL_DestroyTexture(btn_comecar);
    SDL_DestroyTexture(btn_comecar_ver);
    SDL_DestroyTexture(btn_comecar_click);
} 
int main(int argc, char **argv){   
	int flag;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect rbtn_comecar,rbtn_sair;
    rbtn_comecar = criaRect(larguratela/4,100,500,50);
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
	flag = 1;
	int fechamenu = 0;
	
	 while(flag){
	 	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	    SDL_RenderClear(renderer);	
		flag = call_events(rbtn_comecar);
		if(!fechamenu)
		menu(renderer,rbtn_comecar,flag);
		SDL_Delay(100);
		SDL_RenderPresent(renderer);		
		if(flag == 3){
			liberagrafico();
			fechamenu = 1;
		}
	}
    
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

int call_events(SDL_Rect rbtn_comecar){
	int flag = 1;
	
	SDL_Event event;
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	//printf("%ld ",sizeof(SDL_GetKeyboardState));
    if(((m.x >= rbtn_comecar.x) && (m.x <= rbtn_comecar.x + rbtn_comecar.w)) && ((m.y >= rbtn_comecar.y) && (m.y <= rbtn_comecar.y + rbtn_comecar.h))){
		  	 flag = 2; 
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
	}

	return flag;
}


