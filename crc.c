#include <stdio.h>
#include <string.h>

#define MAX 200

// Performs XOR between two binary strings.
// XOR is done from index 1 because index 0 is already handled
// by checking whether the first bit of the remainder is '1'.
void xorOp(char *a, char *b, int n)
{
    for (int i = 1; i < n; i++)
    {
        if (a[i] == b[i])
            a[i] = '0';
        else
            a[i] = '1';
    }
}

// Divides the data by the generator polynomial
// and stores the CRC remainder in remOut.
void divide(char *temp, char *gen, char *remOut)
{
    int genLen = strlen(gen);
    int tempLen = strlen(temp);

    char rem[MAX];

    // Copy the first genLen bits of the data
    // into the remainder.
    strncpy(rem, temp, genLen);
    rem[genLen] = '\0';

    // Perform binary division.
    for (int i = genLen; i <= tempLen; i++)
    {
        // If the first bit is 1, perform XOR
        // with the generator polynomial.
        if (rem[0] == '1')
            xorOp(rem, gen, genLen);

        // Shift the remainder to the left by one position.
        for (int j = 0; j < genLen - 1; j++)
            rem[j] = rem[j + 1];

        // Add the next data bit to the remainder.
        if (i < tempLen)
            rem[genLen - 1] = temp[i];
        else
            rem[genLen - 1] = '0';
    }

    // Null-terminate the remainder.
    rem[genLen - 1] = '\0';

    // Copy the final remainder to remOut.
    strcpy(remOut, rem);
}

// Converts a generator polynomial such as x^3+x+1
// into its binary representation such as 1011.
void polynomialToBinary(char *poly, char *binary)
{
    int degree = 0;

    // Find the highest degree from the polynomial.
    sscanf(poly, "x^%d", &degree);

    // Initially fill the binary array with zeros.
    for (int i = 0; i <= degree; i++)
        binary[i] = '0';

    binary[degree + 1] = '\0';

    char *p = poly;

    while (*p)
    {
        // Check for x^term.
        if (*p == 'x')
        {
            int power;

            // Read the power after x^.
            if (*(p + 1) == '^')
                sscanf(p + 2, "%d", &power);
            else
                power = 1;

            binary[degree - power] = '1';
        }

        // Handle the constant term 1.
        else if (*p == '1' &&
                 (p == poly || *(p - 1) == '+' || *(p - 1) == ' '))
        {
            binary[degree] = '1';
        }

        p++;
    }
}

int main()
{
    char data[MAX], gen[MAX];
    char genPoly[MAX];
    char codeword[MAX], rem[MAX];
    char temp[MAX];

    // Read the original data bits.
    printf("Enter data bits: ");
    scanf("%s", data);

    // Read the generator polynomial.
    printf("Enter generator polynomial: ");
    scanf("%s", genPoly);

    // Convert the polynomial into binary form.
    polynomialToBinary(genPoly, gen);

    printf("Generator in binary form: %s\n", gen);

    // Find the length of the generator.
    int genLen = strlen(gen);

    // Copy the original data into temp.
    strcpy(temp, data);

    // Append genLen - 1 zeros to the data.
    // These zeros are required before CRC division.
    for (int i = 0; i < genLen - 1; i++)
        strcat(temp, "0");

    // Divide the modified data by the generator
    // to obtain the CRC remainder.
    divide(temp, gen, rem);

    // Create the transmitted codeword:
    // original data + CRC remainder.
    strcpy(codeword, data);
    strcat(codeword, rem);

    printf("\nTransmitted codeword: %s\n", codeword);

    // Read the codeword received at the receiver.
    char check[MAX], rem2[MAX];

    printf("\nEnter received codeword: ");
    scanf("%s", check);

    // Divide the received codeword by the generator.
    // If the remainder contains 1, an error occurred.
    divide(check, gen, rem2);

    int error = 0;

    // Check whether the CRC remainder contains any 1.
    for (int i = 0; i < genLen - 1; i++)
    {
        if (rem2[i] == '1')
        {
            error = 1;
            break;
        }
    }

    // Display the final result.
    if (error)
        printf("Result: Error detected in received data!\n");
    else
        printf("Result: No error - Data accepted.\n");

    return 0;
}
