#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "struct.h"

void pause();
int GRAPHIC_Init(int (*pf)());
int GRAPHIC_Clear();
int GRAPHIC_Render();
int GRAPHIC_RenderText(GRAPHIC_Text text);
int GRAPHIC_RenderFPS(char* fps);
GRAPHIC_Text GRAPHIC_GenerateText(char* message, char* fontFile, SDL_Color color, int fontSize);

SDL_Renderer *GRAPHIC_renderer;
int GRAPHIC_RENDER_FPS = 1;

int GRAPHIC_Init(int (*pf)())
{
	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
		return -1;
	}else{
		/* Création de la fenêtre */
		SDL_Window* pWindow = NULL;
		pWindow = SDL_CreateWindow("DruEngine", 0, 270,
			1366/2-5,
			480,
			SDL_WINDOW_SHOWN);

		if(pWindow)
		{
			if (TTF_Init()){
				printf("TTF_Init : %s\n", TTF_GetError());
				//return -1;
			}

			GRAPHIC_renderer = SDL_CreateRenderer(pWindow, -1, 0);

			int continuer = 1;
			SDL_Event event;

			int tempsActuel = 0;
			int tempsPrecedent = 0;
			int frames = 0;
			char FPS[50] = "FPS : 0\nTemps pause : 0";
			int tempsPause = 0;

			int maxFPS = 30;

			while (continuer)
			{
				SDL_PollEvent(&event);
				switch(event.type)
				{
					case SDL_QUIT:
						continuer = 0;
				}

				int tempsRender = SDL_GetTicks();
				(*pf)();
				tempsRender = SDL_GetTicks() - tempsRender;

				if(tempsRender < 1000/maxFPS)
				{
					tempsPause = (int)((1000/maxFPS)-tempsRender)-5;
					if(tempsPause > 0)
					{
						SDL_Delay(tempsPause);
					}
				}

				if(GRAPHIC_RENDER_FPS)
				{
					frames++;

					tempsActuel = SDL_GetTicks();

					if((tempsActuel-tempsPrecedent) > 500)
					{
						int fps = frames*2;
						sprintf(FPS, "FPS : %d", fps);
						tempsPrecedent = SDL_GetTicks();
						frames = 0;
					}

					GRAPHIC_RenderFPS(FPS);
				}

				GRAPHIC_Render();
			}






			//pause();

			SDL_DestroyWindow(pWindow);
		}else{
			fprintf(stderr,"Erreur de création de la fenêtre: %s\n",SDL_GetError());
		}
	}

	SDL_Quit();
}

int GRAPHIC_Clear()
{
	SDL_SetRenderDrawColor(GRAPHIC_renderer, 0, 0, 0, 255);
	SDL_RenderClear(GRAPHIC_renderer);
}

int GRAPHIC_Render()
{
	SDL_RenderPresent(GRAPHIC_renderer);
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

GRAPHIC_Text GRAPHIC_GenerateText(char* message, char* fontFile, SDL_Color color, int fontSize)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile, fontSize);
	if (!font){
		printf("TTF_OpenFont : %s\n", TTF_GetError());
		return;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface* surf = TTF_RenderText_Blended(font, message, color);
	if (!surf){
		TTF_CloseFont(font);
		printf("TTF_RenderText : %s\n", TTF_GetError());
		return;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(GRAPHIC_renderer, surf);
	if (!texture){
		printf("CreateTexture : %s\n", TTF_GetError());
	}
	//Clean up the surface and font


	GRAPHIC_Text text = {};
	text.w = surf->w;
	text.h = surf->h;
	text.texture = texture;
	text.render = &GRAPHIC_RenderText;

	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	return text;
}

int GRAPHIC_RenderText(GRAPHIC_Text text)
{
	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = text.w;
	SrcR.h = text.h;

	DestR.x = text.x;
	DestR.y = text.y;
	DestR.w = text.w;
	DestR.h = text.h;



	SDL_RenderCopy(GRAPHIC_renderer, text.texture, &SrcR, &DestR);
}

int GRAPHIC_RenderFPS(char* fps)
{
	SDL_Color blanc = {255, 255, 255, 255};
	GRAPHIC_Text text = GRAPHIC_GenerateText(fps, "lib/fonts/roboto/Roboto-Thin.ttf", blanc, 32);
	text.render(text);
}