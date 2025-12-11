#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dorm.h"

static void ensure_capacity(StudentList* list, int min_cap) {
    if (list->capacity >= min_cap) return;
    int new_cap = list->capacity > 0 ? list->capacity : 8;
    while (new_cap < min_cap) new_cap *= 2;
    Student* p = (Student*)realloc(list->data, sizeof(Student) * new_cap);
    if (!p) exit(1);
    list->data = p;
    list->capacity = new_cap;
}

void list_init(StudentList* list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void list_free(StudentList* list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

int list_find_index_by_id_linear(const StudentList* list, int id) {
    for (int i = 0; i < list->size; ++i) {
        if (list->data[i].id == id) return i;
    }
    return -1;
}

int list_add(StudentList* list, Student s) {
    if (list_find_index_by_id_linear(list, s.id) != -1) return 0;
    ensure_capacity(list, list->size + 1);
    list->data[list->size++] = s;
    return 1;
}

int list_delete_by_id(StudentList* list, int id) {
    int idx = list_find_index_by_id_linear(list, id);
    if (idx == -1) return 0;
    for (int i = idx + 1; i < list->size; ++i) list->data[i - 1] = list->data[i];
    list->size--;
    return 1;
}

int list_update_by_id(StudentList* list, Student s) {
    int idx = list_find_index_by_id_linear(list, s.id);
    if (idx == -1) return 0;
    list->data[idx] = s;
    return 1;
}

void list_print(const StudentList* list) {
    printf("ID\tName\tRoom\n");
    for (int i = 0; i < list->size; ++i) {
        printf("%d\t%s\t%s\n", list->data[i].id, list->data[i].name, list->data[i].room);
    }
}

int search_linear_by_id(const StudentList* list, int id) {
    return list_find_index_by_id_linear(list, id);
}

int search_binary_by_id(const StudentList* list, int id) {
    int l = 0, r = list->size - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        int mid = list->data[m].id;
        if (mid == id) return m;
        if (mid < id) l = m + 1; else r = m - 1;
    }
    return -1;
}

typedef struct { int key; int index; int used; } HashEntry;

static unsigned int h1(unsigned int x, unsigned int cap) { return x % cap; }
static unsigned int h2(unsigned int x, unsigned int cap) { return 1 + (x % (cap - 1)); }

static int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; ++i) if (n % i == 0) return 0;
    return 1;
}

static int next_prime(int n) {
    while (!is_prime(n)) n++;
    return n;
}

int search_hash_by_id(const StudentList* list, int id) {
    int cap = next_prime(list->size * 2 + 1);
    HashEntry* table = (HashEntry*)calloc(cap, sizeof(HashEntry));
    if (!table) exit(1);
    for (int i = 0; i < list->size; ++i) {
        unsigned int k = (unsigned int)list->data[i].id;
        unsigned int idx = h1(k, cap);
        unsigned int step = h2(k, cap);
        while (table[idx].used) idx = (idx + step) % cap;
        table[idx].key = list->data[i].id;
        table[idx].index = i;
        table[idx].used = 1;
    }
    unsigned int k = (unsigned int)id;
    unsigned int idx = h1(k, cap);
    unsigned int step = h2(k, cap);
    int found = -1;
    while (table[idx].used) {
        if (table[idx].key == id) { found = table[idx].index; break; }
        idx = (idx + step) % cap;
    }
    free(table);
    return found;
}

void sort_insertion_by_id(StudentList* list, int ascending) {
    for (int i = 1; i < list->size; ++i) {
        Student key = list->data[i];
        int j = i - 1;
        while (j >= 0) {
            int cmp = list->data[j].id - key.id;
            if (ascending ? (cmp > 0) : (cmp < 0)) {
                list->data[j + 1] = list->data[j];
                j--;
            } else break;
        }
        list->data[j + 1] = key;
    }
}

static int cmp_name(const char* a, const char* b) {
    return strcmp(a, b);
}

static void quick_by_name(Student* a, int l, int r, int asc) {
    if (l >= r) return;
    int i = l, j = r;
    Student p = a[l + (r - l) / 2];
    while (i <= j) {
        while ((asc ? (cmp_name(a[i].name, p.name) < 0) : (cmp_name(a[i].name, p.name) > 0))) i++;
        while ((asc ? (cmp_name(a[j].name, p.name) > 0) : (cmp_name(a[j].name, p.name) < 0))) j--;
        if (i <= j) { Student t = a[i]; a[i] = a[j]; a[j] = t; i++; j--; }
    }
    if (l < j) quick_by_name(a, l, j, asc);
    if (i < r) quick_by_name(a, i, r, asc);
}

void sort_quick_by_name(StudentList* list, int ascending) {
    if (list->size > 1) quick_by_name(list->data, 0, list->size - 1, ascending);
}

static int cmp_room(const char* a, const char* b) {
    return strcmp(a, b);
}

void sort_bubble_by_room(StudentList* list, int ascending) {
    if (list->size <= 1) return;
    for (int i = 0; i < list->size - 1; ++i) {
        int swapped = 0;
        for (int j = 0; j < list->size - 1 - i; ++j) {
            int cmp = cmp_room(list->data[j].room, list->data[j + 1].room);
            if (ascending ? (cmp > 0) : (cmp < 0)) {
                Student t = list->data[j];
                list->data[j] = list->data[j + 1];
                list->data[j + 1] = t;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

