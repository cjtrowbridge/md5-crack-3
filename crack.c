#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "md5.h"

const int PASS_LEN=50;        // Maximum any password can be
const int HASH_LEN=33;        // Length of MD5 hash strings

struct timespec start, stop;

double elapsed()
{
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    double elapsed = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;    // in microseconds
    return elapsed;
}   

// Given a hash and a plaintext guess, return 1 if
// the hash of the guess matches the given hash.
// That is, return 1 if the guess is correct.
int tryguess(char *hash, char *guess)
{
    /// Hash the guess using MD5
    char *hashedGuess = md5(guess,strlen(guess));
    
    // Compare the two hashesS
    if(strcmp(hash, hashedGuess)==0){
        //returns zero if both strings match
        free(hashedGuess);
        return 0;
    }else{
        free(hashedGuess);
        return 1; 
    }
}

// TODO
// Read in the hash file and return the array of strings.
char **read_hashes(char *filename, int *lines)
{
    //get file size
    struct stat st;
    if (stat(filename, &st) == -1)
    {
        fprintf(stderr, "Can't get info about %s\n", filename);
        exit(1);
    }
    int len = st.st_size;
    
    //malloc space for entire file
    char *file = (char *)malloc(len * sizeof(char));
    
    //read entire file into memory
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Can't open %s for reading\n", filename);
        exit(1);
    }
    fread(file, 1, len, f);
    fclose(f);
    
    //replace \n with \0
    int count = 0;
    for (int i = 0; i <= len; i++)
    {
        if(file[i] == '\n')
        {
            file[i] = '\0';
            count++;
        }
    }
    
    //malloc space for array of pointers
    char ** line = malloc((count+1) * sizeof(char*));
    
    //fill in addresses
    int word = 0;
    line[word] = file; //the first word in the file
    word++;
    for (int i = 1; i < len; i++)
    {
        if(file[i] == '\0' && (i+1) < len)
        {
            line[word] = &file[i + 1]; //the file pointer plus the distance to this newline
            word++;
        }
    }
    
    line[word] = NULL;
    
    //return address of second array
    *lines = word;
    return line;
}


// TODO
// Read in the dictionary file and return the data structure.
// Each entry should contain both the hash and the dictionary
// word.
char **read_dict(char *filename, int *lines)
{
    //get file size
    struct stat st;
    if (stat(filename, &st) == -1)
    {
        fprintf(stderr, "Can't get info about %s\n", filename);
        exit(1);
    }
    int len = st.st_size;
    
    //malloc space for entire file
    char *file = (char *)malloc(len * sizeof(char));
    
    //read entire file into memory
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "Can't open %s for reading\n", filename);
        exit(1);
    }
    fread(file, 1, len, f);
    fclose(f);
    
    //replace \n with \0
    int count = 0;
    for (int i = 0; i <= len; i++)
    {
        if(file[i] == '\n')
        {
            file[i] = '\0';
            count++;
        }
    }
    
    //malloc space for array of pointers
    char ** line = malloc((count+1) * sizeof(char*));
    
    //fill in addresses
    int word = 0;
    line[word] = file; //the first word in the file
    
    char *currentWord = line[word];
    int neededSpace = (HASH_LEN + 1 + strlen( currentWord ) ) * sizeof(char);
    char *hash = md5( currentWord ,strlen( currentWord ));
    
    //allocate memory for the new string
    line[word] = malloc( neededSpace );
    
    //combine the hash and string and put them into the new output array
    sprintf(line[word],"%s %s",hash,currentWord);
    
    //free the hash
    free(hash);
    
    word++;
    for (int i = 1; i < len; i++)
    {
        if(file[i] == '\0' && (i+1) < len)
        {
            char *currentWord = &file[i + 1];
            int neededSpace = (HASH_LEN + 1 + strlen( currentWord ) ) * sizeof(char);
            char *hash = md5( currentWord ,strlen( currentWord ));
            
            //allocate memory for the new string
            line[word] = malloc( neededSpace );
            
            //combine the hash and string and put them into the new output array
            sprintf(line[word],"%s %s",hash,currentWord);
            
            //free the hash
            free(hash);
            
            //increment the pointer counter
            word++;
        }
    }
    
    line[word] = NULL;
    
    //return address of second array
    *lines = word;
    return line;
}

int order(const void *a, const void *b)
{
    return strcmp( *(char **)a, *(char **)b);
}

int prefix_search(const void *t, const void *a)
{
    return strncmp((char *)t, *(char **)a,strlen((char*)t));
}

int main(int argc, char *argv[])
{
    //get time at start
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }
    
    printf("==========================================================================================\nCJ's Fast Cracker\n==========================================================================================\nWorking...\n");
    
    
    // TODO: Read the hash file into an array of strings
    int hashCount = 0;
    char **hashes = read_hashes(argv[1],&hashCount);

    // TODO: Read the dictionary file into an array of strings
    int dictCount = 0;
    char **dict = read_dict(argv[2],&dictCount);
    
    //sort dictionary
    qsort(dict  ,dictCount,sizeof(char *),order);
    
    for(int i = 0; i < hashCount; i++)
    {
        char **cracked = (char **)bsearch(hashes[i],dict,dictCount,sizeof(char *),prefix_search);
        
        printf("%s ", hashes[i] );
        
        if (cracked==NULL)
        {
            printf ("This hash was not in the array.\n");
            continue;
        }
        
        
        printf ("%s\n",*cracked + HASH_LEN);
        
        
    }
    
    
    //show runtime
    printf("==========================================================================================\nCracked %d Hashes. Dictionary Size: %d. Elapsed Time: %lf sec\n==========================================================================================\n",hashCount,dictCount,elapsed()/1000000);
    
}
