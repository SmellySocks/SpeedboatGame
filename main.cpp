#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int fps = 100;
class Car
{
	public:
		static const int BIKE_WIDTH = 40;
		static const int BIKE_HEIGHT = 40;
		
		void handleEvent( SDL_Event& e );
		int b=0;
		void move();
		double velocity=0;
		void render();
		double force;
		double acceleration;
		void gearup();
		int acc;
		int torque_curve[14] = {15, 20, 58, 80, 100, 107, 113, 117, 120, 117, 112, 107, 99, 79 };   
		void setThrottle();
		int torque_index;
		int i;
		int vel;
	int throttle;
	int engine_rpm;
	int rpm_ratio;
	double wheel_rpm;
	int base_torque = 95;
	double torque;
	double final_drive = 4.44;
	double radius = 0.1631;
	int PosX, PosY;
		int VelX;
		double gear[6] = {0, 2.92, 1.55, 1, 0.81, 0.72};
		
};
void Car::gearup()
{
	if(b<5)
		b++;
}
void Car::setThrottle()
{
	throttle=1;
	
}
void Car::move()
{
	wheel_rpm = floor((velocity+1)/(2*M_PI*radius/60));
	engine_rpm = floor(wheel_rpm*gear[b]*final_drive);
	if (engine_rpm>6500)
    engine_rpm=6500;
    if (engine_rpm <1000)
	engine_rpm = 1000;
    torque_index = floor(engine_rpm/500);
	torque = torque_curve[torque_index]*gear[b]*final_drive;
	force = throttle*torque/(2*radius);
	acceleration = force/1080;
	acc=floor(acceleration);
	velocity+=(acceleration*0.016);
	vel = floor(velocity);
	
	
}

int main( int argc, char* agrs[])
{
	SDL_Window* window = NULL;
	SDL_Surface *background = NULL;
	SDL_Surface *vehicle = NULL;
	SDL_Surface *screen = NULL;
    SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	SDL_Texture *carTexture = NULL;
	SDL_Texture *message = NULL;
	SDL_Rect CarRect;
	CarRect.x=15;
	CarRect.y=440;
	CarRect.w=40;
	CarRect.h=40;
	
	SDL_Init (SDL_INIT_EVERYTHING );
	Car hyundai;
	int speed = 0;
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
		 window = SDL_CreateWindow( "1/4 Mile sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		 
		 if( window == NULL )
		 {
			  printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() ); 
		 }
		 else
		 {
			background = SDL_LoadBMP("background.bmp");
			vehicle = SDL_LoadBMP("Hyundai.bmp");
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
			carTexture = SDL_CreateTextureFromSurface(renderer,vehicle);
			if(texture == NULL)
			{
				SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
			}
				SDL_RenderCopy(renderer, texture, &camera, NULL);
				SDL_RenderCopy(renderer, carTexture, &CarRect, NULL);
				SDL_RenderPresent(renderer);
		}
	 }
	 
	bool move = false;
    int curr_gear=0;
    bool running = true;
    while (running)
    {
        start = SDL_GetTicks();
        SDL_Event event;
        

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                running = false;
                cout<<"terminting"<<endl;
                SDL_DestroyTexture(texture);
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
                    move=true;
                    break;
                case SDLK_LEFT:
                    b[1]=1;
                    break;
				case SDLK_d:
					hyundai.gearup();
					hyundai.move();
					cout << hyundai.b;
					cout << "force: " << hyundai.force<<endl;
					cout << "vel: " << hyundai.velocity<<endl;
					cout << "torque: " << hyundai.torque<<endl;
					break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                case SDLK_RIGHT:
                    b[0]=0;
                    break;
                case SDLK_LEFT:
                    b[1]=0;
                    break;
                }
                break;
            }
            
          }
          if(1000/fps>SDL_GetTicks()-start)
        {
            SDL_Delay(1000/fps-(SDL_GetTicks() - start));  
          if(move)
        {
            hyundai.setThrottle();
			hyundai.move();
			if(hyundai.vel >=0)
			speed=hyundai.vel*10/60;
            camera.x += floor(speed);
            //cout<<camera.x<<endl;
            if(camera.x >=4200-640)
            {
				camera.x=4200-640;	
				hyundai.throttle = 0;
        }
        
        else if(move==false)
        {
			speed=floor(hyundai.velocity*10);
            x-=speed;
            camera.x-=speed;
        }
	}
        cout<<"revs: "<<hyundai.engine_rpm<<endl;
        cout<<"speed: "<<hyundai.vel*3.6<<endl;
        cout<<"gear: "<<hyundai.b<<endl;
        cout<<"torque: "<<hyundai.torque<<endl;
        SDL_BlitSurface(vehicle, NULL, screen, &CarRect);
        SDL_BlitSurface(background, &camera, screen, NULL);
        
        SDL_RenderCopy(renderer, texture, &camera, NULL);
        SDL_RenderPresent(renderer);
        
	}
	 
 }
 SDL_Quit();
 return 0;
}

