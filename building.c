
#include "defs.h"

/*
    Note: While this function is provided, you will likely want
    to modify it to suit your solution. This has been provided
    to give you a starting point to lay out the room connectivity,
    but it is entirely possible that you will want to use different
    data structures to represent the rooms and their connectivity,
    or different function names.

    Not following this function's structure exactly will not result
    in a deduction of points.

    The map that this is recreating is visible in the assignment
    specification, and also available by the original creator here:
    https://steamcommunity.com/sharedfiles/filedetails/?id=2251267947

*/
void populateRooms(BuildingType *building)
{
    // First, create each room. Perhaps you want to include more data
    // in the init parameters?
    RoomType *van = calloc(1, sizeof(RoomType));
    initRoom(van, "Van");
    RoomType *hallway = calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");
    RoomType *master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");
    RoomType *boys_bedroom = calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom");
    RoomType *bathroom = calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom");
    RoomType *basement = calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement");
    RoomType *basement_hallway = calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway");
    RoomType *right_storage_room = calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room");
    RoomType *left_storage_room = calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room");
    RoomType *kitchen = calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen");
    RoomType *living_room = calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room");
    RoomType *garage = calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage");
    RoomType *utility_room = calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room");

    // Now create a linked list of rooms using RoomNodeType in the Building
    RoomNodeType *van_node = calloc(1, sizeof(RoomNodeType));
    van_node->room = van;
    RoomNodeType *hallway_node = calloc(1, sizeof(RoomNodeType));
    hallway_node->room = hallway;
    RoomNodeType *master_bedroom_node = calloc(1, sizeof(RoomNodeType));
    master_bedroom_node->room = master_bedroom;
    RoomNodeType *boys_bedroom_node = calloc(1, sizeof(RoomNodeType));
    boys_bedroom_node->room = boys_bedroom;
    RoomNodeType *bathroom_node = calloc(1, sizeof(RoomNodeType));
    bathroom_node->room = bathroom;
    RoomNodeType *basement_node = calloc(1, sizeof(RoomNodeType));
    basement_node->room = basement;
    RoomNodeType *basement_hallway_node = calloc(1, sizeof(RoomNodeType));
    basement_hallway_node->room = basement_hallway;
    RoomNodeType *right_storage_room_node = calloc(1, sizeof(RoomNodeType));
    right_storage_room_node->room = right_storage_room;
    RoomNodeType *left_storage_room_node = calloc(1, sizeof(RoomNodeType));
    left_storage_room_node->room = left_storage_room;
    RoomNodeType *kitchen_node = calloc(1, sizeof(RoomNodeType));
    kitchen_node->room = kitchen;
    RoomNodeType *living_room_node = calloc(1, sizeof(RoomNodeType));
    living_room_node->room = living_room;
    RoomNodeType *garage_node = calloc(1, sizeof(RoomNodeType));
    garage_node->room = garage;
    RoomNodeType *utility_room_node = calloc(1, sizeof(RoomNodeType));
    utility_room_node->room = utility_room;

    // Building->rooms might be a linked list structre, or maybe just a node.
    initRoomList(&building->rooms);

    appendRoom(&building->rooms, van_node);
    appendRoom(&building->rooms, hallway_node);
    appendRoom(&building->rooms, master_bedroom_node);
    appendRoom(&building->rooms, boys_bedroom_node);
    appendRoom(&building->rooms, bathroom_node);
    appendRoom(&building->rooms, basement_node);
    appendRoom(&building->rooms, basement_hallway_node);
    appendRoom(&building->rooms, right_storage_room_node);
    appendRoom(&building->rooms, left_storage_room_node);
    appendRoom(&building->rooms, kitchen_node);
    appendRoom(&building->rooms, living_room_node);
    appendRoom(&building->rooms, garage_node);
    appendRoom(&building->rooms, utility_room_node);

    // Now connect the rooms. It is possible you do not need a separate
    // function for this, but it is provided to give you a starting point.
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // chose a random room to spawn ghost
    int randomRoom = randInt(1, getRoomArraySize(&building->rooms));
    RoomNodeType *curr = building->rooms.head;
    for (int i = 0; i < randomRoom; i++)
    {
        curr = curr->next; // go next
    }

    // put ghost in room
    building->ghost.room = curr->room;
    curr->room->ghost = &building->ghost;

    HunterType h0;
    HunterType h1;
    HunterType h2;
    HunterType h3;

    char name1[MAX_STR];
    char name2[MAX_STR];
    char name3[MAX_STR];
    char name4[MAX_STR];

    printf("Enter the First Hunter Name: ");
    scanf("%s", name1);
    printf("\nEnter the Second Hunter Name: ");
    scanf("%s", name2);
    printf("\nEnter the Third Hunter Name: ");
    scanf("%s", name3);
    printf("\nEnter the Foruth Hunter Name: ");
    scanf("%s", name4);

    initHunter(name1, van, 0, EMF, &h0);
    initHunter(name2, van, 1, TEMPERATURE, &h1);
    initHunter(name3, van, 2, FINGERPRINTS, &h2);
    initHunter(name4, van, 3, SOUND, &h3);

    appendHunter(&building->hunters, &h0);
    appendHunter(&building->hunters, &h1);
    appendHunter(&building->hunters, &h2);
    appendHunter(&building->hunters, &h3);

    appendHunter(van->hunters, &h0);
    appendHunter(van->hunters, &h1);
    appendHunter(van->hunters, &h2);
    appendHunter(van->hunters, &h3);
}

void initBuilding(BuildingType *b)
{
    initRoomList(&b->rooms);
    initGhost(&b->ghost);
    initHunterArray(&b->hunters);
}

void cleanupBuilding(BuildingType *b)
{
    RoomNodeType *curr = b->rooms.head;
    RoomNodeType *next;
    while (curr != NULL)
    {
        next = curr->next;

        // free everything inside adjacent rooms list in room
        RoomNodeType *curr_room = curr->room->rooms->head;
        RoomNodeType *next_room;
        while (curr_room != NULL)
        {
            next_room = curr_room->next;
            free(curr_room);
            curr_room = next_room;
        }

        // free evidence in room
        free(curr->room->evidences->elements);
        // free evidence array in room
        free(curr->room->evidences);
        // free adjacent rooms list in room
        free(curr->room->rooms);
        // free hunter array in room
        free(curr->room->hunters);
        // free room
        free(curr->room);
        // free roomNodes in building
        free(curr);

        curr = next;
    }

    // free hunter
    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        // free evidence array's elements
        free(b->hunters.elements[i].evidences->elements);
        // free hunter's evidence array
        free(b->hunters.elements[i].evidences);
    }
    // clean up hunters' evidence array
}
