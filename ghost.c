#include "defs.h"

/*
    Function: initGhost
    Purpose:  initiliaze every field of ghost
        in:   *ghost
        out:  *ghost
*/
void initGhost(GhostType *ghost)
{
    // randomly choose a ghostType and initliaze it
    switch (randInt(0, 4))
    {
    case 0:
        ghost->ghostType = POLTERGEIST;
        break;
    case 1:
        ghost->ghostType = BANSHEE;
        break;
    case 2:
        ghost->ghostType = BULLIES;
        break;
    case 3:
        ghost->ghostType = PHANTOM;
        break;
    }

    // set ghost boredom to max
    ghost->boredom = BOREDOM_MAX;
}

void *ghostBehaviour(void *ghostArg)
{
    GhostType *ghost = (GhostType *)ghostArg;
    while (1)
    {
        if (ghost->boredom <= 0)
        {
            ghost->room->ghost = NULL;
            break;
        }

        // check if there is hunter in the room
        if (ghost->room->hunters->size == 0)
        {
            // if there is no hunters in the room
            ghost->boredom--; // decrease the boredeom by 1

            // actions: generate evidence, move, no action
            switch (randInt(0, 3))
            {
            case 0:
                // lock the room
                sem_wait(&ghost->room->mutex);

                // leave evidence
                EvidenceType ev;

                // randomely generate an evidence type
                switch (randInt(0, 4))
                {
                case 0:
                    if (ghost->ghostType != PHANTOM)
                    {
                        initEvidence(EMF, randFloat(4.9, 5.0), &ev); // if not PHANTOM then leave ghostly results
                        appendEvidence(ghost->room->evidences, &ev); // add to room's evidence collection
                        printf("Ghost leaves a GHOSTLY evidence in %s\n", ghost->room->name);
                    }
                    break;
                case 1:
                    if (ghost->ghostType != BULLIES)
                    {
                        initEvidence(TEMPERATURE, randFloat(-10.0, 0), &ev); // if not BULLIES then leave ghostly results
                        appendEvidence(ghost->room->evidences, &ev);         // add to room's evidence collection
                        printf("Ghost leaves a GHOSTLY evidence in %s\n", ghost->room->name);
                    }
                    break;
                case 2:
                    if (ghost->ghostType != BANSHEE)
                    {
                        initEvidence(FINGERPRINTS, 1.0, &ev);        // if not BANSHEE leave ghostly results
                        appendEvidence(ghost->room->evidences, &ev); // add to room's evidence collection
                        printf("Ghost leaves a GHOSTLY evidence in %s\n", ghost->room->name);
                    }
                    break;
                case 3:
                    if (ghost->ghostType != POLTERGEIST)
                    {
                        initEvidence(SOUND, randFloat(70.0, 75.0), &ev); // if not POLTERGEIST leave ghostly results
                        appendEvidence(ghost->room->evidences, &ev);     // add to room's evidence collection
                        printf("Ghost leaves a GHOSTLY evidence in %s\n", ghost->room->name);
                    }
                    break;
                }

                // unlock the room
                sem_post(&ghost->room->mutex);
                break;
            case 1:
                // move room

                // generate a new room to go
                RoomNodeType *curr = ghost->room->rooms->head;
                int randomRoom = randInt(0, getRoomArraySize(ghost->room->rooms));
                for (int i = 0; i < randomRoom; i++)
                {
                    curr = curr->next; // go next
                }

                RoomType *oldRoom = ghost->room;
                sem_wait(&ghost->room->mutex); // lock the old room
                sem_wait(&curr->room->mutex);  // lock the new room

                moveGhost(ghost, ghost->room, curr->room); // move to the new room

                sem_post(&oldRoom->mutex);     // unlock the old room
                sem_post(&ghost->room->mutex); // unlock the new room

                // test output
                printf("Ghost moved to %s\n", ghost->room->name);
                break;
            case 2:
                // no actions
                // test output
                printf("Ghost stayed at %s\n", ghost->room->name);
                break;
            }
        }
        else
        {
            // if they stay in a room with hunter
            ghost->boredom = BOREDOM_MAX; // set boredom to max
        }
        usleep(USLEEP_TIME);
    }
}

/*
    Function: moveHunter
    Purpose:  took in an ghost type and move ghosts to new room
        in:   *ghost, *curRoom, *newRoom
        out:  *ghost
*/
void moveGhost(GhostType *ghost, RoomType *curRoom, RoomType *newRoom)
{
    // move ghost pointer
    curRoom->ghost = NULL;
    newRoom->ghost = ghost;

    // move ghost
    ghost->room = newRoom;
}
