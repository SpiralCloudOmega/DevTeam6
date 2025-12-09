#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Example structure demonstrating C programming
 */
typedef struct {
    char name[64];
    int value;
} Data;

/**
 * @brief Initialize a Data structure
 */
Data* data_create(const char* name, int value) {
    Data* data = (Data*)malloc(sizeof(Data));
    if (data == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }
    
    strncpy(data->name, name, sizeof(data->name) - 1);
    data->name[sizeof(data->name) - 1] = '\0';
    data->value = value;
    
    return data;
}

/**
 * @brief Free a Data structure
 */
void data_destroy(Data* data) {
    if (data != NULL) {
        free(data);
    }
}

/**
 * @brief Print data information
 */
void data_print(const Data* data) {
    if (data == NULL) {
        printf("Data: NULL\n");
        return;
    }
    
    printf("Data: { name: \"%s\", value: %d }\n", data->name, data->value);
}

int main(int argc, char* argv[]) {
    printf("C Workspace Template\n");
    printf("====================\n\n");
    
    // Create data structure
    Data* my_data = data_create("C-Workspace", 42);
    if (my_data == NULL) {
        return EXIT_FAILURE;
    }
    
    // Print data
    printf("Created: ");
    data_print(my_data);
    
    printf("\nBuild Configuration:\n");
    #ifdef DEBUG
        printf("  Mode: Debug\n");
    #else
        printf("  Mode: Release\n");
    #endif
    
    printf("  C Standard: __STDC_VERSION__ = %ld\n", __STDC_VERSION__);
    printf("  Compiler: ");
    #ifdef __GNUC__
        printf("GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #elif defined(__clang__)
        printf("Clang %d.%d.%d\n", __clang_major__, __clang_minor__, __clang_patchlevel__);
    #else
        printf("Unknown\n");
    #endif
    
    printf("\n✓ Application completed successfully!\n");
    
    // Cleanup
    data_destroy(my_data);
    
    return EXIT_SUCCESS;
}
