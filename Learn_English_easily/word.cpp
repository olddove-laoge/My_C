#define _CRT_SECURE_NO_WARNINGS 1
#include "word.h"
#include <time.h>
#include <stdlib.h>

// �������ڴ�CSV�ļ��ж�ȡ���ݲ����Word�ṹ��
int readWordFromCSV(FILE* file, Word* word) {
    char line[MAX_WORD_LENGTH * 4]; // ����ÿ�в�����400�ַ�
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0; // �ļ����������
    }

    char* token = strtok(line, ",");
    if (token == NULL) return 0;

    strncpy(word->pos, token, MAX_WORD_LENGTH - 1);
    word->pos[MAX_WORD_LENGTH - 1] = '\0';

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    strncpy(word->chinese, token, MAX_WORD_LENGTH - 1);
    word->chinese[MAX_WORD_LENGTH - 1] = '\0';

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    strncpy(word->english, token, MAX_WORD_LENGTH - 1);
    word->english[MAX_WORD_LENGTH - 1] = '\0';

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    word->correctAnswers = atoi(token); // ���ַ���ת��Ϊ����

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    word->learned = atoi(token); // ���ַ���ת��Ϊ����

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    word->index = atoi(token); // ���ַ���ת��Ϊ����

    return 1; // �ɹ���ȡ
}


void selectRandomUnlearnedWords(Word words[], int wordCount, Word selectedWords[], int numSelected) {
    srand((unsigned int)time(NULL)); // ��ʼ�����������
    int unlearnedCount = 0;
    for (int i = 0; i < wordCount; i++) {
        if (!words[i].learned) unlearnedCount++;
    }

    if (unlearnedCount < numSelected) {
        printf("Not enough unlearned words to select.\n");
        return;
    }

    for (int i = 0; i < numSelected; i++) {
        int randomIndex = rand() % unlearnedCount;
        int currentIndex = 0;

        // Find the random index in the array of unlearned words
        for (int j = 0; j < wordCount; j++) {
            if (!words[j].learned) {
                if (currentIndex == randomIndex) {
                    selectedWords[i] = words[j];
                    break;
                }
                currentIndex++;
            }
        }
    }
}

void selectRandomlearnedWords(Word words[], int wordCount, Word selectedWords[], int numSelected) {
    srand((unsigned int)time(NULL)); // ��ʼ�����������
    int learnedCount = 0;
    for (int i = 0; i < wordCount; i++) {
        if (words[i].learned) learnedCount++;
    }

    if (learnedCount < numSelected) {
        printf("Not enough learned words to select.\n");
        return;
    }

    for (int i = 0; i < numSelected; i++) {
        int randomIndex = rand() % learnedCount;
        int currentIndex = 0;

        // Find the random index in the array of learned words
        for (int j = 0; j < wordCount; j++) {
            if (words[j].learned) {
                if (currentIndex == randomIndex) {
                    selectedWords[i] = words[j];
                    selectedWords[i].correctAnswers = 0; // ���ô�Դ���
                    selectedWords[i].learned = false; // ����Ϊδѧϰ״̬
                    currentIndex++;
                    break;
                }
                currentIndex++;
            }
        }
    }
}

// ��鵥���Ƿ��Ѵ������ļ���
int word_exists_in_file(const char* filename, const char* word) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0; // ����0��ʾ�ļ���ʧ�ܣ�����ʾ���ʴ���
    }
    char line[1024]; // ����ÿ�в�����1024���ַ�
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, word)) { // ����ҵ�����
            fclose(file);
            return 1; // ���ʴ���
        }
    }
    fclose(file);
    return 0; // ���ʲ�����
}

int star_words(char* pos, char* chinese, char* english) {
    const char* filename = "D:/stared_english.csv";
    // ��鵥���Ƿ��Ѵ���
    if (word_exists_in_file(filename, english)) {
       
        return 1; // �����Ѵ��ڣ�����1
    }

    FILE* file1 = fopen(filename, "a");
    if (file1 == NULL) {
        perror("Error opening file");
        return 1;
    }
    // ��� fprintf �Ƿ�ɹ�
    if (fprintf(file1, "%s,%s,%s\n", pos, chinese, english) < 0) {
        perror("Error writing to file");
        fclose(file1);
        return 1;
    }
    // �ɹ�д���ر��ļ�
    fclose(file1);
    return 0;
}

void writeWordsToCSV(const char* filename, Word* words, int wordCount) {
    FILE* file = fopen(filename, "w"); // ���ļ���д�룬����ļ��������򴴽�
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // д����ͷ
    fprintf(file, "pos,chinese,english,correctAnswers,learned,index\n");

    for (int i = 0; i < wordCount; i++) {
        fprintf(file, "%s,%s,%s,%d,%d,%d\n",
            words[i].pos, words[i].chinese, words[i].english,
            words[i].correctAnswers, words[i].learned, words[i].index);
    }

    fclose(file); // �ر��ļ�
    
}

void resetLearningData(const char* filename, Word* words, int wordCount) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // ����ѧϰ����
    for (int i = 0; i < wordCount; i++) {
        words[i].learned = 0;
        words[i].correctAnswers = 0;
    }
    // д�����ú������
    for (int i = 0; i < wordCount; i++) {
        fprintf(file, "%s,%s,%s,%d,%d,%d\n",
            words[i].pos, words[i].chinese, words[i].english,
            words[i].correctAnswers, words[i].learned, words[i].index);
    }

    fclose(file);
    printf("Learning data has been cleared!\n");
}