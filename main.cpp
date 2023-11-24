// g++ ./main.cpp -o main -lSDL2 -lSDL2_image

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = true;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Texture *background= NULL;
SDL_Color color = { 255, 255, 255 };
int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

char *datetime()
{
    char *array = (char*)malloc(sizeof(char)*25);
    time_t result;
    result = time(NULL);
    sprintf(array, "%s", asctime(localtime(&result)));
    array[25] = '\0';
    return array;
}

///////////////////////////////////////////////////////////////////////////////
// Function to poll SDL events and process keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = false;
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render number to screen
///////////////////////////////////////////////////////////////////////////////
void renderText(int col, int x, int y, int scale)
{
    SDL_Rect texturePart;
    SDL_Rect textureRect;

    int row = 0;

    if (col>15)
    {
        while (col > 15) {
            col -= 16;
            row++;
            }
    }

    texturePart = { col * 36+38 ,  row*48+50, 26, 42};
    textureRect = {x*scale,  y, 26*scale, 42*scale};
   	SDL_RenderCopy(renderer, texture, &texturePart , &textureRect);
}

void printString(const char* text, int l, int x,int y, int scale)
{
    for (int i =0;i<l;i++)
    {
        renderText(text[i],x+i*26,y,scale);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    time_t rawtime;
    struct tm * timeinfo;

    char buffer[25];
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    printString(buffer,strlen(buffer), 30,48, 3);

    snprintf(buffer, sizeof(buffer), "%d.%d.%d", timeinfo->tm_mday, timeinfo->tm_mon +1, timeinfo->tm_year+ 1900);
    printString(buffer,strlen(buffer), 0, 1080-48, 1);

    SDL_RenderPresent(renderer);
}

///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
}

///////////////////////////////////////////////////////////////////////////////
// Initializing
///////////////////////////////////////////////////////////////////////////////
void init()
{
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT,SDL_WINDOW_FULLSCREEN, &window, &renderer);
	IMG_Init(IMG_INIT_PNG);
	texture = IMG_LoadTexture(renderer, "Amiga_Charset_16x16.png");
	background = IMG_LoadTexture(renderer, "background.jpeg");
}

///////////////////////////////////////////////////////////////////////////////
// Cleaning up
///////////////////////////////////////////////////////////////////////////////
void quit()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(background);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(void) {
    init();
	while (game_is_running) {
	        process_input();
        	update();
	        render();
        	SDL_Delay(100);
	}
	quit();
	return EXIT_SUCCESS;
}

