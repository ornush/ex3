//
// Created by ornush on 11.1.2020.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 3000
void prinToAssemb(char *string, FILE *file);

int main() {
    unsigned long int index;
    int minMax = 0;
    int min, max, caseNum, label;
    int h = 0, i = 0, j = 0, k = 0, l = 0;

    FILE *switchcFile;
    FILE *switch_assem;
   // switchcFile = fopen("/home/ornush/CLionProjects/mivne/ex3/switch.c", "r");
   switchcFile = fopen("switch.c", "r");
///opening a file for reading
    if (switchcFile == NULL) {
        printf("Could not open file .c");
        return -1;
    }
///opening file for writing
    //switch_assem = fopen("/home/ornush/CLionProjects/mivne/ex3/switch.s", "w");
   switch_assem = fopen("switch.s", "w");
    if (switch_assem == NULL) {
        printf("Could not open file.s");
        return -1;
    }
    fprintf(switch_assem, "%s", ".section .text\n.globl switch2\nswitch2:\n");

     int arrOfCases[MAXLINE];
    int arrOfLabels[MAXLINE];
    char allLines[MAXLINE];
    int arrOfAllCases[MAXLINE];
    ///reading txt for the numbers of cases
    while (fgets(allLines, MAXLINE, switchcFile) != NULL) {
        if (strstr(allLines, "case")) {
            char *token = strtok(allLines, " case ");
            if (token != NULL) {
                char *token2 = strtok(allLines, ":");
                caseNum = atoi(token);
                arrOfCases[i] = caseNum;
                ++i;
                token = strtok(NULL, ":");
                if (minMax == 0) {
                    min = caseNum;
                    max = caseNum;
                }
                if (min > caseNum) {
                    min = caseNum;
                }
                if (max < caseNum) {
                    max = caseNum;
                }
                minMax = 1;
            }
        }
    }
    rewind(switchcFile);

///find the minimum of the cases
    int sub = max - min;
    fprintf(switch_assem, "%s %s %s", "movq", "$0", ",%rax\n");
    fprintf(switch_assem, "%s%d%s", "subq $",min, ",%rdx\n");
    fprintf(switch_assem, "%s%d%s", "cmpq $",sub, ",%rdx\n");
    fprintf(switch_assem, "%s%s%s", "ja .", "Default", "\n");
    fprintf(switch_assem, "%s%s%s", "jmp *", ".JumpTable", "(,%rdx,8)\n");

    int tempMin = min;
    for (j = 0; j < sub + 2; j++) {
        arrOfAllCases[j] = tempMin;
        tempMin++;
    }

    ///reading second time for the operation
    while (fgets(allLines, MAXLINE, switchcFile) != NULL) {
        if (strstr(allLines, ";") || strstr(allLines, ":")) {
            if (strstr(allLines, "case")) {
                char *token = strtok(allLines, " case ");
                if (token != NULL) {
                    caseNum = atoi(token);
                    label = (caseNum - min);
                    fprintf(switch_assem, "%s%d%s", ".L", label + 1, ":\n");
                    arrOfLabels[k] = label;
                    k++;
                }
            } else {
                //send to operation function
                prinToAssemb(allLines, switch_assem);
            }
            char *token3 = strtok(allLines, ":");
            while (token3 != NULL) {
                //   printf("%s\n", token3);
                if (token3 != "\0") {
                    if (strstr(token3, "default"))
                        /*(strstr(token3, "result =")) */{
                        fprintf(switch_assem, "%s%s%s", ".", "Default", ":\n");
                    }
                }
                token3 = strtok(NULL, " ");
            }
        }
    }


///converting arr of "all cases and in between" to ro data list +cases like 7,22,5,24..
    for (h = 0; h < sub + 2; h++) {//arrOfAllCases-all the cases and in between
        for (l = 0; l < sub + 2; l++) {//arrOfCases-all the cases
            if (arrOfAllCases[h] == arrOfCases[l]) {
                arrOfAllCases[h] = arrOfLabels[index] + 1;
                index++;
                break;
            }
        }
    }

///puting "-1" in the places where there isnt a label number
    for (j = 1; j < sub + 2; j++) {
        if (arrOfAllCases[j] >= min) {
            arrOfAllCases[j] = -1;
        }
    }

    fprintf(switch_assem, "%s", ".section .rodata\n.align 8\n");
    fprintf(switch_assem, "%s", ".JumpTable:\t\n");
///printing the rodata into the file
    for (j = 0; j < sub + 2; j++) {
        if (arrOfAllCases[j] == -1) {
            fprintf(switch_assem, "%s%s%s", ".quad .", "Default", "\n");
        } else {
            fprintf(switch_assem, "%s%d%s", ".quad .L", j + 1, "\n");
        }
    }

    fclose(switchcFile);
    fclose(switch_assem);
    return 0;
}

void prinToAssemb(char *string, FILE *file) {
    int idenRight = 0;
    int idenLeft = 0;
    int itsPlus = 0, itsMinus = 0, itsMult = 0, itsShiftR = 0, itsShiftL = 0;
    char *done = "Done";
    ///separate the line finding the operators
    if (strstr(string, "=")) {
        char *token = strtok(string, " ");
        char *tokenLeft = token;
        token = strtok(NULL, " ");
        int u = 0;
        if (token[u] == '+') {
            itsPlus = 1;
        }
        if (token[u] == '-') {
            itsMinus = 1;
        }
        if (token[u] == '*') {
            itsMult = 1;
        }
        if (token[u] == '<') {
            itsShiftL = 1;
        }
        if (token[u] == '>') {
            itsShiftR = 1;
        }
        char *tokenMiddle = token;
        token = strtok(NULL, " ");
        char *tokenRight = token;
        token = strtok(tokenRight, ";");
        ///checking how the line start
        ///if result=pointer
        if (strstr(tokenRight, "*")) {//pointer
            idenRight = 1;
        } else if ((strstr(tokenRight, "result"))) {// regular result
            idenRight = 2;
        } else {
            idenRight = 3;
        }
        ///if pointer=result
        if (strstr(tokenLeft, "*")) {//pointer
            idenLeft = 1;
        } else if ((strstr(tokenLeft, "result"))) {// regular result
            idenLeft = 2;
        }
        ///right
        if (strstr(tokenRight, "p2")) {//initialize the right registers
            tokenRight = "%rsi";
        } else if (strstr(tokenRight, "p1")) {
            tokenRight = "%rdi";
        } else if (strstr(tokenRight, "result")) {
            tokenRight = "%rdx";
        }
        ///left
        if (strstr(tokenLeft, "p2")) {//initialize the left registers
            tokenLeft = "%rsi";
        } else if (strstr(tokenLeft, "p1")) {
            tokenLeft = "%rdi";
        } else if (strstr(tokenLeft, "result")) {
            tokenLeft = "%rdx";
        }
        ///*p1=rdi,*p2=rsi,result=rdx
        ///*p1 = *p2...
        if (idenLeft == 1 && idenRight == 1) {
            if (strstr(tokenMiddle, "+=")) {//*p2 += *p1;
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rcx \n");
                fprintf(file, "%s%s%s", "addq %rcx, (", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "-=")) {//*p2 -= *p1;
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rcx \n");
                fprintf(file, "%s%s%s", "subq %rcx, (", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "*=")) {
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rcx \n");
                fprintf(file, "%s%s%s", "imulq %rcx, (", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "<<=")) {///p1<<=p2
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rdi \n");
                fprintf(file, "%s%s%s", "salq %rdi, (", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, ">>=")) {
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rdi \n");
                fprintf(file, "%s%s%s", "sarq %rdi, (", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "=")
                && itsPlus != 1 && itsMinus != 1 && itsMult != 1 && itsShiftL != 1 && itsShiftR != 1) {
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rcx \n");
                fprintf(file, "%s%s%s", "movq %rcx, (", tokenLeft, ") \n");
            }
        }
        ///if result = *p
        if (idenLeft == 2 && idenRight == 1) {
            //if idenRLeft==2
            tokenLeft = "%rax";
            if (strstr(tokenMiddle, "+=")) {//result += *p1;
                fprintf(file, "%s%s%s", "addq (", tokenRight, "),%rax \n");
            }
            if (strstr(tokenMiddle, "-=")) {//result -= *p1;
                fprintf(file, "%s%s%s", "subq (", tokenRight, "),%rax \n");
            }
            if (strstr(tokenMiddle, "*=")) {
                fprintf(file, "%s%s%s", "imulq (", tokenRight, "),%rax \n");
            }
            if (strstr(tokenMiddle, "<<=")) {
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rcx \n");
                tokenRight = "%cl";
                fprintf(file, "%s%s%s", "sal ", tokenRight, ",%rax \n");
            }
            if (strstr(tokenMiddle, ">>=")) {
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rcx \n");
                tokenRight = "%cl";
                fprintf(file, "%s%s%s", "sar ", tokenRight, ",%rax \n");

                ////להוסיף %CL///////////////////////////////////////////////////////////////////////////////////////
            }
            if (strstr(tokenMiddle, "=") && itsPlus != 1 && itsMinus != 1 && itsMult != 1 && itsShiftL != 1 &&
                itsShiftR != 1) {
                fprintf(file, "%s%s%s", "movq (", tokenRight, "),%rax \n");
            }
        }
        ///if result = result
        if (idenLeft == 2 && idenRight == 2) {
            //if idenRLeft==2
            tokenLeft = "%rax";
            if (strstr(tokenMiddle, "+=")) {//result += result;
                fprintf(file, "%s%s%s", "addq %rax ,", tokenLeft, "\n");
            }
            if (strstr(tokenMiddle, "-=")) {//result -= result;
                fprintf(file, "%s%s%s", "subq %rax ,", tokenLeft, "\n");
            }
            if (strstr(tokenMiddle, "*=")) {
                fprintf(file, "%s%s%s", "imulq %rax ,", tokenLeft, "\n");
            }
            if (strstr(tokenMiddle, "<<=")) {
                fprintf(file, "%s%s%s", "salq %rax ,", tokenLeft, "\n");
            }
            if (strstr(tokenMiddle, ">>=")) {
                fprintf(file, "%s%s%s", "sarq %rax ,", tokenLeft, "\n");
            }
            if (strstr(tokenMiddle, "=") && itsPlus != 1 && itsMinus != 1 && itsMult != 1 && itsShiftL != 1 &&
                itsShiftR != 1) {
                fprintf(file, "%s%s%s", "movq ", tokenRight, ",%rax \n");
            }
        }
        ///if *p = result
        if (idenLeft == 1 && idenRight == 2) {
            if (strstr(tokenMiddle, "+=")) {//p* += result;
                fprintf(file, "%s%s%s", "addq (%rax) ,(", tokenRight, ")\n");
            }
            if (strstr(tokenMiddle, "-=")) {//p* -= result;
                fprintf(file, "%s%s%s", "subq (%rax) ,(", tokenRight, ")\n");
            }
            if (strstr(tokenMiddle, "*=")) {
                fprintf(file, "%s%s%s", "imulq (%rax) ,(", tokenRight, ")\n");
            }
            if (strstr(tokenMiddle, "<<=")) {
                fprintf(file, "%s%s%s", "salq (%rax) ,(", tokenRight, ")\n");
            }
            if (strstr(tokenMiddle, ">>=")) {
                fprintf(file, "%s%s%s", "sarq (%rax) ,(", tokenRight, ")\n");
            }
            if (strstr(tokenMiddle, "=") && itsPlus != 1 && itsMinus != 1 && itsMult != 1 && itsShiftL != 1 &&
                itsShiftR != 1) {
                fprintf(file, "%s%s%s", "movq (%rax) ,(", tokenRight, ")\n");
            }
        }

        ///if *p = number
        if (idenLeft == 1 && idenRight == 3) {
            if (strstr(tokenMiddle, "+=")) {//*p2 += number;
                fprintf(file, "%s%s%s%s%s", "addq $", tokenRight, ",(", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "-=")) {//*p2 -= number;
                fprintf(file, "%s%s%s%s%s", "subq $", tokenRight, ",(", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "*=")) {
                fprintf(file, "%s%s%s%s%s", "imulq $", tokenRight, ",(", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "<<=")) {
                fprintf(file, "%s%s%s%s%s", "salq $", tokenRight, ",(", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, ">>=")) {
                fprintf(file, "%s%s%s%s%s", "sarq $", tokenRight, ",(", tokenLeft, ") \n");
            }
            if (strstr(tokenMiddle, "=")
                && itsPlus != 1 && itsMinus != 1 && itsMult != 1 && itsShiftL != 1 && itsShiftR != 1) {
                fprintf(file, "%s%s%s%s%s", "movq $", tokenRight, ",(", tokenLeft, ") \n");
            }

        }
        ///if result = number
        if (idenLeft == 2 && idenRight == 3) {
                  tokenLeft = "%rax";
            if (strstr(tokenMiddle, "+=")) {//result += number;
                fprintf(file, "%s%s%s%s%s", "addq $", tokenRight, ",", tokenLeft, " \n");
            }
            if (strstr(tokenMiddle, "-=")) {//result -= number;
                fprintf(file, "%s%s%s%s%s", "subq $", tokenRight, ",", tokenLeft, " \n");
            }
            if (strstr(tokenMiddle, "*=")) {
                fprintf(file, "%s%s%s%s%s", "imulq $", tokenRight, ",", tokenLeft, " \n");
            }
            if (strstr(tokenMiddle, "<<=")) {
                fprintf(file, "%s%s%s%s%s", "salq $", tokenRight, ",", tokenLeft, " \n");
            }
            if (strstr(tokenMiddle, ">>=")) {
                fprintf(file, "%s%s%s%s%s", "sarq $", tokenRight, ",", tokenLeft, " \n");
            }
            if (strstr(tokenMiddle, "=")
                && itsPlus != 1 && itsMinus != 1 && itsMult != 1 && itsShiftL != 1 && itsShiftR != 1) {
                fprintf(file, "%s%s%s%s%s", "movq $", tokenRight, ",", tokenLeft, " \n");
            }
        }

    } else if (strstr(string, "break")) {
        fprintf(file, "%s%s%s", "jmp .", done, "\n");
    } else if (strstr(string, "return")) {
        fprintf(file, "%s%s%s", ".", done, ":\n");
        fprintf(file, "%s", "ret\n");
    }
}
