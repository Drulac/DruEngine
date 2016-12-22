#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <string.h>
#include <ctype.h>

#define ANSI_COLOR_RED	 "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE	"\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN	"\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum var_status {VAR_OK, VAR_ALLOCATION_ERROR};

struct _balise
{
	char* balise;
	char* attributs;
	int closing;
};

void pause();
void sdlInit();
void parse(char * code);
char* loadFile(char* filename);

int main(int argc, char *argv[])
{
	char* filename = "test/test.html";

	char* buffer = loadFile(filename);



	parse(buffer);		



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
	int cursor = 0;
	struct _balise content[100000];

	int openedText = 0;
	int openedBalise = 0;

	int baliseNameComplet = 0;
	int baliseClosing = 0;

	char* baliseAttributs = malloc(sizeof(char));
	*baliseAttributs = '\0';

	char* baliseName = malloc(sizeof(char));
	*baliseName = '\0';

	for(i = 0; i < len; i++)
	{

		if(!openedBalise && !openedText && code[i] == '<' && (i == 0 || code[i-1] != '\\'))
		{
			openedBalise = 1;
		}else if(!openedText && code[i] == '>' && (i == 0 || code[i-1] != '\\')){

			struct _balise balise = {baliseName, baliseAttributs, baliseClosing};

			openedBalise = 0;
			baliseNameComplet = 0;
			baliseClosing = 0;

			content[cursor] = balise;


			baliseAttributs = malloc(sizeof(char));
			*baliseAttributs = '\0';

			baliseName = malloc(sizeof(char));
			*baliseName = '\0';

			cursor++;
		}else if(!openedText && code[i] == '"' && (i == 0 || code[i-1] != '\\')){
			openedText = 1;

			char str[] = "\0";
			str[0] = code[i];
			int tmplen = strlen(baliseAttributs);
			baliseAttributs = realloc(baliseAttributs, tmplen + 1 + 1);
			strcat(baliseAttributs, str);

		}else if(openedText && code[i] == '"' && (i == 0 || code[i-1] != '\\')){
			openedText = 0;

			char str[] = "\0";
			str[0] = code[i];
			int tmplen = strlen(baliseAttributs);
			baliseAttributs = realloc(baliseAttributs, tmplen + 1 + 1);
			strcat(baliseAttributs, str);

		}else if(openedBalise == 1){

			//append the char

			if(!baliseNameComplet && code[i] == ' ')
			{
				baliseNameComplet = 1;
			}else if(!baliseNameComplet){
				if(code[i] == '/' && strlen(baliseName) == 0)
				{
					baliseClosing = 1;
				}else{
					char str[] = "\0";
					str[0] = tolower(code[i]);
					int tmplen = strlen(baliseName);
					baliseName = realloc(baliseName, tmplen + 1 + 1);
					strcat(baliseName, str);
				}
			}else{
				char str[] = "\0";
				str[0] = code[i];
				int tmplen = strlen(baliseAttributs);
				baliseAttributs = realloc(baliseAttributs, tmplen + 1 + 1);
				strcat(baliseAttributs, str);
			}

		}
	}

	for(i = 0, cursor; i < cursor; i++)
	{
		printf(ANSI_COLOR_YELLOW);
		printf("%d : %s : %s\n", content[i].closing, content[i].balise, content[i].attributs);
		printf(ANSI_COLOR_RESET);
	}
}