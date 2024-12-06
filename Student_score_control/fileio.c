#define _CRT_SECURE_NO_WARNINGS 1

#include "fileio.h"
#include <stdio.h>

#define FILE_NAME "students.dat"

void saveStudentsToFile() {
    FILE* file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        perror("�޷����ļ�");
        return;
    }
    for (int i = 0; i < studentCount; ++i) {
        fprintf(file, "%d %s", students[i].id, students[i].name);
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            fprintf(file, " %.2f", students[i].scores[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("ѧ����Ϣ�ѱ��浽�ļ���\n");
}

void loadStudentsFromFile() {
    FILE* file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("�޷����ļ�");
        return;
    }
    studentCount = 0;
    while (fscanf(file, "%d %49s", &students[studentCount].id, students[studentCount].name) != EOF) {
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            fscanf(file, "%f", &students[studentCount].scores[j]);
        }
        students[studentCount].total = 0;
        for (int j = 0; j < MAX_SUBJECTS; ++j) {
            students[studentCount].total += students[studentCount].scores[j];
        }
        students[studentCount].average = students[studentCount].total / MAX_SUBJECTS;
        studentCount++;
    }
    fclose(file);
    printf("ѧ����Ϣ�Ѵ��ļ����ء�\n");
}