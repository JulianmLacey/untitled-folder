#pragma once
#include <locale.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum boolean {
    FALSE = 0,
    TRUE = 1,
} Boolean;

typedef struct contact {
    char name[50];
    char phone[12];  // 18005557577
    char email[50];
    char title[20];
} Contact;

typedef struct node {
    Contact data;
    struct node *pNext;
    struct node *pPrev;  // this lab is now using two links
} Node;
