#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	//Get the name of a file to read by using console input:
	
	printf("Input name of file, the word to find, and the word to replace\n");

	//Allocate 512B (char == 1B) for holding the input file path
	char fileName[FILENAME_MAX];

	//This is a word that shall be replaced in this file
	char find[512];

	//This is a word that shall replace the found word
	char replace[512];

	//Read three words from standard input (console) separated by space and store them in fileName, find, replace
	//The user will thus input the file name, and a word to replace, and what to replace that word with.
	scanf("%s %s %s", fileName, find, replace);

	//Open the file that has this name with "r" (read text)
	FILE* filePtr = fopen(fileName,"r");

	//Quit if failed to open
	if(filePtr == NULL) {
		return EXIT_FAILURE;
	}

	//Seek to the end of the file
	fseek(filePtr, 0, SEEK_END);

	//Get the current position (at the end of the file) this will be the file size
	int size = ftell(filePtr); 

	//Seek back to start of the file with SEEK_SET
	fseek(filePtr, 0, SEEK_SET); 

	//Allocate memory for holding all the data of this file
	char* text = malloc(size);

	printf("Allocated %d bytes\n", size);

	//Read the entire file and save its contents to text
	fread(text, 1, size, filePtr);

	printf("ORIGINAL:\n\n%s\n", text);

	//Close the file
	fclose(filePtr);

	//Some book-keeping for later
	int occurences = 0;
	int currentIndexOld = 0;
	int currentIndexReplaced = 0;
	int currentIndexReplacedFile = 0;

	//strlen() finds the length of a C-string by looking for the null-terminator
	int findLength = strlen(find);
	int replaceLength = strlen(replace);

	//Here we put the new file text, we simply ASSUME that a file of double size should be able to fit the replaced words
	//*** Övning 3: Denna malloc kan inte garantera att minnet får plats, alltså kan programmet krascha om den nya texten tar för stor plats.
	//*** Fundera på hur man skulle kunna lösa detta problem, hur kan vi i förväg veta hur mycket minne den nya filen kommer ta? Eller finns nån annan lösning?

	char* currentChar = text;
	while(currentChar != NULL)
	{
		//strstr() finds the first occurence of a string in the string
		currentChar = strstr(currentChar, find);

		//If no occurence was found, break loop
		if(currentChar == NULL)
		{
			break;
		}

		//Move forward in the text too by the length of the found word
		currentChar += findLength;

		//Remember how many we found
		occurences++;
	}

	int colorLength = strlen("\x1B[33m\x1B[0m");
	char* replacedText =  malloc(size + (replaceLength - findLength + colorLength)*occurences);
	char* replacedTextFile = malloc(size + (replaceLength - findLength)*occurences);

	//Find the word the user wanted, we do this in a loop until we reach the end of the string which is the null-terminator '\0'
	//This finds how many occurences of the word there is, so we know how much memory is needed.
	currentChar = text;
	while(currentChar != NULL)
	{
		//Remember where we were before strstr is called
		char* startChar = currentChar;

		//strstr() finds the first occurence of a string in the string
		currentChar = strstr(currentChar, find);

		//If no occurence was found, break loop
		if(currentChar == NULL)
		{
			//Copy the final part of the text
			strcpy(replacedText + currentIndexReplaced, startChar);
			strcpy(replacedTextFile + currentIndexReplacedFile, startChar);
			break;
		}

		//Copy the text up until the found string
		strncpy(replacedText + currentIndexReplaced, startChar, currentChar - startChar);
		strncpy(replacedTextFile + currentIndexReplacedFile, startChar, currentChar - startChar);

		//Move forward for next iteration
		int step = currentChar - startChar;
		currentIndexReplaced += step;
		currentIndexReplacedFile += step;

		//Change color to yellow
		{
			static const char* color = "\x1B[33m";
			strcpy(replacedText + currentIndexReplaced, color);
			currentIndexReplaced += strlen(color);
		}

		//Copy in the word that should replace the found word
		strcpy(replacedText + currentIndexReplaced, replace);
		strcpy(replacedTextFile + currentIndexReplacedFile, replace);

		//Move forward by the length of the replaced word, which is now written to replacedText
		currentIndexReplaced += replaceLength;
		currentIndexReplacedFile += replaceLength;

		//Change back color to default
		{
			static const char* color = "\x1B[0m";
			strcpy(replacedText + currentIndexReplaced, color);
			currentIndexReplaced += strlen(color);
		}

		//Move forward in the text too by the length of the found word
		currentChar += findLength;
	}

	printf("REPLACED TEXT:\n\n%s\n", replacedText);

	printf("\n\nFound %d occurences\n",occurences);

	//Create a new file that has replaced the desired word
	FILE* newFile = fopen("out.txt","w");

	//If failed to open
	if(newFile == NULL)
	{
		printf("Failed to write file\n");
		return -1;
	}

	//*** Övning 3: Utskriften till fil här är inte problemfri för den innehåller färgformattering från tidigare print, 
	//*** det kommer alltså finnas ej önskvärda tecken i den nya filen.
	//*** Utan att ta bort färg-printen, lägg till en annan sträng som används för att skriva till filen nedan:

	//Write the replacedText string to the new file, we get the total size with strlen + 1 for null terminator
	//Each element to write is one byte or one char (sizeof(char))
	fwrite(replacedTextFile, strlen(replacedTextFile),sizeof(char),newFile);

	//close the new file
	fclose(newFile);

	free(text);
	free(replacedText);
	free(replacedTextFile);

	return 0;
}