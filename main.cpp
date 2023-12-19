#include <cstdlib>
#include <ctime>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Control variable to determine whether the game loop should continue running
bool isGameRunning = true;

// SDL-related variables
SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainRenderer = nullptr;
SDL_Texture* characterTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Color textColor = {255, 255, 255};
const int WindowWidth = 1920;
const int WindowHeight = 1080;

// Function to handle SDL events and process keyboard input
void handleEvents(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isGameRunning = false; // Quit the game loop if the window is closed
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isGameRunning = false; // Quit the game loop if the Escape key is pressed
                }
                break;
        }
    }
}

// Render a character sprite on the screen
void renderCharacterSprite(int characterIndex, int screenX, int screenY, int scale) {
    SDL_Rect spriteSourceRect;  // Portion of the character texture to render
    SDL_Rect destinationRect;   // Position and size on the screen

    int spriteRow = 0;

    // Adjust column and row indices for character spritesheet
    if (characterIndex > 15) {
        while (characterIndex > 15) {
            characterIndex -= 16;
            spriteRow++;
        }
    }

    // Define the portion of the texture to render
    spriteSourceRect = {characterIndex * 36 + 38, spriteRow * 48 + 50, 26, 40};
    
    // Define the position and size on the screen
    destinationRect = {screenX * scale, screenY, 26 * scale, 40 * scale};
    
    // Copy the character texture to the renderer
    SDL_RenderCopy(mainRenderer, characterTexture, &spriteSourceRect, &destinationRect);
}

// Render a string to the screen
void renderString(const char* text, int length, int x, int y, int scale) {
    for (int i = 0; i < length; i++) {
        renderCharacterSprite(text[i], x + i * 26, y, scale); // Render each character in the string
    }
}

// Render function to draw game objects in the SDL window
void render(void) {
    // Clear the renderer
    SDL_RenderClear(mainRenderer);

    // Render the background texture
    SDL_RenderCopy(mainRenderer, backgroundTexture, nullptr, nullptr);

    // Get current time
    time_t rawTime;
    struct tm* timeInfo;

    char buffer[25];
    time(&rawTime);
    timeInfo = localtime(&rawTime);

    // Format the time as a string
    snprintf(buffer, sizeof(buffer), "%d-%d-%d %02d:%02d:%02d",
             timeInfo->tm_mday, timeInfo->tm_mon + 1, timeInfo->tm_year + 1900,
             timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    // Render the formatted date and time
    renderString(buffer, strlen(buffer), 250, WindowHeight - 108, 2);

    // Update the screen
    SDL_RenderPresent(mainRenderer);
}

// Update function with a fixed time step
void update(void) {
    // Add any update logic here
}

// Initializing
void initialize() {
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO); // Initialize SDL
    SDL_CreateWindowAndRenderer(WindowWidth, WindowHeight, SDL_WINDOW_FULLSCREEN, &mainWindow, &mainRenderer); // Create window and renderer
    IMG_Init(IMG_INIT_PNG); // Initialize SDL_image
    characterTexture = IMG_LoadTexture(mainRenderer, "Amiga_Charset_16x16.png"); // Load character texture
    backgroundTexture = IMG_LoadTexture(mainRenderer, "background.jpeg"); // Load background texture
}

// Cleaning up
void cleanup() {
    SDL_DestroyTexture(characterTexture); // Free character texture
    SDL_DestroyTexture(backgroundTexture); // Free background texture
    IMG_Quit(); // Quit SDL_image
    SDL_DestroyRenderer(mainRenderer); // Destroy renderer
    SDL_DestroyWindow(mainWindow); // Destroy window
    SDL_Quit(); // Quit SDL
}

// Main function
int main(void) {
    initialize(); // Initialize the game
    while (isGameRunning) {
        handleEvents(); // Process user input
        update(); // Update game state
        render(); // Render the game
        SDL_Delay(100); // Add a delay to control the frame rate
    }
    cleanup(); // Clean up resources
    return EXIT_SUCCESS; // Exit the program
}
