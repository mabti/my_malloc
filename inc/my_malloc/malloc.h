/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stddef.h> // used for size_t and offsetof


void init_my_malloc();
void quit_my_malloc();

void *my_malloc(size_t len);
void my_free(void *ptr);


