#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>


unsigned int time(void)
{
	unsigned int lastTime = 0, currentTime;
	

		currentTime = SDL_GetTicks();

		if(currentTime > lastTime + 1000) {

			lastTime = currentTime;
		}
		return lastTime/1000;
}

int main()
{
	
	int sec = 0, count = 0;

	while(1){

		sec = time();
		
		if(sec > count){

			printf("seconds: %d\n", sec);
			count = sec;
		}
	}

	return 0;
}
