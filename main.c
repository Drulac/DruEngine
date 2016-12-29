#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lib/parseur/parseur.h"
#include "lib/file.h"
#include "lib/graphic/GRAPHIC.h"

int render();

int main(int argc, char *argv[])
{
	char* filename = "test/test.html";

	long l;

	char* buffer = loadFile(filename, &l);

	_tree tree = parse(buffer, l);

	printTree(tree);

	GRAPHIC_Init(&render);

	printf("\n");
	free(buffer);

	return EXIT_SUCCESS; // Fermeture du programme
}

int render()
{
	SDL_Color blanc = {255, 255, 255, 255};

	GRAPHIC_Clear();

	GRAPHIC_Text text = GRAPHIC_GenerateText("Hello World", "lib/fonts/roboto/Roboto-Thin.ttf", blanc, 32);
	text.x = (((1366/2-5)-text.w)/2);
	text.y = (480-text.h)/2;
	text.render(text);
}