#include "header.h"
#define _CRT_SECURE_NO_WARNINGS

Contact make_contact(const char *name, const char *phone, const char *email, const char *title) {
    Contact contact;

    strncpy(contact.name, name, 49);
    contact.name[49] = '\0';

    strncpy(contact.phone, phone, 11);
    contact.phone[11] = '\0';

    strncpy(contact.email, email, 49);
    contact.email[49] = '\0';

    strncpy(contact.title, title, 19);
    contact.title[19] = '\0';

    return contact;
}

void print_contact(Contact contact);

Contact scan_contact();

Node *make_node(Contact contact) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        node->data = contact;
        node->pNext = NULL;
        node->pPrev = NULL;
    }
    return node;
}

void init_list(Node **list) {
    *list = NULL;
}

void deinit_list(Node **list) {
    if (*list != NULL) {
        deinit_list(&(*list)->pNext);
        free(*list);
        *list = NULL;
    }
}

Boolean insert_at_front(Node **list, Contact contact) {
    Node *node = make_node(contact);
    if (node == NULL) return FALSE;

    node->pNext = *list;
    node->pNext->pPrev = node;
    *list = node;
    return TRUE;
}

Boolean insert_at_back(Node **list, Contact contact) {
    Node *node = make_node(contact);
    if (node == NULL) return FALSE;

    if (*list == NULL) {
        *list = node;
    } else {
        Node *search = *list;

        while (search->pNext != NULL) {
            search = search->pNext;
        }
        node->pPrev = search;
        search->pNext = node;
    }
    return TRUE;
}

Boolean insert_in_order(Node **list, Contact contact) {
    Node *node = make_node(contact);
    if (node == NULL) return FALSE;
    if (*list == NULL) {
        printf("First Contact\n");
        *list = node;
        return TRUE;
    }
    if (strcmp(contact.name, (*list)->data.name) <= 0) {
        printf("InsertAtFront Contact\n");
        node->pNext = *list;
        (*list)->pPrev = node;
        return TRUE;
    } else {
        Node *search = *list;
        while (search->pNext != NULL && strcmp(search->pNext->data.name, contact.name) < 0) {
            printf("Search Contact\n");
            search = search->pNext;
        }
        if (search->pNext == NULL) {
            printf("InsertAtBack Contact\n");
            search->pNext = node;
            node->pPrev = search;
            return TRUE;
        } else {
            printf("IsertBetween\n");
            Node *c = search->pNext;

            node->pNext = c;
            node->pPrev = search;

            c->pPrev = node;
            search->pNext = node;
            return TRUE;
        }
    }
    return FALSE;
}

Boolean remove_contact(Node **list, const char *name) {
    if (*list == NULL) {
        return FALSE;
    }
    if (strcmp((*list)->data.name, name) == 0) {
        Node *node = *list;
        *list = node->pNext;
        free(node);
        return TRUE;
    }

    Node *search = *list;
    while (search->pNext != NULL && strcmp(search->pNext->data.name, name) != 0) {
        search = search->pNext;
    }
    if (search->pNext == NULL) return FALSE;

    Node *b = search->pNext;
    search->pNext = b->pNext;
    search->pNext->pPrev = search;
    free(b);

    return TRUE;
}

Boolean edit_contact(Node **list, Contact contact) {
    Node *search = *list;
    while (search != NULL && strcmp(search->data.name, contact.name) != 0) {
        search = search->pNext;
    }
    if (search == NULL) return FALSE;
    search->data = contact;
    return TRUE;
}

Boolean load_contacts(Node **list, FILE *file) {
    char line[1024];
    while (fgets(line, 1024, file) != NULL) {
        const char *name = strtok(line, ",\n");
        const char *phone = strtok(NULL, ",\n");
        const char *email = strtok(NULL, ",\n");
        const char *title = strtok(NULL, ",\n");

        if (title == NULL) {
            return FALSE;
        }

        Contact contact = make_contact(name, phone, email, title);
        insert_in_order(list, contact);
    }

    return TRUE;
}
Boolean store_contacts(Node **list, FILE *file) {
    Node *node = *list;
    while (node != NULL) {
        int error = fprintf(file, "%s,%s,%s,%s\n", node->data.name, node->data.phone, node->data.email, node->data.title);
        if (error < 0) return FALSE;
        node = node->pNext;
    }
    return TRUE;
}
void print_list(Node **list) {
    Node *node = *list;
    while (node != NULL) {
        print_contact(node->data);
        node = node->pNext;
    }
}

int main() {
    Contact contact = {"", "", "", ""};
    Boolean success = FALSE;
    FILE *file = NULL;
    Node *list;
    init_list(&list);

    // Menu loop
    while (TRUE) {
        // Present a menu to the user
        printf(
            "Select an option:\n"
            "1. Add a contact\n"
            "2. Remove a contact\n"
            "3. Edit a contact\n"
            "4. Print the contact list\n"
            "5. Store the contact list\n"
            "6. Load the contact list\n"
            "7. Exit\n"
            ": ");
        int menu = 0;
        while (scanf("%d", &menu) != 1 || 1 > menu || menu > 7) {
            fflush(stdin);
            printf("Please enter a number between 1 and 7\n: ");
        }
        switch (menu) {
            case 1:
                contact = scan_contact();
                success = insert_in_order(&list, contact);
                if (success) {
                    printf("Successfully inserted a contact\n");
                } else {
                    printf("Unable to insert the contact\n");
                }
                break;

            case 2:
                printf("Enter the name of the contact\n: ");
                scanf(" %49[^\n]s", contact.name);
                remove_contact(&list, contact.name);
                if (success) {
                    printf("Successfully removed the contact\n");
                } else {
                    printf("No such contact in the list\n");
                }
                break;

            case 3:
                contact = scan_contact();
                edit_contact(&list, contact);
                if (success) {
                    printf("Successfully edited the contact\n");
                } else {
                    printf("No such contact in the list\n");
                }
                break;

            case 4:
                print_list(&list);
                break;

            case 5:
                file = fopen("contacts.csv", "w");
                if (file != NULL) {
                    success = store_contacts(&list, file);
                    fclose(file);

                    if (success) {
                        printf("Successfully stored the contact list\n");
                    } else {
                        printf("Unable to store the contact list\n");
                    }
                } else {
                    printf("Unable to open 'contacts.csv'\n");
                }
                file = NULL;
                break;

            case 6:
                file = fopen("contacts.csv", "r");
                if (file != NULL) {
                    success = load_contacts(&list, file);
                    fclose(file);

                    if (success) {
                        printf("Successfully loaded the contact list\n");
                    } else {
                        printf("Unable to load the contact list\n");
                    }
                } else {
                    printf("Unable to open 'contacts.csv'\n");
                }
                file = NULL;
                break;

            case 7:
                deinit_list(&list);
                return 0;
        }
    }
}

void print_contact(Contact contact) {
    char area[4], prefix[4], number[5];

    strncpy(area, contact.phone + 1, 3);
    area[3] = '\0';

    strncpy(prefix, contact.phone + 4, 3);
    prefix[3] = '\0';

    strncpy(number, contact.phone + 7, 4);
    number[4] = '\0';

    printf(
        "%s: (%s)-%s-%s, %s, %s\n",
        contact.name, area, prefix, number, contact.email, contact.title);
}

Contact scan_contact() {
    Contact contact;

    // Get the name
    printf("Enter the name of the contact\n: ");
    scanf(" %49[^\n]s", contact.name);

    // Get the phone
    printf("Enter the phone number of the contact in the following form: 123 456 7890\n: ");
    int area, prefix, number;
    while (
        scanf(" %d %d %d", &area, &prefix, &number) != 3 ||
        0 > area || area > 999 ||
        0 > prefix || prefix > 999 ||
        0 > number || number > 9999) {
        // Reset the input stream in case there is a non-number character in there
        fflush(stdin);
        // Help message
        printf("Please use the format 123 456 7890 for writing the phone number\n: ");
    }

    contact.phone[0] = '1';

    for (int i = 0, ten = 100; i < 3; i++, ten /= 10)
        contact.phone[1 + i] = '0' + (area / ten) % 10;

    for (int i = 0, ten = 100; i < 3; i++, ten /= 10)
        contact.phone[4 + i] = '0' + (prefix / ten) % 10;

    for (int i = 0, ten = 1000; i < 4; i++, ten /= 10)
        contact.phone[7 + i] = '0' + (number / ten) % 10;

    contact.phone[11] = '\0';

    // Get the email
    printf("Enter the email of the contact\n: ");
    while (
        scanf(" %49s", contact.email) == 0 ||
        strstr(contact.email, "@") == NULL) {
        fflush(stdin);
        printf("An email must have an @ sign\n: ");
    }

    printf("Enter the title of the contact\n: ");
    scanf(" %19[^\n]s", contact.title);
    return contact;
}
