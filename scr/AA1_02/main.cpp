#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "utilities.h"

#include <exception>
#include <iostream>
#include <string>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int, char*[]) 
{
	// --- INIT SDL ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
		throw "No es pot inicialitzar SDL subsystems";

	// --- WINDOW ---
	SDL_Window *m_window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (m_window == nullptr) 
		throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *m_renderer{ SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (m_renderer == nullptr) 
		throw "No es pot inicialitzar SDL_Renderer";

	//-->SDL_Image 
	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error: SDL_image init";

	//-->SDL_TTF
	TTF_Init();
	TTF_Font *font{ TTF_OpenFont("../../res/ttf/saiyan.ttf", 255) };
	if (font == nullptr) throw "No es pot inicializtar the TTF_Font";

	//-->SDL_Mix
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_Music *music = Mix_LoadMUS("../../res/au/mainTheme.mp3");

	// --- SPRITES ---
		//Background
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		if (bgTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

		//Player
		SDL_Texture* playerTexture{ IMG_LoadTexture(m_renderer, "../../res/img/kintoun.png") };
		if (playerTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect playerRect{ 0, 0, 175, 95 };
		SDL_Rect playerTarget{0, 0, 100, 100 };

		//Rectangulo de PLAY
		SDL_Rect textRectPlay{ 10, 10, 100, 50 };
		//Rectangulo de SOUND OFF
		SDL_Rect textRectSound{ 10, 110, 100, 50 };
		//Rectangulo de EXIT
		SDL_Rect textRectExit{ 10, 210, 100, 50 };


	//-->Animated Sprite ---

	// --- TEXT ---
		//PLAY
		SDL_Surface *tmpSurfPlay{ TTF_RenderText_Blended(font,"Play", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTexturePlay{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };
		SDL_FreeSurface(tmpSurfPlay);

		//SOUND
		tmpSurfPlay = { TTF_RenderText_Blended(font,"Sound Off", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTextureSound{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };
		SDL_FreeSurface(tmpSurfPlay);

		//EXIT
		tmpSurfPlay = { TTF_RenderText_Blended(font,"Exit", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTextureExit{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };
		SDL_FreeSurface(tmpSurfPlay);
		
		TTF_CloseFont(font);

	// --- AUDIO ---
	Mix_PlayMusic(music, 1);

	// --- GAME LOOP ---
	//Variables
	SDL_Event event;
	bool isRunning = true;
	bool sound = true;
	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		
				isRunning = false; 
				break;
			case SDL_KEYDOWN:	
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					isRunning = false;
				}
				break;
			case SDL_MOUSEMOTION:
				int x; int y;
				SDL_GetMouseState(&x, &y);

				playerTarget.x = event.motion.x - 175/2;
				playerTarget.y = event.motion.y - 95/2;

				break;
			case SDL_MOUSEBUTTONDOWN:
				/*if (((x > 10) && (x < 110)))
				{
					if ((y > 210) && (y < 260))
					{
						isRunning = false;
					}
				}

				if (((x > 10) && (x < 110)))
				{
					if ((y > 110) && (y < 160))
					{
						//If music is being played
						if (Mix_PlayingMusic() == 1)
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
				}*/
				break;
			default:;
			}
		}

		// UPDATE
		if (utilities::hover(event.motion.x, event.motion.y, textRectPlay))
		{
			std::cout << "Play" << std::endl;
		}
		else if (utilities::hover(event.motion.x, event.motion.y, textRectSound))
		{
			std::cout << "Sound" << std::endl;
		}
		else if (utilities::hover(event.motion.x, event.motion.y, textRectExit))
		{
			std::cout << "Exit" << std::endl;
		}

		//KINTOUN Smooth Movement
		playerRect.x += (playerTarget.x - playerRect.x) / 15;
		playerRect.y += (playerTarget.y - playerRect.y) / 15;
		// DRAW
		SDL_RenderClear(m_renderer);
			//Background
			SDL_RenderCopy(m_renderer, bgTexture, nullptr, &bgRect);

			//Rectangle Text
			SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(m_renderer, &textRectPlay);
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
			SDL_RenderFillRect(m_renderer, &textRectSound);
			SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(m_renderer, &textRectExit);

			//Text
			SDL_RenderCopy(m_renderer, textTexturePlay, nullptr, &textRectPlay);
			SDL_RenderCopy(m_renderer, textTextureSound, nullptr, &textRectSound);
			SDL_RenderCopy(m_renderer, textTextureExit, nullptr, &textRectExit);

			//Sprite
			SDL_RenderCopy(m_renderer, playerTexture, nullptr, &playerRect);
			
		SDL_RenderPresent(m_renderer);

	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);

	//CLOSE LIBRARIES
	IMG_Quit();
	TTF_Quit();

	SDL_DestroyTexture(textTextureExit);
	SDL_DestroyTexture(textTextureSound);
	SDL_DestroyTexture(textTexturePlay);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}