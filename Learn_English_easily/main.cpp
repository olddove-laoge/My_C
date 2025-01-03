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

int currentWordIndex = 0; // 当前显示的单词索引
bool skipButton = false; // 控制“skip”按钮是否可见的标志位
bool inputVisible = true; // 控制输入框是否可见的标志位
bool isStartVisible = false;//控制开始界面是否展示
bool isSearchVisible = false;//控制搜索界面是否展示
bool isProgressVisible = false;//控制进度界面是否展示
bool allLearned = false;//默认一轮英语学习尚未全部学会
bool setting_button = false;//控制设置界面按钮是否绘制
bool clear_button = false;//同上
bool isMenuVisible = true; // 控制菜单是否可见的标志位
/*以上布尔类型变量都是为了防止不同界面绘制的图形出现在其他界面而设置的*/
bool checkWord(const char* userWord, const char* correctWord) {
    return strcmp(userWord, correctWord) == 0;//用来检测用户输入是否正确，用于学习和复习
}

int wordCount = 0;//初始化单词索引
Word words[MAX_WORDS]; // 存储所有单词
Word selectedWords[10]; // 存储10个选中的单词
char inputWord[MAX_WORD_LENGTH]; // 存储用户输入的英文单词，用于学习/复习界面
char searchWord[MAX_WORD_LENGTH];//同上，用于搜索界面
int state = 0;//1为学习，0为复习，这个布尔类型用来防止用户review界面后没有进行完整的复习就返回主菜单导致已学会单词被恢复为未学习状态
// 按钮结构体定义
typedef struct {
    int x, y, w, h; // 按钮的位置和大小
    const char* text; // 按钮上显示的文本
    bool need_tip;//是否需要提示信息
    const char* tooltip; // 按钮的提示信息
} Button;

Button startBtn = { 180, 365, 140, 35, "START TO LEARN", true, "Tap to learn English!" };
Button starbookBtn = { 180, 400, 140, 35, "STAR BOOK", true, "Review words you stared." };
Button progressBtn = { 180, 435, 140, 35, "PROGRESS", true, "Tap to view progress." };
Button reviewBtn = { 180, 470, 140, 35, "REVIEW", true, "Review the words learned" };
Button searchBtn = { 180, 505, 140, 35, "SEARCH", true, "search words" };
Button settingBtn = { 180, 540, 140, 35, "SETTINGS", true, "Change some settings" };
Button helpBtn = { 180, 575, 140, 35, "HELP", true, "How to use this program" };
Button aboutBtn = { 180, 610, 140, 35, "ABOUT", true, "About this program" };
Button exitBtn = { 180, 645, 140, 35, "EXIT", false, "Tap to exit the program" };
Button clearDataBtn = { 180, 400, 160, 40, "Clear learning data", false, "" };
Button saving = { 180, 200, 160, 40, "Save learning data", false, "" };
Button skipBtn = { 400, 650, 100, 40, "SKIP", false, "" };
Button returnBtn = { 10, 10, 100, 40, "RETURN", false, " " };
Button starBtn = { 400, 600, 100, 40, "STAR", true, "Mark this word as difficult" };

ExMessage msg = { 0 };//将鼠标信息归零，防止按钮连续多次读入用户信息

char** infoArray = NULL;// 全局变量infoArray，用来在界面上展示学习/复习/搜索的单词中文和词性
void drawText() {
    settextstyle(20, 0, "bauhaus 93");
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
}//默认的字体风格

bool inArea(int mx, int my, int x, int y, int w, int h) {
    return mx > x && mx < x + w && my > y && my < y + h;
}//检测鼠标是否点击某个按钮

void drawButton(Button btn) {//用来绘制按钮
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
    if (inArea(msg.x, msg.y, btn.x, btn.y, btn.w, btn.h) && btn.need_tip == true) {//如果有提示信息，则在按钮旁边展示
        setfillcolor(RGB(173, 216, 230)); // 
        settextcolor(RGB(0, 0, 128)); // 
        settextstyle(16, 0, "Arial");
        fillroundrect(btn.x + btn.w + 10, btn.y, btn.x + btn.w + textwidth(btn.tooltip) + 20, btn.y + textheight(btn.tooltip) + 10, 5, 5);
        outtextxy(btn.x + btn.w + 15, btn.y + 5, btn.tooltip);
    }
}

void background() {//用来绘制主界面背景
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
    settextstyle(28, 0, "微软雅黑");
    int wSpace = (500 - textwidth("V 0.1.6     版权所有：李宇航"))/2;
    outtextxy(wSpace, 300, "V 0.1.6     版权所有：李宇航");
}

void clearCurrentWordArea() {//学习单词时，跳转至下一个单词时清除上一个单词的词性和中文绘制
    setfillcolor(WHITE); // 设置填充颜色为白色，以覆盖之前的文本
    clearrectangle(110, 200, 500, 300); // 只清除单词的区域
}

void updateWordsFromSelected() {//学完一轮单词后，将随机抽取的10个单词组成的随机数组的单词学会状态赋值给words数组中对应的单词
    if (state == 1) {
        for (int i = 0; i < 10; i++) {
            int originalIndex = selectedWords[i].index; // 假设每个Word有一个指向words数组中原始单词的索引
            words[originalIndex].learned = selectedWords[i].learned;
            words[originalIndex].correctAnswers = selectedWords[i].correctAnswers;
        }
    }
}

void drawCurrentWord() {//用来跳转单词
    if (!isMenuVisible && isStartVisible) { // 只在 START 界面绘制
        settextstyle(48, 0, "微软雅黑");
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);

        // 清除之前单词的区域
        clearCurrentWordArea();

        // 找到第一个未学会的单词并绘制
        while (currentWordIndex < 10 && selectedWords[currentWordIndex].learned) {
            currentWordIndex++; // 如果当前单词已学会，移动到下一个
        }

        if (currentWordIndex < 10) { // 如果找到了未学会的单词
            Word currentWord = selectedWords[currentWordIndex];

            int centerX = 500 / 2;
            int centerY = 250; // 固定中心位置

            // 计算词性和中文释义的总宽度
            int totalWidth = textwidth(currentWord.pos) + textwidth(currentWord.chinese) + 10;

            // 计算词性的位置，使其位于中文释义的左边
            int posX = centerX - (totalWidth / 2);
            int posY = centerY;

            // 计算中文释义的位置，使其位于词性的右边
            int chineseX = posX + textwidth(currentWord.pos) + 5;
            int chineseY = centerY;

            // 绘制词性
            outtextxy(posX, posY, currentWord.pos);

            // 绘制中文释义
            outtextxy(chineseX, chineseY, currentWord.chinese);

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
        else {//没有未学会的单词时，自动返回菜单
            updateWordsFromSelected();
            clear_button = false; // 设置标志位，表示按钮应该显示
            isMenuVisible = true; // 菜单可见
            setting_button = false; // 重置设置按钮状态
            isStartVisible = false;
            isSearchVisible = false;
            background(); // 重绘菜单界面
        }
    }
}

void reviewUnlearnedWords() {//用来重复提问第一轮（这里只每个单词都已提问一遍后）学习后还未学会的单词
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
        clear_button = false; // 设置标志位，表示按钮应该显示
        isMenuVisible = true; // 菜单可见
        setting_button = false; // 重置设置按钮状态
        background(); // 重绘菜单界面u
    }
}


void processInput() {// 在主循环中处理用户输入
    
    if (!isMenuVisible && inputVisible && isStartVisible) {//确保只有在开始学习界面使用
        
        if (_kbhit()) {
            char ch = _getch();//读取用户键盘消息
            if (ch == '\b') {//按下Backspace键时，清除上一个输入的字母
                if (strlen(inputWord) > 0) {
                    inputWord[strlen(inputWord) - 1] = '\0';
                    clearrectangle(105 + textwidth("Type the English word:"), 305, 500, 340); // 清除输入框中的内容
                    outtextxy(105 + textwidth("Type the English word:"), 305, inputWord); // 显示更新后的输入框内容
                }
            }
            else if (ch == '\r') {//按下回车时，清除用户输入，并将用户输入单词与正确答案进行比对
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
                }
                else {
                    char errorMsg[MAX_WORD_LENGTH * 2];
                    sprintf(errorMsg, "Wrong! The correct word is: %s", selectedWords[currentWordIndex].english);
                    settextstyle(20, 0, "微软雅黑");
                    settextcolor(BLACK);
                    setbkmode(TRANSPARENT);
                    outtextxy(100, 350, errorMsg);
                    selectedWords[currentWordIndex].correctAnswers = 0; // 答错一次，答对次数归零
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


void switchToStartScreen() {
    state = 1;
    clear_button = true;
    isMenuVisible = false;

    setbkcolor(WHITE);
    cleardevice();
    drawButton(returnBtn);
    drawButton(skipBtn);
    drawButton(starBtn);
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
    drawButton(starBtn);
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
    drawButton(returnBtn); // 绘制返回按钮
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

void switchToHelpScreen() {
    clear_button = true; // 设置标志位，表示按钮应该消失
    isMenuVisible = false; // 菜单不可见
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(RGB(249, 231, 255));
    cleardevice();
    drawButton(returnBtn); // 绘制返回按钮
    settextstyle(48, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(340, 30, "HELP");

    settextstyle(24, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 60, "FUNCTION INTRODUCTION:");
    outtextxy(25, 470, "LEARNING RULES:");
    
    settextstyle(18, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 140, "START:");
    outtextxy(25, 180, "STARBOOK:");
    outtextxy(25, 260, "PROCESS:");
    outtextxy(25, 320, "SETTINGS:");
    outtextxy(25, 380, "REVIEW:");
    outtextxy(25, 420, "SEARCH:");
    outtextxy(25, 500, "LEARNING:");
    outtextxy(25, 580, "STAR:");
    outtextxy(25, 640, "SKIP:");

    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 100, "The program is made to help students to learn English more easily.");
    outtextxy(25, 120, "You can click the button in the menu to choose different function.");
    outtextxy(25, 160, "Cilck START to start your English learning journey! ");
    outtextxy(25, 200, "Click STARBOOK to review the words you have stared");
    outtextxy(25, 220, "during the process of reciting so that you can have a");
    outtextxy(25, 240, "better understanding of some difficult and complict words.");
    outtextxy(25, 280, "You can see how many words you have learned here and");
    outtextxy(25, 300, "view the concrete.");
    outtextxy(25, 340, "You can change some system settings here such as");
    outtextxy(25, 360, "saving your learning data and so on.");
    outtextxy(25, 400, "You can review the words you have learned here to enhance memory");
    outtextxy(25, 440, "You can search words here.Only supports search chinese by english");
    outtextxy(25, 520, "In this program,every words should be recited third times");
    outtextxy(25, 540, "correctly and continuely to be passed. Once you make a mistake,");
    outtextxy(25, 560, "the number of correct times will be cleared to zero.");
    outtextxy(25, 600, "You can click STAR button ton star a word you think difficult.");
    outtextxy(25, 620, "Through this, you can have a better understanding of complicate words.");
    outtextxy(25, 660, "Click SKIP to skip the words you think you have grasped.");
    outtextxy(25, 680, "And the words will be recognized as having been learned.");
    
}

void switchtoSearchScreen() {
    
        clear_button = true; // 设置标志位，表示按钮应该消失
        isMenuVisible = false; // 菜单不可见
        initgraph(500, 707, EX_SHOWCONSOLE);
        setbkcolor(WHITE);
        cleardevice();
        drawButton(returnBtn); // 绘制返回按钮
        settextstyle(20, 0, "微软雅黑");
        settextcolor(BLACK);
        setfillcolor(RGB(240, 240, 240));
        fillrectangle(100, 300, 400, 340);
        setlinecolor(RGB(160, 160, 160));
        rectangle(100, 300, 400, 340);
        outtextxy(105, 305, "Type the English word:");
        setbkmode(TRANSPARENT);

    
}

void switchToSettingsScreen() {
    clear_button = true; // 设置标志位，表示按钮应该消失
    isMenuVisible = false; // 菜单不可见
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    drawButton(returnBtn); // 绘制返回按钮
    setting_button = true; // 设置按钮状态为可见
}

void switchToAboutScreen() {
    clear_button = true; // 设置标志位，表示按钮应该消失
    isMenuVisible = false; // 菜单不可见
    initgraph(500, 707, EX_SHOWCONSOLE);
    setbkcolor(RGB(249, 231, 255));
    cleardevice();
    drawButton(returnBtn);// 绘制返回按钮
    settextstyle(48, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(340, 30, "ABOUT");

    settextstyle(36, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 60, "About program:");
    outtextxy(25, 240, "About producer:");

    settextstyle(28, 0, "bauhaus 93");
    settextcolor(RGB(113, 96, 232));
    outtextxy(25, 90, "name:");
    outtextxy(25, 130, "version:");
    outtextxy(25, 170, "development time:");
    outtextxy(25, 270, "name:");
    outtextxy(25, 310, "student ID:");
    outtextxy(25, 350, "major:");
    outtextxy(25, 395, "class:");

    settextstyle(18, 0, "Arial");
    settextcolor(RGB(0, 0, 128));
    outtextxy(25, 115, "RECITE ENGLISH EASILY!!!");
    outtextxy(25, 155, "v 0.1.6");
    outtextxy(25, 195, "2024/12/22");
    outtextxy(25, 295, "LiYuhang/李宇航");
    outtextxy(25, 335, "8002124208");
    outtextxy(25, 375, "Software engineering/软件工程");
    outtextxy(25, 418, "2407");
}

void switchToReviewScreen() {
    state = 0;
    clear_button = true;
    isMenuVisible = false;

    setbkcolor(WHITE);
    cleardevice();
    drawButton(returnBtn);
    drawButton(skipBtn);
    drawButton(starBtn);

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
    drawButton(starBtn);
}


void returnToMenu() {
    if (inArea(msg.x, msg.y, 10, 10, 100, 40) && msg.message == WM_LBUTTONDOWN) {
        updateWordsFromSelected();
        clear_button = false; // 设置标志位，表示按钮应该显示
        isMenuVisible = true; // 菜单可见
        setting_button = false; // 重置设置按钮状态
        isStartVisible = false;
        isSearchVisible = false;
        background(); // 重绘菜单界面
    }
}

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
    clearrectangle(320, 365, 500, 700); // 根据实际需要调整这个区域
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
                    switchToStartScreen();
                }
                else if (inArea(msg.x, msg.y, starbookBtn.x, starbookBtn.y, starbookBtn.w, starbookBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    switchToStarBookScreen();
                }
                else if (inArea(msg.x, msg.y, progressBtn.x, progressBtn.y, progressBtn.w, progressBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = true;
                    switchToProgressScreen();
                }
                else if (inArea(msg.x, msg.y, reviewBtn.x, reviewBtn.y, reviewBtn.w, reviewBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = true;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    switchToReviewScreen();
                }
                else if (inArea(msg.x, msg.y, searchBtn.x, searchBtn.y, searchBtn.w, searchBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isSearchVisible = true;
                    isStartVisible = false;
                    isProgressVisible = false;
                    switchtoSearchScreen();
                }
                else if (inArea(msg.x, msg.y, helpBtn.x, helpBtn.y, helpBtn.w, helpBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    switchToHelpScreen();
                }
                else if (inArea(msg.x, msg.y, settingBtn.x, settingBtn.y, settingBtn.w, settingBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    switchToSettingsScreen();
                }
                else if (inArea(msg.x, msg.y,aboutBtn.x, aboutBtn.y, aboutBtn.w, aboutBtn.h) && msg.message == WM_LBUTTONDOWN) {
                    isStartVisible = false;
                    isSearchVisible = false;
                    isProgressVisible = false;
                    switchToAboutScreen();
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
            drawButton(starbookBtn);
            drawButton(progressBtn);
            drawButton(aboutBtn);
            drawButton(reviewBtn);
            drawButton(searchBtn);
            drawButton(settingBtn);
            drawButton(helpBtn);
            drawButton(exitBtn);
        }
        if (clear_button) {
            drawButton(returnBtn);
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
                    resetStarData("D:/stared_english.csv");
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
