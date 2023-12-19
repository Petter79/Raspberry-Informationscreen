// Compile with: g++ ./main.cpp -o main -lSDL2 -lSDL2_image

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
int game_is_running = true; // In summary, game_is_running is a control variable that determines whether the game loop should continue running or if it's time to exit the game.
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
SDL_Texture* background = NULL;
SDL_Color color = {255, 255, 255};
int WINDOW_WIDTH = 1920;
int WINDOW_HEIGHT = 1080;

///////////////////////////////////////////////////////////////////////////////
// Function to get current date and time
///////////////////////////////////////////////////////////////////////////////
char* datetime() {
    char* array = (char*)malloc(sizeof(char) * 25);
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
                game_is_running = false; // Quit the game loop if the window is closed
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = false; // Quit the game loop if the Escape key is pressed
                }
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render number to screen
///////////////////////////////////////////////////////////////////////////////
void renderText(int col, int x, int y, int scale) {
    SDL_Rect texturePart;
    SDL_Rect textureRect;

    int row = 0;

    if (col > 15) {
        while (col > 15) {
            col -= 16;
            row++;
        }
    }

    texturePart = {col * 36 + 38, row * 48 + 50, 26, 40}; // Define the portion of the texture to render
    textureRect = {x * scale, y, 26 * scale, 40 * scale}; // Define the position and size on the screen
    SDL_RenderCopy(renderer, texture, &texturePart, &textureRect); // Copy the texture to the renderer
}

void printString(const char* text, int l, int x, int y, int scale) {
    for (int i = 0; i < l; i++) {
        renderText(text[i], x + i * 26, y, scale); // Render each character in the string
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function to draw game objects in the SDL window
///////////////////////////////////////////////////////////////////////////////
void render(void) {
    SDL_RenderClear(renderer); // Clear the renderer
    SDL_RenderCopy(renderer, background, NULL, NULL); // Render the background texture

    time_t rawtime;
    struct tm* timeinfo;

    char buffer[25];
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    snprintf(buffer, sizeof(buffer), "%d-%d-%d %02d:%02d:%02d", timeinfo->tm_mday, timeinfo->tm_mon + 1,
             timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    printString(buffer, strlen(buffer), 250, 1080 - 108, 2); // Render the formatted date and time
    SDL_RenderPresent(renderer); // Update the screen
}

///////////////////////////////////////////////////////////////////////////////
// Update function with a fixed time step
///////////////////////////////////////////////////////////////////////////////
void update(void) {
    // Add any update logic here
}

///////////////////////////////////////////////////////////////////////////////
// Initializing
///////////////////////////////////////////////////////////////////////////////
void init() {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO); // Initialize SDL
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN, &window, &renderer); // Create window and renderer
    IMG_Init(IMG_INIT_PNG); // Initialize SDL_image
    texture = IMG_LoadTexture(renderer, "Amiga_Charset_16x16.png"); // Load character texture
    background = IMG_LoadTexture(renderer, "background.jpeg"); // Load background texture
}

///////////////////////////////////////////////////////////////////////////////
// Cleaning up
///////////////////////////////////////////////////////////////////////////////
void quit() {
    SDL_DestroyTexture(texture); // Free character texture
    SDL_DestroyTexture(background); // Free background texture
    IMG_Quit(); // Quit SDL_image
    SDL_DestroyRenderer(renderer); // Destroy renderer
    SDL_DestroyWindow(window); // Destroy window
    SDL_Quit(); // Quit SDL
}

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(void) {
    init(); // Initialize the game
    while (game_is_running) {
        process_input(); // Process user input
        update(); // Update game state
        render(); // Render the game
        SDL_Delay(100); // Add a delay to control the frame rate
    }
    quit(); // Clean up resources
    return EXIT_SUCCESS; // Exit the program
}
