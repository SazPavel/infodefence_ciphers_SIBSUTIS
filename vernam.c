#include "infodef.h"
#include "randombytes.h"
#include <locale.h>

char* vernam_encryp(char *in, char *out)
{
    int_least64_t size;
    int i;
    char tmp, *key;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    FILE *fkey = fopen("vernam_key", "w");
    if(fin == NULL || fout == NULL)
    {
        perror("file encrypt");
        exit(-1);
    }
    fseek(fin, 0, SEEK_END);
    size = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    key = malloc(size * sizeof(char));
    
    for(i = 0; i < size; i++)
    {
        tmp = fgetc(fin);
        randombytes(&key[i], sizeof(key[i]));
        key[i] = fabs(key[i]);
        fprintf(fkey, "%c", key[i]);
        fprintf(fout, "%d ", tmp ^ key[i]);
    }
    
    fclose(fin);
    fclose(fout);
    fclose(fkey);
    return key;
}

void vernam_decryp(char *key, char *in, char *out)
{
    int tmp;
    int i = 0;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
    {
        perror("file decrypt");
        exit(-1);
    }
    while(fscanf(fin, "%d",  &tmp) != EOF)
    {
        fprintf(fout, "%c", tmp ^ key[i]);
        i++;
    }
    fclose(fin);
    fclose(fout);
}


int main(int argc, char *argv[])
{
    setlocale (LC_ALL, "Rus");
    char *key;
    if(argc < 2)
    {
        printf("Input name\n");
        exit(0);
    }    
    key = vernam_encryp(argv[1], "tmp/vernam1");
    vernam_decryp(key, "tmp/vernam1", "vernam_result");
    free(key);
    //execlp("rm", "", "tmp/vernam1", NULL);
    exit(0);
}
