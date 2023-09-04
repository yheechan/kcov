#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <map>
#include <utility>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/Utils.h"

using namespace clang;
using namespace std;

int total_branches = 0;
int branch_id = -1;

std::string singleCondStart = "((";
std::string id2CondEnd(int id) {
    std::string val = ") ? (myCov_onCondTrue(" + to_string(id)+ "), 1) : \
(myCov_onCondFalse(" + to_string(id) + "), 0))";
    return val;
}
std::string readCMD = "myCov_readInitData();\n\t";
std::string writeCMD = "myCov_writeUpdData();\n\t";

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor>
{
public:
    MyASTVisitor(Rewriter &R, const LangOptions &langOptions)
        : TheRewriter(R), LangOpts(langOptions)
    {}
    bool VisitStmt(Stmt *s) {
        // Fill out this function for your homework

        if (isa<IfStmt>(s)) {
            total_branches += 2;
            branch_id++;
            auto ifStmt = dyn_cast<IfStmt>(s);


            // >> get line number
            SourceLocation ifLoc = ifStmt->getBeginLoc();
            SourceManager &srcmgr = TheRewriter.getSourceMgr();
            unsigned int lineNum = srcmgr.getExpansionLineNumber(ifLoc);
            
            // >> get expression string
            Expr* cond = ifStmt->getCond();
            std::string str1;
            llvm::raw_string_ostream os(str1);
            cond->printPretty(os, NULL, LangOpts);
            std::string expr = os.str();

            // single condition; x>10
            TheRewriter.InsertTextBefore(cond->getBeginLoc(), singleCondStart);
            TheRewriter.InsertTextAfter(ifStmt->getRParenLoc(), id2CondEnd(branch_id));

            // >> append initial branch info to tempDat
            writeBranchInitData(branch_id, lineNum, expr);


            // multi condition; x>10 || x<4
            // dynamic condition; x++>10, ++x<10

            /*
            branch_id += 1;
            total_branches += 2;
            printInfo("If", branch_id, s);

            IfStmt * ifStmt = dyn_cast<IfStmt>(s);
            */
        } else if (isa<ForStmt>(s)) {
            total_branches += 2;
            branch_id++;
            printInfo("For", branch_id, s);

            auto forStmt = dyn_cast<ForStmt>(s);

            // >> get line number
            SourceLocation forLoc = forStmt->getBeginLoc();
            SourceManager &srcmgr = TheRewriter.getSourceMgr();
            unsigned int lineNum = srcmgr.getExpansionLineNumber(forLoc);
            
            // >> get expression string
            Expr* cond = forStmt->getCond();
            std::string str1;
            llvm::raw_string_ostream os(str1);
            cond->printPretty(os, NULL, LangOpts);
            std::string expr = os.str();

            writeBranchInitData(branch_id, lineNum, expr);

        }

        return true;
    }
    
    bool VisitFunctionDecl(FunctionDecl *f) {
        // Fill out this function for your homework
        if (f->hasBody()) {
            Stmt *FuncBody = f->getBody();
            auto funcName = f->getName();
            llvm::outs() << "function: " << funcName << "\n";
        }
        return true;
    }

private:
    Rewriter &TheRewriter;
    const LangOptions &LangOpts;

    void writeBranchInitData(int branch_id, int lineNum, std::string expr) {
        ofstream of;
        of.open("tempDat", ios::app);

        // writer branch information
        of << to_string(branch_id) + "," + to_string(lineNum) + ",";
        of << "0,0,";
        of << expr + "\n";

        of.close();
    }

    void printInfo(string name, int id, Stmt *s) {
        // get statement class name
        auto stmtName = s->getStmtClassName();

        // Get line and column information
        SourceLocation startLocation = s->getBeginLoc();
        SourceManager &srcmgr = TheRewriter.getSourceMgr(); // you can get SourceManager from initialization part
        unsigned int lineNum = srcmgr.getExpansionLineNumber(startLocation);
        unsigned int colNum = srcmgr.getExpansionColumnNumber(startLocation);
        llvm::StringRef fileName = srcmgr.getFilename(startLocation);

        llvm::outs() << "\t" << name << "\t";
        llvm::outs() << "ID: " << id << "\t";
        llvm::outs() << "Line: " << lineNum << "\t\t";
        llvm::outs() << "Col: " << colNum << "\t";
        llvm::outs() << "Filename: " << fileName << "\n";
    }
};

class MyASTConsumer : public ASTConsumer
{
public:
    MyASTConsumer(Rewriter &R, const LangOptions &langOptions)
        : Visitor(R, langOptions), //initialize MyASTVisitor
        TheRewriter(R),
        langOpts(langOptions)
    {}

	void OnFirstTopLevelDecl(const Decl *d) {
		std::ifstream file("./probes/utilities.c");
		std::string utilityFile( (std::istreambuf_iterator<char>(file) ),\
							 (std::istreambuf_iterator<char>() ));

		TheRewriter.InsertTextBefore(d->getBeginLoc(), utilityFile);
	}

    void OnMainFuncDecl(const Decl *d) {
        // get body as stmt
        auto body = d->getBody();
        if (isa<CompoundStmt>(body)) {
            auto cmpStmt = dyn_cast<CompoundStmt>(body);
            auto frontStmt = cmpStmt->body_front();
            auto backStmt = cmpStmt->body_back();

            TheRewriter.InsertTextBefore(frontStmt->getBeginLoc(), readCMD);
            TheRewriter.InsertTextBefore(backStmt->getBeginLoc(), writeCMD);
        }

        // init tempDat
        ofstream of;
        of.open("tempDat", ios::trunc);
        of << "";
        of.close();
    }

    // callback functino of ASTConsumer
    // called for each top-level declaration
    virtual bool HandleTopLevelDecl(DeclGroupRef DR) {
        for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b) {

            const Decl* D = *b;

            // at first declaration of a function
            // insert probes for utility function for branch coverage
            if (!firstDecl && D->hasBody() && isa<FunctionDecl>(D)) {
                OnFirstTopLevelDecl(D);
                firstDecl = true;
            }

            // at declaration of main function
            // insert probes for reading initial information of branch
            // insert probes for writing final (updated) information of branch
            if (isa<FunctionDecl>(D) && D->getAsFunction()->isMain()) {
                OnMainFuncDecl(D);
            }

            // Travel each function declaration using MyASTVisitor
            Visitor.TraverseDecl(*b);
        }
        return true;
    }

private:
    MyASTVisitor Visitor;
    Rewriter &TheRewriter;
    const LangOptions &langOpts;
    bool firstDecl = false;
};


int main(int argc, char *argv[])
{
    if (argc != 2) {
        llvm::errs() << "Usage: kcov-branch-identify <filename>\n";
        return 1;
    }

    // CompilerInstance will hold the instance of the Clang compiler for us,
    // managing the various objects needed to run the compiler.
    CompilerInstance TheCompInst;
    
    // Diagnostics manage problems and issues in compile 
    TheCompInst.createDiagnostics(NULL, false);

    // Set target platform options 
    // Initialize target info with the default triple for our platform.
    auto TO = std::make_shared<TargetOptions>();
    TO->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *TI = TargetInfo::CreateTargetInfo(TheCompInst.getDiagnostics(), TO);
    TheCompInst.setTarget(TI);

    // FileManager supports for file system lookup, file system caching, and directory search management.
    TheCompInst.createFileManager();
    FileManager &FileMgr = TheCompInst.getFileManager();
    
    // SourceManager handles loading and caching of source files into memory.
    TheCompInst.createSourceManager(FileMgr);
    SourceManager &SourceMgr = TheCompInst.getSourceManager();
    
    // Prreprocessor runs within a single source file
    TheCompInst.createPreprocessor(TU_Module);

    // Add HeaderSearch Path
    Preprocessor &PP = TheCompInst.getPreprocessor();
    const llvm::Triple &HeaderSearchTriple = PP.getTargetInfo().getTriple();
    
    HeaderSearchOptions &hso = TheCompInst.getHeaderSearchOpts();

    // <Warning!!> -- Platform Specific Code lives here
    // This depends on A) that you're running linux and
    // B) that you have the same GCC LIBs installed that I do.
    /*
    $ gcc -xc -E -v -
    ..
      /usr/lib/gcc/x86_64-linux-gnu/4.9/include
      /usr/local/include
      /usr/lib/gcc/x86_64-linux-gnu/4.9/include-fixed
      /usr/include/x86_64-linux-gnu
      /usr/include
    End of search list.
    */
    const char *include_paths[] = {"/usr/local/include",
        "/usr/include/x86_64-linux-gnu",
        "/usr/lib/gcc/x86_64-linux-gnu/7/include",
        "/usr/include"};
    
    for (int i = 0; i < (sizeof(include_paths) / sizeof(include_paths[0])); i++) {
        hso.AddPath(include_paths[i], clang::frontend::Angled, false, false);
    }
    // </Warning!!> -- End of Platform Specific Code

    ApplyHeaderSearchOptions(PP.getHeaderSearchInfo(), hso,
                             PP.getLangOpts(), HeaderSearchTriple);
    
    // ASTContext holds long-lived AST nodes (such as types and decls) .
    TheCompInst.createASTContext();

    // A Rewriter helps us manage the code rewriting task.
    Rewriter TheRewriter;
    TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

    // Set the main file handled by the source manager to the input file.
    auto expect_filein = FileMgr.getFile(argv[1]);
    if (error_code ec = expect_filein.getError()) {
        llvm::errs() << "Error: " << ec.message() << '\n';
        return 1;
    }
    const FileEntry *FileIn = expect_filein.get();
    SourceMgr.setMainFileID(SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
    
    // Inform Diagnostics that processing of a source file is beginning. 
    TheCompInst.getDiagnosticClient().BeginSourceFile(TheCompInst.getLangOpts(),&TheCompInst.getPreprocessor());
    
    // Create an AST consumer instance which is going to get called by ParseAST.
    MyASTConsumer TheConsumer(TheRewriter, TheCompInst.getLangOpts());

    // Parse the file to AST, registering our consumer as the AST consumer.
    // from slide (note): ParseAST() start building and traversal of an AST.
    ParseAST(TheCompInst.getPreprocessor(), &TheConsumer, TheCompInst.getASTContext());

    llvm::outs() << "Total number of branches: " << total_branches << "\n";

    const RewriteBuffer *RewriteBuf = TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
	if (RewriteBuf && TheRewriter.buffer_begin() != TheRewriter.buffer_end()) {
		char o_file[1024]={0};
		strncpy(o_file, argv[1], strlen(argv[1])-2);
		strcpy(o_file+strlen(o_file), "-cov.c");
		ofstream output(o_file);
		output << string(RewriteBuf->begin(), RewriteBuf->end());
		output.close();
	}

    return 0;
}