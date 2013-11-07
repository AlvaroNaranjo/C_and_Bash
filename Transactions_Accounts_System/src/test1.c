#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lists.h"

int test1(void) {
	printf("GOT TO TEST1 \n");
    Group *g = NULL;
	Group **pg = &g;
    add_group(pg, "first");
	add_group(pg, "second");
	add_group(pg, "third");
	list_groups(g);
	Group *First;
	First = find_group(g, "second");
	printf("First is %s \n", First->name);
	add_user(g, "alvaro");
	add_user(g, "patricia");
	add_user(g, "anna");
	list_users(g);
	list_users(g);
	under_paid(g);
	return 0;
}


