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

int main(int argc, char *argv[])
{
    setlocale (LC_ALL, "Rus");
    int_least64_t p, g, x, y;
    if(argc < 2)
    {
        printf("Input name\n");
        exit(0);
    }    
    prime_safe_generate(&p, &g);
    lgamal_generate_xy(p, g, &x, &y);
    FILE *fout = fopen("lgamal_key", "w");
    fprintf(fout, "p %"PRId64"\ng %"PRId64"\nx %"PRId64"\ny %"PRId64"\n", p, g, x, y);
    fclose(fout);
    lgamal_encryp(g, p, y, argv[1], "tmp/lgamal1");
    lgamal_decryp(p, x, "tmp/lgamal1", "lgamal_result");

   // execlp("rm", "", "tmp/lgamal1", NULL);
    exit(0);
}

