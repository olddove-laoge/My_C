#define _CRT_SECURE_NO_WARNINGS 1

#include "score.h"
#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 比较函数，用于 qsort 按总分从高到低排序
int compareTotal(const void* a, const void* b) {
    Student* studentA = (Student*)a;
    Student* studentB = (Student*)b;
    return (studentB->total - studentA->total) > 0 ? 1 : -1;
}

// 比较函数，用于 qsort 按总分从低到高排序
int compareTotalDesc(const void* a, const void* b) {
    Student* studentA = (Student*)a;
    Student* studentB = (Student*)b;
    return (studentB->total - studentA->total) < 0 ? 1 : -1;
}

// 比较函数，用于 qsort 按学号排序
int compareId(const void* a, const void* b) {
    Student* studentA = (Student*)a;
    Student* studentB = (Student*)b;
    return studentA->id - studentB->id;
}

// 比较函数，用于 qsort 按姓名排序
int compareName(const void* a, const void* b) {
    Student* studentA = (Student*)a;
    Student* studentB = (Student*)b;
    return strcmp(studentA->name, studentB->name);
}

void calculateSubjectTotalAndAverage() {
    float totals[MAX_SUBJECTS] = { 0 };
    float averages[MAX_SUBJECTS] = { 0 };

    for (int i = 0; i < studentCount; ++i) {
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            totals[j] += students[i].scores[j];
        }
    }

    for (int i = 0; i < MAX_SUBJECTS; ++i) {
        averages[i] = totals[i] / studentCount;
        printf("课程 %d 总分：%.2f, 平均分：%.2f\n", i + 1, totals[i], averages[i]);
    }
}

void rankStudentsByTotal() {
    qsort(students, studentCount, sizeof(Student), compareTotal);
    printf("按总分从高到低排名：\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("排名 %d: 学号 %d, 姓名 %s, 总分 %.2f, 平均分 %.2f, 成绩：",
            i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void rankStudentsByTotalDesc() {
    qsort(students, studentCount, sizeof(Student), compareTotalDesc);
    printf("按总分从低到高排名：\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("排名 %d: 学号 %d, 姓名 %s, 总分 %.2f, 平均分 %.2f, 成绩：",
            i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void sortStudentsById() {
    qsort(students, studentCount, sizeof(Student), compareId);
    printf("按学号排序：\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("学号 %d, 姓名 %s, 总分 %.2f, 平均分 %.2f, 成绩：",
            students[i].id, students[i].name, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void sortStudentsByName() {
    qsort(students, studentCount, sizeof(Student), compareName);
    printf("按姓名排序：\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("姓名 %s, 学号 %d, 总分 %.2f, 平均分 %.2f, 成绩：",
            students[i].name, students[i].id, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void statisticsByGrade() {
    // 为每个科目创建一个成绩分段计数数组
    int gradeCounts[MAX_SUBJECTS][5] = { {0} }; // 5个分段：优秀、良好、中等、及格、不及格

    // 首先统计每个科目的总学生数
    int totalStudents[MAX_SUBJECTS] = { 0 };
    for (int j = 0; j < MAX_SUBJECTS; ++j) {
        totalStudents[j] = studentCount; // 假设每个科目都有相同数量的学生
    }

    for (int i = 0; i < studentCount; ++i) {
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            if (students[i].scores[j] >= 90) gradeCounts[j][4]++;
            else if (students[i].scores[j] >= 80) gradeCounts[j][3]++;
            else if (students[i].scores[j] >= 70) gradeCounts[j][2]++;
            else if (students[i].scores[j] >= 60) gradeCounts[j][1]++;
            else gradeCounts[j][0]++;
        }
    }

    printf("各科目成绩分段统计：\n");
    for (int j = 0; j < MAX_SUBJECTS; ++j) {
        printf("科目 %d: ", j + 1);
        for (int k = 0; k < 5; k++) {
            float percentage = (float)gradeCounts[j][k] / totalStudents[j] * 100;
            printf("%s：%d 人 (%.2f%%)\n", k == 0 ? "不及格" : k == 1 ? "及格" : k == 2 ? "中等" : k == 3 ? "良好" : "优秀", gradeCounts[j][k], percentage);
        }
    }
}
void queryStudentById() {
    int id;
    printf("输入要查询的学生学号：");
    scanf("%d", &id);
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].id == id) {
            printf("排名 %d: 学号 %d, 姓名 %s, 总分 %.2f, 平均分 %.2f, 成绩：",
                i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                printf("%.2f ", students[i].scores[j]);
            }
            printf("\n");
            return;
        }
    }
    printf("未找到学号为 %d 的学生。\n", id);
}

void queryStudentByName() {
    char name[50];
    printf("输入要查询的学生姓名：");
    scanf("%s", name);
    for (int i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].name, name) == 0) {
            printf("排名 %d: 学号 %d, 姓名 %s, 总分 %.2f, 平均分 %.2f, 成绩：",
                i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                printf("%.2f ", students[i].scores[j]);
            }
            printf("\n");
            return;
        }
    }
    printf("未找到姓名为 %s 的学生。\n", name);
}
