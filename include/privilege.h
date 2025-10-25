/*
 * Privilege management header
 */

#ifndef PRIVILEGE_H
#define PRIVILEGE_H

#include <stdbool.h>

// Check if running as root
bool is_root(void);

// Drop privileges if needed
void drop_privileges(void);

#endif // PRIVILEGE_H
