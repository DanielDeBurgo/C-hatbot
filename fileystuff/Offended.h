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

void getTheRight(char *splitting, char tosplit);
void getTheLeft(char *splitting, char tosplit);
matchAndNo searchdb(FILE *db, char *toFind, int omit);
void findnums(FILE *lookingIn, matchAndNo finding);
void readFileToMe(FILE * db);
void rewrite(FILE *db, FILE *out, double numToReWrite, int lineNo, numStruct num);
void replaceinp(FILE *userCalls, FILE *botResponses, FILE *out, char *userCall, char *offensiveResponse, char *repWith);
