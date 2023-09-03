

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
int totBranchCount = -1;

void myCov_onCondTrue(int bId) {
    branches[bId].thenCnt++;
}

void myCov_onCondFalse(int bId) {
    branches[bId].elseCnt++;
}

void myCov_readInitData() {
    FILE* fp;

    fp = fopen("tempDat", "r");
    int iter = 0;
    
    char line[str_MAX];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%d,%d,%d,%[^\n]s\n",\
        &branches[iter].id, &branches[iter].lineNum,\
        &branches[iter].thenCnt, &branches[iter].elseCnt,\
        branches[iter].conditionExpr);

        totBranchCount++;
        iter++;
    }

    fclose(fp);
}

void myCov_writeUpdData() {
    FILE* fp;
    fp = fopen("tempDat", "wb");

    int iter = 0;
    for (iter; iter<totBranchCount+1; iter++) {
        fprintf(fp, "%d,%d,%d,%d,%s\n",\
        branches[iter].id, branches[iter].lineNum,\
        branches[iter].thenCnt, branches[iter].elseCnt,\
        branches[iter].conditionExpr);
    }
    
    fclose(fp);
}

