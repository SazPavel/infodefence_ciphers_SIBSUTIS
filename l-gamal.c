#include "infodef.h"
#include "randombytes.h"
#include <locale.h>

void lgamal_generate_b(int_least64_t *p, int_least64_t *g, int_least64_t *x, int_least64_t *y)
{
    int_least64_t q;
    randombytes(p, sizeof(*p));
    *p = fabs(*p % (int_least64_t)1e4) + 127;
    while(1)
    {
        if(modpow(*p, 1, 12) == 11)
            break;
        *p += 1;
    }
    for(;; *p += 12)
    {
        q = (*p - 1) / 2;
        if(*p >= 11 && (modpow(*p, 1, 12) == 11) && prime_test(*p) && prime_test(q))
        {
            break;
        }
    }
    for(*g = 2; *g < *p - 1; *g += 1)
    {
        if(modpow(*g, q, *p) != 1) 
            break;
    }

    randombytes(x, sizeof(*x));
    *x = fabs(*x % (*p - 3)) + 2;
    *y = modpow(*g, *x, *p);
}

void lgamal_generate_a(int_least64_t p, int_least64_t *k)
{
    randombytes(k, sizeof(*k));
    *k = fabs(*k % (p - 3)) + 2;
}

void lgamal_encryp(int_least64_t g, int_least64_t k, int_least64_t p, int_least64_t y, char *in, char *out)
{
    int_least64_t tmp;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
        perror("no file en  ");
    while((tmp = fgetc(fin)) != EOF)
    {
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
        perror("no file de");
    while(fscanf(fin, "%"PRId64" %"PRId64, &a, &b) != EOF)
    {
        fprintf(fout, "%c", (char)((b * modpow(a, p - 1 - x, p)) % p));
    }
    fclose(fin);
    fclose(fout);
}

int main()
{
    setlocale (LC_ALL, "Rus");
    int_least64_t p, g, x, y, k;
    lgamal_generate_b(&p, &g, &x, &y);
    lgamal_generate_a(p, &k);
    printf("p %"PRId64" g %"PRId64" x %"PRId64" y %"PRId64" k %"PRId64"\n", p, g, x, y, k);

    lgamal_encryp(g, k, p, y, "test", "lgamal1");
    lgamal_decryp(p, x, "lgamal1", "lgamal_result");

    //execlp("rm", "", "lgamal1", NULL);
    exit(0);
}

