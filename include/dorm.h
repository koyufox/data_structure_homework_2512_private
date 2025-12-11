#ifndef DORM_H
#define DORM_H

#include <stddef.h>

typedef struct {
    int id;
    char name[32];
    char room[16];
} Student;

typedef struct {
    Student* data;
    int size;
    int capacity;
} StudentList;

void list_init(StudentList* list);
void list_free(StudentList* list);
int list_add(StudentList* list, Student s);
int list_delete_by_id(StudentList* list, int id);
int list_update_by_id(StudentList* list, Student s);
int list_find_index_by_id_linear(const StudentList* list, int id);
void list_print(const StudentList* list);

int search_linear_by_id(const StudentList* list, int id);
int search_binary_by_id(const StudentList* list, int id);
int search_hash_by_id(const StudentList* list, int id);

void sort_insertion_by_id(StudentList* list, int ascending);
void sort_quick_by_name(StudentList* list, int ascending);
void sort_bubble_by_room(StudentList* list, int ascending);

#endif

