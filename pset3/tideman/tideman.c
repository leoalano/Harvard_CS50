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
        if (strcmp(candidates[z], name) == 0)
        {
            for (int d = rank; d < candidate_count; d++)
            {
                ranks[d] = z;
            }
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
        {
            preferences[ranks[i]][ranks[z]] = preferences[ranks[i]][ranks[z]] + 1;
        }
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

        if (winnermax != y) //swap
        {
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
// auxiliary function to lock_pairs
bool cycle(int end, int start)
{
    if (end == start) //base case, if end equals to start it means it HAS a cycle.
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        // loop through different locked pairs (different starts). At the start all locked pairs are false so no if triggered, then
        // function returns false. If locked pair has a winner that is the end of the chain (loser) then if is triggered as maybe there
        //will be a cycle.
        if (locked[end][i])
        {
            // loop through different locked pairs (different ends). the function is called to find out if the loser of the locked pair
            // is at the start. If so we have a cycle.
            if (cycle(i, start))
            {
                return true; // true (has cycle)
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    //if locking generates no candidate that is only winner do not lock.
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}



// Print the winner of the election
void print_winner(void)
{
    // TODO
    // looping through locked pairs
    for (int i = 0; i < pair_count; i++)
    {
        int counter = 0;
        for (int x = 0; x < candidate_count; x++)
        {
            if (!locked[x][i])
            {
                //count number of false locked pairs for each winner
                counter = counter + 1;
                //if number of false locked pairs for the winner is equal to all candidates, that means we got our guy
                if (counter == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}
