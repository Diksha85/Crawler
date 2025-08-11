#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <direct.h>
#include <sys/stat.h>
#include "crawler.h"
#include "stringutils.h"
#include "stopwords.h"

using namespace std;

const int MAX_WORDS = 1000;
const int MAX_WORD_LENGTH = 64;

struct WordFreq {
    char word[MAX_WORD_LENGTH];
    int count;
};

Crawler::Crawler(const char* dir) : targetDir(dir) {}

void Crawler::printUsage() {
    cerr << "Usage: ./crawler [SEED_URL] [TARGET_DIRECTORY] [MAX_CRAWLING_DEPTH]\n";
}

bool Crawler::parseDepth(const char* str, int& result) {
    return strToInt(str, result);
}

bool Crawler::ensureDirectoryExists(const char* dirPath) {
    struct _stat info;
    if (_stat(dirPath, &info) != 0 || !(info.st_mode & _S_IFDIR)) {
        cout << "[INFO] Creating directory: " << dirPath << endl;
        if (_mkdir(dirPath) != 0) {
            cerr << "[ERROR] Failed to create directory.\n";
            return false;
        }
    }
    return true;
}

void Crawler::generateUniqueFilename(char* outFilename) {
    auto timestamp = chrono::system_clock::now().time_since_epoch().count();
    char timestampStr[32];
    intToStr(timestamp, timestampStr);

    int len = strLength(targetDir);
    char dirWithSlash[256];
    strCopy(dirWithSlash, targetDir);

    if (dirWithSlash[len - 1] != '\\') {
        dirWithSlash[len] = '\\';
        dirWithSlash[len + 1] = '\0';
    }

    strCopy(outFilename, dirWithSlash);
    strCat(outFilename, "page_");
    strCat(outFilename, timestampStr);
    strCat(outFilename, ".html");
}

bool Crawler::downloadPage(const char* url, const char* outputPath) {
    const char* wgetPath = "C:\\ProgramData\\chocolatey\\bin\\wget.exe";

    char checkCommand[1024];
    strCopy(checkCommand, "cmd /c \"");
    strCat(checkCommand, "\"");
    strCat(checkCommand, wgetPath);
    strCat(checkCommand, "\" -q --spider --max-redirect=10 --user-agent=\"Mozilla/5.0\" \"");
    strCat(checkCommand, url);
    strCat(checkCommand, "\"\"");

    cout << "[CHECKING] URL: " << url << endl;
    if (system(checkCommand) != 0) {
        cerr << "[ERROR] URL check failed: " << url << endl;
        return false;
    }

    char downloadCommand[1024];
    strCopy(downloadCommand, "cmd /c \"");
    strCat(downloadCommand, "\"");
    strCat(downloadCommand, wgetPath);
    strCat(downloadCommand, "\" -q -O \"");
    strCat(downloadCommand, outputPath);
    strCat(downloadCommand, "\" \"");
    strCat(downloadCommand, url);
    strCat(downloadCommand, "\"\"");

    cout << "[DOWNLOADING] URL: " << url << " -> " << outputPath << endl;
    if (system(downloadCommand) != 0) {
        cerr << "[ERROR] Download failed: " << url << endl;
        return false;
    }

    return true;
}
bool isAllDigits(const char* str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

void Crawler::extractAndSaveKeyword(const char* filepath, const char* url) {
    ifstream file(filepath);
    if (!file) {
        cerr << "[ERROR] Cannot open file: " << filepath << endl;
        return;
    }

    char ch;
    char word[MAX_WORD_LENGTH];
    int wordLen = 0;

    WordFreq freqList[MAX_WORDS];
    int freqSize = 0;

    bool inTag = false;
    bool inScript = false;
    bool inStyle = false;
    bool inBody = false;

    char tagBuffer[32];
    int tagLen = 0;

    while (file.get(ch)) {
        if (ch == '<') {
            inTag = true;
            tagLen = 0;
            continue;
        }

        if (inTag) {
            if (tagLen < 31) tagBuffer[tagLen++] = tolower(ch);
            tagBuffer[tagLen] = '\0';

            if (ch == '>') {
                inTag = false;

                if (strStartsWith(tagBuffer, "script")) inScript = true;
                else if (strStartsWith(tagBuffer, "/script")) inScript = false;
                else if (strStartsWith(tagBuffer, "style")) inStyle = true;
                else if (strStartsWith(tagBuffer, "/style")) inStyle = false;
                else if (strStartsWith(tagBuffer, "body")) inBody = true;
                else if (strStartsWith(tagBuffer, "/body")) inBody = false;
            }
            continue;
        }

        if (!inScript && !inStyle && inBody) {
            if (isalnum(ch)) {
                if (wordLen < MAX_WORD_LENGTH - 1) {
                    word[wordLen++] = tolower(ch);
                }
            } else if (wordLen > 0) {
                word[wordLen] = '\0';

             if (!StopWords::isStopWord(word) && strLength(word) > 3 && !isAllDigits(word)) {
         bool found = false;
                    for (int i = 0; i < freqSize; i++) {
                        if (strCompare(freqList[i].word, word) == 0) {
                            freqList[i].count++;
                            found = true;
                            break;
                        }
                    }

                    if (!found && freqSize < MAX_WORDS) {
                        strCopy(freqList[freqSize].word, word);
                        freqList[freqSize].count = 1;
                        freqSize++;
                    }
                }

                wordLen = 0;
            }
        }
    }

    file.close();

    KeywordEntry entry;
    strCopy(entry.url, url);
    entry.keywordCount = 0;

    for (int k = 0; k < 5 && k < freqSize; k++) {
        int maxIdx = -1, maxCount = 0;
        for (int i = 0; i < freqSize; i++) {
            if (freqList[i].count > maxCount) {
                maxCount = freqList[i].count;
                maxIdx = i;
            }
        }

        if (maxIdx != -1) {
            strCopy(entry.keywords[entry.keywordCount], freqList[maxIdx].word);
            entry.keywordCount++;
            freqList[maxIdx].count = -1;
        }
    }

    cout << "[KEYWORDS] " << filepath << " -> ";
    for (int i = 0; i < entry.keywordCount; i++) {
        cout << entry.keywords[i];
        if (i < entry.keywordCount - 1) cout << ", ";
    }
    cout << endl;

    insert(wordList, entry);

ofstream indexFile("keywordIndex.txt", ios::app);
if (indexFile.is_open()) {
    for (int i = 0; i < entry.keywordCount; i++) {
        indexFile << entry.keywords[i] << " ::: " << url << "\n";

    }
    indexFile.close();
} else {
    cerr << "Failed to open keywordIndex.txt for writing.\n";
}




}

void Crawler::saveKeywordsToFile(const char* outputFile) {
    ofstream out(outputFile);
    if (!out) {
        cerr << "[ERROR] Cannot write to: " << outputFile << endl;
        return;
    }

    Node<KeywordEntry>* curr = wordList;
    while (curr) {
        out << "URL: " << curr->data.url << "\n";
        out << "Keywords: ";
        for (int i = 0; i < curr->data.keywordCount; i++) {
            out << curr->data.keywords[i];
            if (i < curr->data.keywordCount - 1) out << ", ";
        }
        out << "\n\n";
        curr = curr->next;
    }

    out.close();
    cout << "[SAVED] Keywords written to " << outputFile << endl;
}

void Crawler::crawl(const char* url, int depth) {
    if (depth < 0 || totalLinksCrawled >= MAX_LINKS) return;

    if (visited.isVisited(url)) {
        cout << "[SKIP] Already visited: " << url << endl;
        return;
    }

    visited.markVisited(url);
    cout << "\n[CRAWL] URL: " << url << " | Depth: " << depth << endl;

    char filename[256];
    generateUniqueFilename(filename);
    if (!downloadPage(url, filename)) return;

    cout << "[SAVED] Page: " << filename << endl;

    extractAndSaveKeyword(filename, url);

    Node<char*>* head = nullptr;
    int linkCount = extractLinks(filename, head);
    cout << "[EXTRACTED] Found " << linkCount << " link(s) in: " << url << endl;

    Node<char*>* curr = head;
    while (curr && totalLinksCrawled < MAX_LINKS) {
        char normalized[2048];
        normalizeUrl(url, curr->data, normalized);

        cout << "   [LINK] " << normalized << endl;

        if (!visited.isVisited(normalized)) {
            totalLinksCrawled++;
            crawl(normalized, depth - 1);
        } else {
            cout << "   [SKIP] Already visited." << endl;
        }

        curr = curr->next;
    }

    freeList(head);
}
bool Crawler::hasVisited(const char* url) {
    return visited.isVisited(url);
}
