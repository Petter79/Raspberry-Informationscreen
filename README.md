# Raspberry-Informationscreen
How to run an infoscreen on a Raspberry pi
Written in c with SDL

In summary, the program initializes SDL, creates a window, loads textures, enters a game loop, processes user input, updates the game state, renders the game, and exits when the user chooses to do so. The main focus of this example is rendering a background, displaying the date and time, and handling user input to exit the program.

Initialization (init function):

Initializes the SDL (Simple DirectMedia Layer) library for handling window creation and rendering.
Creates a window and a renderer using SDL.
Initializes the SDL_image library for loading image textures.
Loads two textures: one for characters (Amiga_Charset_16x16.png) and one for the background (background.jpeg).
Main Game Loop (main function):

Sets up the game by calling the init function.
Enters a while loop that continues as long as game_is_running is true.
Inside the loop:
Calls process_input to handle user input (checks for window close or the Escape key to exit the game).
Calls update for updating the game state (currently empty in this example, but you can add game logic here).
Calls render to draw the game on the screen, including rendering the background and displaying the current date and time.
Input Processing (process_input function):

Uses the SDL event system to check for events (e.g., window close or key presses).
If the user closes the window or presses the Escape key, it sets game_is_running to false, indicating that the game should exit.
Rendering (render function):

Clears the renderer and renders the background.
Retrieves the current date and time and formats it into a string.
Calls printString to render the formatted date and time on the screen.
Presents the rendered content to the window.
Cleaning Up (quit function):

Destroys textures, renderer, and window.
Quits SDL and SDL_image.
Date and Time Formatting (datetime function):

Allocates memory for a string and populates it with the current date and time.
Character Rendering (renderText and printString functions):

renderText takes a character and renders it on the screen at a specified position and scale.
printString uses renderText to render an entire string on the screen.
Update Function (update function):

Currently empty. You can add game logic or state updates here.
Exiting the Program:

When the user decides to exit the game (closes the window or presses the Escape key), game_is_running is set to false.
The loop exits, and the program proceeds to the cleanup phase (quit function).
The program exits with a success status (EXIT_SUCCESS).
