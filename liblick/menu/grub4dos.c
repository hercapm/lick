#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grub4dos.h"
#include "utils.h"
#include "../drives.h"
#include "../lickdir.h"
#include "../utils.h"

void write_entry(FILE *f, entry_t *e) {
    fprintf(f, "\n\n");
    fprintf(f, "# %s\n", e->title);
    fprintf(f, "title %s\n", e->title);
    fprintf(f, "\tfind --set-root --ignore-floppies %s", e->initrd);
    fprintf(f, "\tkernel %s %s\n", e->kernel, e->options);
    fprintf(f, "\tinitrd %s\n", e->initrd);
    fprintf(f, "\tboot\n");
}

int regenerate_grub4dos(lickdir_t *lick) {
    drive_t *win_drive = get_windows_drive();
    // TODO: location
    char *menu_lst = concat_strs(2, win_drive->path, "\\menu.lst");

    FILE *menu = fopen(menu_lst, "w");
    fprintf(menu, "This file was generated by LICK.\n");
    fprintf(menu, "DO NOT MODIFY THIS FILE DIRECTLY.");

    node_t *files = get_conf_files(lick->menu);
    for(node_t *n = files; n != NULL; n = n->next) {
        FILE *f = fopen(n->val, "r");
        if(!f)
            continue;
        while(1) {
            entry_t *e = get_entry(f);
            if(e == NULL)
                break;
            write_entry(menu, e);
            free_entry(e);
        }
        fclose(f);
    }

    free_list(files, free);
    free(menu_lst);
    free_drive(win_drive);
    return 1;
}

int install_grub4dos(lickdir_t *lick) {
    char *header = concat_strs(2, lick->menu, "\\00_header.conf");
    FILE *f = fopen(header, "w");
    free(header);

    if(!f)
        return 0;

    fprintf(f, "timeout=5\n");
    fprintf(f, "default=0\n");
    char *gfxmenu = concat_strs(2, lick->res, "\\grub4dos-gui.exe");
    fprintf(f, "gfxmenu=%s\n", gfxmenu);
    free(gfxmenu);

    fclose(f);
    return 0;
}

int uninstall_grub4dos(lickdir_t *lick) {
    char *header = concat_strs(2, lick->menu, "\\00_header.conf");
    unlinkFile(header);
    free(header);

    drive_t *win_drive = get_windows_drive();
    char *menu_lst = concat_strs(2, win_drive->path, "\\menu.lst");
    unlinkFile(menu_lst);
    free(menu_lst);
    free_drive(win_drive);
}

menu_t get_grub4dos() {
    menu_t menu;
    menu.regenerate = regenerate_grub4dos;
    menu.install = install_grub4dos;
    menu.uninstall = uninstall_grub4dos;
    return menu;
}
