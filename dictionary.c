// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table. N chosen based on experimentation & speed comparison
const unsigned int N = 10000;

// Initialize variables
int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Set cursor to head of linked list
    node *cursor = table[hash(word)];

    // Continue moving cursor as long as it is not NULL
    while (cursor != NULL)
    {
        // Check if word that cursor is pointing to is the desired word
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        // Point cursor to next node
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Initialize variables
    long total = 0;

    // Cycle through each letter in word
    for (int i = 0; i < strlen(word); i++)
    {
        // Convert letter to lowercase and add ASCII value to total
        total += tolower(word[i]);
    }

    // Return value within range of bucket count
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary and read into file
    FILE *input = fopen(dictionary, "r");

    // Check if input file is valid
    if (input == NULL)
    {
        return false;
    }

    // Create buffer array where word will be read into
    char buffer[LENGTH + 1];

    // Continue scanning words until end of file is reached
    while (fscanf(input, "%s", buffer) != EOF)
    {
        // Allocate memory for new node to store word
        node *new_node = malloc(sizeof(node));

        // Check if there is sufficient memory
        if (new_node == NULL)
        {
            // If insufficient memory, close file and exit
            fclose(input);
            return false;
        }

        else
        {
            // Set new node's next pointer to current head of linked list
            new_node->next = table[hash(buffer)];

            // Point head of linked list to new node
            table[hash(buffer)] = new_node;

            // Copy word in buffer to new node
            strcpy(new_node->word, buffer);
        }

        // Increment word count
        word_count++;
    }

    // Close input file
    fclose(input);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Return word count if words were loaded into dictionary
    if (word_count > 0)
    {
        return word_count;
    }

    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Cycle through each bucket in hash table
    for (int i = 0; i < N; i++)
    {
        // Direct pointer to current head of linked list
        node *pointer = table[i];

        // Continue freeing memory as long as pointer is not NULL
        while (pointer != NULL)
        {
            // Initialize temporary node and set equal to pointer
            node *tmp = pointer;

            // Move pointer to next node
            pointer = pointer->next;

            // Free temporary node
            free(tmp);
        }
    }

    return true;
}
