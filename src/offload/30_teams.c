#include <stdio.h>
#include <omp.h>

int main(void)
{
    #pragma omp target teams
    {
        int team_id = omp_get_team_num();
        int nteams  = omp_get_num_teams();

        int thread_id = omp_get_thread_num();

        if (thread_id == 0)
        {
            printf("Team %d of %d teams\n",
                   team_id, nteams);
        }
    }

    return 0;
}