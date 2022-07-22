#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}



//END OF MAIN


// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    int z, s, n;

    for (z = 0; z < candidate_count; z++)
    {
        if(strcmp(candidates[z], name) == 0)
        {
            for (int d = rank; d < candidate_count; d++)
            ranks[d] = z;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int z = i + 1; z < candidate_count; z++)
        preferences[ranks[i]][ranks[z]] = preferences[ranks[i]][ranks[z]] + 1;
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO

    int x = 0;

    for (int e = 0; e < candidate_count; e++)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            if (preferences[e][i] > preferences[i][e])
            {
                pairs[x].winner = e;
                pairs[x].loser = i;
                printf("Este é o pairs[%i].winner = %i\n", x, pairs[x].winner);
                x = x + 1;
                pair_count = pair_count + 1;

            }
        }
    }


    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    //sort the preference arrays than assign to pairs array
    //new solution
    
    int newarray[pair_count];
    int v = 0;


        for (int e = 0; e < candidate_count; e++)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (preferences[e][i] > preferences[i][e])
                {
                    printf("este é o preferences[%i][%i] %i\n", e, i, preferences[e][i]);
                    newarray[v] = preferences[e][i];
                    v = v + 1;
                }
            }
        }

    printf("newarray[0] = %i\n", newarray[0]);
    printf("newarray[1] = %i\n", newarray[1]);
    printf("newarray[2] = %i\n", newarray[2]);
    
    int y = 0;
    int troca;
    int winnermax = 0;

    for (y = 0; y < pair_count - 1; y++)
    {
        winnermax = y;

        for (int f = y + 1; f < pair_count; f++)
        {
            if (newarray[f] > newarray[winnermax])
            {
                winnermax = f;
            }
        }

        if (winnermax != y)
        {   //swap
            int temp = newarray[y];
            newarray[y] = newarray[winnermax];
            newarray[winnermax] = temp;
        }
    }
    printf("newarray[0] = %i\n", newarray[0]);
    printf("newarray[1] = %i\n", newarray[1]);
    printf("newarray[2] = %i\n", newarray[2]);
    
    printf("o pairs[%i].winner é %i\n", 0, pairs[0].winner);
    printf("o pairs[%i].winner é %i\n", 1, pairs[1].winner);
    printf("o pairs[%i].winner é %i\n", 2, pairs[2].winner);
    
    printf("o pairs[%i].loser é %i\n", 0, pairs[0].loser);
    printf("o pairs[%i].loser é %i\n", 1, pairs[1].loser);
    printf("o pairs[%i].loser é %i\n", 2, pairs[2].loser);
    
    int x = 0;

    for (x = 0; x < candidate_count; x++)
    {
        for (int e = 0; e < candidate_count; e++)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (newarray[x] == preferences[e][i])//descobrir winner/loser do array
                {
                    for (int w = 0; w < pair_count; w++)
                    {
                        if ((e == pairs[w].winner) || (i == pairs[w].loser))//descobrir qual o pair relacionado ao array
                        {

                            pairs[x] = pairs[w];
                            
                            printf("após o pairs[%i].winner é %i\n", 0, pairs[0].winner);
                            printf("após o pairs[%i].winner é %i\n", 1, pairs[1].winner);
                            printf("após o pairs[%i].winner é %i\n", 2, pairs[2].winner);
    
                            printf("após o pairs[%i].loser é %i\n", 0, pairs[0].loser);
                            printf("após o pairs[%i].loser é %i\n", 1, pairs[1].loser);
                            printf("após o pairs[%i].loser é %i\n", 2, pairs[2].loser);
                        }
                    }    
                }
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    
    return;
}

