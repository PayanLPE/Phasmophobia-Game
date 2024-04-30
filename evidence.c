#include "defs.h"

/*
    Function: initEvidence
    Purpose:  initiliaze every field of evidence
        in:   evType, value, *ev
        out:  *ev
*/
void initEvidence(EvidenceClassType evType, float value, EvidenceType *ev)
{
    ev->category = evType;
    ev->data = value;
}

/*
    Function: initEvidenceArray
    Purpose:  initiliaze every field of evidence array
        in:   *arr
        out:  *arr
*/
void initEvidenceArray(EvidenceArrayType *arr)
{
    arr->size = 0;
    arr->capacity = 256;
    arr->elements = (EvidenceType *)calloc(arr->capacity, sizeof(EvidenceType));
}

/*
    Function: copyEvidence
    Purpose:  took in an old evidence variable and assign every filed to new evidence
        in:   *newEv, *oldEv
        out:  *newEv
*/
void copyEvidence(EvidenceType *newEv, EvidenceType *oldEv)
{
    initEvidence(oldEv->category, oldEv->data, newEv); // deep copy old EvidenceType to new EvidenceType
}

/*
    Function: appendEvidence
    Purpose:  took in an evidence array and append new evidence to it
        in:   *arr, *ev
        out:  *arr
*/
void appendEvidence(EvidenceArrayType *arr, EvidenceType *ev)
{
    if (arr->size < 256)
    {
        copyEvidence(&(arr->elements[arr->size]), ev);
        arr->size++;
    }
}

/*
    Function: removeEvidence
    Purpose:  took in an evidence array and remove evidence
        in:   *arr, *ev
        out:  *arr
*/
void removeEvidence(EvidenceArrayType *arr, EvidenceType *ev)
{
    // find the index of evidence
    int index = -1;
    for (int i = 0; i < arr->size; ++i)
    {
        if (arr->elements[i].data == ev->data && arr->elements[i].category == ev->category)
        {
            index = i;
            break;
        }
    }

    // move all the elements to one place froward
    for (int i = index; i < arr->size - 1; ++i)
    {
        copyEvidence(&arr->elements[i], &arr->elements[i + 1]);
    }

    arr->size--; // decrease size of arr
}

/*
    Function: isGhostlyEvidence
    Purpose:  took in an evidence and check if it's a ghostly evidence
        in:   *ev
        return: 1 if it's ghostly evidence; 0 otherwise
*/
int isGhostlyEvidence(EvidenceType *ev)
{
    switch (ev->category)
    {
    case EMF:
        if (ev->data > 4.9f)
        {
            return 1;
        }
        break;
    case TEMPERATURE:
        if (ev->data < 0.0f)
        {
            return 1;
        }
        break;
    case FINGERPRINTS:
        if (ev->data == 1.0f)
        {
            return 1;
        }
        break;
    case SOUND:
        if (ev->data > 70.0f)
        {
            return 1;
        }
        break;
    }
    return 0;
}
