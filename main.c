#include "defs.h"

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // You may change this code; this is for demonstration purposes
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);

    pthread_t ghost_thread;
    pthread_t hunters_threads[MAX_HUNTERS];

    pthread_create(&ghost_thread, NULL, ghostBehaviour, &building.ghost);
    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        pthread_create(&(hunters_threads[i]), NULL, hunterBehaviour, &building.hunters.elements[i]);
    }

    // join threads
    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        pthread_join(hunters_threads[i], NULL);
    }

    pthread_join(ghost_thread, NULL);

    // check&print result
    printf("Results: \n");
    // check all hunter's fear
    int countFearHunter = 0;
    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        if (building.hunters.elements[i].fear >= 100)
        {
            printf("    *%s has run away in fear!\n", building.hunters.elements[i].name);
            countFearHunter++;
        }
        else
        {
            printf("    *%s has run out of things to do!\n", building.hunters.elements[i].name);
        }
    }

    // check if any
    if (countFearHunter == 4)
    {
        printf("Ghost win!");
    }
    else
    {
        int hunterWin = 0;
        // check if any hunter win
        for (int i = 0; i < MAX_HUNTERS; i++)
        {
            if (determineGhostType(building.hunters.elements[i].evidences) != -1)
            {
                printf("The following hunters have won the game\n");
                hunterWin = 1;
                break;
            }
        }

        if (hunterWin == 1)
        {
            GhostClassType hunterAssumption = -1;
            for (int i = 0; i < MAX_HUNTERS; i++)
            {
                if (determineGhostType(building.hunters.elements[i].evidences) != -1)
                {
                    printf("    *%s\n", building.hunters.elements[i].name);
                    hunterAssumption = determineGhostType(building.hunters.elements[i].evidences);
                }
            }

            printf("They have found the following evidence to help win: \n");
            switch (hunterAssumption)
            {
            case POLTERGEIST:
                printf("    *EMF\n");
                printf("    *TEMPERATURE\n");
                printf("    *FINGERPRINT\n");
                printf("And with this, they've determined that the ghost is a POLTERGEIST");
                break;
            case BANSHEE:
                printf("    *EMF\n");
                printf("    *TEMPERATURE\n");
                printf("    *SOUND\n");
                printf("And with this, they've determined that the ghost is a BANSHEE");
                break;
            case BULLIES:
                printf("    *EMF\n");
                printf("    *FINGERPRINT\n");
                printf("    *SOUND\n");
                printf("And with this, they've determined that the ghost is a BULLIES");
                break;
            case PHANTOM:
                printf("    *TEMPERATURE\n");
                printf("    *FINGERPRINT\n");
                printf("    *SOUND\n");
                printf("And with this, they've determined that the ghost is a PHANTOM");
                break;
            }
        }
        else
        {
            printf("Ghost Win!\n");
        }
    }

    cleanupBuilding(&building); // clean up everything

    return 0;
}

/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number,
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1)
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number,
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1)
*/
float randFloat(float a, float b)
{
    // Get a percentage between rand() and the maximum
    float random = ((float)rand()) / (float)RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}
