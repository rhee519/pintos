/**
 * [PROJECT-1] Jiho Rhee
 * additional.c
 *
 * execute 2 additional system calls, fibonacci() and max_of_four_int().
 */

#include <stdio.h>
#include <syscall.h>
#include <limits.h> /* INT_MIN, INT_MAX */

int str_to_long(char *str);

int str_to_long(char *str)
{
    int read = 0;
    int sign = 1;
    int s_idx = 0;
    int noting_to_convert = 1;

    if (str[0] == '-')
    {
        sign = -1;
        s_idx++;
    }

    for (int i = s_idx; str[i] != '\0'; i++)
    {
        if (str[i] < '0' || '9' < str[i])
        {
            printf("format error: '%c' in '%s' is not a digit.\n", str[i], str);
            exit(EXIT_FAILURE);
        }

        noting_to_convert = 0;
        read *= 10;
        read += sign * (str[i] - '0');
        if (read < INT_MIN || INT_MAX < read)
        {
            printf("Out-of-range: Input '%s' is out of range(INT).\n", str);
            exit(EXIT_FAILURE);
        }
    }

    if (noting_to_convert)
    {
        printf("format error: nothing to convert from '%s'.\n", str);
    }

    return read;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: ./additional <num1> <num2> <num3> <num4>\n");
        return EXIT_FAILURE;
    }

    int num[4];
    int fibo_num = 0, max = INT_MIN;

    /* Read 4 integers.*/
    for (int i = 0; i < 4; i++)
        num[i] = str_to_long(argv[i + 1]);

    /* Compute fibonacci number. */
    fibo_num = fibonacci(num[0]);

    /* Get maximum integer. */
    max = max_of_four_int(num[0], num[1], num[2], num[3]);

    /* Print result. */
    printf("%d %d\n", fibo_num, max);

    return EXIT_SUCCESS;
}
