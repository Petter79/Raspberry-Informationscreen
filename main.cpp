#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctime>
#include <cstring>

// Global variables
bool isGameRunning = true;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* characterTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

// Function to handle user input
void handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            isGameRunning = false;
        }
    }
}

// Function to render a character sprite
void renderCharacter(int charIndex, int x, int y, int scale) {
    int row = charIndex / 16;
    int col = charIndex % 16;
    
    SDL_Rect src = {col * 36 + 38, row * 48 + 50, 26, 40};
    SDL_Rect dest = {x * scale, y, 26 * scale, 40 * scale};
    
    SDL_RenderCopy(renderer, characterTexture, &src, &dest);
}

// Function to render a string of characters
void renderString(const char* text, int x, int y, int scale) {
    for (int i = 0; text[i] != '\0'; i++) {
        renderCharacter(text[i], x + i * 26, y, scale);
    }
}

// Function to get current time as a string
void getCurrentTime(char* buffer, int bufferSize) {
    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);
    strftime(buffer, bufferSize, "%d-%m-%Y %H:%M:%S", timeInfo);
}

// Function to render everything on screen
void render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    char timeString[25];
    getCurrentTime(timeString, sizeof(timeString));
    renderString(timeString, 400, 30, 2);

    SDL_RenderPresent(renderer);
}

// Function to initialize SDL and load resources
bool initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (!(window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_FULLSCREEN))) return false;
    if (!(renderer = SDL_CreateRenderer(window, -1, 0))) return false;
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) return false;

    characterTexture = IMG_LoadTexture(renderer, "Amiga_Charset_16x16.png");
    backgroundTexture = IMG_LoadTexture(renderer, "background.jpeg");
    if (!characterTexture || !backgroundTexture) return false;

    return true;
}

// Function to clean up resources
void cleanup() {
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

// Main function
int main() {
    if (!initialize()) {
        cleanup();
        return 1;
    }

    while (isGameRunning) {
        handleEvents();
        render();
        SDL_Delay(100);  // Add a small delay to control frame rate
    }

    cleanup();
    return 0;
}