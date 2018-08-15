#include <stdio.h>

typedef struct node {
	    int val;
	    struct node * next;
	} node_t;

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%d\n", current->val);
        current = current->next;
    }
}

void append(node_t * head, int val) {
    node_t * current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    /* now we can add a new variable */
    current->next = malloc(sizeof(node_t));
    current->next->val = val;
    current->next->next = NULL;
}

void prepend(node_t** head, int val) {
    node_t* new_node;
    new_node = malloc(sizeof(node_t));

    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}


int main(){
	
	node_t * head = NULL;
	
	head = malloc(sizeof(node_t));
	if (head == NULL) {
		return 1;
	}

	for(int i=1; i<10; i++){
		append(head, i);
	}

	//prepend(&head , 98);

   	print_list(head);
}

