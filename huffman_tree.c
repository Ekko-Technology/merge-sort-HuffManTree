#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>

//assuming that there are only 110 possible unique characters in a text file
#define textChars 110

typedef struct
{
    char letter;
    int count;
}
char_count;

int checkrepeat(char buffer, int count, char_count *character_count);
void mergesort(char_count *actual_charcount, int element_count);
void mergesort_recursion(char_count *actual_charcount, int left, int right);
void merge_sorted_arrays(char_count* actual_charcount, int left, int middle, int right);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./huffman_tree input.txt output.txt");
    }
    // open uncompressed file
    FILE *uncompressed_file = fopen(argv[1], "r");
    if (uncompressed_file == NULL)
    {
        printf("input file unable to read\n");
        return 1;
    }

    // allocating number of occurences to each character in text file
    // buffer to hold character read
    char buffer;
    int count = 0;
    // initialize global variable char_count to arbitrary value 110
    char_count character_count[textChars] = {0};
    // loop through char in text file and update char_count
    while (fread(&buffer, sizeof(char), 1, uncompressed_file) == 1)
    {
        // if letter does not exist in character_count struct, add the letter into it
        if (!checkrepeat(buffer, count, character_count))
        {
            character_count[count].letter = buffer;
            character_count[count].count++;
            count++;
        }
    }

    // Debug: printing and checking if whole list exist
    for (int j = 0; j < textChars; j++)
    {
        printf("char %i - %c: %i times\n", j, character_count[j].letter, character_count[j].count);
    }


    // identifying how many elements are used out of the 110 allocated inside character_count
    int element_count = 0;
    for (int index = 0; index < textChars; index++)
    {
        if (character_count[index].count != 0)
        {
            element_count++;
        }
    }

    printf("%i/110 unique elements in text file.\n", element_count);

    // transferring elements into another character_count list to remove empty elements
    char_count actual_charcount[element_count];
    for (int index2 = 0; index2 < element_count; index2++)
    {
        actual_charcount[index2] = character_count[index2];
    }


    // sorting character_count list in ascending order via merge sort
    mergesort(actual_charcount, element_count); // actual_charcount is a list of data type, where each data type holds a single character and a integer to indicate the number of occurences inside the uncompressed text file.element_count refers to length of actual_charcount list
    char_count *sortedCharacterCount = actual_charcount;



    // Debug: printing and checking if whole list is inputted inside and sorted properly
    for (int k = 0; k < element_count; k++)
    {
        printf("char %i - %c: %i times\n", k, sortedCharacterCount[k].letter, sortedCharacterCount[k].count);
    }

    fclose(uncompressed_file);
    return(0);
}


int checkrepeat(char buffer, int count, char_count *character_count)
{
    for (int i = 0; i < count; i++)
    {
        if (character_count[i].letter == buffer)
        {
            // update count variable in its character_count
            character_count[i].count++;
            return 1;
        }
    }

    return 0;
}

// using merge sort --- failed :(
void mergesort(char_count *actual_charcount, int element_count)
{
    // pass down starting indexs of char_count list to the recursion function
    mergesort_recursion(actual_charcount, 0, element_count - 1);
}
// takes in list, left portion and right portion
void mergesort_recursion(char_count *actual_charcount, int left, int right)
{
    // once right = left, stop recursing
    if (left < right)
    {
        // splitting list in the middle
        int middle = (left + right)/2;

        // splitting left portion of list
        mergesort_recursion(actual_charcount, left, middle);

        // splitting right portion of list
        mergesort_recursion(actual_charcount, middle + 1, right);

        // merging both side once they are seperated
        merge_sorted_arrays(actual_charcount, left, middle, right);
    }
}

void merge_sorted_arrays(char_count* actual_charcount, int left, int middle, int right)
{
    int left_length = middle - left + 1;
    int right_length = right - middle;
    // statically assigning temporary left and right char_count structs
    char_count temp_left[left_length];
    char_count temp_right[right_length];


    for (int i = 0; i < left_length; i++)
    {
        // copy values of left portion from char_count list into temp_left
        temp_left[i] = actual_charcount[left + i];
    }

    for (int i = 0; i < right_length; i++)
    {
        // copy values of right portion from char_count list into temp_right
        temp_right[i] = actual_charcount[middle + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < left_length && j < right_length && k <= right)
    {
        // initialize i to be index for left region and j for the right region
        if (temp_left[i].count < temp_right[j].count)
        {
            // if left element is greater than right element, input right element back first into original char_count list
            actual_charcount[k] = temp_left[i];
            i++;
        }
        else
        {
            // if tie or right element is greater, return right element into original char_count list
            actual_charcount[k] = temp_right[j];
            j++;
        }
        k++;

        // in the case that left_length and right_length are not equal and have missed not outstanding elements in temp_left or temp_right
        while (i < left_length)
        {
            actual_charcount[k] = temp_left[i];
            i++;
            k++;
        }

        while (j < right_length)
        {
            actual_charcount[k] = temp_right[j];
            j++;
            k++;
        }
    }
}
