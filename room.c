#include "defs.h"

/*
    Function: initRoom
    Purpose:  initialize every field of room
        in:   *room, *name
        out:  *room
*/
void initRoom(RoomType *room, char *name) {
    strcpy(room->name, name);
    room->ghost = NULL;
    room->rooms = calloc(1, sizeof(RoomArrayType));
    room->hunters = calloc(1, sizeof(HunterArrayType));
    room->evidences = calloc(1, sizeof(EvidenceArrayType));
    initRoomList(room->rooms);
    initHunterArray(room->hunters);
    initEvidenceArray(room->evidences);

    // initiliaze semaphore
    if(sem_init(&room->mutex, 0, 1) < 0) {
        exit(1);
    }
}

/*
    Function: initRoomList
    Purpose:  initialize head and tail of roomArray
        in:   *roomArray
        out:  *roomArray
*/
void initRoomList(RoomArrayType *roomArray) {
    roomArray->head = NULL;
    roomArray->tail = NULL; 
}

/*
    Function: appendRoom
    Purpose:  append room node to room array
        in:   *room, *roomNode
        out:  *room
*/
void appendRoom(RoomArrayType *room, RoomNodeType *roomNode) {
    // add to linkedlist
    if(room->head == NULL && room->tail == NULL) {
        roomNode->next = NULL;
        room->head = roomNode;
        room->tail = roomNode;
    } else {
        RoomNodeType* curr = room->head;
        while(curr->next != NULL) {
            curr = curr->next;
        } 
        roomNode->next = NULL;
        curr->next = roomNode;
        room->tail = roomNode;
    }
}

/*
    Function: connectRooms
    Purpose:  connects the 2 inputted rooms together
        in:   *r1, *r2
        out:  *r1, *r2
*/
void connectRooms(RoomType* r1, RoomType* r2) {
    // add one to the other
    RoomNodeType* r1_node = calloc(1, sizeof(RoomNodeType));
    RoomNodeType* r2_node = calloc(1, sizeof(RoomNodeType));
    r1_node->room = r1;
    r2_node->room = r2;
    appendRoom(r1->rooms, r2_node);    
    appendRoom(r2->rooms, r1_node);
}

/*
    Function: getRoomArraySize
    Purpose: returns size of the room array
        return value: size
        in:   *room
*/
int getRoomArraySize(RoomArrayType *room) {
    int result = 0;
    RoomNodeType* curr = room->head;
    if (curr == NULL) {
        return result;
    }
    result++;
    while(curr->next != NULL) {
        curr = curr->next;
        result++;
    }
    return result;
}