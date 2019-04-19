#include <stdio.h>
#include <stdlib.h>

char *base64E(char input[], int length)
{
    char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *result = (char *)malloc(1000 * sizeof(char));
    int index, no_of_bits = 0, padding = 0, val, count = 0, temp;
    int i, j, k = 0;
    for (i = 0; i < length; i += 3)
    {
        val = 0, count = 0, no_of_bits = 0;

        for (j = i; j < length && j <= i + 2; j++)
        {
            val = val << 8;
            val = val | input[j];
            count++;
        }
        no_of_bits = count * 8;
        padding = no_of_bits % 3;
        while (no_of_bits != 0)
        {
            if (no_of_bits >= 6)
            {
                temp = no_of_bits - 6;
                index = (val >> temp) & 63;
                no_of_bits -= 6;
            }
            else
            {
                temp = 6 - no_of_bits;
                index = (val << temp) & 63;
                no_of_bits = 0;
            }
            result[k++] = b64[index];
        }
    }
    for (i = 1; i <= padding; i++)
    {
        result[k++] = '=';
    }

    result[k] = '\0';

    return result;
}

char *base64D(char encoded[], int len_str)
{
    char *decoded_string;

    decoded_string = (char *)malloc(sizeof(char) * 1000);

    int i, j, k = 0;
    int num = 0;
    int count_bits = 0;
    for (i = 0; i < len_str; i += 4)
    {
        num = 0, count_bits = 0;
        for (j = 0; j < 4; j++)
        {
            if (encoded[i + j] != '=')
            {
                num = num << 6;
                count_bits += 6;
            }
            if (encoded[i + j] >= 'A' && encoded[i + j] <= 'Z')
                num = num | (encoded[i + j] - 'A');
            else if (encoded[i + j] >= 'a' && encoded[i + j] <= 'z')
                num = num | (encoded[i + j] - 'a' + 26);
            else if (encoded[i + j] >= '0' && encoded[i + j] <= '9')
                num = num | (encoded[i + j] - '0' + 52);
            else if (encoded[i + j] == '+')
                num = num | 62;
            else if (encoded[i + j] == '/')
                num = num | 63;
            else
            {
                num = num >> 2;
                count_bits -= 2;
            }
        }

        while (count_bits != 0)
        {
            count_bits -= 8;
            decoded_string[k++] = (num >> count_bits) & 255;
        }
    }
    decoded_string[k] = '\0';

    return decoded_string;
}

int main() 
{ 
    char input_string[] = "Rishi Ravikumar"; 
    int len_str = sizeof(input_string)-1; 
    char *e = base64E(input_string, len_str);
    printf("Encoded string : %s\n", e); 
    printf("Decoded_string : %s\n", base64D(e, len_str)); 

    return 0; 
} 