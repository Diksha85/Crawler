#include <iostream>
#include <fstream>
#include "stringutils.h"
#include "search.h"

const int MAX_LINE = 2048;
const int MAX_KEYWORD = 100;
const int MAX_URLS = 100;
const int MAX_URL = 2048;

void searchKeyword() {
    char inputKeyword[MAX_KEYWORD];
    char line[MAX_LINE];
    char urls[MAX_URLS][MAX_URL];
    int urlCount = 0;

    std::cout << "Enter keyword: ";
    std::cin.getline(inputKeyword, MAX_KEYWORD);

    for (int i = 0; inputKeyword[i]; ++i) {
        inputKeyword[i] = toLower(inputKeyword[i]);
    }

    std::ifstream file("keywordIndex.txt", std::ios::in);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Cannot open keywordIndex.txt\n";
        return;
    }

    while (file.getline(line, MAX_LINE)) {
        const char* arrowPos = strFindSubstring(line, ":::");  


        if (!arrowPos) continue;

        char fileKeyword[MAX_KEYWORD];
        int i = 0;
        const char* ptr = line;
        while (ptr < arrowPos && i < MAX_KEYWORD - 1) {
            fileKeyword[i++] = toLower(*ptr++);
        }
        fileKeyword[i] = '\0';

        if (strCompare(fileKeyword, inputKeyword) == 0) {
            arrowPos += 3; 

            while (*arrowPos == ' ' || *arrowPos == '\t') ++arrowPos;

            char currentUrl[MAX_URL];
            int j = 0;
            while (*arrowPos) {
                if (*arrowPos == ',' || *arrowPos == '\n' || *arrowPos == '\r') {
                    currentUrl[j] = '\0';

                    int start = 0;
                    while (currentUrl[start] == ' ' || currentUrl[start] == '\t') ++start;

                    bool isDuplicate = false;
                    for (int u = 0; u < urlCount; ++u) {
                        if (strCompare(urls[u], &currentUrl[start]) == 0) {
                            isDuplicate = true;
                            break;
                        }
                    }

                    if (!isDuplicate && urlCount < MAX_URLS) {
                        strCopy(urls[urlCount++], &currentUrl[start]);
                    }

                    j = 0;
                    ++arrowPos;
                } else {
                    if (j < MAX_URL - 1)
                        currentUrl[j++] = *arrowPos++;
                    else
                        ++arrowPos;  
                }
            }

            if (j > 0) {
                currentUrl[j] = '\0';
                int start = 0;
                while (currentUrl[start] == ' ' || currentUrl[start] == '\t') ++start;

                bool isDuplicate = false;
                for (int u = 0; u < urlCount; ++u) {
                    if (strCompare(urls[u], &currentUrl[start]) == 0) {
                        isDuplicate = true;
                        break;
                    }
                }

                if (!isDuplicate && urlCount < MAX_URLS) {
                    strCopy(urls[urlCount++], &currentUrl[start]);
                }
            }
        }
    }

    file.close();

    if (urlCount == 0) {
        std::cout << "[INFO] No URLs found for keyword: " << inputKeyword << "\n";
    } else {
        std::cout << "[FOUND] URLs for '" << inputKeyword << "':\n";
        for (int i = 0; i < urlCount; ++i) {
            std::cout << " - " << urls[i] << "\n";
        }
    }
}
