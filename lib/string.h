void strapp(char* dest, char c); 
int compare(const char* chaine1, const char* chaine2);

int compare(const char* chaine1, const char* chaine2)
{
	unsigned int i=0;
	if( strlen(chaine1) != strlen(chaine2) )
		return 0;
	for(i=0;i<strlen(chaine1);i++)
		if( chaine1[i] != chaine2[i])
			return 0;
	return 1;
}

void strapp(char* dest, char c)
{
	char str[] = "\0\0";
	str[0] = c;
	str[1] = '\0';
	strcat(dest, str);
}