#define _CRT_SECURE_NO_WARNINGS 1

#include "score.h"
#include "student.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �ȽϺ��������� qsort ���ִܷӸߵ�������
int compareTotal(const void* a, const void* b) {
    Student* studentA = (Student*)a;
    Student* studentB = (Student*)b;
    return (studentB->total - studentA->total) > 0 ? 1 : -1;
}

// �ȽϺ��������� qsort ���ִܷӵ͵�������
int compareTotalDesc(const void* a, const void* b) {
    Student* studentA = (Student*)a;
    Student* studentB = (Student*)b;
    return (studentB->total - studentA->total) < 0 ? 1 : -1;
}

// �ȽϺ��������� qsort ��ѧ������
int compareId(const void* a, const void* b) {
    Student* studentA = (Student*)a;
    Student* studentB = (Student*)b;
    return studentA->id - studentB->id;
}

// �ȽϺ��������� qsort ����������
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
        printf("�γ� %d �ܷ֣�%.2f, ƽ���֣�%.2f\n", i + 1, totals[i], averages[i]);
    }
}

void rankStudentsByTotal() {
    qsort(students, studentCount, sizeof(Student), compareTotal);
    printf("���ִܷӸߵ���������\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("���� %d: ѧ�� %d, ���� %s, �ܷ� %.2f, ƽ���� %.2f, �ɼ���",
            i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void rankStudentsByTotalDesc() {
    qsort(students, studentCount, sizeof(Student), compareTotalDesc);
    printf("���ִܷӵ͵���������\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("���� %d: ѧ�� %d, ���� %s, �ܷ� %.2f, ƽ���� %.2f, �ɼ���",
            i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void sortStudentsById() {
    qsort(students, studentCount, sizeof(Student), compareId);
    printf("��ѧ������\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("ѧ�� %d, ���� %s, �ܷ� %.2f, ƽ���� %.2f, �ɼ���",
            students[i].id, students[i].name, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void sortStudentsByName() {
    qsort(students, studentCount, sizeof(Student), compareName);
    printf("����������\n");
    for (int i = 0; i < studentCount; ++i) {
        printf("���� %s, ѧ�� %d, �ܷ� %.2f, ƽ���� %.2f, �ɼ���",
            students[i].name, students[i].id, students[i].total, students[i].average);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            printf("%.2f ", students[i].scores[j]);
        }
        printf("\n");
    }
}

void statisticsByGrade() {
    // Ϊÿ����Ŀ����һ���ɼ��ֶμ�������
    int gradeCounts[MAX_SUBJECTS][5] = { {0} }; // 5���ֶΣ����㡢���á��еȡ����񡢲�����

    // ����ͳ��ÿ����Ŀ����ѧ����
    int totalStudents[MAX_SUBJECTS] = { 0 };
    for (int j = 0; j < MAX_SUBJECTS; ++j) {
        totalStudents[j] = studentCount; // ����ÿ����Ŀ������ͬ������ѧ��
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

    printf("����Ŀ�ɼ��ֶ�ͳ�ƣ�\n");
    for (int j = 0; j < MAX_SUBJECTS; ++j) {
        printf("��Ŀ %d: ", j + 1);
        for (int k = 0; k < 5; k++) {
            float percentage = (float)gradeCounts[j][k] / totalStudents[j] * 100;
            printf("%s��%d �� (%.2f%%)\n", k == 0 ? "������" : k == 1 ? "����" : k == 2 ? "�е�" : k == 3 ? "����" : "����", gradeCounts[j][k], percentage);
        }
    }
}
void queryStudentById() {
    int id;
    printf("����Ҫ��ѯ��ѧ��ѧ�ţ�");
    scanf("%d", &id);
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].id == id) {
            printf("���� %d: ѧ�� %d, ���� %s, �ܷ� %.2f, ƽ���� %.2f, �ɼ���",
                i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                printf("%.2f ", students[i].scores[j]);
            }
            printf("\n");
            return;
        }
    }
    printf("δ�ҵ�ѧ��Ϊ %d ��ѧ����\n", id);
}

void queryStudentByName() {
    char name[50];
    printf("����Ҫ��ѯ��ѧ��������");
    scanf("%s", name);
    for (int i = 0; i < studentCount; ++i) {
        if (strcmp(students[i].name, name) == 0) {
            printf("���� %d: ѧ�� %d, ���� %s, �ܷ� %.2f, ƽ���� %.2f, �ɼ���",
                i + 1, students[i].id, students[i].name, students[i].total, students[i].average);
            for (int j = 0; j < MAX_SUBJECTS; ++j) {
                printf("%.2f ", students[i].scores[j]);
            }
            printf("\n");
            return;
        }
    }
    printf("δ�ҵ�����Ϊ %s ��ѧ����\n", name);
}
