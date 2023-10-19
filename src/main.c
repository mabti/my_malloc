/* 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <my_malloc/malloc.h>

#include <string.h> // testing
#include <stdio.h>

int main() {
	init_my_malloc();

	char *data = my_malloc(15);
	char *data2 = my_malloc(15);
	strncpy(data, "Hello, World!\n", 15);
	
	printf("%s", data);
	my_free(data);

	strncpy(data2, "Foobar\n", 8);
	printf("%s", data2);

	my_free(data2);
	quit_my_malloc();
	return 0;
}

