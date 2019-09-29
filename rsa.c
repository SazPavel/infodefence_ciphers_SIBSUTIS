#include "infodef.h"
#include "randombytes.h"
#include <locale.h>

void rsa_generate(int_least64_t *n, int_least64_t *c, int_least64_t *d)
{
    int_least64_t p, q, am[3], bm[3], f;

    //generate p
    randombytes(&p, sizeof(p));
    p = fabs(p % (int_least64_t)1e5) + 127;
    if(p % 2 == 0)
        p++;
    for(;; p += 2)
    {
        if(prime_test(p))
        {
            break;
        }
    }

    //generate q
    randombytes(&q, sizeof(q));
    q = fabs(q % (int_least64_t)1e5) + 1;
    if(q % 2 == 0)
        q++;
    for(;;  q += 2)
    {
        if(prime_test(q))
        {
            break;
        }
    }
    *n = p * q;
    f = (p - 1) * (q - 1);

    //generate c & d
    do
    {
        randombytes(d, sizeof(*d));
        *d = fabs(*d % f);
        am[0] = f; am[1] = 1; am[2] = 0;
        bm[0] = *d; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
    }while(am[0] != 1);

    if(am[2] < 0)
        am[2] += f;
    *c = am[2];
}

void rsa_encryp(int_least64_t n, int_least64_t d, char *in, char *out)
{
    int_least64_t tmp;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
        perror("no file en  ");
    while((tmp = fgetc(fin)) != EOF)
    {
        fprintf(fout, "%"PRId64" ", modpow(tmp, d, n));
    }
    fclose(fin);
    fclose(fout);
}

void rsa_decryp(int_least64_t n, int_least64_t c, char *in, char *out)
{
    int_least64_t tmp;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
        perror("no file de");
    while(fscanf(fin, "%"PRId64, &tmp) != EOF)
    {
        fprintf(fout, "%c", (char)modpow(tmp, c, n));
    }
    fclose(fin);
    fclose(fout);
}
int main()
{
    setlocale (LC_ALL, "Rus");
    int_least64_t n, c, d;
    rsa_generate(&n, &c, &d);
    printf("n %"PRId64" c %"PRId64" d %"PRId64" \n", n, c, d);
    
    rsa_encryp(n, d, "test", "rsa1");
    rsa_decryp(n, c, "rsa1", "rsa_result");
    
    execlp("rm", "", "rsa1", NULL);
    exit(0);
}
