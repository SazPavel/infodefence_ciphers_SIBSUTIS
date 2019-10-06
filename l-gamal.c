#include "infodef.h"
#include "randombytes.h"
#include <locale.h>

void lgamal_generate_xy(int_least64_t p, int_least64_t g, int_least64_t *x, int_least64_t *y)
{
    randombytes(x, sizeof(*x));
    *x = fabs(*x % (p - 4)) + 2;
    *y = modpow(g, *x, p);
}

void lgamal_encryp(int_least64_t g, int_least64_t p, int_least64_t y, char *in, char *out)
{
    int_least64_t tmp, k;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
    {
        perror("file encrypt");
        exit(-1);
    }
    while((tmp = fgetc(fin)) != EOF)
    {
        randombytes(&k, sizeof(k));
        k = fabs(k % (p - 4)) + 2;
        fprintf(fout, "%"PRId64" ", modpow(g, k, p));
        fprintf(fout, "%"PRId64"\n", (tmp * modpow(y, k, p)) % p);
    }
    fclose(fin);
    fclose(fout);
}

void lgamal_decryp(int_least64_t p, int_least64_t x, char *in, char *out)
{
    int_least64_t a, b;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
    {
        perror("file decrypt");
        exit(-1);
    }
    while(fscanf(fin, "%"PRId64" %"PRId64, &a, &b) != EOF)
    {
        fprintf(fout, "%c", (char)((b * modpow(a, p - 1 - x, p)) % p));
    }
    fclose(fin);
    fclose(fout);
}

void lgamal_save_public_key(int_least64_t p, int_least64_t y, int_least64_t g)
{
    FILE *fout = fopen("tmp/lgamal_public_key", "w");
    if(fout == NULL)
    {
        perror("file public key");
        exit(-1);
    }
    fprintf(fout, "%"PRId64" %"PRId64" %"PRId64"\n", p, y, g);
    fclose(fout);
}

void lgamal_save_private_key(int_least64_t p, int_least64_t x)
{
    FILE *fout = fopen("tmp/lgamal_private_key", "w");
    if(fout == NULL)
    {
        perror("file private key");
        exit(-1);
    }
    fprintf(fout, "%"PRId64" %"PRId64"\n", p, x);
    fclose(fout);
}

void lgamal_load_public_key(int_least64_t *p, int_least64_t *y, int_least64_t *g)
{
    FILE *fout = fopen("tmp/lgamal_public_key", "r");
    if(fout == NULL)
    {
        perror("file public key");
        exit(-1);
    }
    fscanf(fout, "%"PRId64" %"PRId64" %"PRId64, p, y, g);
    fclose(fout);
}

void lgamal_load_private_key(int_least64_t *p, int_least64_t *x)
{
    FILE *fout = fopen("tmp/lgamal_private_key", "r");
    if(fout == NULL)
    {
        perror("file public key");
        exit(-1);
    }
    fscanf(fout, "%"PRId64" %"PRId64, p, x);
    fclose(fout);
}

int main(int argc, char *argv[])
{
    setlocale (LC_ALL, "Rus");
    int_least64_t p, g, x, y;
    int temp;
    if(argc < 3)
    {
        printf("example: ./lgamal name command(1 - generate keys, 2 - encrypt, 3 - decrypt, 4 - all)\n");
        exit(0);
    }
    sscanf(argv[2], "%d", &temp);
    switch(temp)
    {
        case 1:
            prime_safe_generate(&p, &g);
            lgamal_generate_xy(p, g, &x, &y);
            lgamal_save_public_key(p, y, g);
            lgamal_save_private_key(p, x);
            break;  
        case 2:
            lgamal_load_public_key(&p, &y, &g);
            lgamal_encryp(g, p, y, argv[1], "tmp/lgamal1");
            break;
        case 3:
            lgamal_load_private_key(&p, &x);
            lgamal_decryp(p, x, "tmp/lgamal1", "lgamal_result");
            break;
        case 4:
            prime_safe_generate(&p, &g);
            lgamal_generate_xy(p, g, &x, &y);
            lgamal_encryp(g, p, y, argv[1], "tmp/lgamal1");
            lgamal_decryp(p, x, "tmp/lgamal1", "lgamal_result");
            break;        
    }

   // execlp("rm", "", "tmp/lgamal1", NULL);
    exit(0);
}

