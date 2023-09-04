

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

    // read from tempDat
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

    // write header for coverage.dat
    fp = fopen("coverage.dat", "w+");

    fprintf(fp,"|Line#\t|# of execution\t|# of execution\t|condition\n");
    fprintf(fp,"|\t\t|of then branch\t|of else branch\t|expression\n");
    fprintf(fp,"-----------------------------------------------------\n");

    fclose(fp);
}

void myCov_writeUpdData() {
    FILE* fp1;
    FILE* fp2;

    // write update information to tempDat
    fp1 = fopen("tempDat", "wb");
    fp2 = fopen("coverage.dat", "a");

    int iter = 0;
    for (iter; iter<totBranchCount+1; iter++) {
        fprintf(fp1, "%d,%d,%d,%d,%s\n",\
        branches[iter].id, branches[iter].lineNum,\
        branches[iter].thenCnt, branches[iter].elseCnt,\
        branches[iter].conditionExpr);

        fprintf(fp2, "|%d\t|\t\t%d\t\t|\t\t%d\t\t|\t%s\n",\
        branches[iter].lineNum,\
        branches[iter].thenCnt, branches[iter].elseCnt,\
        branches[iter].conditionExpr);
    }

    fprintf(fp2,"-----------------------------------------------------\n");
    
    fclose(fp1);
    fclose(fp2);
}

