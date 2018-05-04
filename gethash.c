/*Link to the task: https://www.fi.muni.cz/pb071/hw/homework-04/ */
#include "stdio.h"
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "hash_helper.h"
#include <stdbool.h>
#define MAX_LEN 1024

typedef struct
{
    size_t len; //length of file
    int hex;  //switch for hexadecimal output
    unsigned char xorResult;
    crc16_context crc16_ctx;
    unsigned int c32Result;
    unsigned char md5Result[16];
} hashResults;

typedef struct
{
    bool x0r;
    bool c16;
    bool c32;
    bool md5;
} hashFound;

hashResults initResStruct (char *arg[],size_t size);
hashFound initFoundStruct (char *arg[],size_t size);
void errorMsg();
void *openFile(char* file);
void calculateHashes(FILE *f,char *arg[],size_t size);
void printResults(hashResults x, hashFound f);
int In(char* element,char* array[],size_t size);
int noHashArgument(char* args[],size_t size);
int noFileArgument(char* args[],size_t size);
int hexOutput(char *args[],size_t size);
int noValidArgument(char* args[],size_t size);
int countFileArgument(char *args[], size_t size);
int filePosition(char* args[],size_t size);

int main(int argc,char* argv[])
{
    FILE *f;
    if (argc == 1 || noHashArgument(argv,argc)|| noValidArgument(argv,argc) || countFileArgument(argv,argc) > 1){
        errorMsg();
        return 42;
    }
    if (noFileArgument(argv,argc))
        f = stdin;
    else{
        f = openFile(argv[filePosition(argv,argc)]);
        if (f == NULL)
            return 42;
    }
    calculateHashes(f,argv,argc);
    if (f != stdin)
        fclose(f);
    return 0;
}

void calculateHashes(FILE *f,char *arg[],size_t size)
{
    unsigned char buffer[MAX_LEN];
    unsigned int temp = 0xFFFFFFFF;
    hashResults results = initResStruct(arg,size);
    hashFound found = initFoundStruct(arg,size);
    MD5_CTX md5;
    MD5_Init(&md5);
    int bytesRead = MAX_LEN;
    bool nothingRead = false;
    bool firstIteration = true;

    while (bytesRead == MAX_LEN){
        bytesRead = fread(buffer,sizeof(char),MAX_LEN,f);
        results.len += bytesRead;
        if (bytesRead == 0 && firstIteration){ // if file is empty
            nothingRead = true;
            break;
        }
        if (found.x0r)
           xor_update(&results.xorResult,firstIteration,buffer,bytesRead);
        if (found.c16)
            for (int i = 0; i < bytesRead; i++)
                crc16_update(&results.crc16_ctx,(unsigned char)buffer[i]);

        if (found.c32)
           temp = crc32a(buffer,temp,bytesRead);
        if (found.md5)
            MD5_Update(&md5,buffer,bytesRead);

        if (firstIteration)
            firstIteration = false;
    }

    if (found.c32 && !nothingRead)
        results.c32Result = temp;
    if (found.md5)
        MD5_Final(results.md5Result , &md5);
    printResults(results,found);
}


void printResults(hashResults x, hashFound f)
{
    printf ("Length: %lu bytes\n",x.len);
    if (!x.hex){
        if (f.x0r)
            printf("XOR: %u\n",x.xorResult);
        if (f.c16)
            printf("CRC-16: %u\n",x.crc16_ctx.crc);
        if (f.c32)
            printf("CRC-32: %u\n",x.c32Result);
        if (f.md5){
            printf("MD5: ");
            for(int i=0; i<16; i++)
                printf("%02x", x.md5Result[i]);
            printf("\n");
        }
    }
    else{
        if (f.x0r)
            printf("XOR: 0x%02x\n",x.xorResult);
        if (f.c16)
            printf("CRC-16: 0x%04x\n",x.crc16_ctx.crc);
        if (f.c32)
            printf("CRC-32: 0x%08x\n",x.c32Result);
        if (f.md5){
            printf("MD5: ");
            for(int i=0; i<16; i++)
                printf("%02x", x.md5Result[i]);
             printf("\n");
        }
    }
}


hashResults initResStruct (char *arg[],size_t size)
{
    hashResults temp = { .len = 0, .xorResult = 0, .c32Result = 0, .md5Result = "" , .crc16_ctx = {.crc = 0xFFFF}};
    temp.hex = hexOutput(arg,size);
    return temp;
}

hashFound initFoundStruct (char *arg[],size_t size)
{
    hashFound temp = {.x0r = false, .c16 = false, .c32 = false , .md5 = false};
    temp.x0r = In("-xor",arg,size);
    temp.c16 = In("-c16",arg,size);
    temp.c32 = In("-c32",arg,size);
    temp.md5 = In("-md5",arg,size);
    return temp;
}
void errorMsg()
{
    fprintf(stderr,"Error!\n");
}

int In(char* element,char* array[],size_t size)
{ //check if ELEMENT occurs in ARRAY
    for (size_t i = 0; i < size; i++)
        if (strcmp(element,array[i]) == 0)
            return 1;
    return 0;
}

int noHashArgument(char* args[],size_t size)
{ // check if there is nonhash argument
    char *validArguments[4] = {"-xor","-c16","-c32","-md5"};
    unsigned int counter = 0;
    for (int i = 0; i < 4; i++)
        if (In(validArguments[i],args,size))
            counter++;
    if (counter == 0) // hash argument not found
        return 1;
    return 0;
}

int noValidArgument(char* args[],size_t size)
{ // check if there is  nonvalid argument
    char* validArgs[6] = {"-xor","-c16","-c32","-md5","-hex","-f"};
    for (size_t i = 1; i < size; i++)
    {
        if (strcmp(args[i],"-f") == 0){
            i++;
            continue;
        }
        if (!In(args[i],validArgs,6))
            return 1;
    }
    return 0;
}

int filePosition(char* args[],size_t size)
{ // returns position of file argument
    for (size_t i = 0; i < size; i++)
        if (strcmp(args[i],"-f") == 0)
            return i+1;
    return -1;
}

void *openFile(char* file)
{
    FILE *f = fopen(file,"rb");
    if (f == NULL){
        errorMsg();
        return NULL;
    }
    return f;
}

int hexOutput(char *args[],size_t size)
{
    return (In("-hex",args,size));
}

int noFileArgument(char *args[], size_t size)
{
    return (!In("-f",args,size));
}

int countFileArgument(char *args[], size_t size)
{
    int counter = 0;
    for (size_t i = 0; i < size; i++)
        if (strcmp("-f",args[i]) == 0)
            counter++;
    return counter;
}

