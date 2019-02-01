#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int fps = 60;

int main( int argc, char* agrs[])
{
	SDL_Window* window = NULL;
	SDL_Surface *background = NULL;
	SDL_Surface *screen = NULL;
	SDL_Surface *car = NULL;
    SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	
	SDL_Init (SDL_INIT_EVERYTHING );
	
	const int speed = 5;
	SDL_Rect imgloc = {120, 394, 40, 40};
	SDL_Rect camera;
    camera.x = 0; //Don't worry about this camera, I need this after i get the background working.
    camera.y = 0;
    camera.w = 640;
    camera.h = 480;
    bool b[2] = {0,0};
    int x = 0, y = 0;
    Uint32 start;
	if ( SDL_Init (SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	
	else 
	{
		 window = SDL_CreateWindow( "1/4 Mile Race", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		 
		 if( window == NULL )
		 {
			  printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() ); 
		 }
		 else
		 {
			background = SDL_LoadBMP("background.bmp");
			
			if(background == NULL)
			{
				SDL_ShowSimpleMessageBox(0, "Background init error",         SDL_GetError(), window);
			}
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL)
			{
				SDL_ShowSimpleMessageBox(0, "Renderer init error", SDL_GetError(), window);
			}
			texture = SDL_CreateTextureFromSurface(renderer,background);
			if(texture == NULL)
			{
				SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
			}
				SDL_RenderCopy(renderer, texture, &camera, NULL);
				SDL_RenderPresent(renderer);
		}
	 }
	 
    
    bool running = true;
    while (true)
    {
        start = SDL_GetTicks();
        SDL_Event event;

        while(SDL_WaitEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                running = false;
                SDL_DestroyTexture(texture);
                SDL_FreeSurface(background);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    b[0]=1;
                    break;
                case SDLK_LEFT:
                    b[1]=1;
                    break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                case SDLK_UP:
                    b[0]=0;
                    break;
                case SDLK_LEFT:
                    b[1]=0;
                    break;
                }
                break;
            }
            if(b[0])
        {
            x+=speed;
            camera.x += speed;
        }
        else if(b[1])
        {
            x-=speed;
            camera.x-=speed;
        }
         SDL_RenderCopy(renderer, texture, &camera, NULL);
        SDL_BlitSurface(background, &camera, screen, NULL);
        SDL_RenderPresent(renderer);
        
        if(1000/fps>SDL_GetTicks()-start)
        {
            SDL_Delay(1000/fps-(SDL_GetTicks() - start));
        }
     }
        
	 return 0;
 }
}
class Bike
{
	public:
		static const int BIKE_WIDTH = 40;
		static const int BIKE_HEIGHT = 40;
		
		Bike();
		
		void handleEvent( SDL_Event& e );
		
		void move();
		
		void render();
		
	private:
		int PosX, PosY;
		int VelX;
};
