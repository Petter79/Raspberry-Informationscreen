#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctime>
#include <string>

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* characterTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
bool isRunning = true;

// Initialize SDL and create window, renderer, and textures
bool initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (!(window = SDL_CreateWindow("Time Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN))) return false;
    if (!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    
    characterTexture = IMG_LoadTexture(renderer, "Amiga_Charset_16x16.png");
    backgroundTexture = IMG_LoadTexture(renderer, "background.jpeg");
    if (!characterTexture || !backgroundTexture) return false;

    return true;
}

// Clean up resources
void cleanup() {
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

// Handle SDL events
void handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            isRunning = false;
        }
    }
}

// Render a single character from the sprite sheet
void renderCharacter(char c, int x, int y, int scale) {
    int index = static_cast<int>(c);
    int row = index / 16;
    int col = index % 16;

    SDL_Rect src = {col * 36 + 38, row * 48 + 50, 26, 40};
    SDL_Rect dst = {x, y, 26 * scale, 40 * scale};
    SDL_RenderCopy(renderer, characterTexture, &src, &dst);
}

// Render a string using the character sprite sheet
void renderString(const std::string& text, int x, int y, int scale) {
    for (size_t i = 0; i < text.length(); i++) {
        renderCharacter(text[i], x + i * 26 * scale, y, scale);
    }
}

// Get current time as a formatted string
std::string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", ltm);
    return std::string(buffer);
}

// Render the scene
void render() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    renderString(getCurrentTime(), 250, WINDOW_HEIGHT - 108, 2);
    SDL_RenderPresent(renderer);
}

int main() {
    if (!initialize()) {
        cleanup();
        return 1;
    }

    while (isRunning) {
        handleEvents();
        render();
        SDL_Delay(1000); // Update every second
    }

    cleanup();
    return 0; 