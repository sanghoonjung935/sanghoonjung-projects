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
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

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
    // Going through each candidate to see if there is a match between name input and one of the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // Calling strcmp() to compare name input and current candidate name in the loop
        if (strcmp(candidates[i].name, name) == 0)
        {
            // Updating preferences[][] to store what each voter voted for and the rank; candidate represented as an index i
            preferences[voter][rank] = i;
            // If there is a match, return true and break out of loop
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // For loop to go through each voter and count their ballot
    for (int i = 0; i < voter_count; i++)
    {
        // Go through each candidate in case that all but one are eliminated
        for (int j = 0; j < candidate_count; j++)
        {
            // Since preferences[i][j] is an index, and priority starts from 0, we use preferences[i][j] to check i voter's choices
            // in their ranking
            // Their highest non-eliminated candidate is given a vote
            if (candidates[preferences[i][j]].eliminated == false)
            {
                // If candidate exists and is valid, they are given a vote, and if statement is broken
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Go through all the candidates to see if any of them have the majority vote
    for (int i = 0; i < candidate_count; i++)
    {
        // Majority vote is anything more than 50%, voter_count / 2
        if (candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Initializing variable to be used
    int minimum_vote = 101;
    // Going through all the candidates to check who has the least number of votes out of the candidates still in
    for (int i = 0; i < candidate_count; i++)
    {
        // Filtering for non-eliminated candidates
        if (candidates[i].eliminated == false)
        {
            // This condition will always be true for the i = 0 because maximum # of voters is 100, and I initialized minimum_votes
            // as 101
            if (candidates[i].votes < minimum_vote)
            {
                minimum_vote = candidates[i].votes;
            }
        }
    }
    return minimum_vote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Going through every candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Weeding out eliminated candidates
        if (candidates[i].eliminated == false)
        {
            // If there is not a tie, there will be a candidate with more than the minimum number of votes
            if (candidates[i].votes > min)
            {
                return false;
            }
        }
    }
    // If there is not a candidate with more than the number of minimum votes, that means everyone has the same number of minimum
    // votes, and there is a tie
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // Going through each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // Weeding out eliminated candidates
        if (candidates[i].eliminated == false)
        {
            // If a non-eliminated candidate has the minimum number of votes, they should be eliminated
            if (candidates[i].votes == min)
            {
                // Eliminate them by changing their eliminated status to true
                candidates[i].eliminated = true;
            }
        }
    }
    return;
}