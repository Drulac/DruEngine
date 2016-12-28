char* loadFile(char* filename, long* l);

char* loadFile(char* filename, long* l)
{
	char* buffer = 0;
	long len;
	FILE* f = fopen(filename, "r");

	if(f != NULL)
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

	*l = len;

	return buffer;
}