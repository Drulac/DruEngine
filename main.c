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

struct _attribut
{
	char* name;
	char* value;
};

struct _balise
{
	char* balise;
	struct _attribut attributs[255];
	int closing;
	size_t attributsLen;
};

struct _treeElement
{
	int type;
	struct _balise balise;
	char* text;
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
	struct _treeElement* content;
	content = malloc(sizeof(struct _treeElement) * 1000);
	content = realloc(content, sizeof(struct _treeElement) * 2000);

	int openedText = 0;
	int openedBalise = 0;
	int outBalise = 0;

	int baliseNameComplet = 0;
	int baliseClosing = 0;

	char* baliseAttributs = malloc(sizeof(char));
	*baliseAttributs = '\0';

	char* baliseName = malloc(sizeof(char));
	*baliseName = '\0';

	char* outBaliseText = malloc(sizeof(char));
	*outBaliseText = '\0';

	struct _attribut attr;
	attr.name = malloc(sizeof(char));
	*attr.name = '\0';
	attr.value = malloc(sizeof(char));
	*attr.value = '\0';

	struct _attribut* attrs;
	attrs = malloc(sizeof(struct _attribut) * 255);

	int attrsLen = 0;

	int attrNameComplet = 0;

	for(i = 0; i < len; i++)
	{

		if(!openedBalise && !openedText && code[i] == '<' && (i == 0 || code[i-1] != '\\'))
		{
			openedBalise = 1;
			if(outBalise)
			{
				//add the text to content array
				if(strlen(outBaliseText) != 0)
				{
					struct _treeElement element;
					element.type = 1;
					element.text = outBaliseText;

					content[cursor] = element;

					cursor++;

					if(cursor > 100)
					{
						printf("cursor too large : %d\n", cursor);
					}
				}

				outBalise = 0;

				outBaliseText = malloc(sizeof(char));
				*outBaliseText = '\0';
			}
		}else if(outBalise){
			if(code[i] != '\n' && code[i] != '\v' && code[i] != '\t' && code[i] != '\r' && code[i] != '\f' && (code[i] != ' ' || (i == 0 || code[i-1] != ' ')))
			{
				char str[] = "\0";
				str[0] = code[i];
				int tmplen = strlen(outBaliseText);
				outBaliseText = realloc(outBaliseText, tmplen + 1 + 1);
				strcat(outBaliseText, str);
			}
		}else if(!openedText && code[i] == '>' && (i == 0 || code[i-1] != '\\')){

			if(*attr.name != 0)
			{
				attrs[attrsLen] = attr;
				attrsLen++;
			}

			struct _treeElement element;

			struct _balise balise;

			balise.balise = baliseName;
			memcpy(balise.attributs, attrs, 255*sizeof(struct _attribut));
			balise.closing = baliseClosing;
			balise.attributsLen = attrsLen;

			element.type = 0;
			element.balise = balise;

			attrs = malloc(sizeof(struct _attribut) * 255);

			outBalise = 1;
			openedBalise = 0;
			baliseNameComplet = 0;
			baliseClosing = 0;
			attrsLen = 0;

			content[cursor] = element;

			attr.name = malloc(sizeof(char));
			*attr.name = '\0';
			attr.value = malloc(sizeof(char));
			*attr.value = '\0';

			baliseAttributs = malloc(sizeof(char));
			*baliseAttributs = '\0';

			baliseName = malloc(sizeof(char));
			*baliseName = '\0';

			cursor++;

			if(cursor > 100)
			{
				printf("cursor too large : %d\n", cursor);
			}
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
				if(code[i] == ' ')
				{
					attrs[attrsLen] = attr;
					attrsLen++;

					attr.name = malloc(sizeof(char));
					*attr.name = '\0';
					attr.value = malloc(sizeof(char));
					*attr.value = '\0';

					attrNameComplet = 0;
				}else{
					if(code[i] == '=' && !openedText && (i == 0 || code[i-1] != '\\'))
					{
						attrNameComplet = 1;
					}else if(attrNameComplet){
						if(code[i] == '"' && !openedText && (i == 0 || code[i-1] != '\\')){
								openedText = 1;
						}else if(code[i] == '"' && openedText && (i == 0 || code[i-1] != '\\')){
								openedText = 0;
						}else{
							char str[] = "\0";
							str[0] = code[i];
							int tmplen = strlen(attr.value);
							attr.value = realloc(attr.value, tmplen + 1 + 1);
							strcat(attr.value, str);
						}
					}else{
						char str[] = "\0";
						str[0] = tolower(code[i]);
						int tmplen = strlen(attr.name);
						attr.name = realloc(attr.name, tmplen + 1 + 1);
						strcat(attr.name, str);
					}
				}
			}
		}
	}

	printf(ANSI_COLOR_RED);
	printf("%d\n", cursor);
	printf(ANSI_COLOR_RESET);

	for(i = 0; i < cursor; i++)
	{
		printf("%d : ", content[i].type);
		if(content[i].type == 0)
		{
			printf("%d : %s : %d :", content[i].balise.closing, content[i].balise.balise, content[i].balise.attributsLen);
			int attrsLen = content[i].balise.attributsLen;
			int j = 0;
			for(j = 0; j < attrsLen; j++)
			{
				printf(" %s = %s,", content[i].balise.attributs[j].name, content[i].balise.attributs[j].value);
			}
		}else{
			printf(ANSI_COLOR_BLUE);
			printf("%s", content[i].text);
			printf(ANSI_COLOR_RESET);
		}
		printf("\n");
	}

	printf("%s : ", content[2].balise.balise);

	printf(ANSI_COLOR_RESET);
}