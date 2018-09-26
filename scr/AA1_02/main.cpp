#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
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

		//SONIDO
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
		Mix_Music *music = Mix_LoadMUS("../../res/music1.mp3");
		Mix_PlayMusic(music, 1);

		//CARGAMOS IMAGEN
		SDL_Surface * image = IMG_Load("../../res/background.jpg");
		SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

		//MOUSE EN EL CENTRO DE LA PANTALLA
		SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

		//BOTON PLAY
		TTF_Init();
		TTF_Font *font{ TTF_OpenFont("../../res/opensans.ttf", 255) };
		SDL_Surface *tmpSurfPlay{ TTF_RenderText_Blended(font,"Play", SDL_Color{ 0,0,0,0 }) };
		SDL_Texture *textTexturePlay{ SDL_CreateTextureFromSurface(renderer,tmpSurfPlay) };

		//BOTON SOUNDOFF
		SDL_Surface *tmpSurfSound{ TTF_RenderText_Blended(font,"Sound Off", SDL_Color{ 0,0,0,0 }) };
		SDL_Texture *textTextureSound{ SDL_CreateTextureFromSurface(renderer,tmpSurfSound) };

		//BOTON EXTI
		SDL_Surface *tmpSurfExit{ TTF_RenderText_Blended(font,"Exit", SDL_Color{ 0,0,0,0 }) };
		SDL_Texture *textTextureExit{ SDL_CreateTextureFromSurface(renderer,tmpSurfExit) };
		
		//RECTANGULO DE COLOR PLAY
		SDL_Rect textRectPlay{ 10, 10, 100, 50 };

		//RECTANGULO DE COLOR SOUND OFF
		SDL_Rect textRectSound{ 10, 110, 100, 50 };

		////RECTANGULO DE COLOR EXIT
		SDL_Rect textRectExit{ 10, 210, 100, 50 };

		//LIBERAMOS
		SDL_FreeSurface(tmpSurfPlay);
		SDL_FreeSurface(tmpSurfSound);
		SDL_FreeSurface(tmpSurfExit);

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
					case SDL_MOUSEMOTION:
						int x; int y;
						SDL_GetMouseState(&x, &y);

						//ENCIMA DE PLAY
						if(((x > 10) && (x < 110)))
						{
							if((y > 10) && (y < 60))
							{
								std::cout << "Play" << std::endl;
							}
						}

						//ENCIMA DE SOUNDOFF
						if (((x > 10) && (x < 110)))
						{
							if ((y > 110) && (y < 160))
							{
								//std::cout << "Sound Off" << std::endl;
							}
						}

						//ENCIMA DE EXIT
						if (((x > 10) && (x < 110)))
						{
							if ((y > 210) && (y < 260))
							{
								std::cout << "Exit" << std::endl;
							}
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (((x > 10) && (x < 110)))
						{
							if ((y > 210) && (y < 260))
							{
								quit = true;
							}
						}

						if (((x > 10) && (x < 110)))
						{
							if ((y > 110) && (y < 160))
							{
								//If music is being played
								if(Mix_PlayingMusic() == 1)
								{
									//If the music is paused
									if (Mix_PausedMusic() == 1)
									{
										//Resume the music
										Mix_ResumeMusic();
									}
									//If the music is playing
									else
									{
										//Pause the music
										Mix_PauseMusic();
									}
								}
							}
						}

						break;
				default:
					break;
				}
			}
			//BACKGROUND
			SDL_RenderCopy(renderer, texture, NULL, NULL);

			//RECTANGULO
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(renderer, &textRectPlay);
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			SDL_RenderFillRect(renderer, &textRectSound);
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(renderer, &textRectExit);

			//TEXT
			SDL_RenderCopy(renderer, textTexturePlay, nullptr, &textRectPlay);
			SDL_RenderCopy(renderer, textTextureSound, nullptr, &textRectSound);
			SDL_RenderCopy(renderer, textTextureExit, nullptr, &textRectExit);

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