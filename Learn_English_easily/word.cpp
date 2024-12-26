#define _CRT_SECURE_NO_WARNINGS 1
#include "word.h"
#include <time.h>
#include <stdlib.h>

// 函数用于从CSV文件中读取数据并填充Word结构体
int readWordFromCSV(FILE* file, Word* word) {
    char line[MAX_WORD_LENGTH * 4]; // 假设每行不超过400字符
    if (fgets(line, sizeof(line), file) == NULL) {
        return 0; // 文件结束或错误
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
    word->correctAnswers = atoi(token); // 将字符串转换为整数

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    word->learned = atoi(token); // 将字符串转换为整数

    token = strtok(NULL, ",");
    if (token == NULL) return 0;
    word->index = atoi(token); // 将字符串转换为整数

    return 1; // 成功读取
}


void selectRandomUnlearnedWords(Word words[], int wordCount, Word selectedWords[], int numSelected) {
    srand((unsigned int)time(NULL)); // 初始化随机数种子
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
    srand((unsigned int)time(NULL)); // 初始化随机数种子
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
                    selectedWords[i].correctAnswers = 0; // 重置答对次数
                    selectedWords[i].learned = false; // 重置为未学习状态
                    currentIndex++;
                    break;
                }
                currentIndex++;
            }
        }
    }
}

// 检查单词是否已存在于文件中
int word_exists_in_file(const char* filename, const char* word) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0; // 返回0表示文件打开失败，不表示单词存在
    }
    char line[1024]; // 假设每行不超过1024个字符
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, word)) { // 如果找到单词
            fclose(file);
            return 1; // 单词存在
        }
    }
    fclose(file);
    return 0; // 单词不存在
}

int star_words(char* pos, char* chinese, char* english) {
    const char* filename = "D:/stared_english.csv";
    // 检查单词是否已存在
    if (word_exists_in_file(filename, english)) {
       
        return 1; // 单词已存在，返回1
    }

    FILE* file1 = fopen(filename, "a");
    if (file1 == NULL) {
        perror("Error opening file");
        return 1;
    }
    // 检查 fprintf 是否成功
    if (fprintf(file1, "%s,%s,%s\n", pos, chinese, english) < 0) {
        perror("Error writing to file");
        fclose(file1);
        return 1;
    }
    // 成功写入后关闭文件
    fclose(file1);
    return 0;
}

void writeWordsToCSV(const char* filename, Word* words, int wordCount) {
    FILE* file = fopen(filename, "w"); // 打开文件以写入，如果文件不存在则创建
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // 写入列头
    fprintf(file, "pos,chinese,english,correctAnswers,learned,index\n");

    for (int i = 0; i < wordCount; i++) {
        fprintf(file, "%s,%s,%s,%d,%d,%d\n",
            words[i].pos, words[i].chinese, words[i].english,
            words[i].correctAnswers, words[i].learned, words[i].index);
    }

    fclose(file); // 关闭文件
    
}

void resetLearningData(const char* filename, Word* words, int wordCount) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    // 重置学习数据
    for (int i = 0; i < wordCount; i++) {
        words[i].learned = 0;
        words[i].correctAnswers = 0;
    }
    // 写入重置后的数据
    for (int i = 0; i < wordCount; i++) {
        fprintf(file, "%s,%s,%s,%d,%d,%d\n",
            words[i].pos, words[i].chinese, words[i].english,
            words[i].correctAnswers, words[i].learned, words[i].index);
    }

    fclose(file);
    printf("Learning data has been cleared!\n");
}