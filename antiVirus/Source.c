#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "dirent.h"

#pragma warning(disable:4996)

#define FALSE 0
#define TRUE !FALSE
#define MAX_PATH _MAX_PATH + 1


struct FileContent
{
	char* content;
	long  length;
}typedef FileContent;


void FullvirusCheck(FileContent* file, FileContent* mark, char* filename)
{
	int infected = TRUE;
	int index = 0;
	if (mark->length == 0)
	{
		printf("the mark is empty");
		return;
	}
	for (int i = 0; i <= (file->length - mark->length); i++)
	{
		infected = TRUE;
		for (int j = 0; j < mark->length; j++)
		{
			if (file->content[j + i] != mark->content[j])
			{
				infected = FALSE;
			}
		}
		if (infected)
		{
			printf("%s: the file is infected!\n", filename);
			return;
		}
	}
	printf("%s: the file is not infected!\n", filename);
}
void Quickviruscheck(FileContent* file, FileContent* mark, char* filename)
{
	int infected = TRUE;
	int index = 0;
	if (mark->length == 0)
	{
		printf("the mark is empty");
		return;
	}
	for (int i = 0; i < (file->length * 0.2); i++)
	{
		infected = TRUE;
		for (int j = 0; j < mark->length; j++)
		{
			if (file->content[j + i] != mark->content[j])
			{
				infected = FALSE;
			}
		}
		if (infected)
		{
			printf("%s: the file is infected!\n", filename);
			return;
		}
	}
	for (int i = (file->length * 0.8); i < file->length; i++)
	{
		infected = TRUE;
		for (int j = 0; j < mark->length; j++)
		{
			if (file->content[j + i] != mark->content[j])
			{
				infected = FALSE;
			}
		}
		if (infected)
		{
			printf("%s: the file is infected!\n", filename);
			return;
		}
	}
	printf("%s: the file is not infected!\n", filename);

}
void  freeFileContent(FileContent* f)
{
	free(f->content);
	free(f);
}
FileContent* openFile(char* input)
{
	FILE* fileptr;
	FileContent* file = calloc(1, sizeof(FileContent));


	fileptr = fopen(input, "rb");					// Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);					// Jump to the end of the file
	file->length = ftell(fileptr);						// Get the current byte offset in the file
	fseek(fileptr, 0, SEEK_SET);					// Jump back to the beginning of the file

	file->content = (char*)malloc(file->length * sizeof(char)); // Enough memory for the file
	fread(file->content, file->length, 1, fileptr);				// Read in the entire file
	fclose(fileptr); // Close the file
	return file;
}
char* input()
{
	char* input = calloc(MAX_PATH, sizeof(char));
	fgets(input, MAX_PATH, stdin);
	char* ptr = strstr(input, "\n");
	if (ptr != NULL)
	{
		*ptr = 0;
	}
	return input;
}
void scanDir(char* file, char* mark, char scanType)
{
	DIR* filedir = opendir(file);
	struct dirent* filecontent;
	if (filedir == NULL)
	{
		printf("the file folder is empty!");
		return;
	}
	filecontent = readdir(filedir);
	while (filecontent != NULL)
	{
		if (filecontent->d_type == DT_REG)
		{
			char* filePath = calloc(strlen(file) + filecontent->d_namlen + 2, sizeof(char));
			sprintf(filePath, "%s\\%s", file, filecontent->d_name);
			FileContent* cfile = openFile(filePath);
			FileContent* cmark = openFile(mark);
			if (scanType == '0')
			{
				FullvirusCheck(cfile, cmark, filecontent->d_name);
			}
			else
			{
				Quickviruscheck(cfile, cmark, filecontent->d_name);
			}
			freeFileContent(cfile);
			freeFileContent(cmark);
			free(filePath);
		}
		filecontent = readdir(filedir);
	}
}


int main(int argc, char** argv)
{
	char c = 0;
	//make quick scan
	printf("welcome to my antiVirus!\nplease press 0 for a normal scan and any other key for quick scan: ");
	char	scanType = getchar(); while ((c = getchar()) != '\n');
	printf("please enter the location of the files: ");
	char*	filePath = input();
	printf("please enter the location of the signature: ");
	char*	markPath = input();
	scanDir(filePath, markPath, scanType);
	free(filePath);
	free(markPath);
	printf("\nscan complete!");
	return 0;
}