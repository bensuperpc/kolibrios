int strcmp(const char* string1, const char* string2)
{
	while (1)
	{
		if (*string1<*string2)
			return -1;
		if (*string1>*string2)
			return 1;
		if (*string1=='\0')
			return 0;
		string1++;
		string2++;
	}
}
