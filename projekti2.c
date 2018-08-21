#include "projekti.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



static int numberOfCountries = 0;
static struct country *countries = NULL;

void updateMedals(char *name, int gold, int silver, int bronze){

	int i;
	for (i = 0; i < numberOfCountries; i++){ 

		//find the correct country
		if (strcmp(countries[i].name, name) == 0){
			countries[i].bronze += bronze;
			countries[i].silver += silver;
			countries[i].gold += gold;
			break;
		}
	}

	//if given country wasn't found in the loop
	if (i == numberOfCountries) {
		printf("Invalid country.\n");
	}
	else {
		printf("Updated medals successfully.\n");
	}

}

struct country *addCountry(char *name){

	//check if user is trying to add an already added country
	for (int i = 0; i < numberOfCountries; i++){ 
		if (strcmp(countries[i].name, name) == 0){
			printf("Country has already been added.\n");
			return countries;
		}
	}	

	//allocate memory for the new country in the array
	struct country *newCountries = realloc(countries, (numberOfCountries + 1) * 
										sizeof(struct country));

	// if allocating memory did not work
	if (newCountries == NULL) {
        return NULL; 			
    }

	//allocate memory for name 
	newCountries[numberOfCountries].name = malloc((strlen(name) + 2) * sizeof(char));
	if (newCountries[numberOfCountries].name == NULL) {
        return NULL; 			
    }
	strcpy(newCountries[numberOfCountries].name, name);

	//initialize medals
	newCountries[numberOfCountries].bronze = 0;
	newCountries[numberOfCountries].silver = 0;
	newCountries[numberOfCountries].gold = 0;

	printf("Country %s added.\n", newCountries[numberOfCountries].name);

	numberOfCountries++;
	return newCountries;
}

int comp (const void * elem1, const void * elem2) {
    struct country a = *((struct country*)elem1);
    struct country b = *((struct country*)elem2);

    if (a.gold > b.gold) return  1;
    printf("cmp-1");
    if (a.gold < b.gold) return -1;
    printf("cmp-3");

    if (a.silver > b.silver) return  1;
    printf("cmp-3");
    if (a.silver < b.silver) return -1;
    printf("cmp-4");

    if (a.bronze > b.bronze) return  1;
    printf("cmp-5");
    if (a.bronze < b.bronze) return -1;
    printf("cmp-6");

    return 0;
}




void printMedals(){

	//qsort(countries, numberOfCountries, sizeof(struct country *), comp);

	for (int i = 0; i < numberOfCountries; i++){
		printf("%s %d %d %d\n", 
			countries[i].name, 
			countries[i].gold, 
			countries[i].silver, 
			countries[i].bronze);
	}
	printf("Printing completed.\n");
}




void saveArray(char *fileName){

	FILE *fp = fopen(fileName, "w");
	if (fp == NULL) {
		printf("Opening file failed.\n");
	}
	else {
		//write the content of the array into file
		size_t a = fwrite(countries, sizeof(struct country), numberOfCountries, fp);
		if (a == numberOfCountries) {
			printf("Array written to file %s\n", fileName);
		}
		fclose(fp);
	}
}




struct country *loadArray(char *fileName){
	FILE *fp = fopen(fileName, "r");
	if (!fp) {
		printf("Loading from file failed.\n");
		return countries;
	}

	printf("File opened successfully.\n");

	//find the length of array in file
	fseek(fp, 0L, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	printf("%lu\n", len);

	//allocate memory for the array to be loaded
	struct country *newCountries = malloc(sizeof(struct country) * len);

	//read the array from the file
	size_t howmany = fread(newCountries, sizeof(struct country), len, fp);
		
	if (howmany != len) {
		printf("There was an error in reading from the file.\n");
		return countries;
	}

	fclose(fp);
	return newCountries;
}



void releaseMemory(){

	for (int i = 0; i < numberOfCountries; i++){
		free(countries[i].name);
	}
	free(countries);
	printf("Memory released.\n");
}

int main(void){

    char command;
	do {

		//initialize some values
		char *input = (char *)malloc(32 * sizeof(char));

		char name[20];
	
		unsigned long inputLen;
		int gold = 0, silver = 0, bronze = 0;

		getline(&input, &inputLen, stdin);	//take input from user, remember to free input
		sscanf(input, "%c", &command);		//identify the command letter
		input += 2;							//skip command and whitespace

		switch(command) {					//choose a right function to use based on the command letter
			case 'A' :
				sscanf(input, "%s", name);
				countries = addCountry(name);
				if (countries == NULL) {
       				printf("Adding country failed. Exiting program.\n");
       				return -1;
   				}
				break;

			case 'M' :
				sscanf(input, "%s %d %d %d", name, &gold, &silver, &bronze);
				updateMedals(name, gold, silver, bronze);
				break;

			case 'L' :
				printMedals();
				break;

			case 'W' :
				sscanf(input, "%s", name);
				saveArray(name);
				break;

			case 'O' :
				sscanf(input, "%s", name);
				loadArray(name);
				break;

			case 'Q' :
				releaseMemory();
				break;

			default :
				printf("Unknown command\n");

		}
		//free(input);

	} while(command != 'Q' && command != -1);		//** Remember to modify so that EOF situation gets checked **

	printf("Exited program.\n");
	return 1;

}