#include <SDL.h>
#include <SDL_image.h>
#include <exception>
#include <iostream>
#include <string>
#include <map>

#define ASSERT(cnd, msg) if (cnd) throw std::exception(&(msg)[0]);

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 400

void Init(SDL_Window* &window, SDL_Renderer* &renderer) {
	ASSERT(SDL_Init(SDL_INIT_VIDEO) < 0, "SDL could not initialize!");
	ASSERT(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"), "Warning: Linear texture filtering not enabled!");
	window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	ASSERT(window == nullptr, "Window could not be created!");
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	ASSERT(renderer == nullptr, "Renderer could not be created!");
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void Close(SDL_Window* &window, SDL_Renderer* &renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;
	SDL_Quit();
}

int main(int argc, char *argv[]) {


	try {
		//KEYBOARD
		std::map<int, bool> keyboard;

		//VARIABLES DE WINDOW Y RENDERER
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		//VARIABLES NUEVAS

		//INICIO DE PANTALLA
		Init(window, renderer);

		//CARGAMOS IMAGEN
		IMG_Init(IMG_INIT_JPG);
		SDL_Surface * image = IMG_Load("../../res/background.jpg");
		SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

		//MOUSE EN EL CENTRO DE LA PANTALLA
		SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

		//BOTONES
		SDL_Rect box;

		//EVENTOS
		SDL_Event e;
		bool quit = false;
		while (!quit) {
			//INIT CODE
			if (!SDL_PollEvent(&e)) {
				switch (e.type)
				{
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym)
						{
						case SDLK_ESCAPE:  
							quit = true;
							break;
						}
						break;
				default:
					break;
				}
			}
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
		Close(window, renderer);
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		auto sdlError = SDL_GetError();
		if (strlen(sdlError)) std::cout << "SDL Error: " << sdlError << std::endl;
		std::cin.get();
	}
	return 0;
}