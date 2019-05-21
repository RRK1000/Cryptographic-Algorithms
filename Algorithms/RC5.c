#include <stdio.h>
#include <stdlib.h>
#define w 16 //word size
#define r 8  //number of rounds
#define b 8  //key size
#define c 4  //words in the key, c=b/(w/8)
#define t 18 //size=2*(r+1)
unsigned int S[t] = {0};
unsigned int L[c] = {0};
unsigned int P = 0xb7e1, Q = 0x9e37; /* magic constants*/

unsigned int rotate_left(unsigned int x, unsigned int y)
{
    return (x << (y & (w - 1))) | (x >> (w - (y & (w - 1))));
}
unsigned int rotate_right(unsigned int x, unsigned int y)
{
    return (x >> (y & (w - 1))) | (x << (w - (y & (w - 1))));
}

void initialize(unsigned char *K)
{
    unsigned int u = w / 8, A, B;
    int i, j, k;
    A = B = i = j = k = 0;
    L[c-1] = 0;
    for (i = b - 1; i != -1; i--)
        L[i / u] = rotate_left(L[i / u], 8) + K[i];

    S[0] = P;
    for (int i = 1; i < t; i++)
        S[i] = S[i - 1] + Q;

    for (k = 0; k <= 3 * t; k++)
    {
        A = S[i] = rotate_left(S[i] + (A + B), 3);
        B = L[j] = rotate_left(L[j] + (A + B), (A + B));
        i = (i + 1) % t;
        j = (j + 1) % c;
    }
}

void RC5E(unsigned int *plain_text, unsigned int *output)
{
    unsigned int A = plain_text[0] + S[0], B = plain_text[1] + S[1];
    for (int i = 1; i <= r; i++)
    {
        A = rotate_left(A ^ B, B) + S[2 * i];
        B = rotate_left(B ^ A, A) + S[2 * i + 1];
    }
    output[0] = A;
    output[1] = B;
}

void RC5D(unsigned int *ct, unsigned int *pt)
{
    unsigned int B = ct[1], A = ct[0];
    for (int i = r; i != 0; i--)
    {
        B = rotate_right(B - S[2 * i + 1], A) ^ A;
        A = rotate_right(A - S[2 * i], B) ^ B;
    }
    pt[1] = B - S[1];
    pt[0] = A - S[0];
}

int main()
{
    unsigned int plain_text1[2], plain_text2[2], cipher_text[2];
    unsigned char key[b];
    plain_text1[0] = 47;
    plain_text1[1] = 88;
    initialize(key);
    RC5E(plain_text1, cipher_text);
    RC5D(cipher_text, plain_text2);
    unsigned int k;
    printf("KEY: ");
    for (int j = 0; j < b; j++)
        printf("%X", key[j]);
    printf("\nPlain Text:\n");
    for (k = 0; k < w; k += 8)
    {
        printf("%X", (plain_text1[0] >> k) & 0xFF);
        printf("%X", (plain_text1[1] >> k) & 0xFF);
    }

    printf("\nCipher Text:\n");
    for (k = 0; k < w; k += 8)
    {
        printf("%X", (cipher_text[1] >> k) & 0xFF);
        printf("%X", (cipher_text[0] >> k) & 0xFF);
    }
    printf("\n");
}