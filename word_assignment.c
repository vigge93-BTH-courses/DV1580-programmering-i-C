#include <stdio.h>
#include <string.h>

int main()
{
    char input_string[] = "KallePelleAnna";
    char new_string[256];
    char character = 'l';

    // Remove occurrences
    int k = 0;
    for (int j = 0; j < strlen(input_string); j++)
    {
        if (input_string[j] != character)
        {
            new_string[k++] = input_string[j];
        }
    }
    new_string[k] = 0;
    printf("%s\n", new_string);

    // Count number of occurrences
    int number_of_occurrences = 0;


    // Your code goes here...
    number_of_occurrences = strlen(input_string) - strlen(new_string);
    
    printf("%d\n", number_of_occurrences);

    return 0;
}