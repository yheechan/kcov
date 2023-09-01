

struct branch {
    char **conditionList;
    int lineCnt;
    int thenCnt;
    int elseCnt;
};

#define MAX 100000

struct branch branches[MAX];
int branchCount = 0;

