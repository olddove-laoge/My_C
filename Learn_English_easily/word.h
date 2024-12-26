#ifndef WORD_H
#define WORD_H

#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 100

// ����ṹ�� Word
typedef struct {
    char pos[MAX_WORD_LENGTH];      // ���ԣ��ַ�������
    char chinese[MAX_WORD_LENGTH];  // ���ģ��ַ�������
    char english[MAX_WORD_LENGTH];  // Ӣ�ģ��ַ�������
    int correctAnswers;             // �ش���ȷ����
    int learned;                    //�Ƿ�ѧ��
    int index;
} Word;

// ��������
int readWordFromCSV(FILE* file, Word* word);
void selectRandomUnlearnedWords(Word words[], int wordCount, Word selectedWords[], int numSelected);
void selectRandomlearnedWords(Word words[], int wordCount, Word selectedWords[], int numSelected);
int star_words(char* pos, char* chinese, char* english);
int word_exists_in_file(const char* filename, const char* word);
void writeWordsToCSV(const char* filename, Word* words, int wordCount);
void resetLearningData(const char* filename, Word* words, int wordCount);

#endif // WORD_H