#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lib/parseur.h"
#include "lib/file.h"
#include "lib/sdlFunc.h"

int main(int argc, char *argv[])
{
	char* filename = "test/test.html";

	long l;

	char* buffer = loadFile(filename, &l);

	_tree tree = parse(buffer, l);

	printTree(tree);

	


	printf("\n");
	free(buffer);

	return EXIT_SUCCESS; // Fermeture du programme
}