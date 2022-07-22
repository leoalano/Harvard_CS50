#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}
//END INT MAIN



// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    int z, s, n;

    for (z = 0; z < candidate_count; z++)
    {
        int counterx = 0;
        for (s = 0, n = strlen(name); s < n; s++)
        {
            if (name[s] == candidates[z].name[s])
            {
                counterx = counterx + 1;
            }
        }

        if (strlen(name) == 0)
        {

        }
        else if (counterx == strlen(name))
        {
            candidates[z].votes = candidates[z].votes + 1;
            return true;
        }

    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    int x;
    int max = 0;
    int f = 0;
    for (f = 1; f < candidate_count; f++)
    {
        if (candidates[f].votes > candidates[max].votes)
        {
            max = f;
        }
    }

    for (int r = 0; r < candidate_count; r++)
    {
        if ((candidates[max].votes == candidates[r].votes) && (candidates[max].name != candidates[r].name))
        {
            printf("%s\n", candidates[r].name);
        }
    }
    printf("%s\n", candidates[max].name);


    return;
}

