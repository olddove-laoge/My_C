#define _CRT_SECURE_NO_WARNINGS 1
#include "student.h"
#include "score.h"
#include "fileio.h"
#include <stdio.h>
#include<stdlib.h>

void printMenu() {
    printf("|������������������������������������������������������������������������������������|\n");
    printf("| ѧ���ɼ�����ϵͳ                         |\n");
    printf("|------------------------------------------|\n");
    printf("| 1. ����ѧ����Ϣ                          |\n");
    printf("| 2. �����ܷ�&ƽ����                       |\n");
    printf("| 3. ����ѧ����Ϣ                          |\n");
    printf("| 4. ��ѯѧ�������ͳɼ�                    |\n");
    printf("| 5. ��ȡ�򱣴��ļ�                        |\n");
    printf("| 6. �˳�ϵͳ                              |\n");
    printf("|------------------------------------------|\n");
    printf("| ������ѡ�                             |\n");
    printf("|__________________________________________|\n");
}

int main() {
    int choice, subChoice;
    do {
        printMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\n����ѧ����Ϣ\n");
                printf("1. ����ѧ��������Ϣ\n");
                printf("2. �޸�ѧ���ɼ�\n");
                printf("3. ɾ��ѧ��\n");
                printf("������ѡ�");
                scanf("%d", &subChoice);
                switch (subChoice) {
                    case 1:
                        createStudentInfo();
                        system("pause");
                        break;
                    case 2:
                        modifyStudentScores();
                        system("pause");
                        break;
                    case 3:
                        deleteStudent();
                        system("pause");
                        break;
                    default:
                        printf("��Чѡ����������롣\n");
                }
                break;
            case 2:
                printf("\n�����ܷ�&ƽ����\n");
                printf("1. ����ÿ��ѧ�����ſγ̵��ֺܷ�ƽ����\n");
                printf("2. ����ÿ�ſγ̵��ֺܷ�ƽ����\n");
                printf("������ѡ�");
                scanf("%d", &subChoice);
                switch (subChoice) {
                    case 1:
                        calculateTotalAndAverage();
                        system("pause");
                        break;
                    case 2:
                        calculateSubjectTotalAndAverage();
                        system("pause");
                        break;
                    default:
                        printf("��Чѡ����������롣\n");
                        system("pause");
                }
                break;
            case 3:
                printf("\n����ѧ����Ϣ\n");
                printf("1. ��ѧ������\n");
                printf("2. ����������\n");
                printf("3. ���ܷ�����\n");
                printf("4.���Ʒ����ֶ�����\n");
                printf("������ѡ�");
                scanf("%d", &subChoice);
                switch (subChoice) {
                    case 1:
                        sortStudentsById();
                        system("pause");
                        break;
                    case 2:
                        sortStudentsByName();
                        system("pause");
                        break;
                    case 3:
                        printf("1.�Ӵ�С");
                        printf("2.��С����");
                        scanf("%d", &subChoice);
                        switch (subChoice) {
                            case 1:
                                rankStudentsByTotal();
                                system("pause");
                                break;
                            case 2:
                                rankStudentsByTotalDesc();
                                system("pause");
                                break;
                        }
                        break;
                    case 4:
                        statisticsByGrade();
                        system("pause");
                        break;
                    default:
                        printf("��Чѡ����������롣\n");
                }
                break;
            case 4:
                printf("\n��ѯѧ�������ͳɼ�\n");
                printf("1. ��ѧ�Ų�ѯѧ���������俼�Գɼ�\n");
                printf("2. ��������ѯѧ���������俼�Գɼ�\n");
                printf("������ѡ�");
                scanf("%d", &subChoice);
                switch (subChoice) {
                    case 1:
                        queryStudentById();
                        system("pause");
                        break;
                    case 2:
                        queryStudentByName();
                        system("pause");
                        break;
                    default:
                        printf("��Чѡ����������롣\n");
                }
                break;
            case 5:
                printf("\n��ȡ�򱣴��ļ�\n");
                printf("1. ����ѧ����Ϣ���ļ�\n");
                printf("2. ���ļ�����ѧ����Ϣ\n");
                printf("������ѡ�");
                scanf("%d", &subChoice);
                switch (subChoice) {
                    case 1:
                        saveStudentsToFile();
                        system("pause");
                        break;
                    case 2:
                        loadStudentsFromFile();
                        system("pause");
                        break;
                    default:
                        printf("��Чѡ����������롣\n");
                }
                break;
            case 6:
                printf("�˳�ϵͳ��\n");
                break;
            default:
                printf("��Чѡ����������롣\n");
        }
        system("cls");
    } while (choice != 6);

    return 0;
}