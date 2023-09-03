

#define MAX 100000
#define str_MAX 1000

struct branch {
    int id;
    char conditionExpr[str_MAX];
    int lineNum;
    int thenCnt;
    int elseCnt;
};

struct branch branches[MAX] = {{0}};

void myCov_onCondTrue(int bId, int lineNum) {
    branches[bId].lineNum = lineNum;
    branches[bId].thenCnt++;

    printf("-- ran myCov_onCondTrue: %d\n", bId);
    printf("conditionExpr: %s\n", branches[bId].conditionExpr);
    printf("lineNum: %d\n", branches[bId].lineNum);
    printf("thenCnt: %d\n", branches[bId].thenCnt);
    printf("elseCnt: %d\n\n", branches[bId].elseCnt);
}

void myCov_onCondFalse(int bId, int lineNum) {
    branches[bId].lineNum = lineNum;
    branches[bId].elseCnt++;

    printf("-- ran myCov_onCondFalse: %d\n", bId);
    printf("conditionExpr: %s\n", branches[bId].conditionExpr);
    printf("lineNum: %d\n", branches[bId].lineNum);
    printf("thenCnt: %d\n", branches[bId].thenCnt);
    printf("elseCnt: %d\n\n", branches[bId].elseCnt);
}

/*
void readDat() {
    FILE* fp;

    fp = fopen("tempDat", "r");
    int iter = 0;
    
    char line[str_MAX];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%d,%d,%d,%s",\
        branches[iter].id, branches[iter].lineNum,\
        branches[iter].thenCnt, branches[iter].elseCnt,\
        branches[iter].conditionExpr);

        printf("-- %d %d %d %d %s\n",\
        branches[iter].id, branches[iter].lineNum,\
        branches[iter].thenCnt, branches[iter].elseCnt,\
        branches[iter].conditionExpr);
    }

    
    getdeclcontext()
    if
    functiondecl
    ismain()
}
*/

