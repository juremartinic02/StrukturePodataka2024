#ifndef DEFINICIJASTRUKTURA_H
#define DEFINICIJASTRUKTURA_H

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<crtdbg.h>

#define MAX_NAME_LENGTH 64
#define MAX_LINE_LENGTH 256
#define MAX_FILENAME_LENGTH 128
#define FILE_ERROR -1
#define MEMORY_ERROR NULL

struct _date;
typedef struct _date* DatePosition;
typedef struct _date {
    int day;
    int month;
    int year;
} Date;

struct _article;
typedef struct _article* ArticlePosition;
typedef struct _article {
    char name[MAX_NAME_LENGTH];
    int quantity;
    double price;
    ArticlePosition next;
} Article;

struct _reciept;
typedef struct _reciept* RecieptPosition;
typedef struct _reciept {
    Date dateOfIssue;
    ArticlePosition articleHead;
    RecieptPosition next;
} Reciept;

#endif