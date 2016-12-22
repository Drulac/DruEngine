#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <string.h>
#include <ctype.h>

void pause();
void sdlInit();
void parse(char * code);
int findNextOpenTag(char * code, int cursor);
int findNextCloseTag(char * code, int cursor);
char* loadFile(char* filename);

int main(int argc, char *argv[])
{
	char* filename = "test/test.html";

	char* buffer = loadFile(filename);

	parse(buffer);		



	printf("\n");
	printf("\n");
	int i = 0;
	int len = strlen(buffer);
	for(i = 0; i < len; i++)
	{
		printf("%c", buffer[i]);
	}

	printf("\n");
	free(buffer);

	return EXIT_SUCCESS; // Fermeture du programme
}

void sdlInit()
{
	SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL

	SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF); // Ouverture de la fenêtre

	pause(); // Mise en pause du programme

	SDL_Quit(); // Arrêt de la SDL
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

char* loadFile(char* filename)
{
	char* buffer = 0;
	long len;
	FILE* f = fopen(filename, "r");

	if(f)
	{
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = malloc(len);
		if(buffer)
		{
			fread(buffer, 1, len, f);
		}
		fclose(f);
	}

	return buffer;
}

void parse(char * code)
{
	int i = 0;
	int len = strlen(code);

	int openTag = findNextOpenTag(code, 1);
	int closeTag = findNextCloseTag(code, openTag);

	printf("%d", openTag);
	printf("\n");
	printf("%d", closeTag);
	/*
	for(i = 0; i < len; i++)
	{
		printf("%c", code[i]);
		code[i] = code[len-i];
	}
	*/
}

int findNextOpenTag(char * code, int cursor)
{
	int i = 0;
	int len = strlen(code);
	int openedText = 0;

	for(i = cursor; i < len; i++)
	{
		if(!openedText && code[i] == '<' && (i == 0 || code[i-1] != '\\'))
		{
			return i;
		}else if(!openedText && code[i] == '"' && (i == 0 || code[i-1] != '\\')){
			openedText = 1;
		}else if(openedText && code[i] == '"' && (i == 0 || code[i-1] != '\\')){
			openedText = 0;
		}
	}

	return -1;
}

int findNextCloseTag(char * code, int cursor)
{
	int i = 0;
	int len = strlen(code);
	int openedText = 0;

	for(i = cursor; i < len; i++)
	{
		if(!openedText && code[i] == '>' && (i == 0 || code[i-1] != '\\'))
		{
			return i;
		}else if(!openedText && code[i] == '"' && (i == 0 || code[i-1] != '\\')){
			openedText = 1;
		}else if(openedText && code[i] == '"' && (i == 0 || code[i-1] != '\\')){
			openedText = 0;
		}
	}

	return -1;
}