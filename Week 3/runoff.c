#include <cs50.h>
#include <stdio.h>
#include <string.h>
// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];
// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;
int newCount;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    newCount = candidate_count;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();
        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    int index[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        index[i] = i;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            //printf("%s vote for can\n", name);
            preferences[voter][rank] = index[i];
            //printf("%i\n", preferences[voter][rank]);
            for (int j = 0; j < candidate_count; j++)
            {
                //printf("%i ", preferences[i][j]);
            }
            return true;
        }
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int index = 0;
    int check = 0;
    for (int i = 0; i < voter_count; i++)
    {
        index = preferences[i][check];
        if (!candidates[index].eliminated)
        {
            candidates[index].votes++;
        }
        else
        {
            while (candidates[index].eliminated)
            {
                check++;
                index = preferences[i][check];
            }
            candidates[index].votes++;
            check = 0;
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int max = -1;
    int counter = 0;
    int new_voter_count = voter_count;
    if (voter_count % 2 != 0)
    {
        new_voter_count = voter_count + 1;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > max)
        {
            max = candidates[i].votes;
            counter++;
        }
        if (candidates[i].votes == new_voter_count / 2)
        {
            return false;
        }
        if (candidates[i].votes > new_voter_count / 2)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    if (!is_tie(find_min()))
    {
        return false;
    }
    string names[counter];
    int index = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == max)
        {
            names[index] = candidates[i].name;
            index++;
        }
    }
    for (int i = 0; i < counter; i++)
    {
        printf("%s\n", names[i]);
    }
    return true;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = 101;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes < min && !candidates[i].eliminated)
        {
            min = candidates[i].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int i = 0;
    for (i = 0; i < candidate_count - 1; i++)
    {
        if (!candidates[i].eliminated)
        {
            if (candidates[i].votes != candidates[i + 1].votes && !candidates[i + 1].eliminated)
            {
                return false;
            }
            continue;
        }
    }
    if (candidates[i + 1].eliminated)
    {
        return false;
    }
    return true;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (min == candidates[i].votes)
        {
            candidates[i].eliminated = true;
            newCount--;
        }
    }
}
