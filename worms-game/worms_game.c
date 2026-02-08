#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WormPart {
    int x, y;
};

struct Node {
    struct WormPart* data;
    struct Node* next;
    struct Node* previous;
};

struct DoublyList {
    struct Node* head;
    struct Node* tail;
    int elemcount;
};

void addFront(struct DoublyList* list, struct WormPart* new_element) {
    struct Node* newnode = (struct Node*) malloc(sizeof(struct Node));
    newnode->data = new_element;
    newnode->next = list->head;
    newnode->previous = NULL;

    if (list->head != NULL)
        list->head->previous = newnode;

    list->head = newnode;
    list->elemcount++;

    if (list->elemcount == 1)
        list->tail = newnode;
}

void addBack(struct DoublyList* list, struct WormPart* new_element) {
    struct Node* newnode = (struct Node*) malloc(sizeof(struct Node));
    newnode->data = new_element;
    newnode->next = NULL;
    newnode->previous = list->tail;

    if (list->tail != NULL)
        list->tail->next = newnode;

    list->tail = newnode;
    list->elemcount++;

    if (list->elemcount == 1)
        list->head = newnode;
}

void removeFront(struct DoublyList* list) {
    if (list->head != NULL) {
        struct Node* old = list->head;
        if (list->head->next != NULL)
            list->head->next->previous = NULL;
        list->head = list->head->next;
        free(old->data);
        free(old);
        list->elemcount--;
    }
}

void removeBack(struct DoublyList* list) {
    if (list->tail != NULL) {
        struct Node* old = list->tail;
        if (list->tail->previous != NULL)
            list->tail->previous->next = NULL;
        list->tail = list->tail->previous;
        free(old->data);
        free(old);
        list->elemcount--;
    }
}

void removeAt(struct DoublyList* list, int position) {
    if (position == 0) {
        removeFront(list);
        return;
    } else if (position == list->elemcount - 1) {
        removeBack(list);
        return;
    } else if (position >= list->elemcount) {
        return;
    }

    struct Node* prev_node = NULL;
    struct Node* position_pointer = list->head;

    for (int i = 0; i < position; i++) {
        prev_node = position_pointer;
        position_pointer = position_pointer->next;
    }

    struct Node* old = position_pointer;
    prev_node->next = position_pointer->next;

    if (position_pointer->next != NULL)
        position_pointer->next->previous = prev_node;

    free(old->data);
    free(old);
    list->elemcount--;
}

void removeAtNew(struct DoublyList** list,int position,int worm_count){
    for(int i = 0 ; i < worm_count ; i++){
        if(i == position){
            free(list[i]);
            for(int j = i ; j < worm_count - 1 ; j++){
                list[j] = list[j+1];
            }
            return;
        }
    }
}

struct DoublyList* create_worm(char* filename) {
    struct DoublyList* worm = (struct DoublyList*) malloc(sizeof(struct DoublyList));
    worm->head = NULL;
    worm->tail = NULL;
    worm->elemcount = 0;

    FILE* infile = fopen(filename, "r");

    char line[256];
    while (fgets(line, sizeof(line), infile)) {
        int x, y;
        sscanf(line, "%d %d", &x, &y);

        struct WormPart* new_part = (struct WormPart*) malloc(sizeof(struct WormPart));
        new_part->x = x;
        new_part->y = y;

        addFront(worm, new_part);
    }

    fclose(infile);
    return worm;
}

int main() {
    struct DoublyList* wormfield[100];
    int wormcount = 4;

    char* filenames[4] = {"worms/worm1.txt", "worms/worm2.txt", "worms/worm3.txt", "worms/worm4.txt"};

    for (int i = 0; i < 4; i++) {
        struct DoublyList* new_worm = create_worm(filenames[i]);        
        wormfield[i] = new_worm;
    }

    while (1) {
        int option;
        printf("------------------------------------\n");
        printf("1- View the worms\n");
        printf("2- Attack\n");
        printf("3- Quit\n");
        printf("------------------------------------\n");
        printf("Please enter an action: ");
        scanf("%d", &option);

        if (option == 1) {

            printf("Worm List\n");

            for (int i = 0; i < wormcount; i++) {
                printf("Worm %d ", i);

                struct Node* wormnode_ptr = wormfield[i]->head;
                while (wormnode_ptr != NULL) {
                    printf("(%d,%d) ", wormnode_ptr->data->x, wormnode_ptr->data->y);
                    wormnode_ptr = wormnode_ptr->next;
                }
                printf("\n");
            }

        } else if (option == 2) { //implemented by sait_cglr
            printf("Enter x y coordinates to attack\n");
            int x, y;
            scanf("%d %d", &x, &y);
            int control = 0;
            for(int i = 0 ; i < wormcount ; i++){
                struct Node* current_node = wormfield[i]->head;
                while(current_node != NULL){
                    if(current_node->data->x == x && current_node->data->y == y){
                        printf("HIT! x = %d and y = %d\n", x, y);
                        control = 1;
                        if(current_node->previous == NULL){
                            if(current_node->next == NULL){
                                printf("(%d,%d) is destroyed\n",x,y);
                                printf("Worm is dead!\n");
                                removeFront(wormfield[i]);
                                removeAtNew(wormfield,i,wormcount);
                                wormcount--;
                            }else if(current_node->next->next == NULL){
                                printf("(%d,%d)->(%d,%d) are destroyed!\n",x,y,current_node->next->data->x,current_node->next->data->y);
                                printf("Worm is dead!\n");
                                removeFront(wormfield[i]);
                                removeFront(wormfield[i]);
                                removeAtNew(wormfield,i,wormcount);
                                wormcount--;
                            }else{
                                printf("(%d,%d)->(%d,%d) are destroyed!\n",x,y,current_node->next->data->x,current_node->next->data->y);
                                removeFront(wormfield[i]);
                                removeFront(wormfield[i]);
                            }
                        }else if(current_node->previous->previous == NULL){
                            if(current_node->next == NULL){
                                printf("(%d,%d)->(%d,%d) are destroyed!\n", current_node->previous->data->x,current_node->previous->data->y,x,y);
                                printf("Worm is dead!\n");
                                removeFront(wormfield[i]);
                                removeFront(wormfield[i]);
                                removeAtNew(wormfield,i,wormcount);
                                wormcount--;
                            }else if(current_node->next->next == NULL){
                                printf("(%d,%d)->(%d,%d)->(%d,%d) are destroyed!\n", current_node->previous->data->x,current_node->previous->data->y,x,y,current_node->next->data->x,current_node->next->data->y);
                                printf("Worm is dead!\n");
                                removeFront(wormfield[i]);
                                removeFront(wormfield[i]);
                                removeFront(wormfield[i]);
                                removeAtNew(wormfield,i,wormcount);
                                wormcount--;
                            }else{
                                printf("(%d,%d)->(%d,%d)->(%d,%d) are destroyed!\n", current_node->previous->data->x,current_node->previous->data->y,x,y,current_node->next->data->x,current_node->next->data->y);
                                removeFront(wormfield[i]);
                                removeFront(wormfield[i]);
                                removeFront(wormfield[i]);
                            }
                        }else{
                            if(current_node->next == NULL){
                                printf("(%d,%d)->(%d,%d) are destroyed!\n", current_node->previous->data->x,current_node->previous->data->y,x,y);
                                removeBack(wormfield[i]);
                                removeBack(wormfield[i]);
                            }else if(current_node->next->next == NULL){
                                printf("(%d,%d)->(%d,%d)->(%d,%d) are destroyed!\n", current_node->previous->data->x,current_node->previous->data->y,x,y,current_node->next->data->x,current_node->next->data->y);
                                removeBack(wormfield[i]);
                                removeBack(wormfield[i]);
                                removeBack(wormfield[i]);
                            }else{
                                printf("(%d,%d)->(%d,%d)->(%d,%d) are destroyed!\n", current_node->previous->data->x,current_node->previous->data->y,x,y,current_node->next->data->x,current_node->next->data->y);
                                struct DoublyList* new_worm = (struct DoublyList*) malloc(sizeof(struct DoublyList));
                                struct Node* iterator;
                                new_worm->head = current_node->next->next;
                                new_worm->head->previous = NULL;
                                new_worm->tail = wormfield[i]->tail;
                                int counter = 0;
                                for(iterator = new_worm->head ; iterator != NULL ; iterator = iterator->next){
                                    counter++;
                                }
                                new_worm->elemcount = counter;
                                counter = 0;
                                wormfield[i]->tail = current_node->previous->previous;
                                wormfield[i]->tail->next = NULL;
                                for(iterator = wormfield[i]->head ; iterator != NULL ; iterator = iterator->next){
                                    counter++;
                                }
                                wormfield[i]->elemcount = counter;
                                struct Node* free1 = current_node->previous;
                                struct Node* free2 = current_node->next;
                                struct Node* free3 = current_node;
                                free(free1->data);
                                free(free1);
                                free(free2->data);
                                free(free2);
                                free(free3->data);
                                free(free3);
                                wormfield[wormcount++] = new_worm;
                            }
                        }
                    break;
                    }else{
                        current_node = current_node->next;
                    }
                    
                }
            }
            if(!control) printf("Miss!\n");
            if(wormcount < 1) printf("All worms are destroyed!\n");
        }else if (option == 3) {
            break;
        } else {
            printf("Wrong Input!\n");
        }
    }

    return 0;
}
