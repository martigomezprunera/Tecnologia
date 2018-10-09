#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "utilities.h"
#include <time.h>

#include <exception>
#include <iostream>
#include <string>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

enum class MovementsPlayer1 { top, down, left, right, quieto };
enum class MovementsPlayer2 { top, down, left, right, quieto };

int main(int, char*[]) 
{
	//CONSTANTES
	const int FPSs = 60;
	const int DELAY_TIME = 1000.0f / FPSs;

	//SCORE
	int score1 = 0;
	int score2 = 0;
	std::string score1STR;
	std::string score2STR;

	//TIEMPO
	int min = 0;
	int seconds = 60;
	int auxTimeFrame = 0;
	int centesimas = 0;
	int decimas = 0;

	//RANDOM
	srand(time(NULL));

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

	//FUENTE
	TTF_Font *font2{ TTF_OpenFont("../../res/ttf/arial.ttf", 255) };
	if (font2 == nullptr) throw "No es pot inicializtar the TTF_Font2";

	//-->SDL_Mix
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		throw "No se puede inicializar SDL_mixer";
	}
	Mix_Music *music = Mix_LoadMUS("../../res/au/mainTheme.mp3");
	if (!music)
	{
		throw "No se puede abrir dicha musica";
	}

	// --- SPRITES ---
		//Background1
		SDL_Texture* bgTexture{ IMG_LoadTexture(m_renderer, "../../res/img/bg.jpg") };
		if (bgTexture == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };

		//Background2
		SDL_Texture* bgTexture1 = { IMG_LoadTexture(m_renderer, "../../res/img/bgCastle.jpg") };
		if (bgTexture1 == nullptr) throw "Error: bgTexture init";
		SDL_Rect bgRect1{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_Texture *bgTextPlayAux = bgTexture;

		//DINERO
		SDL_Texture* moneyTexture = { IMG_LoadTexture(m_renderer, "../../res/img/gold.png") };
		if (moneyTexture == nullptr) throw "Error: moneyTexture init";
		SDL_Rect moneyRect{ 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_Texture *moneyTextPlayAux = moneyTexture;

		//Rectangulo de PLAY
		SDL_Rect textRectPlay{ 10, 10, 100, 50 };
		//Rectangulo de SOUND OFF
		SDL_Rect textRectSound{ 10, 110, 100, 50 };
		//Rectangulo de EXIT
		SDL_Rect textRectExit{ 10, 210, 100, 50 };

		//RECTANGULOS TEXTS SCORE
		SDL_Rect textRectScore1{ 10, 10, 200, 50 };
		SDL_Rect textRectScore2{ 10, 110, 200, 50 };

		//RECTANGULO DINERO
		SDL_Rect textRectMoney{ (rand() % (SCREEN_WIDTH - 50)) , (rand() % (SCREEN_HEIGHT - 250)) + 200, 50, 50 };

		//RECTANGULO SUPERIOR
		SDL_Rect RectTop{ 0 , 0, SCREEN_WIDTH, 200};


	//-->Animated Sprite ---
	//SPRITE
	SDL_Texture* spriteTexture{ IMG_LoadTexture(m_renderer, "../../res/img/spCastle.png") };
	if (spriteTexture == nullptr) throw "Error: spriteTexture init";

	//SPRITE PLAYER 1
	SDL_Rect spriteRect, spritePosition;
	int frameWidth, frameHeight, textwidth, textheight;
	SDL_QueryTexture(spriteTexture, NULL, NULL, &textwidth, &textheight);
	frameHeight = (textwidth / 12);
	frameWidth = (textheight / 8);
	spritePosition.x = 150;
	spritePosition.y = 300;
	spriteRect.x = 0;
	spriteRect.y = frameHeight * 2;
	spritePosition.h = spriteRect.h = frameHeight;
	spritePosition.w = spriteRect.w = frameWidth;

	//SPRITE PLAYER 2
	SDL_Rect spriteRect1, spritePosition1;
	SDL_QueryTexture(spriteTexture, NULL, NULL, &textwidth, &textheight);
	spritePosition1.x = 300;
	spritePosition1.y = 300;
	spriteRect1.x = 0;
	spriteRect1.y = frameHeight * 5;
	spritePosition1.h = spriteRect1.h = frameHeight;
	spritePosition1.w = spriteRect1.w = frameWidth;
	SDL_Texture *spriteAux = spriteTexture;

	//SCORE1
	SDL_Texture* scoreTexture{ IMG_LoadTexture(m_renderer, "../../res/img/numeros.png") };
	if (scoreTexture == nullptr) throw "Error: numTexture init";
	SDL_Rect scoreRect, scorePosition;
	int frameWidthScore, frameHeightScore, textwidthScore, textheightScore;
	SDL_QueryTexture(scoreTexture, NULL, NULL, &textwidthScore, &textheightScore);
	frameHeightScore = textwidthScore / 10;
	frameWidthScore = textheightScore;
	scorePosition.x = 220;
	scorePosition.y = 0;
	scoreRect.x = 0;
	scoreRect.y = 0;
	scorePosition.h = scoreRect.h = frameHeightScore;
	scorePosition.w = scoreRect.w = frameWidthScore;
	SDL_Texture *scoreAux = scoreTexture;

	//SCORE2
	SDL_Rect scoreRect1, scorePosition1;
	SDL_QueryTexture(scoreTexture, NULL, NULL, &textwidthScore, &textheightScore);
	scorePosition1.x = 220;
	scorePosition1.y = 100;
	scoreRect1.x = 0;
	scoreRect1.y = 0;
	scorePosition1.h = scoreRect1.h = frameHeightScore;
	scorePosition1.w = scoreRect1.w = frameWidthScore;
	SDL_Texture *scoreAux1 = scoreTexture;

	//TIME Seconds1
	SDL_Rect timeRect, timePosition;
	SDL_QueryTexture(scoreTexture, NULL, NULL, &textwidthScore, &textheightScore);
	timePosition.x = 700;
	timePosition.y = 100;
	timeRect.x = 0;
	timeRect.y = 0;
	timePosition.h = timeRect.h = frameHeightScore;
	timePosition.w = timeRect.w = frameWidthScore;
	SDL_Texture *timeAux = scoreTexture;

	//TIME Seconds2
	SDL_Rect timeRect1, timePosition1;
	SDL_QueryTexture(scoreTexture, NULL, NULL, &textwidthScore, &textheightScore);
	timePosition1.x = 625;
	timePosition1.y = 100;
	timeRect1.x = frameHeightScore * 6;
	timeRect1.y = 0;
	timePosition1.h = timeRect1.h = frameHeightScore;
	timePosition1.w = timeRect1.w = frameWidthScore;
	SDL_Texture *timeAux1 = scoreTexture;

	//TIME

	//FRAME
	int frameTime = 0;

	// --- TEXT ---
		//PLAY
		SDL_Surface *tmpSurfPlay{ TTF_RenderText_Blended(font,"Play", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTexturePlay{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };

		tmpSurfPlay = { TTF_RenderText_Blended(font,"Play",SDL_Color{ 0,255,255,0 }) };
		SDL_Texture *textTextPlayHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };

		SDL_Texture *textTextPlayAux = textTexturePlay;
		SDL_FreeSurface(tmpSurfPlay);

		//SOUND
		//SOUND OFF
		tmpSurfPlay = { TTF_RenderText_Blended(font,"Sound Off", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTextureSound{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };

		tmpSurfPlay = { TTF_RenderText_Blended(font,"Sound Off",SDL_Color{ 0,255,255,0 }) };
		SDL_Texture *textTextSoundOnHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };
		SDL_Texture *textTextSoundOnAux = textTextureSound;

		//SOUND ON
		tmpSurfPlay = { TTF_RenderText_Blended(font,"Sound On",SDL_Color{ 0,0,0,0 }) };
		SDL_Texture *textTextSoundOn1{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };

		tmpSurfPlay = { TTF_RenderText_Blended(font,"Sound On",SDL_Color{ 0,255,255,0 }) };
		SDL_Texture *textTextSoundOnHover1{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };
		SDL_FreeSurface(tmpSurfPlay);

		//EXIT
		tmpSurfPlay = { TTF_RenderText_Blended(font,"Exit", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTextureExit{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };

		tmpSurfPlay = { TTF_RenderText_Blended(font,"Exit",SDL_Color{ 0,255,255,0 }) };
		SDL_Texture *textTextExitHover{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };

		SDL_Texture *textTextExitAux = textTextureExit;
		SDL_FreeSurface(tmpSurfPlay);

		//SCORE 1
		tmpSurfPlay = { TTF_RenderText_Blended(font, "Score Green", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTextureScore1{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };
		SDL_Texture *textTextureScore1Aux = textTextureScore1;
		SDL_FreeSurface(tmpSurfPlay);

		//SCORE 2
		tmpSurfPlay = { TTF_RenderText_Blended(font, "Score Blue", SDL_Color{ 0,0,0,0 }) };
		if (tmpSurfPlay == nullptr) throw "Unable to crate the SDL text surface";
		SDL_Texture *textTextureScore2{ SDL_CreateTextureFromSurface(m_renderer,tmpSurfPlay) };
		SDL_Texture *textTextureScore2Aux = textTextureScore2;
		SDL_FreeSurface(tmpSurfPlay);
		
		TTF_CloseFont(font2);

	// --- AUDIO ---
	Mix_PlayMusic(music, -1);

	// --- GAME LOOP ---
	// ------ TIME ------
	clock_t lastTime = clock();
	float timeDown = 60;
	float deltaTime = 0;
	int auxTimeDown = 0;

	Uint32 frameStartES, frameTimeES;

	//Variables
	SDL_Event event;
	bool isRunning = true;
	bool sound = true;
	bool mousePressed = true;
	bool soundActivated = true;
	bool bg1 = true;
	bool changeEscene = false;

	//PLAYER1
	MovementsPlayer1 mov1 = MovementsPlayer1::quieto;

	//PLAYER2
	MovementsPlayer2 mov2 = MovementsPlayer2::quieto;

	//TIEMPO
	double tiempo;

	//STANDBY 
	bool quieto = false;
	bool quieto2 = false;

	while (isRunning) {
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		
				isRunning = false; 
				break;
			case SDL_KEYDOWN:	
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_w:
					mov1 = MovementsPlayer1::top;
					break;
				case SDLK_s:
					mov1 = MovementsPlayer1::down;
					break;
				case SDLK_a:
					mov1 = MovementsPlayer1::left;
					break;
				case SDLK_d:
					mov1 = MovementsPlayer1::right;
					break;
				case SDLK_UP:
					mov2 = MovementsPlayer2::top;
					break;
				case SDLK_DOWN:
					mov2 = MovementsPlayer2::down;
					break;
				case SDLK_LEFT:
					mov2 = MovementsPlayer2::left;
					break;
				case SDLK_RIGHT:
					mov2 = MovementsPlayer2::right;
					break;

				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					mov1 = MovementsPlayer1::quieto;
					break;
				case SDLK_s:
					mov1 = MovementsPlayer1::quieto;
					break;
				case SDLK_a:
					mov1 = MovementsPlayer1::quieto;
					break;
				case SDLK_d:
					mov1 = MovementsPlayer1::quieto;
					break;
				case SDLK_UP:
					mov2 = MovementsPlayer2::quieto;
					break;
				case SDLK_DOWN:
					mov2 = MovementsPlayer2::quieto;
					break;
				case SDLK_LEFT:
					mov2 = MovementsPlayer2::quieto;
					break;
				case SDLK_RIGHT:
					mov2 = MovementsPlayer2::quieto;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				mousePressed = true;
				break;
			default:;
			}

			frameStartES = SDL_GetTicks();
		}

		// UPDATE
		deltaTime = (clock() - lastTime);
		lastTime = clock();
		deltaTime /= CLOCKS_PER_SEC;
		timeDown -= deltaTime;

		//TIEMPO
		//CONVERSION A INTS
		auxTimeDown = timeDown;
		if (auxTimeDown < 60)
		{
			min = 0;
		}
		else
		{
			min = auxTimeDown / 60;
		}

		//EVENTOS POR ENCIMA DE LOS BOTONES
		if (utilities::hover(event.motion.x, event.motion.y, textRectPlay))
		{
			//std::cout << "Play" << std::endl;
			textTextPlayAux = textTextPlayHover;
		}
		else
		{
			textTextPlayAux = textTexturePlay;
		}

		if (!soundActivated)
		{
			if (utilities::hover(event.motion.x, event.motion.y, textRectSound))
			{
				textTextSoundOnAux = textTextSoundOnHover;
				//std::cout << "Sound OFF" << std::endl;
			}
			else
			{
				textTextSoundOnAux = textTextureSound;
			}
		}
		else
		{
			if (utilities::hover(event.motion.x, event.motion.y, textRectSound))
			{
				textTextSoundOnAux = textTextSoundOnHover1;
				//std::cout << "Sound ON" << std::endl;
			}
			else
			{
				textTextSoundOnAux = textTextSoundOn1;
			}
		}
		
		if (utilities::hover(event.motion.x, event.motion.y, textRectExit))
		{
			//std::cout << "Exit" << std::endl;
			textTextExitAux = textTextExitHover;
		}
		else
		{
			textTextExitAux = textTextureExit;
		}

		//EVENTOS POR PULSAR EN LOS BOTONES
		if (mousePressed)
		{
			//EXIT
			if (utilities::hover(event.motion.x, event.motion.y, textRectExit))
			{
				textTextExitAux = textTextExitHover;
				isRunning = false;
			}
			else
			{
				textTextExitAux = textTextureExit;
			}

			//SOUND
			if (!soundActivated)
			{
				if (utilities::hover(event.motion.x, event.motion.y, textRectSound))
				{
					textTextSoundOnAux = textTextSoundOn1;
					Mix_ResumeMusic();
					soundActivated = true;
				}
				else
				{
					textTextSoundOnAux = textTextSoundOnHover1;
				}
			}
			else
			{
				soundActivated = false;
				Mix_PauseMusic();
			}

			//PLAY
			if (!bg1)
			{
				if (utilities::hover(event.motion.x, event.motion.y, textRectPlay))
				{
					bgTextPlayAux = bgTexture1;

					//CAMBIAMOS TEXTO
					changeEscene = true;
				}
			}
			else
			{
				bgTextPlayAux = bgTexture;
				bg1 = false;
			}
			
			//RESETEAMOS EL MOUSE PRESIONADO
			mousePressed = false;
		}
		

		//EVENTOS POR PULSAR TECLADO DE LOS PLAYERS
		//PLAYER 1
		switch (mov1)
		{
		case MovementsPlayer1::top:
			if (!utilities::colision(spritePosition, RectTop))
			{
					spriteRect.y = frameHeight * 3;
					frameTime++;
					if (FPS / frameTime <= 2)
					{
						frameTime = 0;
						spriteRect.x += frameWidth;
						if (spriteRect.x >= (frameWidth * 3))
						{
							spriteRect.x = 0;
						}
					}
					spritePosition.y--;
			}
			break;
		case MovementsPlayer1::down:
			if (spritePosition.y < SCREEN_HEIGHT - spritePosition.h)
			{
					spriteRect.y = 0;
					frameTime++;
					if (FPS / frameTime <= 2)
					{
						frameTime = 0;
						spriteRect.x += frameWidth;
						if (spriteRect.x >= (frameWidth * 3))
						{
							spriteRect.x = 0;
						}
					}
					spritePosition.y++;
			}
			break;
		case MovementsPlayer1::left:
			if (spritePosition.x > 0)
			{
					spriteRect.y = frameHeight;
					frameTime++;
					if (FPS / frameTime <= 2)
					{
						frameTime = 0;
						spriteRect.x += frameWidth;
						if (spriteRect.x >= (frameWidth * 3))
						{
							spriteRect.x = 0;
						}
					}
					spritePosition.x--;
			}
			break;
		case MovementsPlayer1::right:
			if (spritePosition.x < SCREEN_WIDTH - spritePosition.w)
			{
					spriteRect.y = frameHeight * 2;
					frameTime++;
					if (FPS / frameTime <= 2)
					{
						frameTime = 0;
						spriteRect.x += frameWidth;
						if (spriteRect.x >= (frameWidth * 3))
						{
							spriteRect.x = 0;
						}
					}
					spritePosition.x++;
			}
			break;
		default:
			break;
		}

		//PLAYER 2
		switch (mov2)
		{
		case MovementsPlayer2::top:
			if (!utilities::colision(spritePosition1, RectTop)) {
				spriteRect1.y = frameHeight * 7;
				frameTime++;
				if (FPS / frameTime <= 2)
				{
					frameTime = 0;
					spriteRect1.x += frameWidth;
					if (spriteRect1.x >= (frameWidth * 3))
					{
						spriteRect1.x = 0;
					}
				}
				spritePosition1.y--;
			}
			break;
		case MovementsPlayer2::down:
			if (spritePosition1.y < SCREEN_HEIGHT - spritePosition1.h)
			{
				spriteRect1.y = frameHeight * 4;
				frameTime++;
				if (FPS / frameTime <= 2)
				{
					frameTime = 0;
					spriteRect1.x += frameWidth;
					if (spriteRect1.x >= (frameWidth * 3))
					{
						spriteRect1.x = 0;
					}
				}
				spritePosition1.y++;
			}
			break;
		case MovementsPlayer2::left:
			if (spritePosition1.x > 0)
			{
				spriteRect1.y = frameHeight * 5;
				frameTime++;
				if (FPS / frameTime <= 2)
				{
					frameTime = 0;
					spriteRect1.x += frameWidth;
					if (spriteRect1.x >= (frameWidth * 3))
					{
						spriteRect1.x = 0;
					}
				}
				spritePosition1.x--;
			}
			break;
		case MovementsPlayer2::right:
			if (spritePosition1.x < SCREEN_WIDTH - spritePosition1.w)
			{
				spriteRect1.y = frameHeight * 6;
				frameTime++;
				if (FPS / frameTime <= 2)
				{
					frameTime = 0;
					spriteRect1.x += frameWidth;
					if (spriteRect1.x >= (frameWidth * 3))
					{
						spriteRect1.x = 0;
					}
				}
				spritePosition1.x++;
			}
			break;
		default:
			break;
		}

		//COLISION MONEDA PLAYER 1
		if (utilities::colision(spritePosition, textRectMoney))
		{
			//std::cout << "Hola 1" << std::endl;
			//RESETAMOS RECT DE MONEY
			textRectMoney = { (rand() % (SCREEN_WIDTH - 50)) , (rand() % (SCREEN_HEIGHT - 250)) + 200, 50, 50 };

			//CAMBIAMOS SPRITE DE PUNTUACION DEL JUGADOR 1
			score1++;
			scoreRect.x = frameHeightScore * score1;
		}

		//COLISION MONEDA PLAYER 2
		if (utilities::colision(spritePosition1, textRectMoney))
		{
			//std::cout << "Hola 2" << std::endl;
			//RESETAMOS RECT DE MONEY
			textRectMoney = { (rand() % (SCREEN_WIDTH - 50)) , (rand() % (SCREEN_HEIGHT - 250)) + 200, 50, 50 };

			//CAMBIAMOS SPRITE DE PUNTUACION DEL JUGADOR 2
			score2++;
			scoreRect1.x = frameHeightScore * score2;

		}
		
		//TEXTO DE ESCENA
		if (changeEscene)
		{
			//TEXTO VACIO
			textTextPlayAux = nullptr;
			textTextSoundOnAux = nullptr;
			textTextExitAux = nullptr;

			//SPRITES EN SEGUNDA PANTALLA
			spriteAux = spriteTexture;

			//SCORE 
			textTextureScore1Aux = textTextureScore1;
			textTextureScore2Aux = textTextureScore2;

			//SCORE IMG
			scoreAux = scoreTexture;
			scoreAux1 = scoreTexture;

			//MONEY
			moneyTextPlayAux = moneyTexture;

			//TIME
			timeAux = scoreTexture;
			timeAux1 = scoreTexture;
		}
		else
		{
			//SPRITE EN PRIMERA PANTALLA
			spriteAux = nullptr;
			textTextureScore1Aux = nullptr;
			textTextureScore2Aux = nullptr;
			scoreAux = nullptr;
			scoreAux1 = nullptr;
			moneyTextPlayAux = nullptr;
			timeAux = nullptr;
			timeAux1 = nullptr;
		}

		if (auxTimeDown == 0)
		{
			changeEscene = false;
			seconds = 60;
			score1 = 0;
			score2 = 0;
		}

		//GANAMOS
		if (score1 == 9)
		{
			changeEscene = false;
		}

		if (score2 == 9)
		{
			changeEscene = false;
		}

		//FRAME ESTABLES
		/*frameTimeES = SDL_GetTicks() - frameStartES;
		if (frameTimeES < DELAY_TIME)
		{
			SDL_Delay((int)(DELAY_TIME - frameTimeES));
		}*/

		//CENTESIMAS 
		centesimas = auxTimeDown % 10;
		timeRect.x = frameHeightScore * centesimas;
		
		//DECIMAS
		decimas = auxTimeDown / 10;
		timeRect1.x = frameHeightScore * decimas;

		// DRAW
		SDL_RenderClear(m_renderer);
		//Background
		SDL_RenderCopy(m_renderer, bgTextPlayAux, nullptr, &bgRect);

		//Text
		SDL_RenderCopy(m_renderer, textTextPlayAux, nullptr, &textRectPlay);
		SDL_RenderCopy(m_renderer, textTextSoundOnAux, nullptr, &textRectSound);
		SDL_RenderCopy(m_renderer, textTextExitAux, nullptr, &textRectExit);

		//SPRITES
		SDL_RenderCopy(m_renderer, spriteAux, &spriteRect, &spritePosition);
		SDL_RenderCopy(m_renderer, spriteAux, &spriteRect1, &spritePosition1);

		//TEXT SCORE
		SDL_RenderCopy(m_renderer, textTextureScore1Aux, nullptr, &textRectScore1);
		SDL_RenderCopy(m_renderer, textTextureScore2Aux, nullptr, &textRectScore2);

		//IMG SCORE
		SDL_RenderCopy(m_renderer, scoreAux, &scoreRect, &scorePosition);
		SDL_RenderCopy(m_renderer, scoreAux1, &scoreRect1, &scorePosition1);

		//MONEY
		SDL_RenderCopy(m_renderer, moneyTextPlayAux, nullptr, &textRectMoney);

		//IMG TIME
		//SECONDS
		SDL_RenderCopy(m_renderer, timeAux, &timeRect, &timePosition);
		SDL_RenderCopy(m_renderer, timeAux1, &timeRect1, &timePosition1);


		SDL_RenderPresent(m_renderer);
	}

	// --- DESTROY --
	//BACKGROUND
	SDL_DestroyTexture(bgTexture);

	//TEXT
	SDL_DestroyTexture(textTextureExit);
	SDL_DestroyTexture(textTextureSound);
	SDL_DestroyTexture(textTexturePlay);
	SDL_DestroyTexture(textTextPlayAux);
	SDL_DestroyTexture(textTextSoundOnAux);
	SDL_DestroyTexture(textTextExitAux);
	SDL_DestroyTexture(textTextExitHover);
	SDL_DestroyTexture(textTextPlayHover);
	SDL_DestroyTexture(textTextSoundOnHover);
	SDL_DestroyTexture(textTextSoundOnHover1);
	SDL_DestroyTexture(textTextSoundOn1);
	SDL_DestroyRenderer(m_renderer);

	//CLOSE LIBRARIES
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();

	SDL_DestroyWindow(m_window);

	// --- QUIT ---
	SDL_Quit();

	return 0;
}