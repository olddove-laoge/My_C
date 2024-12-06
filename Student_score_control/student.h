#ifndef STUDENT_H
#define STUDENT_H

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 6

typedef struct {
    int id;
    char name[50];
    float scores[MAX_SUBJECTS];
    float total;
    float average;
} Student;

extern Student students[MAX_STUDENTS];
extern int studentCount;

void createStudentInfo();
void calculateTotalAndAverage();
void modifyStudentScores();
void deleteStudent();

#endif // STUDENT_H