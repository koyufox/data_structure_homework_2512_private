#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dorm.h"

static void read_string(char* buf, int n) {
    if (scanf("%31s", buf) != 1) strcpy(buf, "");
}

static Student read_student() {
    Student s;
    printf("输入ID: ");
    scanf("%d", &s.id);
    printf("输入姓名: ");
    read_string(s.name, 31);
    printf("输入宿舍: ");
    read_string(s.room, 15);
    return s;
}

static void menu() {
    printf("\n宿舍管理系统\n");
    printf("1 添加\n");
    printf("2 删除\n");
    printf("3 修改\n");
    printf("4 列表\n");
    printf("5 查找\n");
    printf("6 排序\n");
    printf("0 退出\n");
    printf("选择: ");
}

static void menu_search() {
    printf("\n查找算法\n");
    printf("1 顺序查找(按ID)\n");
    printf("2 二分查找(按ID)\n");
    printf("3 哈希查找(按ID)\n");
    printf("选择: ");
}

static void menu_sort() {
    printf("\n排序算法\n");
    printf("1 插入排序(按ID)\n");
    printf("2 快速排序(按姓名)\n");
    printf("3 冒泡排序(按宿舍)\n");
    printf("升序输入1, 降序输入0: ");
}

int main() {
    StudentList list;
    list_init(&list);
    int choice;
    for (;;) {
        menu();
        if (scanf("%d", &choice) != 1) break;
        if (choice == 0) break;
        if (choice == 1) {
            Student s = read_student();
            int ok = list_add(&list, s);
            printf(ok ? "已添加\n" : "ID重复, 未添加\n");
        } else if (choice == 2) {
            int id;
            printf("输入要删除的ID: ");
            scanf("%d", &id);
            int ok = list_delete_by_id(&list, id);
            printf(ok ? "已删除\n" : "未找到\n");
        } else if (choice == 3) {
            Student s = read_student();
            int ok = list_update_by_id(&list, s);
            printf(ok ? "已修改\n" : "未找到\n");
        } else if (choice == 4) {
            list_print(&list);
        } else if (choice == 5) {
            menu_search();
            int alg; scanf("%d", &alg);
            int id; printf("输入要查找的ID: "); scanf("%d", &id);
            int idx = -1;
            if (alg == 1) idx = search_linear_by_id(&list, id);
            else if (alg == 2) idx = search_binary_by_id(&list, id);
            else if (alg == 3) idx = search_hash_by_id(&list, id);
            if (idx >= 0) printf("找到: %d %s %s\n", list.data[idx].id, list.data[idx].name, list.data[idx].room);
            else printf("未找到\n");
        } else if (choice == 6) {
            menu_sort();
            int alg; scanf("%d", &alg);
            int asc; scanf("%d", &asc);
            if (alg == 1) sort_insertion_by_id(&list, asc);
            else if (alg == 2) sort_quick_by_name(&list, asc);
            else if (alg == 3) sort_bubble_by_room(&list, asc);
            printf("已排序\n");
        }
    }
    list_free(&list);
    return 0;
}

