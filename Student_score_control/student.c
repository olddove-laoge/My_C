#define _CRT_SECURE_NO_WARNINGS 1
#include "student.h"
#include <stdio.h>
#include <string.h>

Student students[MAX_STUDENTS];
int studentCount = 0;

void createStudentInfo() {
    if (studentCount >= MAX_STUDENTS) {
        printf("学生人数已达上限。\n");
        return;
    }

    Student* s = &students[studentCount++];
    printf("输入学生学号：");
    scanf("%d", &s->id);
    printf("输入学生姓名：");
    scanf("%s", s->name);
    printf("输入学生各科成绩（输入-1结束）：");
    for (int i = 0; i < MAX_SUBJECTS; ++i) {
        float score;
        scanf("%f", &score);
        if (score == -1) break;
        s->scores[i] = score;
    }
}

void calculateTotalAndAverage() {
    for (int i = 0; i < studentCount; ++i) {
        Student* s = &students[i];
        s->total = 0;
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            s->total += s->scores[j];
        }
        s->average = s->total / (float)MAX_SUBJECTS;
    }
}

void modifyStudentScores() {
    int id;
    printf("输入要修改成绩的学生学号：");
    scanf("%d", &id);
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].id == id) {
            printf("当前成绩：");
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                printf("%.2f ", students[i].scores[j]);
            }
            printf("\n输入新的成绩（输入-1结束）：");
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                float score;
                scanf("%f", &score);
                if (score == -1) break;
                students[i].scores[j] = score;
            }
            calculateTotalAndAverage(); // 更新总分和平均分
            return;
        }
    }
    printf("未找到学号为 %d 的学生。\n", id);
}

void deleteStudent() {
    int id;
    printf("输入要删除的学生学号：");
    scanf("%d", &id);
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].id == id) {
            for (int j = i; j < studentCount - 1; ++j) {
                students[j] = students[j + 1];
            }
            studentCount--;
            printf("学生 %d 已删除。\n", id);
            return;
        }
    }
    printf("未找到学号为 %d 的学生。\n", id);
}