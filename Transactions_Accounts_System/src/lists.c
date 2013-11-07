#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
	
	/*Create a new Group Dynamically*/
	Group *new_group = malloc(sizeof(Group));
	if (new_group == NULL) {
		perror("Error allocating space for new group"); 
		exit(1);
	}
	new_group->name = malloc(strlen(group_name)+1);
	if (new_group->name == NULL) {
		perror("Error allocating space for group's name"); 
		exit(1);
	}
	/*Initialize new Group's variables*/

	new_group->users = NULL;
	new_group->xcts = NULL;
	new_group->next = NULL;
	
	/*Insert new Group to group_list*/
	Group *curr = *group_list_ptr;
	/*Insert as head*/
	if (curr == NULL) {
		*group_list_ptr = new_group;
		return 0;
	}

	/*name checking*/
	while (curr->next != NULL){
		/*repeated name*/
		if (strcmp(curr->name, new_group->name) == 0) {
			free(new_group->name);
			free(new_group);
			return -1;
		}
		curr = curr->next;
	}

	/*While loop hits the tail so insert after final name check*/
	if (strcmp(curr->name, new_group->name) == 0) {
		free(new_group->name);
		free(new_group);
		return -1;
	} else{
		curr->next = new_group;
		return 0;
	}
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
	Group *curr = group_list;
	while (curr != NULL){
		printf("%s \n", curr->name);
		curr = curr->next;
	}
	return;
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
	Group *curr = group_list;

	/*Return NULL if group_list is empty*/
	if (group_list == NULL){
		free(curr);
		return NULL;
	}

	/*name checking*/
	while (curr->next != NULL){	
		/*found a name*/
		if (strcmp(curr->name, group_name) == 0){
			return curr;
		}
		curr = curr->next;
	}
	/*Hit the tail but need to check its name*/
	if (strcmp(curr->name, group_name) == 0){
		return curr;
	} else {
		return NULL;
	}
}
/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
	/*Create and initialize new user*/
	User *new_user = malloc(sizeof(User));
	if (new_user == NULL) {
		perror("Error allocating space for new user"); 
		exit(1);
	}

	new_user->name = malloc(strlen(user_name) + 1);
	if (new_user->name == NULL) {
		perror("Error allocating space for new user's name"); 
		exit(1);
	}

	strncpy(new_user->name, user_name, strlen(user_name) + 1);
	new_user->balance = 0;
	new_user->next = NULL;

	/*Inser new_user to group's users*/
	User *curr = group->users;
	/*Insert directly if group's users list is empty*/
	if (group->users == NULL){
		group->users = new_user;
		return 0;
	}
	
	/*name checking*/
	while (curr->next != NULL){
		if (strcmp(curr->name, user_name) == 0) {
			free(new_user->name);
			free(new_user);
			return -1;
		}
		curr = curr->next;
	}
	/*Hit the tail but need to check its name*/
	if (strcmp(curr->name, user_name) == 0){
		free(new_user->name);
		free(new_user);
		return -1;
	} else {
		/*Insert at front because balance of 0 is minimal*/
		new_user->next = group->users;
		group->users = new_user;
		return 0;
	}
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
	User *prev = find_prev_user(group, user_name);
	/*check if user_name not found or removing from empty list*/
	if (prev == NULL){
		return -1;
	} else{
	/*remove user found in the list*/
		if (strcmp(prev->name, user_name) == 0){
			/*remove from beginning of list*/
			remove_xct(group, user_name);
			free(prev);
			prev = prev->next;
			return 0;
		}
		else{
			/*remove from middle or end of list*/
			remove_xct(group, user_name);
			User *temp = prev->next->next;
			free(prev->next);
			prev->next = temp;
			return 0;
		}
	 }
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
	User *curr = group->users;
	while (curr != NULL){
		printf("User: %s, Balance: %.2f \n", curr->name, curr->balance);
		curr = curr->next;
	}
	return;
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
	User *prev = find_prev_user(group, user_name);
	//check if matching user is in the list
	if(prev == NULL){
		return -1;
	//check if prev is the matching user
	} else if(strcmp(prev->name, user_name) == 0){
		printf("%.2f \n", prev->balance); 
		return 0;
	} else{
	//use prev to access user
		printf("%.2f \n", prev->next->balance); 
		return 0;
	}
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
	double minbal;
	if (group->users == NULL){
	return -1;
	}
	minbal = group->users->balance;
	User *curr = group->users;
	while ((curr != NULL) && (curr->balance == minbal)){
		printf("%s \n", curr->name);
		curr = curr->next;
	}
    return 0;
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
	/*return the first node whose next field points to user name*/
	
    User *prev_user = group->users;
	/*check NULL users list*/
	if (group->users == NULL){
		return NULL;
		}
	/*check first in list*/
	else if (strcmp(group->users->name, user_name) == 0){
		return group->users;
	}
	/*check inside users list*/
	while (prev_user->next != NULL){
		if (strcmp(prev_user->next->name, user_name) == 0){
			return prev_user;
		}
		prev_user = prev_user->next;
	}

	/*hit tail and previous user not found*/
	return NULL;
}

/* Add the transaction represented by user_name and amount to the appropriate
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
	User *prev = find_prev_user(group, user_name);
	User *curr;
	/*check if user exists*/
	if (prev == NULL){
		return -1;
	}
	/*get current user which is prev if current user is fist*/
	/*to update the balances*/
	if (strcmp(prev->name, user_name) == 0)
		curr = prev;
	else
		curr = prev->next;

	/*create and initialize Xct*/
	Xct *bill = malloc(sizeof(Xct));
	if (bill == NULL) {
		perror("Error allocating space for new bill"); 
		exit(1);
	}

	bill->name = malloc(strlen(user_name) + 1);
	if (bill->name == NULL) {
		perror("Error allocating space for new bill's name"); 
		exit(1);
	}

	strncpy(bill->name, user_name, strlen(user_name) + 1);
	bill->amount = amount;
	bill->next = NULL;
	
	/*insert to empty xcts*/
	if(group->xcts == NULL){
		curr->balance += amount;
		group->xcts = bill;
	} else {
	/*insert Xct to beginning of group's xcts*/
		curr->balance += amount;
		bill->next = group->xcts;
		group->xcts = bill;
	}
	/*reorder due to change in balances*/
    /*User balance increased, move towards the tail*/
	while (curr->next != NULL){
		/*swap users and update prev*/
		if(curr->balance > curr->next->balance){
			User *temp1 = curr;
			User *temp2 = curr->next->next;
			curr = curr->next;
			curr->next = temp1;
			curr->next->next = temp2;
			if(strcmp(curr->next->name, prev->name) != 0) {
				prev->next = curr;
			} else {
				/*update prev of user with changed balance*/ 
				/*from beggining of list*/
				prev = curr;
				group->users = prev;
			}
		} else {
		/*keep checking if swap needed, update prev accordingly*/
			User *temp = curr;
			curr = curr->next;
			prev = temp;
			}
	}
	return 0;
}
/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
/*this function should work but my implementation of add_xct is faulty*/
	if (group->xcts == NULL){
		return;
	}
	Xct *curr = group->xcts;
	long i = 0;
	while (i < nu_xct && curr != NULL){
		printf("Transaction Name: %s, Amount: %.2f \n", curr->name, curr->amount);
		curr = curr->next;
		i++;
	}
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/

void remove_xct(Group *group, const char *user_name) {
	Xct *curr_xct = group->xcts;

	/*check empty transactions list*/
	if(curr_xct == NULL){
		return;
	}

	/*find all transactions for a user
 	* first remove from middle and end of xcts list
 	* curr_xct is better thought as prev_xct*/
	while (curr_xct->next != NULL){
		/*check to remove curr_xct, to deal with consecutive removals*/
		if (strcmp(curr_xct->name, user_name) == 0){
			Xct *temp = curr_xct->next;
			free(curr_xct);
			curr_xct = temp;
			continue;
		/*check to remove curr_xct->next*/
		} else if (strcmp(curr_xct->next->name, user_name) == 0){
			Xct *temp = curr_xct->next->next;
			free(curr_xct->next);
			curr_xct->next = temp;
		}
		curr_xct = curr_xct->next;
	}
	
	/*remove tail or from beginning of single node transactions list*/
	if (strcmp(curr_xct->name, user_name) == 0){
		Xct *temp = curr_xct->next;
		free(curr_xct);
		curr_xct = temp;
	}

	return;
}

