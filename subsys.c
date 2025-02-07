#include "subsystem.h"
#include <string.h>

/*
    This file should contain the implementations for Subsystem specific functionality.
    Initialization, printing, setting and getting data and statuses.
*/

int subsys_init(Subsystem *subsystem, const char *name, char status) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    subsystem->status = status;

    // copy onlu up til the second last character to leave room for the zero null terminator
    strncpy(subsystem->name, name, MAX_STR -1);
    // fill it with the null terminator
    subsystem->name[MAX_STR - 1] = '\0';

    return ERR_SUCCESS;
}

int subsys_print(Subsystem *subsystem) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // extract status bits
    int pwr = (subsystem->status >> STATUS_POWER) & 1;
    int data = (subsystem->status >> STATUS_DATA) & 1;
    int act = (subsystem->status >> STATUS_ACTIVITY) & 1;
    int err = (subsystem->status >> STATUS_ERROR) & 1;
    int perf = (subsystem->status >> 2) & 0x03;  // Performance uses 2 bits
    int res = subsystem->status & 0x03;  // Resource uses 2 bits

    // Print the subsystem details in the required format
    printf("\n[Name: %-16s, Status: ", subsystem->name);
    subsys_status_print(subsystem); // call the status print function

    // if there are values at data
    if (data) {
        printf(", Data: 0x%08X", subsystem->data);
        subsystem->data = 0;
        subsystem->status &= ~(1 << STATUS_DATA);
    }

    printf("]");
    return ERR_SUCCESS;
}

int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    if (value > 3) {
        return ERR_INVALID_STATUS;
    }

    // check which status wants to be modified, then set the byte
    switch(status) {
        case STATUS_POWER: // bit 7
            if (value > 1) return ERR_INVALID_STATUS; // can only turn it on or off (1 or 0)
            subsystem->status &= ~(1 << STATUS_POWER); // clear the bit first at the bit
            subsystem->status |= (value << status); // set the status power bit to the value
            break;
        case STATUS_DATA: //bit 6
            if (value > 1) return ERR_INVALID_STATUS; // can only have no data or queue data (1 or 0)
            subsystem->status &= ~(1 << STATUS_DATA);
            subsystem->status |= (value << status);
            break;
        case STATUS_ACTIVITY: // bit 5
            if (value > 1) return ERR_INVALID_STATUS; // can only turn on and off (1 or 0)
            subsystem->status &= ~(1 << STATUS_ACTIVITY);
            subsystem->status |= (value << status);
            break;
        case STATUS_ERROR: // bit 4
            if (value > 1) return ERR_INVALID_STATUS; // can only be error or no error (1 or 0)
            subsystem->status &= ~(1 << STATUS_ERROR);
            subsystem->status |= (value << status);
            break;
        case STATUS_PERFORMANCE: // bit 3, 2
            if (value > 3) return ERR_INVALID_STATUS; // can only modify the two bit either none, low or optimal (00, 10, 11)
            subsystem->status &= ~(0x03 << STATUS_PERFORMANCE); 
            subsystem->status |= (value << STATUS_PERFORMANCE);
            break;
        case STATUS_RESOURCE: // bit 1, 0
            if (value > 3) return ERR_INVALID_STATUS;
            subsystem->status &= ~(0x03 << STATUS_PERFORMANCE);
            subsystem->status |= (value << STATUS_RESOURCE);
            break;
    }

    return ERR_SUCCESS;
}

int subsys_status_print(const Subsystem *subsystem) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    int pwr = (subsystem->status >> STATUS_POWER) & 1;
    int data = (subsystem->status >> STATUS_DATA) & 1;
    int act = (subsystem->status >> STATUS_ACTIVITY) & 1;
    int err = (subsystem->status >> STATUS_ERROR) & 1;
    int perf = (subsystem->status >> 2) & 0x03;  // Performance uses 2 bits
    int res = subsystem->status & 0x03;  // Resource uses 2 bits


    printf("(PWR: %d| DATA: %d| ACT: %d| ERR: %d| PERF: %d| RES: %d)", pwr, data, act, err, perf, res);

    return ERR_SUCCESS;
}

int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data) {
    if (subsystem == NULL) {
        return ERR_NULL_POINTER;
    }

    // store the current data into the old data
    if (old_data != NULL) {
        *old_data = subsystem->data;
    }

    subsystem->data = new_data;

    // set the data queued status
    subsystem->status |= (1 << STATUS_DATA);

    return ERR_SUCCESS;
}
 
int subsys_data_get(Subsystem *subsystem, unsigned int *dest) {
    if (subsystem == NULL || dest == NULL) {
        return ERR_NULL_POINTER;
    }

    // check if there is no data and set it to zero
    if ((subsystem->status & (1 << STATUS_DATA)) == 0) {
        *dest = 0;
        return ERR_NO_DATA;
    } else {
        *dest = subsystem->data;
        subsystem->data = 0;
        subsystem->status &= ~(1 << STATUS_DATA);
    }
    return ERR_SUCCESS;
}
