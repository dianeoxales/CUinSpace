#include <stdio.h>
#include <string.h>

// Error Codes
#define ERR_SUCCESS 0
#define ERR_INVALID_INDEX -1
#define ERR_NO_DATA -2
#define ERR_INVALID_STATUS -3
#define ERR_MAX_CAPACITY -4
#define ERR_NULL_POINTER -5
#define ERR_SYS_NOT_FOUND -6

// Status Bits
#define STATUS_POWER 7
#define STATUS_DATA 6
#define STATUS_ACTIVITY 5
#define STATUS_ERROR 4
#define STATUS_PERFORMANCE 2
#define STATUS_RESOURCE 0

// Magic Numbers
#define MAX_STR 32
#define MAX_ARR 100

// Subsystem Structure
typedef struct {
    char name[MAX_STR];
    unsigned char status;
    unsigned int data;
} Subsystem;

// Subsystem Collection Structure
typedef struct {
    Subsystem subsystems[MAX_ARR];
    unsigned int size;
} SubsystemCollection;

// Forward Declarations
int subsys_init(Subsystem *subsystem, const char *name, char status);
int subsys_collection_init(SubsystemCollection *subsystems);
int subsys_append(SubsystemCollection *subsystems, const Subsystem *subsystem);
int subsys_find(const SubsystemCollection *subsystems, const char *name);
int subsys_print(Subsystem *subsystem);
int subsys_collection_print(SubsystemCollection *subsystems);
int subsys_status_set(Subsystem *subsystem, unsigned char status, unsigned char value);
int subsys_status_print(const Subsystem *subsystem);
int subsys_data_set(Subsystem *subsystem, unsigned int new_data, unsigned int *old_data);
int subsys_data_get(Subsystem *subsystem, unsigned int *dest);
int subsys_remove(SubsystemCollection *subsystems, int index);
int subsys_filter(const SubsystemCollection *src, SubsystemCollection *dest, const unsigned char *filter);
