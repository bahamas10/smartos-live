#include "../dockerinit/json-nvlist/json-nvlist.h"

/*
 * read the file "fname" and return a buffer on the heap
 * with the entire contents + a nul byte
 * must be free()d by caller
 */
char *
read_file(const char *fname)
{
	long fsize;
	char *string;
	FILE *f;

	f = fopen(fname, "r");
	if (f == NULL)
		return NULL;

	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	string = malloc(fsize + 1);
	if (string == NULL) {
		fclose(f);
		return NULL;
	}
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = '\0';
	return string;
}

nvlist_t *
read_json_file(const char *fname)
{
	nvlist_t *nvl = NULL;
	char *json = read_file(fname);

	if (json == NULL)
		return NULL;

	if (nvlist_parse_json(json, strlen(json), &nvl, NVJSON_FORCE_INTEGER, NULL) != 0) {
		free(json);
		return NULL;
	}

	free(json);
	return nvl;
}
