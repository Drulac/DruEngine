#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void pause();
int sdlInit();

int sdlInit()
{
	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
		return -1;
	}else{
		/* Création de la fenêtre */
		SDL_Window* pWindow = NULL;
		pWindow = SDL_CreateWindow("DruEngine",SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640,
			480,
			SDL_WINDOW_SHOWN);

		if(pWindow)
		{
			TTF_Init();

			TTF_Font *police = NULL;
			police = TTF_OpenFont("/home/drulac/Bureau/C/DruEngine/0.0.0/lib/fonts/lane/LANENAR_.ttf", 12);

			if(police == NULL){
				printf("Échec du chargement de la police : %s\n", TTF_GetError());
				return;
			}

			SDL_Color couleurNoire = {0, 0, 0};







			pause(); /* Attendre trois secondes, que l'utilisateur voit la fenêtre */

			SDL_DestroyWindow(pWindow);
		}else{
			fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
		}
	}

	SDL_Quit();

	/*
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Erreur lors de l'initialisation de la SDL : %s\n", SDL_GetError());
		SDL_Quit();
		
		return;
	}

	SDL_Window *screen = SDL_CreateWindow("My Game Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_OPENGL);

	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, 0);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);


	//SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	TTF_Init();
	
	
	// Création de la fenêtre

	TTF_Font *police = NULL;
	police = TTF_OpenFont("lib/fonts/roboto/Roboto-Medium.ttf", 65);

	if(police == NULL){ 
		return;
	}

	SDL_Color couleurNoire = {0, 0, 0};

	SDL_Surface *text_surface;
	text_surface=TTF_RenderText_Blended(police, "Salut les Zér0s !", couleurNoire);


	pause(); // Mise en pause du programme

	TTF_CloseFont(police);
	TTF_Quit();

	SDL_DestroyWindow(screen);

	SDL_Quit(); // Arrêt de la SDL
	*/
}

void pause()
{
	int continuer = 1;
	SDL_Event event;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
		}
	}
}