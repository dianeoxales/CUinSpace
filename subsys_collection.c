#include "subsystem.h"
#include <string.h>

/*
    This file should contain your documentation and implementations for SubsystemCollection 
    specific functionality; initialization, appending, removal, printing, finding, and filtering.
*/

int subsys_collection_init(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    subsystems->size = 0;

    // initialize through the maximum array length
    for (int i = 0; i < MAX_ARR; i++) {
        memset(subsystems->subsystems[i].name, 0, MAX_STR);
        subsystems->subsystems[i].data = 0;
        subsystems->subsystems[i].status = 0;
    }

    return ERR_SUCCESS;
}

int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem) {
    // check if there is room
    if (subsystems->size >= MAX_ARR) {
        return ERR_MAX_CAPACITY;
    }

    // copy subsystem data to the next available space
    subsystems->subsystems[subsystems->size] = *subsystem;
    subsystems->size++;

    return ERR_SUCCESS;
}

int subsys_find(const SubsystemCollection *subsystems, const char *name) {
    // loop through the subsystem
    for (int i = 0; i < subsystems->size; i++) {
        // check if the name is equal
        if (strcmp(subsystems->subsystems[i].name, name) == 0) {
            return i;
        }
    }
    return ERR_SYS_NOT_FOUND;
}

int subsys_collection_print(SubsystemCollection *subsystems) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    // if there are no subsystems, return error code and dont print anything
    if (subsystems->size == 0) {
        return ERR_SYS_NOT_FOUND;
    }

    // iterate through each subsystem
    for (int i = 0; i < subsystems->size; i++) {
        subsys_print(&subsystems->subsystems[i]);

    }
    return ERR_SUCCESS;
}

int subsys_remove(SubsystemCollection *subsystems, int index) {
    if (subsystems == NULL) {
        return ERR_NULL_POINTER;
    }

    if (index < 0 || index >= subsystems->size) {
        return ERR_NO_DATA;
    }

    // iterate through the size of the subsystems, subtract one from the size because index starts at 0
    for (int i = index; i < subsystems->size - 1; i++) {
       subsystems->subsystems[i] = subsystems->subsystems[i + 1];
    }

    // clear the last subsystem after shifting
    memset(&subsystems->subsystems[subsystems->size - 1], 0, sizeof(Subsystem));

    // decrease the size
    subsystems->size--;

    return ERR_SUCCESS;
}

int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter) {
    if (src == NULL || filter == NULL) {
        return ERR_NULL_POINTER;
    }

    unsigned char filter_mask = 0;
    unsigned char wildcard_mask = 0;
    dest->size = 0;

    // set the masks
    for (int i = 0; i < 8; i++) {
        if (filter[i] == '1') {
            filter_mask |= (1 << (7 - i));
        } else if (filter[i] == '*') {
            wildcard_mask |= (1 << (7 - i));
        }
    }

    // flip the filter mask
    filter_mask = ~filter_mask;

    // go through each subsystem in the src collection
    for (int i = 0; i < src->size; i++) {
        unsigned char result = ((filter_mask ^ src->subsystems[i].status) | wildcard_mask);

        // check if the result is all ones (0xFF)
        if (result == 0xFF) {
            // append the subsystem to the destination collection
            if (subsys_append(dest, &src->subsystems[i]) != ERR_SUCCESS) {
                return ERR_MAX_CAPACITY; // Handle error if adding fails
            }
        }
    }

    return ERR_SUCCESS;
}