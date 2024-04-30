#include "defs.h"

/*
    Function: initHunter
    Purpose:  initialiaze every field of hunter
        in:   *hunter, *name
        out:  *hunter
*/
void initHunter(char *name, RoomType *currRoom, int id, EvidenceClassType tool, HunterType *hunter)
{
    // initialize every field of hunter
    strcpy(hunter->name, name);
    hunter->room = currRoom;
    hunter->fear = 0;
    hunter->id = id;
    hunter->timer = BOREDOM_MAX;
    hunter->evidences = calloc(1, sizeof(EvidenceArrayType));
    initEvidenceArray(hunter->evidences);
    hunter->tool = tool; // assign anevidence equipment tool
}

/*
    Function: initHunterArrays
    Purpose:  initialiaze every field of hunter array
        in:   *arr
        out:  *arr
*/
void initHunterArray(HunterArrayType *arr)
{
    arr->size = 0;
}

/*
    Function: appendHunter
    Purpose:  took in an hunter array and append new hunter to it
        in:   *arr, *hunter
        out:  *arr
*/
void appendHunter(HunterArrayType *arr, HunterType *hunter)
{
    arr->elements[arr->size] = *hunter;
    arr->size++;
}

/*
    Function: removeHunter
    Purpose:  took in an hunter array and remove hunter
        in:   *arr, *hunter
        out:  *arr
*/
void removeHunter(HunterArrayType *arr, HunterType *hunter)
{
    // find hunter pointer in the array
    for (int i = 0; i < arr->size; i++)
    {
        if (arr->elements[i].id == hunter->id)
        {
            // delete hunter from the array
            for (int j = i; j < arr->size - 1; j++)
            {
                arr->elements[j] = arr->elements[j + 1];
            }
            arr->size--;
            break;
        }
    }
}

void *hunterBehaviour(void *hunterArg)
{
    HunterType *hunter = (HunterType *)hunterArg;
    while (1)
    {
        hunter->timer--; // decrease its timer

        // check if there is a ghost in the room
        if (hunter->room->ghost != NULL)
        {
            // if there is a ghost in the room feat will increase by 1
            hunter->fear += FEAR_RATE;
            hunter->timer = BOREDOM_MAX;
        }

        // check if hunter will exit
        if (hunter->timer < 0 || hunter->fear >= MAX_FEAR || determineGhostType(hunter->evidences) != -1)
        {
            // remove hunter from room
            removeHunter(hunter->room->hunters, hunter);
            break;
        }

        // action: move, read room evidence, communicates evidence
        switch (randInt(0, 3))
        {
        case 0:
            // move room

            RoomNodeType *curr = hunter->room->rooms->head;
            int randomRoom = randInt(0, getRoomArraySize(hunter->room->rooms));
            for (int i = 0; i < randomRoom; i++)
            {
                curr = curr->next; // go next
            }

            RoomType *oldRoom = hunter->room;
            sem_wait(&hunter->room->mutex); // lock the old room
            sem_wait(&curr->room->mutex);   // lock the new room

            moveHunter(hunter, hunter->room, curr->room); // move to the new room

            sem_post(&oldRoom->mutex);      // unlock the old room
            sem_post(&hunter->room->mutex); // unlock the new room

            printf("%s moved to %s\n", hunter->name, hunter->room->name); // test output
            break;
        case 1:
            // LOCK the room
            sem_wait(&hunter->room->mutex);

            // read room evidence
            int addEvidence = 0;
            // iterate through all evidence
            for (int i = 0; i < hunter->room->evidences->size; i++)
            {
                // append ghostly evidence to hunter and remove in room
                if (hunter->tool == hunter->room->evidences->elements[i].category)
                {
                    hunter->timer = BOREDOM_MAX;
                    printf("%s is reading GHOSTLY evidence in %s\n", hunter->name, hunter->room->name);
                    // printEvidence(&hunter->room->evidences->elements[i]);

                    appendEvidence(hunter->evidences, &hunter->room->evidences->elements[i]);
                    removeEvidence(hunter->room->evidences, &hunter->room->evidences->elements[i]);
                    addEvidence = 1;
                    break;
                }
            }

            // there is no evidence in room
            if (addEvidence == 0)
            {
                // generate random normal data
                EvidenceType ev;
                switch (hunter->tool)
                {
                case EMF:
                    initEvidence(EMF, randFloat(0.0, 4.9), &ev);
                case TEMPERATURE:
                    initEvidence(TEMPERATURE, randFloat(0.0, 27.0), &ev);
                case FINGERPRINTS:
                    initEvidence(FINGERPRINTS, 0.0, &ev);
                case SOUND:
                    initEvidence(SOUND, randFloat(40.0, 70.0), &ev);
                }

                printf("%s is reading NORMAL evidence in %s\n", hunter->name, hunter->room->name);
                appendEvidence(hunter->evidences, &ev);
            }

            // unlock room
            sem_post(&hunter->room->mutex);
            break;

        case 2:
            // communicate evidence
            int random = randInt(0, hunter->room->hunters->size);

            if (hunter->room->hunters->elements[random].id != hunter->id)
            {
                printf("%s is communicating in %s with %s\n", hunter->name, hunter->room->name, hunter->room->hunters->elements[random].name);
                // iterate through this hunter's evidence array
                for (int i = 0; i < hunter->evidences->size; i++)
                {
                    // check if the value is ghostly data, if so, add to another hunter's evidence array
                    if (isGhostlyEvidence(&hunter->evidences->elements[i]))
                    {
                        appendEvidence(hunter->room->hunters->elements[random].evidences, &hunter->evidences->elements[i]);
                    }
                }
            }
            break;
        }
        usleep(USLEEP_TIME);
    }
}

/*
    Function: moveHunter
    Purpose:  took in an hunter type and move hunter to new room
        in:   *hunter, *curRoom, *newRoom
        out:  *hunter
*/
void moveHunter(HunterType *hunter, RoomType *curRoom, RoomType *newRoom)
{
    // move hunter pointer
    removeHunter(curRoom->hunters, hunter);
    appendHunter(newRoom->hunters, hunter);

    // move hunter
    hunter->room = newRoom;
}

GhostClassType determineGhostType(EvidenceArrayType *arr)
{
    int emfChk = 0;
    int tempChk = 0;
    int fingerChk = 0;
    int soundChk = 0;

    for (int i = 0; i < arr->size; i++)
    {

        switch (arr->elements[i].category)
        {
        case EMF:
            if (arr->elements[i].data > 4.9)
            {
                emfChk = 1;
            }
            break;
        case TEMPERATURE:
            if (arr->elements[i].data < 0.0)
            {
                tempChk = 1;
            }
            break;
        case FINGERPRINTS:
            if (arr->elements[i].data == 1.0)
            {
                fingerChk = 1;
            }
            break;
        case SOUND:
            if (arr->elements[i].data > 70.0)
            {
                soundChk = 1;
            }
            break;
        }
    }

    if (emfChk && tempChk && fingerChk)
    {
        // poltergeist
        return POLTERGEIST;
    }
    else if (emfChk && tempChk && soundChk)
    {
        // banshee
        return BANSHEE;
    }
    else if (emfChk && fingerChk && soundChk)
    {
        // bullies
        return BULLIES;
    }
    else if (tempChk && fingerChk && soundChk)
    {
        // phantom
        return PHANTOM;
    }
    return -1;
}
