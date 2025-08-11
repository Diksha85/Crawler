#include <iostream>
#include <fstream>
#include "crawler.h"
#include "stringutils.h"

void searchKeywordInIndex(const char* keyword) {
    std::ifstream indexFile("keywordIndex.txt");
    if (!indexFile) {
        std::cerr << "[ERROR] Cannot open keywordIndex.txt\n";
        return;
    }

    char line[1024];
    bool found = false;

    while (indexFile.getline(line, sizeof(line))) {
        char lowerLine[1024], lowerKeyword[256];
        strCopy(lowerLine, line);
        strCopy(lowerKeyword, keyword);

        for (int i = 0; lowerLine[i]; i++) lowerLine[i] = toLower(lowerLine[i]);
        for (int i = 0; lowerKeyword[i]; i++) lowerKeyword[i] = toLower(lowerKeyword[i]);

        if (strFindSubstring(lowerLine, lowerKeyword)) {
            std::cout << line << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "No results found for: " << keyword << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        Crawler::printUsage();
        return 1;
    }

    char seedUrl[1000];
    char targetDir[1000];
    int maxDepth = 0;

    strCopy(seedUrl, argv[1]);
    strCopy(targetDir, argv[2]);

    if (!Crawler::parseDepth(argv[3], maxDepth)) return 1;
    if (!Crawler::ensureDirectoryExists(targetDir)) return 1;

    Crawler crawler(targetDir);
    crawler.crawl(seedUrl, maxDepth);

    char userUrl[1000];
    char choice[10];

    while (true) {
        std::cout << "\nEnter a URL to check/crawl (or type 'exit' to quit): ";
        std::cin.getline(userUrl, sizeof(userUrl));
        if (strCompareIgnoreCase(userUrl, "exit") == 0) break;
        if (strLength(userUrl) == 0) continue;

        if (crawler.hasVisited(userUrl)) {
            std::cout << "[INFO] This URL was already crawled.\n";
            std::cout << "Do you want to crawl it again? (yes/no): ";
            std::cin.getline(choice, sizeof(choice));

            if (strCompareIgnoreCase(choice, "yes") == 0) {
                crawler.crawl(userUrl, maxDepth);
            } else {
                std::cout << "[INFO] Skipping crawl.\n";
            }
        } else {
            std::cout << "[INFO] URL not crawled yet. Crawling now...\n";
            crawler.crawl(userUrl, maxDepth);
        }

        char keyword[256];
        std::cout << "Enter a keyword to search: ";
        std::cin.getline(keyword, sizeof(keyword));
        searchKeywordInIndex(keyword);
    }

    return 0;
}
