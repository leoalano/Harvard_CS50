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
                    newarray[v] = preferences[e][i];
                    v = v + 1;
                }
            }
        }

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
                        if ((e == pairs[w].winner) && (i == pairs[w].loser))//descobrir qual o pair relacionado ao array
                        {

                            pair temp = pairs[x];
                            pairs[x] = pairs[w];
                            pairs[w] = temp;
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
    printf("pairs[0].winner é %i\n", pairs[0].winner);
    printf("pairs[1].winner é %i\n", pairs[1].winner);
    printf("pairs[2].winner é %i\n", pairs[2].winner);
    printf("pairs[0].loser é %i\n", pairs[0].loser);
    printf("pairs[1].loser é %i\n", pairs[1].loser);
    printf("pairs[2].loser é %i\n", pairs[2].loser);
    // TODO
    //if locking generates no candidate that is only winner do not lock.

    int nocycle = 0;

    for (int x = 0; x < pair_count; x++)
    {
        int counter = 0;
        for (int k = 0; k < pair_count; k++)
        {
           if (pairs[x].winner != pairs[k].loser)
           {
                counter = counter + 1;
           }
        }
        if (counter == pair_count)
        {
            nocycle = nocycle + 1;
        }
    }

    for (int t = 0; t < pair_count - 1; t++)
        {
            locked[pairs[t].winner][pairs[t].loser] = true;
        }

    if (nocycle != 0)
    {
        locked[pairs[pair_count - 1].winner][pairs[pair_count - 1].loser] = true;
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO


    return;
}

