#ifndef WORD_H
#define WORD_H

#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

// 定义结构体 Word
typedef struct {
    char pos[MAX_WORD_LENGTH];      // 词性，字符串类型
    char chinese[MAX_WORD_LENGTH];  // 中文，字符串类型
    char english[MAX_WORD_LENGTH];  // 英文，字符串类型
    int correctAnswers;             // 回答正确次数
    int learned;                    //是否学会
    int index;
} Word;

// 函数声明
int readWordFromCSV(FILE* file, Word* word);
void selectRandomUnlearnedWords(Word words[], int wordCount, Word selectedWords[], int numSelected);
void selectRandomlearnedWords(Word words[], int wordCount, Word selectedWords[], int numSelected);
int star_words(char* pos, char* chinese, char* english);
int word_exists_in_file(const char* filename, const char* word);
void writeWordsToCSV(const char* filename, Word* words, int wordCount);
void resetLearningData(const char* filename, Word* words, int wordCount);

#endif // WORD_H