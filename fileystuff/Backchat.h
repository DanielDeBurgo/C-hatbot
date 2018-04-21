typedef struct matchAndNo {
    int lineNum;
    char *lineText;
} matchAndNo;

typedef struct numStruct {
    int lineNum;
    char *lineText;
    double probability;
} numStruct;

char globTemp[512];
char globTemp2[512][512];
char toReturn[512][512];
char readLine[512];
numStruct toReturn2[512];
char arrsplitby[512];
char arrsplitby2[512];
int numsCount = 0, linesCount = 0;

char *respond(char *userCall, FILE *userCalls, FILE *botResponses);
