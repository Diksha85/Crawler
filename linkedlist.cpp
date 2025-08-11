#include "linkedlist.h"
#include "stringutils.h"
#include <cstdio>

bool strEndsWith(const char* str, const char* suffix) {
    int lenStr = strLength(str);
    int lenSuf = strLength(suffix);
    if (lenStr < lenSuf) return false;
    for (int i = 0; i < lenSuf; i++) {
        if (str[lenStr - lenSuf + i] != suffix[i]) return false;
    }
    return true;
}

bool isHtmlLink(const char* url) {
    return !(
        strEndsWith(url, ".pdf") ||
        strEndsWith(url, ".jpg") ||
        strEndsWith(url, ".jpeg") ||
        strEndsWith(url, ".png") ||
        strEndsWith(url, ".gif") ||
        strEndsWith(url, ".css") ||
        strEndsWith(url, ".js") ||
        strEndsWith(url, ".svg") ||
        strEndsWith(url, ".ico")
    );
}

int extractLinks(const char* filename, Node<char*>*& head) {
    FILE* file = fopen(filename, "r");
    if (!file) return 0;

    char line[2048];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        const char* pos = line;

        while ((pos = strFindSubstring(pos, "<a href=\"")) != nullptr) {
            pos += 9; 

            const char* end = strFindChar(pos, '"');
            if (end != nullptr) {
                int length = end - pos;

                if (length > 0 && length < 1024) {
                    char* url = new char[length + 1];
                    for (int i = 0; i < length; ++i)
                        url[i] = pos[i];
                    url[length] = '\0';

                    unescapeHtmlEntities(url);

                    if (isHtmlLink(url)) {
                        insert(head, url);
                        count++;
                    } else {
                        delete[] url;
                    }
                }

                pos = end + 1;  
            } else {
                break; 
            }
        }
    }

    fclose(file);
    return count;
}
