#include "linkedlist.h"
#include "hashtable.h"

struct KeywordEntry {
    char url[1000];
    char keywords[5][100];  
    int keywordCount = 0;
};

class Crawler {
    const char* targetDir;
    int totalLinksCrawled = 0;
    static const int MAX_LINKS = 100;

private:
    HashTable<char*> visited;
    Node<KeywordEntry>* wordList = nullptr;  

public:
    Crawler(const char* dir);

    static void printUsage();
    static bool parseDepth(const char* str, int& result);
    static bool ensureDirectoryExists(const char* dirPath);
    void generateUniqueFilename(char* outFilename);

    bool downloadPage(const char* url, const char* outputPath);
    void crawl(const char* url, int depth);

    bool hasVisited(const char* url);


    void extractAndSaveKeyword(const char* filepath, const char* url);
    void saveKeywordsToFile(const char* outputFile);
};


