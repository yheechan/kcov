

#define MAX 100000
#define str_MAX 1000

struct branch {
    int id;
    char conditionExpr[str_MAX];
    int lineNum;
    int thenCnt;
    int elseCnt;
    int numBranch;
};

struct branch branches[MAX] = {{0}};
int branch_id=-1;
int totBranchCount = 0;

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
        sscanf(line, "%d,%d,%d,%d,%d,%[^\n]s\n",\
        &branches[iter].id, &branches[iter].lineNum,\
        &branches[iter].thenCnt, &branches[iter].elseCnt,\
        &branches[iter].numBranch,\
        branches[iter].conditionExpr);

        totBranchCount += branches[iter].numBranch;
        iter++;
        branch_id++;
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

    int covered = 0;
    int iter = 0;
    for (iter; iter<branch_id; iter++) {
        fprintf(fp1, "%d,%d,%d,%d,%d,%s\n",\
        branches[iter].id, branches[iter].lineNum,\
        branches[iter].thenCnt, branches[iter].elseCnt,\
        branches[iter].numBranch,\
        branches[iter].conditionExpr);

        fprintf(fp2, "|%d\t|\t\t%d\t\t|\t\t%d\t\t|\t%s\n",\
        branches[iter].lineNum,\
        branches[iter].thenCnt, branches[iter].elseCnt,\
        branches[iter].conditionExpr);

        if (branches[iter].thenCnt > 0) covered += 1;
        if (branches[iter].elseCnt > 0) covered += 1;
    }

    fprintf(fp2,"-----------------------------------------------------\n");
    float perc = (covered/(float)totBranchCount)*100.0;
    fprintf(fp2,"Covered: %d / %d = %f \%\n", covered, totBranchCount, perc);
    
    fclose(fp1);
    fclose(fp2);
}

