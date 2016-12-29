#include "structs.h"
#include "orphelinsTags.h"
#include "colors.h"
#include "string.h"

_tree parse(char * code, long codeLen);
void printTree(_tree tree);

_tree parse(char* code, long codeLen)
{
	_tree tree;
	int baliseLen = 1000;
	tree.content = malloc(sizeof(_treeElement) * baliseLen);

	int i = 0;
	int cursor = 0;
	int baliseNameComplete = 0;
	int attributNameComplete = 0;
	int inStr = 0;
	int attrCursor = 0;

	for(i = 0; i < codeLen; i++)
	{
		char c = code[i];

		/*
		if(c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
		{
			c = ' ';
		}
		*/

		if(c == '<' && (i == 0 || code[i-1] != '\\'))
		{
			//isOpenChar

			if(tree.content[cursor].text && tree.content[cursor].text != "" && (strlen(tree.content[cursor].text) > 0))
			{
				if(cursor+1 == baliseLen)
				{
					baliseLen = baliseLen*2;
					tree.content = realloc(tree.content, sizeof(_treeElement) * baliseLen);
				}

				cursor++;
			}

			baliseNameComplete = 0;

			tree.content[cursor].type = 0;

			tree.content[cursor].balise.balise = malloc(sizeof(char));
			*tree.content[cursor].balise.balise = '\0';

			tree.content[cursor].balise.attributs[attrCursor].name = malloc(sizeof(char));
			*tree.content[cursor].balise.attributs[attrCursor].name = '\0';
			
			tree.content[cursor].balise.attributs[attrCursor].value = malloc(sizeof(char));
			*tree.content[cursor].balise.attributs[attrCursor].value = '\0';
		}else if(c == '>' && (i == 0 || code[i-1] != '\\')){
			//isCloseChar

			tree.content[cursor].balise.orphelin = 0;
			int j = 0;
			for(j = 0; j < 17; j++)
			{
				tree.content[cursor].balise.orphelin += compare(tree.content[cursor].balise.balise, orphelinsTag[j]);
			}

			if(cursor+1 == baliseLen)
			{
				baliseLen = baliseLen*2;
				tree.content = realloc(tree.content, sizeof(_treeElement) * baliseLen);
			}

			cursor++;
			baliseNameComplete = 0;
			attrCursor = 0;

			tree.content[cursor].type = 1;
			tree.content[cursor].text = malloc(sizeof(char));
			*tree.content[cursor].text = '\0';
		}else if(tree.content[cursor].type == 1){
			if(!(!(tree.content[cursor].text && tree.content[cursor].text != "" && (strlen(tree.content[cursor].text) > 0)) && isspace(c)))
			{
				int tmplen = 0;
				if(tree.content[cursor].text != "")
				{
					tmplen = strlen(tree.content[cursor].text);
				}
				tree.content[cursor].text = realloc(tree.content[cursor].text, tmplen + 1 + 1);

				strapp(tree.content[cursor].text, c);
			}
		}else if(tree.content[cursor].type == 0){
			if(c == '/' && (!tree.content[cursor].balise.balise || tree.content[cursor].balise.balise == "" || (strlen(tree.content[cursor].balise.balise) == 0)))
			{
				tree.content[cursor].balise.closing = 1;
			}else if(baliseNameComplete == 0){
				if(c == ' ' && (i == 0 || code[i-1] != '\\'))
				{
					baliseNameComplete = 1;
					attributNameComplete = 0;
					attrCursor = 0;
				}else{
					int tmplen = 0;
					if(tree.content[cursor].balise.balise != "")
					{
						tmplen = strlen(tree.content[cursor].balise.balise);
					}
					tree.content[cursor].balise.balise = realloc(tree.content[cursor].balise.balise, tmplen + 1 + 1);

					strapp(tree.content[cursor].balise.balise, tolower(c));
				}
			}else if(baliseNameComplete == 1){
				//quand le nom de la balise est récupéré
				//on parse les attributs
				if(attributNameComplete == 0){
					//Attribut name
					if(c == '=' && (i == 0 || code[i-1] != '\\'))
					{
						attributNameComplete = 1;
						tree.content[cursor].balise.attributsLen++;
					}else{
						int tmplen = 0;
						if(tree.content[cursor].balise.attributs[attrCursor].name != "")
						{
							tmplen = strlen(tree.content[cursor].balise.attributs[attrCursor].name);
						}
						tree.content[cursor].balise.attributs[attrCursor].name = realloc(tree.content[cursor].balise.attributs[attrCursor].name, tmplen + 1 + 1);

						strapp(tree.content[cursor].balise.attributs[attrCursor].name, c);
					}
				}else if(attributNameComplete == 1){
					//Attribut value
					if(isspace(c) && !inStr && (i == 0 || code[i-1] != '\\'))
					{
						//next attribut
						attrCursor++;
						attributNameComplete = 0;

						tree.content[cursor].balise.attributs[attrCursor].name = malloc(sizeof(char));
						*tree.content[cursor].balise.attributs[attrCursor].name = '\0';

						tree.content[cursor].balise.attributs[attrCursor].value = malloc(sizeof(char));
						*tree.content[cursor].balise.attributs[attrCursor].value = '\0';

					}else if(c == '"' && !inStr && (i == 0 || code[i-1] != '\\')){
						inStr = 1;
						//on commence a parser la valeur
					}else if(c == '"' && inStr && (i == 0 || code[i-1] != '\\')){
						inStr = 0;
						//on a finit de parser la valeur
					}else if(tree.content[cursor].balise.attributs[attrCursor].value)
					{
						int tmplen = 0;
						if(tree.content[cursor].balise.attributs[attrCursor].value != "")
						{
							tmplen = strlen(tree.content[cursor].balise.attributs[attrCursor].value);
						}
						tree.content[cursor].balise.attributs[attrCursor].value = realloc(tree.content[cursor].balise.attributs[attrCursor].value, tmplen + 1 + 1);

						strapp(tree.content[cursor].balise.attributs[attrCursor].value, c);
					}
				}
			}
		}
	}

	tree.len = cursor;

	return tree;
}

void printTree(_tree tree)
{
	printf(ANSI_COLOR_RESET);
	int tabs = 0;
	int j = 0, i = 0;

	for(i = 0; i < tree.len; i++)
	{
		if(tree.content[i].type == 0 && tree.content[i].balise.closing)
		{
			tabs--;
		}

		for(j = 0; j < tabs; j++)
		{
			printf("\t");
		}

		if(tree.content[i].type == 0)
		{
			printf("<");
			int attrsLen = tree.content[i].balise.attributsLen;
			if(tree.content[i].balise.closing)
			{
				printf("/");
			}else if(!tree.content[i].balise.orphelin){
				tabs++;
			}

			printf("%s", tree.content[i].balise.balise, attrsLen);

			for(j = 0; j < attrsLen; j++)
			{
				printf(" %s=\"%s\"", tree.content[i].balise.attributs[j].name, tree.content[i].balise.attributs[j].value);
			}
			printf(">\n");
		}else{
			printf(ANSI_COLOR_BLUE);
			printf("%s", tree.content[i].text);
			printf(ANSI_COLOR_RESET);
			printf("\n");
		}
	}

	printf(ANSI_COLOR_BLUE);
	printf("\n%d\n", tree.len);
	printf(ANSI_COLOR_RESET);
}