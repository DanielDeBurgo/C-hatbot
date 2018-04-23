typedef struct matchAndNo {
    int lineNum;
    char *lineText;
} matchAndNo;

typedef struct numStruct {
    int lineNum;
    char *lineText;
    double probability;
} numStruct;

void getTheRight(char *splitting, char tosplit);
void getTheLeft(char *splitting, char tosplit);
matchAndNo searchdb(FILE *db, char *toFind, int omit);
void findnums(FILE *lookingIn, matchAndNo finding);
void readFileToMe(FILE * db);
void rewrite(FILE *db, FILE *out, double numToReWrite, int lineNo, numStruct num);
void replaceinp(FILE *userCalls, FILE *botResponses, FILE *out, char *userCall, char *offensiveResponse, char *repWith);
