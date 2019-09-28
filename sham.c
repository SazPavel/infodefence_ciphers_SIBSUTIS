#include "infodef.h"
#include "randombytes.h"
#include <locale.h>

int sham_generate_p(int_least64_t *p)
{
    randombytes(p, sizeof(*p));
    *p = fabs(*p % (int_least64_t)1e9) + 127;
    if(*p % 2 == 0)
        *p += 1;
    for(;; *p += 2)
    {
        if(prime_test(*p))
        {
            break;
        }
    }
    return 0;
}

int sham_generate_en(int_least64_t p, int_least64_t *ca, int_least64_t *da)
{
    int_least64_t am[3], bm[3];
    do
    {
        randombytes(ca, sizeof(*ca));
        *ca = fabs(*ca % (int_least64_t)1e9);
        am[0] = p - 1; am[1] = 1; am[2] = 0;
        bm[0] = *ca; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
    }while(am[0] != 1);
    if(am[2] < 0)
        am[2] += (p - 1);
    *da = am[2];
    return 0;
}

int sham_generate_de(int_least64_t p, int_least64_t *cb, int_least64_t *db)
{
    int_least64_t am[3], bm[3];
    do
    {
        randombytes(cb, sizeof(*cb));
        *cb = fabs(*cb % (int_least64_t)1e9);
        am[0] = p - 1; am[1] = 1; am[2] = 0;
        bm[0] = *cb; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
    }while(am[0] != 1);
    if(am[2] < 0)
        am[2] += (p - 1);
    *db = am[2];
    return 0;
}

int sham_first_en(int_least64_t p, int_least64_t ca, char *in, char *out)
{
    int_least64_t tmp;
    int length, i;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
        perror("no file");
    while((tmp = fgetc(fin)) != EOF)
    {
        fprintf(fout, "%"PRId64" ", modpow(tmp, ca, p));
    }
    fclose(fin);
    fclose(fout);
}

int sham_first_de(int_least64_t p, int_least64_t cb, char *in, char *out)
{
    int_least64_t tmp;
    int length, i;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
        perror("no file");
    while(fscanf(fin, "%"PRId64, &tmp) != EOF)
    {
        fprintf(fout, "%"PRId64" ", modpow(tmp, cb, p));
    }
    fclose(fin);
    fclose(fout);
}

int sham_second_en(int_least64_t p, int_least64_t da, char *in, char *out)
{
    int_least64_t tmp;
    int length, i;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
        perror("no file");
    while(fscanf(fin, "%"PRId64, &tmp) != EOF)
    {
        fprintf(fout, "%"PRId64" ", modpow(tmp, da, p));
    }
    fclose(fin);
    fclose(fout);
}

int sham_second_de(int_least64_t p, int_least64_t db, char *in, char *out)
{
    int_least64_t tmp;
    int length, i;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
        perror("no file");
    while(fscanf(fin, "%"PRId64, &tmp) != EOF)
    {
        fprintf(fout, "%c", (char)modpow(tmp, db, p));
    }
    fclose(fin);
    fclose(fout);
}

int main()
{
    setlocale (LC_ALL, "Rus");
    int_least64_t  p, ca, da, cb, db;
    sham_generate_p(&p);
    sham_generate_en(p, &ca, &da);
    sham_generate_de(p, &cb, &db);
    printf("p %"PRId64" ca %"PRId64" da %"PRId64" cb %"PRId64" db %"PRId64" \n", p,ca,da,cb,db);

    sham_first_en(p, ca, "test", "sham1");
    sham_first_de(p, cb, "sham1", "sham2");
    sham_second_en(p, da, "sham2", "sham1");
    sham_second_de(p, db, "sham1", "sham_result");

    execlp("rm", "", "sham1", "sham2", NULL);
    exit(0);
}
