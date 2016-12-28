typedef struct
{
	char* name;
	char* value;
} _attribut;

typedef struct
{
	char* balise;
	_attribut attributs[255];
	size_t attributsLen;
	int closing;
	int orphelin;
} _balise;

typedef struct
{
	int type;
	_balise balise;
	char* text;
} _treeElement;

typedef struct
{
	int len;
	_treeElement* content;
} _tree;