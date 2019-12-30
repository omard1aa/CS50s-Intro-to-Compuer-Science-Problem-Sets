typedef struct sllist{
    int value;
    struct sllist* next;
}
node;

node* create(int val){
    node* head = (node*)malloc(sizeof(node));
    head->value = val;
    printf("Linked list created! and %i inserted in the first node.\n", val);
    return head;
}

node* insert(node* head, int val){
    node* ptr = (node*)malloc(sizeof(node));
    ptr->value = val;
    ptr->next = head;
    head = ptr;
    printf("%i inserted!\n", ptr->value);
    return head;
}

node* delete(node* head, int val){
    node* temp = head;
    node* prev = NULL;
    bool found = false;
    while(temp != NULL){
        if(temp->value == val){
            free(prev->next);
            prev->next = prev->next->next;
            found = true;
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    temp = head;
    if(found)
        printf("%i deleted!\n", val);
    else
        printf("%i not found!\n", val);
    return temp;
}

void display(node* head){
    while(head != NULL){
        printf("%i ", head->value);
        head = head->next;
    }
    printf("\n");
}
