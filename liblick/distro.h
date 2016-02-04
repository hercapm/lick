/**
 * @file
 * @brief Picks the kernel, initrd and options for a given distro
 */

// TODO: more distributions

#pragma once

#include "llist.h"

typedef enum {
    DISTRO_PUPPY = 1,
    DISTRO_FATDOG = 2,
} distro_e;

/**
 * @brief Information taken from a list of files
 */
typedef struct {
    /// a human-readable name
    char *name;
    /// the path to the kernel
    char *kernel;
    /// the path to the initrd
    char *initrd;
    /// options to pass to the kernel
    char *options;
    /// if a more advanced setup is needed, put the entire menu section here
    char *full_text;
} distro_info_t;

typedef node_t *(*distro_info_f)(node_t *files, const char *dst,
        const char *name); // TODO: menu type
typedef int (*distro_filter_f)(const char *f);

/**
 * @brief Information about a distribution
 */
typedef struct {
    /// a numeric identifier
    distro_e id;
    /// a string identifier
    const char *key;
    /// a human-readable name
    const char *name;
    /// hook for getting menu entry information
    distro_info_f info;
    /// hook for filtering files
    distro_filter_f filter;
} distro_t;

distro_t *get_distro(distro_e distro);
distro_t *get_distro_by_key(const char *key);

void free_distro(distro_t *d);
void free_distro_info(distro_info_t *d);
void free_distro_info_list(node_t *n);

char *menu_path(char *path);