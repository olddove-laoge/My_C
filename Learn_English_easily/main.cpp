#define _CRT_SECURE_NO_WARNINGS 1
#include <stdlib.h> // 包含stdlib.h以使用EXIT_FAILUREw
#include <easyx.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "word.h"
#include <conio.h>
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 1000 // 假设最多有1000个单词

Word words[MAX_WORDS]; // 存储所有单词
int wordCount = 0;
Word selectedWords[10]; // 存储10个选中的单词
int currentWordIndex = 0; // 当前显示的单词索引
bool skipButton = false; // 控制“skip”按钮是否可见的标志位
char inputWord[MAX_WORD_LENGTH]; // 存储用户输入的英文单词
bool inputVisible = true; // 控制输入框是否可见的标志位
bool isStartVisible = false;
bool isSearchVisible = false;
bool isProgressVisible = false;
bool allLearned = false;
bool checkWord(const char* userWord, const char* correctWord) {
    return strcmp(userWord, correctWord) == 0;
}
char searchWord[MAX_WORD_LENGTH];

// 按钮结构体定义
typedef struct {
    int x, y, w, h; // 按钮的位置和大小
    const char* text; // 按钮上显示的文本
    bool need_tip;
    const char* tooltip; // 按钮的提示信息
} Button;

bool setting_button = false;
bool clear_button = false;
bool isMenuVisible = true; // 控制菜单是否可见的标志位

ExMessage msg = { 0 };



// 全局变量infoArray
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

    // 仅在鼠标悬停时绘制提示信息
    if (inArea(msg.x, msg.y, btn.x, btn.y, btn.w, btn.h) && btn.need_tip == true) {
        setfillcolor(RGB(173, 216, 230)); // 浅蓝色
        settextcolor(RGB(0, 0, 128)); // 深蓝色
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
    loadimage(NULL, _T("C:/Users/lyh23/Pictures/南昌大学.jpg"));
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
    setfillcolor(WHITE); // 设置填充颜色为白色，以覆盖之前的文本
    // 计算需要清除的区域，这里假设单词显示的最大高度为100像素
    int clearHeight = 100;
    clearrectangle(110, 200, 500, 200 + clearHeight); // 只清除单词的区域
}

void getUserInput() {
    inputVisible = true; // 显示输入框
    char userWord[MAX_WORD_LENGTH];
    // 显示输入框并获取用户输入
    InputBox(userWord, MAX_WORD_LENGTH, "Enter the word");
    if (userWord != NULL) {
        // 检查用户输入的单词是否正确
        if (checkWord(userWord, selectedWords[currentWordIndex].english)) {
            // 单词正确
            printf("Correct!\n");
        }
        else {
            // 单词错误
            printf("Wrong! The correct word is: %s\n", selectedWords[currentWordIndex].english);
        }
        inputVisible = false; // 隐藏输入框
    }
}

void drawCurrentWord() {
    if (!isMenuVisible && isStartVisible) { // 只在 START 界面绘制
        settextstyle(48, 0, "微软雅黑");
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);

        // 清除之前单词的区域
        
        clearCurrentWordArea();

        int centerX = 500 / 2;
        int centerY = 250; // 固定中心位置

        // 计算词性和中文释义的总宽度
        int totalWidth = textwidth(selectedWords[currentWordIndex].pos) + textwidth(selectedWords[currentWordIndex].chinese) + 10;

        // 计算词性的位置，使其位于中文释义的左边
        int posX = centerX - (totalWidth / 2);
        int posY = centerY;

        // 计算中文释义的位置，使其位于词性的右边
        int chineseX = posX + textwidth(selectedWords[currentWordIndex].pos) + 5;
        int chineseY = centerY;

        // 绘制词性
        outtextxy(posX, posY, selectedWords[currentWordIndex].pos);

        // 绘制中文释义
        outtextxy(chineseX, chineseY, selectedWords[currentWordIndex].chinese);

        // 绘制输入框
        settextstyle(20, 0, "微软雅黑");
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        setfillcolor(RGB(240, 240, 240));
        fillrectangle(100, 300, 400, 340);
        setlinecolor(RGB(160, 160, 160));
        rectangle(100, 300, 400, 340);

        // 绘制提示文本
        outtextxy(105, 305, "Type the English word:");

        // 如果输入框可见，绘制当前输入的单词
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
        int originalIndex = selectedWords[i].index; // 假设每个Word有一个指向words数组中原始单词的索引
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
        clear_button = false; // 设置标志位，表示按钮应该显示
        isMenuVisible = true; // 菜单可见
        setting_button = false; // 重置设置按钮状态
        background(); // 重绘菜单界面u
    }
}


// 在主循环中处理用户输入
void processInput() {
    
    if (!isMenuVisible && inputVisible && isStartVisible) {
        
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\b') {
                if (strlen(inputWord) > 0) {
                    inputWord[strlen(inputWord) - 1] = '\0';
                    clearrectangle(105 + textwidth("Type the English word:"), 305, 500, 340); // 清除输入框中的内容
                    outtextxy(105 + textwidth("Type the English word:"), 305, inputWord); // 显示更新后的输入框内容
                }
            }
            else if (ch == '\r') {
                setfillcolor(WHITE); // 设置填充颜色为白色，以清除之前的输出
                clearrectangle(100, 350, 400, 390); // 清除之前的输出区域

                if (checkWord(inputWord, selectedWords[currentWordIndex].english)) {
                    if (!selectedWords[currentWordIndex].learned) {
                        selectedWords[currentWordIndex].correctAnswers++;
                        char correctMsg[MAX_WORD_LENGTH * 2];
                        sprintf(correctMsg, "Correct! (%d/3)", selectedWords[currentWordIndex].correctAnswers);
                        settextstyle(20, 0, "微软雅黑");
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
                    settextstyle(20, 0, "微软雅黑");
                    settextcolor(BLACK);
                    setbkmode(TRANSPARENT);
                    outtextxy(100, 350, errorMsg);
                    selectedWords[currentWordIndex].correctAnswers = 0; // 答错一次，答对次数归零
                    selectedWords[currentWordIndex].learned = false; // 答错一次，重置为未学会状态
                }
                strcpy(inputWord, ""); // 清空输入框
                inputVisible = false;
                if (currentWordIndex < 9) {
                    currentWordIndex++;
                    clearrectangle(300, 500, 500, 598);
                    drawCurrentWord(); // 绘制下一个单词
                    inputVisible = true; // 重新显示输入框
                }
                else {
                    // 所有单词都已回答，准备重新提问未学会的单词
                    currentWordIndex = 0;
                    reviewUnlearnedWords();
                }
            }
            else if (ch < 128) {
                int len = strlen(inputWord);
                if (len < MAX_WORD_LENGTH - 1) {
                    inputWord[len] = ch;
                    inputWord[len + 1] = '\0';
                    clearrectangle(105 + textwidth("Type the English word:"), 305, 500, 340); // 清除输入框中的内容
                    outtextxy(105 + textwidth("Type the English word:"), 305, inputWord); // 显示更新后的输入框内容
                }
            }
        }
        if (inArea(msg.x, msg.y, 400, 650, 100, 40) && msg.message == WM_LBUTTONDOWN) {
            if (currentWordIndex < 9) {
                selectedWords[currentWordIndex].learned = true;
                currentWordIndex++;
                clearCurrentWordArea(); // 清除当前单词区域
                clearrectangle(300, 500, 500, 598);
                strcpy(inputWord, ""); // 清空输入框
                drawCurrentWord(); // 绘制下一个单词

            }
            else if (currentWordIndex == 9) {
                selectedWords[currentWordIndex].learned = true;
                reviewUnlearnedWords();
                if (allLearned) {
                    updateWordsFromSelected();
                    clear_button = false; // 设置标志位，表示按钮应该显示
                    isMenuVisible = true; // 菜单可见
                    setting_button = false; // 重置设置按钮状态
                    background(); // 重绘菜单界面
                }

            }
        }
       
    }
}

void searchInput() {
    settextstyle(20, 0, "微软雅黑");
    settextcolor(BLACK);
    setbkmode(TRANSPARENT); // 设置背景模式为透明
    
    if (!isMenuVisible && isSearchVisible) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == '\b') {
                if (strlen(searchWord) > 0) {
                    searchWord[strlen(searchWord) - 1] = '\0';
                    setbkcolor(RGB(240, 240, 240));
                    clearrectangle(105 + textwidth("Type the English word:"), 301, 399, 339); // 清除输入框中的内容
                    outtextxy(105 + textwidth("Type the English word:"), 305, searchWord); // 显示更新后的输入框内容
                    
                }
            }
            else if (ch == '\r' ) {
                setbkcolor(WHITE);
                setfillcolor(WHITE); // 设置填充颜色为白色，以清除之前的输出
                setbkcolor(RGB(240, 240, 240));
                solidrectangle(100, 100, 500, 298);
                solidrectangle(0, 350, 500, 700);
                solidrectangle(0, 350, 500, 700);
                for (int i = 0; i < MAX_WORDS; i++) {
                    if (strcmp(searchWord, words[i].english) == 0) { // 使用 strcmp 进行字符串比较
                        settextstyle(48, 0, "微软雅黑");
                        int centerX = 500 / 2;
                        int centerY = 250; // 固定中心位置

                        // 计算词性和中文释义的总宽度
                        int totalWidth = textwidth(words[i].pos) + textwidth(words[i].chinese) + 10;

                        // 计算词性的位置，使其位于中文释义的左边
                        int posX = centerX - (totalWidth / 2);
                        int posY = centerY;

                        // 计算中文释义的位置，使其位于词性的右边
                        int chineseX = posX + textwidth(words[i].pos) + 5;
                        int chineseY = centerY;
                        // 绘制词性
                        outtextxy(posX, posY, words[i].pos);

                        // 绘制中文释义
                        outtextxy(chineseX, chineseY, words[i].chinese);
                        if (words[i].learned) {
                            settextstyle(24, 0, "微软雅黑");
                            int wSpace = (500 - textwidth("Learned"))/2;
                            outtextxy(wSpace, 350, "Learned");
                        }
                        else {
                            settextstyle(24, 0, "微软雅黑");
                            int wSpace = (500 - textwidth("Unlearned")) / 2;
                            outtextxy(wSpace, 350, "Unlearned");
                        }

                    }
                    else {
                        int wSpace = (500 - textwidth("No result")) / 2;
                        outtextxy(wSpace, 350, "No Result");
                    }
                }
                strcpy(searchWord, ""); // 清空输入框
                clearrectangle(256 , 301, 399, 339); // 清除输入框中的内容
            }
            else if (ch < 128) {
                int len = strlen(searchWord);
                if (len < MAX_WORD_LENGTH - 1) {
                    searchWord[len] = ch;
                    searchWord[len + 1] = '\0';
                    outtextxy(105 + textwidth("Type the English word:"), 305, searchWord); // 显示更新后的输入框内容
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
    // 重新选择10个单词
    selectRandomUnlearnedWords(words, wordCount, selectedWords, 10);
    currentWordIndex = 0;
    inputVisible = true; // 显示输入框
    strcpy(inputWord, ""); // 清空输入框内容

    // 重新填充二维数组
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
        settextstyle(24,0, "微软雅黑");
        outtextxy(355, 575, "Star successfully!");
        
    }
}

void switchToStarBookScreen() {
    ShellExecute(NULL, "open", "notepad","D:/stared_english.csv",  NULL, SW_SHOW);
}


void switchToProgressScreen() {
    clear_button = true; // 设置标志位，表示按钮应该消失
    isMenuVisible = false; // 菜单不可见
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton(); // 绘制返回按钮
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
        char progressString[100]; // 确保这个数组足够大以存放拼接后的字符串
        sprintf(progressString, "%d/%d", learned_words, unlearn_words);
        settextstyle(32, 0, "微软雅黑");
        int wSpace = (500 - textwidth(progressString)) / 2;
        outtextxy(wSpace, 300, progressString);
        int total_words = learned_words + unlearn_words;



        // 计算绿色和灰色部分的长度
        float greenLength = (float)learned_words / total_words * 300;
        float grayLength = (float)unlearn_words / total_words * 300;


        // 绘制已学单词的绿色部分
        setfillcolor(GREEN);
        fillrectangle(100, 400, 100 + greenLength, 430);

        // 绘制未学单词的灰色部分
        setfillcolor(RGB(240, 240, 240));
        fillrectangle(100 + greenLength, 400, 400, 430);
    }
    
}

void switchToAboutScreen() {
    clear_button = true; // 设置标志位，表示按钮应该消失
    isMenuVisible = false; // 菜单不可见
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(RGB(249, 231, 255));
    cleardevice();
    drawReturnButton(); // 绘制返回按钮
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
    clear_button = true; // 设置标志位，表示按钮应该消失
    isMenuVisible = false; // 菜单不可见
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton(); // 绘制返回按钮
    setting_button = true; // 设置按钮状态为可见
}

void switchToReviewScreen() {
    clear_button = true;
    isMenuVisible = false;

    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton();
    drawSkipButton();
    drawStarButton();

    // 重新选择已学习的单词
    selectRandomlearnedWords(words, wordCount, selectedWords, 10);
    currentWordIndex = 0;
    inputVisible = true; // 显示输入框
    strcpy(inputWord, ""); // 清空输入框内容

    // 重新填充二维数组
    for (int i = 0; i < 10; i++) {
        sprintf(infoArray[i * 3], "%s", selectedWords[i].english);
        sprintf(infoArray[i * 3 + 1], "%s", selectedWords[i].pos);
        sprintf(infoArray[i * 3 + 2], "%s", selectedWords[i].chinese);
    }
    drawCurrentWord();
    drawStarButton();
}

void switchToSearchScreen() {
    clear_button = true; // 设置标志位，表示按钮应该消失
    isMenuVisible = false; // 菜单不可见
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    drawReturnButton(); // 绘制返回按钮

}

void returnToMenu() {
    if (inArea(msg.x, msg.y, 10, 10, 100, 40) && msg.message == WM_LBUTTONDOWN) {
        clear_button = false; // 设置标志位，表示按钮应该显示
        isMenuVisible = true; // 菜单可见
        setting_button = false; // 重置设置按钮状态
        isStartVisible = false;
        isSearchVisible = false;
        background(); // 重绘菜单界面
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
        settextstyle(20, 0, "微软雅黑");
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
    // 清除提示信息区域，这里假设提示信息的最大宽度为200，高度为50
    setfillcolor(WHITE);
    clearrectangle(320, 400, 500, 700); // 根据实际需要调整这个区域
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

    // 初始化infoArray
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
        msg.message = 0; // 重置消息
        if (peekmessage(&msg, EX_MOUSE)) { // 检查是否有鼠标消息
            // 处理菜单界面的按钮点击事件
            if (isMenuVisible) {
                
                if (inArea(msg.x, msg.y, startBtn.x, startBtn.y, startBtn.w, startBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = true;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    strcpy(inputWord, ""); // 清空输入框
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
                // 处理非菜单界面的按钮点击事件
               
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
                    settextstyle(24, 0, "微软雅黑");
                    outtextxy(wSpace, 300, "Save learning data successfully!");
                }

                if (inArea(msg.x, msg.y, 180, 400, 160, 40) && msg.message == WM_LBUTTONDOWN) {
                    resetLearningData("D:/C_data/practice9/final_task/english.csv", words, wordCount);
                    settextstyle(24, 0, "微软雅黑");
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

    // 释放infoArray分配的内存
    for (int i = 0; i < 10 * 3; i++) {
        free(infoArray[i]);
    }
    free(infoArray);

    return 0;
}


