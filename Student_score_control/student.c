#define _CRT_SECURE_NO_WARNINGS 1
#include "student.h"
#include <stdio.h>
#include <string.h>

Student students[MAX_STUDENTS];
int studentCount = 0;

void createStudentInfo() {
    if (studentCount >= MAX_STUDENTS) {
        printf("ѧ�������Ѵ����ޡ�\n");
        return;
    }

    Student* s = &students[studentCount++];
    printf("����ѧ��ѧ�ţ�");
    scanf("%d", &s->id);
    printf("����ѧ��������");
    scanf("%s", s->name);
    printf("����ѧ�����Ƴɼ�������-1��������");
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
    printf("����Ҫ�޸ĳɼ���ѧ��ѧ�ţ�");
    scanf("%d", &id);
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].id == id) {
            printf("��ǰ�ɼ���");
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                printf("%.2f ", students[i].scores[j]);
            }
            printf("\n�����µĳɼ�������-1��������");
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                float score;
                scanf("%f", &score);
                if (score == -1) break;
                students[i].scores[j] = score;
            }
            calculateTotalAndAverage(); // �����ֺܷ�ƽ����
            return;
        }
    }
    printf("δ�ҵ�ѧ��Ϊ %d ��ѧ����\n", id);
}

void deleteStudent() {
    int id;
    printf("����Ҫɾ����ѧ��ѧ�ţ�");
    scanf("%d", &id);
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].id == id) {
            for (int j = i; j < studentCount - 1; ++j) {
                students[j] = students[j + 1];
            }
            studentCount--;
            printf("ѧ�� %d ��ɾ����\n", id);
            return;
        }
    }
    printf("δ�ҵ�ѧ��Ϊ %d ��ѧ����\n", id);
}