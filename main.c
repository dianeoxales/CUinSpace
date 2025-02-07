#include <stdio.h>
#include <stdlib.h>
#include "subsystem.h"

#define MENU_ADD 1
#define MENU_PRINT 2
#define MENU_PRINTALL 3
#define MENU_STATUS 4
#define MENU_REMOVE 5
#define MENU_FILTER 6
#define MENU_DATA 7
#define MENU_EXIT 0
#define MENU_MIN 0
#define MENU_MAX 7

int print_menu(int *choice);

int main() {
    // Use a while loop to print the menu for the user and call the appropriate functions.
    // The definitions above are provided to assist with this.
    int choice = -1;
    Subsystem subsystem;
    SubsystemCollection subsystems;
    char name[MAX_STR];
    unsigned char status;
    unsigned int new_data;
    unsigned int old_data = 0;
    unsigned char value; // for the value 0-3
    int index;

    if (subsys_collection_init(&subsystems) != ERR_SUCCESS) {
        printf("Error initializing...");
    }
    
    do {
        print_menu(&choice);

        switch(choice) {
            case MENU_ADD:
                printf("\nEnter subsystem name(no spaces): ");
                scanf("%s", name);

                if (subsys_init(&subsystem, name, status) != ERR_SUCCESS) {
                    printf("\nError initializing...");
                    break;
                } 

                if (subsys_append(&subsystems, &subsystem) != ERR_SUCCESS) {
                    printf("\nFailed to add reading.");
                    break;
                } else {
                    printf("\nSubsystem added successfully.");
                    break;
                }

            case MENU_PRINT:
                printf("\nEnter subsystem name to print: ");
                scanf("%s", name);

                index = subsys_find(&subsystems, name);
                if(index >= 0) {
                    if(subsys_print(&subsystems.subsystems[index]) != ERR_SUCCESS) {
                        printf("\nError printing the subsystem.\n");
                        break;
                    }
                } else {
                    printf("\nSubsystem not found.\n");
                    break;
                }

                // clear the value of the name after
                memset(name, 0, MAX_STR);
                break;
            case MENU_PRINTALL:
                if (subsys_collection_print(&subsystems) == ERR_SYS_NOT_FOUND) {
                    printf("not found\n");
                    break;
                }
                break;
            case MENU_STATUS:
                printf("\nEnter <Subsystem name> <Status ID, 7, 6, 5, 4, 2, 0> <New Value (0-3)>: ");
                scanf("%s %hhd %hhd", name, &status, &value);
                index = subsys_find(&subsystems, name);

                if (index >= 0) {
                    if (subsys_status_set(&subsystems.subsystems[index], status, value) != ERR_SUCCESS) {
                        printf("\ndidn't work\n");
                        break;
                    } else {
                        printf("\nStatus updated successfully.\n");
                        break;
                }
                } else {
                    printf("\nThere is nothing to show...\n");
                }
                break;
            case MENU_REMOVE:
                printf("\nEnter subsystem name to remove: ");
                scanf("%s", name);
                index = subsys_find(&subsystems, name);

                if (index >= 0) {
                    if (subsys_remove(&subsystems, index) != ERR_SUCCESS) {
                        printf("\nError removing subsystem '%s'.\n", name);
                        break;
                    } else {
                        printf("\nSubsystem '%s' successfully removed. \n", name);
                        break;
                    }
                }
                break;
            case MENU_FILTER:
                char filter[9];
                int i;
                SubsystemCollection dest;
                printf("\nEnter filter string (8 characters of 1, 0 *): ");
                scanf("%8s", filter);

                if (subsys_collection_init(&dest) != ERR_SUCCESS) {
                    printf("\nError initializing the destination collection.\n"); 
                    break;
                }

                if (subsys_filter(&subsystems, &dest, filter) != ERR_SUCCESS) {
                    printf("\nError filtering the subysystem.\n");
                    break;
                } else { 
                    if(dest.size > 0) {
                        if(subsys_collection_print(&dest) != ERR_SUCCESS) {
                            printf("\nNo systems match this filter.\n");
                            break;
                        } else {
                            printf("\nSuccess.\n");
                        }
                    }
                    break;
                }
                break;
            case MENU_DATA:
                printf("\nEnter: <Subsystem name> <Data, uppercase hex without 0x>: ");
                if (scanf("%s %X", name, &new_data) != 2) {
                    printf("\nInvalid input. Input the proper values please.\n");
                    break;
                }

                index = subsys_find(&subsystems, name);
                if (index >= 0) {
                    if (subsys_data_set(&subsystems.subsystems[index], new_data, &old_data) != ERR_SUCCESS) {
                        printf("\nFailed to update data.");
                    } else {
                        printf("\nData updated successfully.");
                    }
                }
                break;
            case MENU_EXIT:
                printf("Exiting...");
        }
    } while (choice != MENU_EXIT);
}

/* 
    Prints a menu to the user and prompts them until they enter a valid menu option.

    out: *choice, an integer corresponding to a MENU_ definition defining the choice made by the user.

    Returns: 
        - ERR_SUCCESS if there is no error
        - ERR_NULL_POINTER if `choice` is NULL

*/
int print_menu(int *choice) {
    if (choice == NULL) {
        return ERR_NULL_POINTER;
    }

    *choice = 0;

    do {
        printf("\n--- Subsystem Management Menu ---\n");
        printf("%d. Add Subsystem\n", MENU_ADD);
        printf("%d. Print Subsystem\n", MENU_PRINT);
        printf("%d. Print All Subsystems\n", MENU_PRINTALL);
        printf("%d. Set Subsystem Status\n", MENU_STATUS);
        printf("%d. Remove Subsystem\n", MENU_REMOVE);
        printf("%d. Filter Subsystems\n", MENU_FILTER);
        printf("%d. Set Subsystem Data\n", MENU_DATA);
        printf("%d. Exit\n", MENU_EXIT);
        printf("Select an option: ");
        scanf("%d", choice);
        while (getchar() != '\n');

        if (*choice < MENU_MIN || *choice > MENU_MAX) {
            printf("Error: Invalid choice. Please try again.\n");
        }
    } while (*choice < MENU_MIN || *choice > MENU_MAX);

    return ERR_SUCCESS;
}