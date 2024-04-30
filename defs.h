#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR 64
#define FEAR_RATE 1
#define MAX_FEAR 100
#define MAX_HUNTERS 4
#define USLEEP_TIME 50000
#define BOREDOM_MAX 99

// You may rename these types if you wish
typedef enum
{
    EMF,
    TEMPERATURE,
    FINGERPRINTS,
    SOUND
} EvidenceClassType;
typedef enum
{
    POLTERGEIST,
    BANSHEE,
    BULLIES,
    PHANTOM
} GhostClassType;

int randInt(int, int);         // Generates a pseudorandom integer between the parameters
float randFloat(float, float); // Generates a pseudorandom float between the parameters

// void populateRooms(BuildingType *); // Populates the building with sample data for rooms

typedef struct RoomArray
{
    struct Node *head;
    struct Node *tail;
} RoomArrayType;

typedef struct room
{
    char name[MAX_STR];              // name of room
    struct RoomArray *rooms;         // other rooms it connects too
    struct HunterArray *hunters;     // collection of hunters
    struct Ghost *ghost;             // if ghost in the room, NULL ow.
    struct EvidenceArray *evidences; // evidences in the room
    sem_t mutex;
} RoomType;

typedef struct Node
{
    RoomType *room;
    struct Node *next;
} RoomNodeType;

typedef struct Ghost
{
    GhostClassType ghostType; // type of ghost
    RoomType *room;           // which room the ghost is in
    int boredom;              // if reach to max it moves to next room
} GhostType;

typedef struct hunter
{
    int id;
    char name[MAX_STR];
    RoomType *room;                  // pointer of the room they currently in
    struct EvidenceArray *evidences; // evidences they currently holding
    int fear;
    int timer;
    EvidenceClassType tool;
} HunterType;

typedef struct HunterArray
{
    HunterType elements[MAX_HUNTERS];
    int size;
} HunterArrayType;

typedef struct
{
    EvidenceClassType category; // category of the evidence;
    float data;                 // value of the evidence
} EvidenceType;

typedef struct EvidenceArray
{
    EvidenceType *elements;
    int capacity;
    int size;
} EvidenceArrayType;

typedef struct
{
    RoomArrayType rooms;     // a collection of rooms
    GhostType ghost;         // ghost in the building
    HunterArrayType hunters; // a collection of hunters
} BuildingType;

void appendHunter(HunterArrayType *, HunterType *);
void initRoom(RoomType *, char *);
void initRoomList(RoomArrayType *);
void appendRoom(RoomArrayType *, RoomNodeType *);
void connectRooms(RoomType *, RoomType *);
int getRoomArraySize(RoomArrayType *);
void populateRooms(BuildingType *);
void initBuilding(BuildingType *);
void initHunter(char *, RoomType *, int, EvidenceClassType, HunterType *);
void initHunterArray(HunterArrayType *);
void appendHunter(HunterArrayType *, HunterType *);
void removeHunter(HunterArrayType *, HunterType *);
void moveHunter(HunterType *, RoomType *, RoomType *);
// ghost.c
void initGhost(GhostType *);
void *ghostBehaviour(void *);
void leaveEvidence(GhostClassType, EvidenceType *);
void moveGhost(GhostType *, RoomType *, RoomType *);
// evidence.c
void initEvidence(EvidenceClassType, float, EvidenceType *);
void initEvidenceArray(EvidenceArrayType *);
void copyEvidence(EvidenceType *, EvidenceType *);
void appendEvidence(EvidenceArrayType *, EvidenceType *);
void removeEvidence(EvidenceArrayType *, EvidenceType *);

void *hunterBehaviour(void *);
void cleanupBuilding(BuildingType *);
int isGhostlyEvidence(EvidenceType *);

int checkGhostlyEvidence(EvidenceArrayType *);
GhostClassType determineGhostType(EvidenceArrayType *);