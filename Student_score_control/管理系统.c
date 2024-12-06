#define _CRT_SECURE_NO_WARNINGS 1
#include "student.h"
#include "score.h"
#include "fileio.h"
#include <stdio.h>
#include<stdlib.h>

void printMenu() {
    printf("|▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔|\n");
    printf("| 学生成绩管理系统                         |\n");
    printf("|------------------------------------------|\n");
    printf("| 1. 管理学生信息                          |\n");
    printf("| 2. 计算总分&平均分                       |\n");
    printf("| 3. 排序学生信息                          |\n");
    printf("| 4. 查询学生排名和成绩                    |\n");
    printf("| 5. 读取或保存文件                        |\n");
    printf("| 6. 退出系统                              |\n");
    printf("|------------------------------------------|\n");
    printf("| 请输入选项：                             |\n");
    printf("|__________________________________________|\n");
}

int main() {
    int choice, subChoice;
    do {
        printMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\n管理学生信息\n");
                printf("1. 创建学生基本信息\n");
                printf("2. 修改学生成绩\n");
                printf("3. 删除学生\n");
                printf("请输入选项：");
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
                        printf("无效选项，请重新输入。\n");
                }
                break;
            case 2:
                printf("\n计算总分&平均分\n");
                printf("1. 计算每个学生各门课程的总分和平均分\n");
                printf("2. 计算每门课程的总分和平均分\n");
                printf("请输入选项：");
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
                        printf("无效选项，请重新输入。\n");
                        system("pause");
                }
                break;
            case 3:
                printf("\n排序学生信息\n");
                printf("1. 按学号排序\n");
                printf("2. 按姓名排序\n");
                printf("3. 按总分排序\n");
                printf("4.各科分数分段排序\n");
                printf("请输入选项：");
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
                        printf("1.从大到小");
                        printf("2.从小到大");
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
                        printf("无效选项，请重新输入。\n");
                }
                break;
            case 4:
                printf("\n查询学生排名和成绩\n");
                printf("1. 按学号查询学生排名及其考试成绩\n");
                printf("2. 按姓名查询学生排名及其考试成绩\n");
                printf("请输入选项：");
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
                        printf("无效选项，请重新输入。\n");
                }
                break;
            case 5:
                printf("\n读取或保存文件\n");
                printf("1. 保存学生信息到文件\n");
                printf("2. 从文件加载学生信息\n");
                printf("请输入选项：");
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
                        printf("无效选项，请重新输入。\n");
                }
                break;
            case 6:
                printf("退出系统。\n");
                break;
            default:
                printf("无效选项，请重新输入。\n");
        }
        system("cls");
    } while (choice != 6);

    return 0;
}