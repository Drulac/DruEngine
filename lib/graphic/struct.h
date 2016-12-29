typedef struct GRAPHIC_Text
{
	int w;
	int h;
	SDL_Texture* texture;
	int x;
	int y;
	int (*render)(struct GRAPHIC_Text);
} GRAPHIC_Text;