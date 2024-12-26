#define _CRT_SECURE_NO_WARNINGS 1
#include <stdlib.h> // ����stdlib.h��ʹ��EXIT_FAILUREw
#include <easyx.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "word.h"
#include <conio.h>
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000 // ���������1000������

Word words[MAX_WORDS]; // �洢���е���
int wordCount = 0;
Word selectedWords[10]; // �洢10��ѡ�еĵ���
int currentWordIndex = 0; // ��ǰ��ʾ�ĵ�������
bool skipButton = false; // ���ơ�skip����ť�Ƿ�ɼ��ı�־λ
char inputWord[MAX_WORD_LENGTH]; // �洢�û������Ӣ�ĵ���
bool inputVisible = true; // ����������Ƿ�ɼ��ı�־λ
bool isStartVisible = false;
bool isSearchVisible = false;
bool isProgressVisible = false;
bool allLearned = false;
bool checkWord(const char* userWord, const char* correctWord) {
    return strcmp(userWord, correctWord) == 0;
}
char searchWord[MAX_WORD_LENGTH];

// ��ť�ṹ�嶨��
typedef struct {
    int x, y, w, h; // ��ť��λ�úʹ�С
    const char* text; // ��ť����ʾ���ı�
    bool need_tip;
    const char* tooltip; // ��ť����ʾ��Ϣ
} Button;

bool setting_button = false;
bool clear_button = false;
bool isMenuVisible = true; // ���Ʋ˵��Ƿ�ɼ��ı�־λ

ExMessage msg = { 0 };



// ȫ�ֱ���infoArray
char** infoArray = NULL;
void drawText() {
    settextstyle(20, 0, "bauhaus 93");
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
}

bool inArea(int mx, int my, int x, int y, int w, int h) {
    return mx > x && mx < x + w && my > y && my < y + h;
}

void drawButton(Button btn) {
    settextstyle(20, 0, "bauhaus 93");
    if (inArea(msg.x, msg.y, btn.x, btn.y, btn.w, btn.h)) {
        setfillcolor(RGB(93, 107, 153));
        settextcolor(WHITE);
    }
    else {
        setfillcolor(RGB(230, 231, 232));
        settextcolor(BLACK);
    }
    fillroundrect(btn.x, btn.y, btn.x + btn.w, btn.y + btn.h, 5, 5);
    int hSpace = (btn.w - textwidth(btn.text)) / 2;
    int vSpace = (btn.h - textheight(btn.text)) / 2;
    outtextxy(btn.x + hSpace, btn.y + vSpace, btn.text);

    // ���������ͣʱ������ʾ��Ϣ
    if (inArea(msg.x, msg.y, btn.x, btn.y, btn.w, btn.h) && btn.need_tip == true) {
        setfillcolor(RGB(173, 216, 230)); // ǳ��ɫ
        settextcolor(RGB(0, 0, 128)); // ����ɫ
        settextstyle(16, 0, "Arial");
        fillroundrect(btn.x + btn.w + 10, btn.y, btn.x + btn.w + textwidth(btn.tooltip) + 20, btn.y + textheight(btn.tooltip) + 10, 5, 5);
        outtextxy(btn.x + btn.w + 15, btn.y + 5, btn.tooltip);
    }
}

void background() {
    initgraph(500, 707, EX_SHOWCONSOLE);
    setlinecolor(RGB(128, 128, 128));
    setlinestyle(PS_SOLID, 1);
    setbkcolor(WHITE);
    cleardevice();
    loadimage(NULL, _T("C:/Users/lyh23/Pictures/�ϲ���ѧ.jpg"));
    drawText();
    settextstyle(48, 0, "Showcard Gothic");
    int title_x = (500 - textwidth("Recite English Easily!!!")) / 2;
    int title_y = (500 - textheight("Recite English Easily!!!")) / 2;
    outtextxy(title_x, title_y, "Recite English Easily!!!");
}


void drawReturnButton() {
    Button returnBtn = { 10, 10, 100, 40, "RETURN", false, "Back to menu" };
    drawText();
    drawButton(returnBtn);
}

void clearCurrentWordArea() {
    setfillcolor(WHITE); // ���������ɫΪ��ɫ���Ը���֮ǰ���ı�
    // ������Ҫ���������������赥����ʾ�����߶�Ϊ100����
    int clearHeight = 100;
    clearrectangle(110, 200, 500, 200 + clearHeight); // ֻ������ʵ�����
}

void getUserInput() {
    inputVisible = true; // ��ʾ�����
    char userWord[MAX_WORD_LENGTH];
    // ��ʾ����򲢻�ȡ�û�����
    InputBox(userWord, MAX_WORD_LENGTH, "Enter the word");
    if (userWord != NULL) {
        // ����û�����ĵ����Ƿ���ȷ
        if (checkWord(userWord, selectedWords[currentWordIndex].english)) {
            // ������ȷ
            printf("Correct!\n");
        }
        else {
            // ���ʴ���
            printf("Wrong! The correct word is: %s\n", selectedWords[currentWordIndex].english);
        }
        inputVisible = false; // ���������
    }
}

void drawCurrentWord() {
    if (!isMenuVisible && isStartVisible) { // ֻ�� START �������
        settextstyle(48, 0, "΢���ź�");
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);

        // ���֮ǰ���ʵ�����
        
        clearCurrentWordArea();

        int centerX = 500 / 2;
        int centerY = 250; // �̶�����λ��

        // ������Ժ�����������ܿ��
        int totalWidth = textwidth(selectedWords[currentWordIndex].pos) + textwidth(selectedWords[currentWordIndex].chinese) + 10;

        // ������Ե�λ�ã�ʹ��λ��������������
        int posX = centerX - (totalWidth / 2);
        int posY = centerY;

        // �������������λ�ã�ʹ��λ�ڴ��Ե��ұ�
        int chineseX = posX + textwidth(selectedWords[currentWordIndex].pos) + 5;
        int chineseY = centerY;

        // ���ƴ���
        outtextxy(posX, posY, selectedWords[currentWordIndex].pos);

        // ������������
        outtextxy(chineseX, chineseY, selectedWords[currentWordIndex].chinese);

        // ���������
        settextstyle(20, 0, "΢���ź�");
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        setfillcolor(RGB(240, 240, 240));
        fillrectangle(100, 300, 400, 340);
        setlinecolor(RGB(160, 160, 160));
        rectangle(100, 300, 400, 340);

        // ������ʾ�ı�
        outtextxy(105, 305, "Type the English word:");

        // ��������ɼ������Ƶ�ǰ����ĵ���
        if (strlen(inputWord) * textwidth(' ') < 300) {
            outtextxy(105 + textwidth("Type the English word:"), 305, inputWord);
        }
        else {
            int start = strlen(inputWord) - (300 / textwidth(' '));
            char tempWord[MAX_WORD_LENGTH];
            strncpy(tempWord, inputWord + start, sizeof(tempWord));
            tempWord[sizeof(tempWord) - 1] = '\0';
            outtextxy(105 + textwidth("Type the English word:"), 305, tempWord);
        }
    }
}


void updateWordsFromSelected() {
    for (int i = 0; i < 10; i++) {
        int originalIndex = selectedWords[i].index; // ����ÿ��Word��һ��ָ��words������ԭʼ���ʵ�����
        words[originalIndex].learned = selectedWords[i].learned;
        words[originalIndex].correctAnswers = selectedWords[i].correctAnswers;
    }
}

void reviewUnlearnedWords() {
    allLearned = true;
    for (int i = 0; i < 10; i++) {
        if (!selectedWords[i].learned) {
            allLearned = false;
            currentWordIndex = i;
            drawCurrentWord();
            inputVisible = true;
            break;
        }
    }
    if (allLearned) {
        updateWordsFromSelected();
        printf("All words have been learned. Starting a new cycle.\n");
        clear_button = false; // ���ñ�־λ����ʾ��ťӦ����ʾ
        isMenuVisible = true; // �˵��ɼ�
        setting_button = false; // �������ð�ť״̬
        background(); // �ػ�˵�����u
    }
}


// ����ѭ���д����û�����
void processInput() {
    
    if (!isMenuVisible && inputVisible && isStartVisible) {
        
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\b') {
                if (strlen(inputWord) > 0) {
                    inputWord[strlen(inputWord) - 1] = '\0';
                    clearrectangle(105 + textwidth("Type the English word:"), 305, 500, 340); // ���������е�����
                    outtextxy(105 + textwidth("Type the English word:"), 305, inputWord); // ��ʾ���º�����������
                }
            }
            else if (ch == '\r') {
                setfillcolor(WHITE); // ���������ɫΪ��ɫ�������֮ǰ�����
                clearrectangle(100, 350, 400, 390); // ���֮ǰ���������

                if (checkWord(inputWord, selectedWords[currentWordIndex].english)) {
                    if (!selectedWords[currentWordIndex].learned) {
                        selectedWords[currentWordIndex].correctAnswers++;
                        char correctMsg[MAX_WORD_LENGTH * 2];
                        sprintf(correctMsg, "Correct! (%d/3)", selectedWords[currentWordIndex].correctAnswers);
                        settextstyle(20, 0, "΢���ź�");
                        settextcolor(BLACK);
                        setbkmode(TRANSPARENT);
                        outtextxy(100, 350, correctMsg);
                        if (selectedWords[currentWordIndex].correctAnswers >= 3) {
                            selectedWords[currentWordIndex].learned = true;
                            outtextxy(100, 370, "This word has been learned.");
                        }
                    }
                    else {
                        outtextxy(100, 350, "This word is already learned.");
                    }
                }
                else {
                    char errorMsg[MAX_WORD_LENGTH * 2];
                    sprintf(errorMsg, "Wrong! The correct word is: %s", selectedWords[currentWordIndex].english);
                    settextstyle(20, 0, "΢���ź�");
                    settextcolor(BLACK);
                    setbkmode(TRANSPARENT);
                    outtextxy(100, 350, errorMsg);
                    selectedWords[currentWordIndex].correctAnswers = 0; // ���һ�Σ���Դ�������
                    selectedWords[currentWordIndex].learned = false; // ���һ�Σ�����Ϊδѧ��״̬
                }
                strcpy(inputWord, ""); // ��������
                inputVisible = false;
                if (currentWordIndex < 9) {
                    currentWordIndex++;
                    clearrectangle(300, 500, 500, 598);
                    drawCurrentWord(); // ������һ������
                    inputVisible = true; // ������ʾ�����
                }
                else {
                    // ���е��ʶ��ѻش�׼����������δѧ��ĵ���
                    currentWordIndex = 0;
                    reviewUnlearnedWords();
                }
            }
            else if (ch < 128) {
                int len = strlen(inputWord);
                if (len < MAX_WORD_LENGTH - 1) {
                    inputWord[len] = ch;
                    inputWord[len + 1] = '\0';
                    clearrectangle(105 + textwidth("Type the English word:"), 305, 500, 340); // ���������е�����
                    outtextxy(105 + textwidth("Type the English word:"), 305, inputWord); // ��ʾ���º�����������
                }
            }
        }
        if (inArea(msg.x, msg.y, 400, 650, 100, 40) && msg.message == WM_LBUTTONDOWN) {
            if (currentWordIndex < 9) {
                selectedWords[currentWordIndex].learned = true;
                currentWordIndex++;
                clearCurrentWordArea(); // �����ǰ��������
                clearrectangle(300, 500, 500, 598);
                strcpy(inputWord, ""); // ��������
                drawCurrentWord(); // ������һ������

            }
            else if (currentWordIndex == 9) {
                selectedWords[currentWordIndex].learned = true;
                reviewUnlearnedWords();
                if (allLearned) {
                    updateWordsFromSelected();
                    clear_button = false; // ���ñ�־λ����ʾ��ťӦ����ʾ
                    isMenuVisible = true; // �˵��ɼ�
                    setting_button = false; // �������ð�ť״̬
                    background(); // �ػ�˵�����
                }

            }
        }
       
    }
}

void searchInput() {
    settextstyle(20, 0, "΢���ź�");
    settextcolor(BLACK);
    setbkmode(TRANSPARENT); // ���ñ���ģʽΪ͸��
    
    if (!isMenuVisible && isSearchVisible) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\b') {
                if (strlen(searchWord) > 0) {
                    searchWord[strlen(searchWord) - 1] = '\0';
                    setbkcolor(RGB(240, 240, 240));
                    clearrectangle(105 + textwidth("Type the English word:"), 301, 399, 339); // ���������е�����
                    outtextxy(105 + textwidth("Type the English word:"), 305, searchWord); // ��ʾ���º�����������
                    
                }
            }
            else if (ch == '\r' ) {
                setbkcolor(WHITE);
                setfillcolor(WHITE); // ���������ɫΪ��ɫ�������֮ǰ�����
                setbkcolor(RGB(240, 240, 240));
                solidrectangle(100, 100, 500, 298);
                solidrectangle(0, 350, 500, 700);
                solidrectangle(0, 350, 500, 700);
                for (int i = 0; i < MAX_WORDS; i++) {
                    if (strcmp(searchWord, words[i].english) == 0) { // ʹ�� strcmp �����ַ����Ƚ�
                        settextstyle(48, 0, "΢���ź�");
                        int centerX = 500 / 2;
                        int centerY = 250; // �̶�����λ��

                        // ������Ժ�����������ܿ��
                        int totalWidth = textwidth(words[i].pos) + textwidth(words[i].chinese) + 10;

                        // ������Ե�λ�ã�ʹ��λ��������������
                        int posX = centerX - (totalWidth / 2);
                        int posY = centerY;

                        // �������������λ�ã�ʹ��λ�ڴ��Ե��ұ�
                        int chineseX = posX + textwidth(words[i].pos) + 5;
                        int chineseY = centerY;
                        // ���ƴ���
                        outtextxy(posX, posY, words[i].pos);

                        // ������������
                        outtextxy(chineseX, chineseY, words[i].chinese);
                        if (words[i].learned) {
                            settextstyle(24, 0, "΢���ź�");
                            int wSpace = (500 - textwidth("Learned"))/2;
                            outtextxy(wSpace, 350, "Learned");
                        }
                        else {
                            settextstyle(24, 0, "΢���ź�");
                            int wSpace = (500 - textwidth("Unlearned")) / 2;
                            outtextxy(wSpace, 350, "Unlearned");
                        }

                    }
                    else {
                        int wSpace = (500 - textwidth("No result")) / 2;
                        outtextxy(wSpace, 350, "No Result");
                    }
                }
                strcpy(searchWord, ""); // ��������
                clearrectangle(256 , 301, 399, 339); // ���������е�����
            }
            else if (ch < 128) {
                int len = strlen(searchWord);
                if (len < MAX_WORD_LENGTH - 1) {
                    searchWord[len] = ch;
                    searchWord[len + 1] = '\0';
                    outtextxy(105 + textwidth("Type the English word:"), 305, searchWord); // ��ʾ���º�����������
                }
            }
        }
    }
}

void drawSkipButton() {
    Button skipBtn = { 400, 650, 100, 40, "SKIP", false, "" };
    drawText();
    drawButton(skipBtn);
}

void drawStarButton() {
    Button starBtn = { 400, 600, 100, 40, "STAR", true, "Mark this word as difficult" };
    drawText();
    drawButton(starBtn);
}


void switchToStartScreen() {
    clear_button = true;
    isMenuVisible = false;

    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton();
    drawSkipButton();
    drawStarButton();
    // ����ѡ��10������
    selectRandomUnlearnedWords(words, wordCount, selectedWords, 10);
    currentWordIndex = 0;
    inputVisible = true; // ��ʾ�����
    strcpy(inputWord, ""); // ������������

    // ��������ά����
    for (int i = 0; i < 10; i++) {
        sprintf(infoArray[i * 3], "%s", selectedWords[i].english);
        sprintf(infoArray[i * 3 + 1], "%s", selectedWords[i].pos);
        sprintf(infoArray[i * 3 + 2], "%s", selectedWords[i].chinese);
    }
    drawCurrentWord();
    drawStarButton();
}

void starWordOnButtonClick() {
    if (inArea(msg.x, msg.y, 400, 600, 100, 40) && msg.message == WM_LBUTTONDOWN) {
        star_words(selectedWords[currentWordIndex].pos, selectedWords[currentWordIndex].chinese, selectedWords[currentWordIndex].english);
        settextstyle(24,0, "΢���ź�");
        outtextxy(355, 575, "Star successfully!");
        
    }
}

void switchToStarBookScreen() {
    ShellExecute(NULL, "open", "notepad","D:/stared_english.csv",  NULL, SW_SHOW);
}


void switchToProgressScreen() {
    clear_button = true; // ���ñ�־λ����ʾ��ťӦ����ʧ
    isMenuVisible = false; // �˵����ɼ�
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton(); // ���Ʒ��ذ�ť
    if (isProgressVisible) {
        int learned_words = 0;
        int unlearn_words = 0;
        for (int i = 0; i < 496; i++) {
            if (words[i].learned) {
                learned_words++;
            }
            else {
                unlearn_words++;
            }
        }
        char progressString[100]; // ȷ����������㹻���Դ��ƴ�Ӻ���ַ���
        sprintf(progressString, "%d/%d", learned_words, unlearn_words);
        settextstyle(32, 0, "΢���ź�");
        int wSpace = (500 - textwidth(progressString)) / 2;
        outtextxy(wSpace, 300, progressString);
        int total_words = learned_words + unlearn_words;



        // ������ɫ�ͻ�ɫ���ֵĳ���
        float greenLength = (float)learned_words / total_words * 300;
        float grayLength = (float)unlearn_words / total_words * 300;


        // ������ѧ���ʵ���ɫ����
        setfillcolor(GREEN);
        fillrectangle(100, 400, 100 + greenLength, 430);

        // ����δѧ���ʵĻ�ɫ����
        setfillcolor(RGB(240, 240, 240));
        fillrectangle(100 + greenLength, 400, 400, 430);
    }
    
}

void switchToAboutScreen() {
    clear_button = true; // ���ñ�־λ����ʾ��ťӦ����ʧ
    isMenuVisible = false; // �˵����ɼ�
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(RGB(249, 231, 255));
    cleardevice();
    drawReturnButton(); // ���Ʒ��ذ�ť
    settextstyle(48, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(340, 30, "ABOUT");
    settextstyle(24, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 60, "FUNCTION INTRODUCTION:");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 100, "The program is made to help students to learn English more easily.");
    outtextxy(25, 120, "You can click the button in the menu to choose different function.");
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 140, "START:");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 160, "Cilck START to start your English learning journey! ");
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 180, "STARBOOK:");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 200, "Click STARBOOK to review the words you have stared");
    outtextxy(25, 220, "during the process of reciting so that you can have a");
    outtextxy(25, 240, "better understanding of some difficult and complict words.");
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 260, "PROCESS");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 280, "You can see how many words you have learned here and");
    outtextxy(25, 300, "view the concrete.");
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 320, "SETTINGS");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 340, "You can change some system settings here such as");
    outtextxy(25, 360, "saving your learning data and so on.");
    settextstyle(24, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 400, "LEARNING RULES");
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 430, "LEARNING:");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 450, "In this program,every words should be  recited third times");
    outtextxy(25, 470, "correctly and continuely to be passed. Once you make a mistake,");
    outtextxy(25, 490, "the number of correct times will be cleared to zero.");
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 510, "STAR:");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 530, "You can click STAR button ton star a word you think difficult.");
    outtextxy(25, 550, "Through this, you can have a better understanding of complicate words.");
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 570, "SKIP");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 590, "Click SKIP to skip the words you think you have grasped.");
    outtextxy(25, 610, "And the words will be recognized as having been learned.");
    settextstyle(24, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 640, "PRODUCER:");
    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 670, "LiYuhang");

}

void switchToSettingsScreen() {
    clear_button = true; // ���ñ�־λ����ʾ��ťӦ����ʧ
    isMenuVisible = false; // �˵����ɼ�
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton(); // ���Ʒ��ذ�ť
    setting_button = true; // ���ð�ť״̬Ϊ�ɼ�
}

void switchToReviewScreen() {
    clear_button = true;
    isMenuVisible = false;

    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton();
    drawSkipButton();
    drawStarButton();

    // ����ѡ����ѧϰ�ĵ���
    selectRandomlearnedWords(words, wordCount, selectedWords, 10);
    currentWordIndex = 0;
    inputVisible = true; // ��ʾ�����
    strcpy(inputWord, ""); // ������������

    // ��������ά����
    for (int i = 0; i < 10; i++) {
        sprintf(infoArray[i * 3], "%s", selectedWords[i].english);
        sprintf(infoArray[i * 3 + 1], "%s", selectedWords[i].pos);
        sprintf(infoArray[i * 3 + 2], "%s", selectedWords[i].chinese);
    }
    drawCurrentWord();
    drawStarButton();
}

void switchToSearchScreen() {
    clear_button = true; // ���ñ�־λ����ʾ��ťӦ����ʧ
    isMenuVisible = false; // �˵����ɼ�
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton(); // ���Ʒ��ذ�ť

}

void returnToMenu() {
    if (inArea(msg.x, msg.y, 10, 10, 100, 40) && msg.message == WM_LBUTTONDOWN) {
        clear_button = false; // ���ñ�־λ����ʾ��ťӦ����ʾ
        isMenuVisible = true; // �˵��ɼ�
        setting_button = false; // �������ð�ť״̬
        isStartVisible = false;
        isSearchVisible = false;
        background(); // �ػ�˵�����
    }
}
void start_to_learn() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 400, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        switchToStartScreen();
    }
}

void star_to_review() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 435, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        switchToStarBookScreen();

    }
}

void view_progress() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 470, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        switchToProgressScreen();
    }
}

void review_words() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 540, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        switchToReviewScreen();
    }
}

void search_words() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 575, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        switchToProgressScreen();
        settextstyle(20, 0, "΢���ź�");
        settextcolor(BLACK);
        setfillcolor(RGB(240, 240, 240));
        fillrectangle(100, 300, 400, 340);
        setlinecolor(RGB(160, 160, 160));
        rectangle(100, 300, 400, 340);
        outtextxy(105, 305, "Type the English word:");
        setbkmode(TRANSPARENT);

    }
}


void about_program() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 505, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        switchToAboutScreen();
    }
}

Button clearDataBtn = { 180, 400, 160, 40, "Clear learning data", false, "" };
Button saving = { 180, 200, 160, 40, "Save learning data", false, "" };

void settings() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 610, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        switchToSettingsScreen();
    }
}


void exit_program() {
    if (isMenuVisible && inArea(msg.x, msg.y, 180, 640, 140, 35) && msg.message == WM_LBUTTONDOWN) {
        closegraph();
        exit(0);
    }
}

void clearTooltipArea() {
    // �����ʾ��Ϣ�������������ʾ��Ϣ�������Ϊ200���߶�Ϊ50
    setfillcolor(WHITE);
    clearrectangle(320, 400, 500, 700); // ����ʵ����Ҫ�����������
}

int main() {

    FILE* file = fopen("D:/C_data/practice9/final_task/english.csv", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    while (readWordFromCSV(file, &words[wordCount])) {
        wordCount++;
    }
    fclose(file);

    // ��ʼ��infoArray
    infoArray = (char**)malloc(10 * 3 * sizeof(char*));
    for (int i = 0; i < 10 * 3; i++) {
        infoArray[i] = (char*)malloc(MAX_WORD_LENGTH * sizeof(char));
    }

    background();

    Button startBtn = { 180, 400, 140, 35, "START TO LEARN", true, "Tap to learn English!" };
    Button starBtn = { 180, 435, 140, 35, "STAR BOOK", true, "Review words you stared." };
    Button progressBtn = { 180, 470, 140, 35, "PROGRESS", true, "Tap to view progress." };
    Button aboutBtn = { 180, 505, 140, 35, "ABOUT", true, "About this program." };
    Button reviewBtn = { 180, 540, 140, 35, "REVIEW", true, "Review the words learned" };
    Button searchBtn = { 180, 575, 140, 35, "SEARCH", true, "search words" };
    Button settingBtn = { 180, 610, 140, 35, "SETTINGS", true, "Change some settings" };
    Button exitBtn = { 180, 645, 140, 35, "EXIT", false, "Tap to exit the program" };

    
    while (true) {
        setbkmode(TRANSPARENT);
        msg.message = 0; // ������Ϣ
        if (peekmessage(&msg, EX_MOUSE)) { // ����Ƿ��������Ϣ
            // ����˵�����İ�ť����¼�
            if (isMenuVisible) {
                
                if (inArea(msg.x, msg.y, startBtn.x, startBtn.y, startBtn.w, startBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = true;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    strcpy(inputWord, ""); // ��������
                    start_to_learn();
                }
                else if (inArea(msg.x, msg.y, starBtn.x, starBtn.y, starBtn.w, starBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    star_to_review();
                }
                else if (inArea(msg.x, msg.y, progressBtn.x, progressBtn.y, progressBtn.w, progressBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = true;
                    view_progress();
                }
                else if (inArea(msg.x, msg.y, reviewBtn.x, reviewBtn.y, reviewBtn.w, reviewBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = true;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    review_words();
                }
                else if (inArea(msg.x, msg.y, searchBtn.x, searchBtn.y, searchBtn.w, searchBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isSearchVisible = true;
                    isStartVisible = false;
                    isProgressVisible = false;
                    search_words();
                }
                else if (inArea(msg.x, msg.y, aboutBtn.x, aboutBtn.y, aboutBtn.w, aboutBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    about_program();
                }
                else if (inArea(msg.x, msg.y, settingBtn.x, settingBtn.y, settingBtn.w, settingBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    settings();
                }
                else if (inArea(msg.x, msg.y, exitBtn.x, exitBtn.y, exitBtn.w, exitBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    exit_program();
                }
            }
            else {
                // ����ǲ˵�����İ�ť����¼�
               
                if (inArea(msg.x, msg.y, 400, 600, 100, 40) && msg.message == WM_LBUTTONDOWN) {
                    starWordOnButtonClick();
                }
                else if (inArea(msg.x, msg.y, 10, 10, 100, 40) && msg.message == WM_LBUTTONDOWN) {
                    returnToMenu();
                }
            }
        }

        BeginBatchDraw();
        if (isMenuVisible && !clear_button) {
            clearTooltipArea();
            drawButton(startBtn);
            drawButton(starBtn);
            drawButton(progressBtn);
            drawButton(aboutBtn);
            drawButton(reviewBtn);
            drawButton(searchBtn);
            drawButton(settingBtn);
            drawButton(exitBtn);
        }
        if (clear_button) {
            drawReturnButton();
        }
        if (setting_button) {
            drawButton(saving);
            drawButton(clearDataBtn);
            if (setting_button) {
                if (inArea(msg.x, msg.y, 180, 200, 160, 40) && msg.message == WM_LBUTTONDOWN) {
                    writeWordsToCSV("D:/C_data/practice9/final_task/english.csv", words, wordCount);
                    int wSpace = (500 - textwidth("Save learning data successfully!"))/2;
                    settextstyle(24, 0, "΢���ź�");
                    outtextxy(wSpace, 300, "Save learning data successfully!");
                }

                if (inArea(msg.x, msg.y, 180, 400, 160, 40) && msg.message == WM_LBUTTONDOWN) {
                    resetLearningData("D:/C_data/practice9/final_task/english.csv", words, wordCount);
                    settextstyle(24, 0, "΢���ź�");
                    settextcolor(BLACK);
                    int wSpace = (500 - textwidth("Learning data has been cleared!")) / 2;
                    outtextxy(wSpace, 500, "Learning data has been cleared!");
                }
            }
        }
        processInput();
        searchInput();
        drawCurrentWord();
        EndBatchDraw();
    }

    // �ͷ�infoArray������ڴ�
    for (int i = 0; i < 10 * 3; i++) {
        free(infoArray[i]);
    }
    free(infoArray);

    return 0;
}


