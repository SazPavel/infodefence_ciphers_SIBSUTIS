#include "infodef.h"
#include "randombytes.h"
#include <locale.h>

void rsa_generate(int_least64_t *n, int_least64_t *c, int_least64_t *d)
{
    int_least64_t p, q, am[3], bm[3], f;

    //generate p
    randombytes(&p, sizeof(p));
    p = fabs(p % (int_least64_t)1e5) + 256;
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
    q = fabs(q % (int_least64_t)1e4) + 2;
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
    {
        perror("file encrypt");
        exit(-1);
    }
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
    {
        perror("file decrypt");
        exit(-1);
    }
    while(fscanf(fin, "%"PRId64, &tmp) != EOF)
    {
        fprintf(fout, "%c", (char)modpow(tmp, c, n));
    }
    fclose(fin);
    fclose(fout);
}

int main(int argc, char *argv[])
{
    setlocale (LC_ALL, "Rus");
    int_least64_t n, c, d;
    if(argc < 2)
    {
        printf("Input name\n");
        exit(0);
    }
    rsa_generate(&n, &c, &d);
    FILE *fout = fopen("rsa_key", "w");
    fprintf(fout, "n %"PRId64"\nc %"PRId64"\nd %"PRId64" \n", n, c, d);
    fclose(fout);
    
    rsa_encryp(n, d, argv[1], "tmp/rsa1");
    rsa_decryp(n, c, "tmp/rsa1", "rsa_result");
    
    //execlp("rm", "", "tmp/rsa1", NULL);
    exit(0);
}
