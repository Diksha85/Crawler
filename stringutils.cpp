#include "stringutils.h"

bool strToInt(const char* str, int& result) {
    result = 0;
    int i = 0;
    bool negative = false;

    if (str[0] == '-') {
        negative = true;
        i++;
    }

    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9') return false;
        result = result * 10 + (str[i] - '0');
    }

    if (negative) result = -result;
    return true;
}

void intToStr(long long value, char* buffer) {
    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    char temp[25];
    int i = 0;
    bool negative = false;

    if (value < 0) {
        negative = true;
        value = -value;
    }

    while (value > 0) {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (negative) temp[i++] = '-';

    int j = 0;
    while (i > 0) {
        buffer[j++] = temp[--i];
    }
    buffer[j] = '\0';
}

void unescapeHtmlEntities(char* str) {
    char* src = str;
    char* dest = str;

    while (*src) {
        if (strStartsWith(src, "&amp;")) {
            *dest++ = '&';
            src += 5;
        } else {
            *dest++ = *src++;
        }
    }
    *dest = '\0';
}

void normalizeUrl(const char* baseUrl, const char* relativeUrl, char* result) {
    if (strStartsWith(relativeUrl, "http://") || strStartsWith(relativeUrl, "https://")) {
        strCopy(result, relativeUrl);
        return;
    }

    int i = 0;
    int protoEnd = -1;
    while (baseUrl[i + 2] != '\0') {
        if (baseUrl[i] == ':' && baseUrl[i + 1] == '/' && baseUrl[i + 2] == '/') {
            protoEnd = i + 3;
            break;
        }
        i++;
    }

    if (protoEnd == -1) {
        strCopy(result, relativeUrl);
        return;
    }

    char protocol[16];
    for (i = 0; i < protoEnd; i++) {
        protocol[i] = baseUrl[i];
    }
    protocol[i] = '\0';

    char domain[256];
    int j = 0;
    while (baseUrl[protoEnd + j] != '\0' && baseUrl[protoEnd + j] != '/') {
        domain[j] = baseUrl[protoEnd + j];
        j++;
    }
    domain[j] = '\0';

    int pathStart = protoEnd + j;
    char path[512];
    path[0] = '\0';
    if (baseUrl[pathStart] == '/') {
        int k = 0;
        while (baseUrl[pathStart + k] != '\0') {
            path[k] = baseUrl[pathStart + k];
            k++;
        }
        path[k] = '\0';

        int len = strLength(path);
        for (int p = len - 1; p >= 0; p--) {
            if (path[p] == '/') {
                path[p + 1] = '\0';
                break;
            }
        }
    } else {
        strCopy(path, "/");
    }

    if (relativeUrl[0] == '/') {
        strCopy(result, protocol);
        strCat(result, domain);
        strCat(result, relativeUrl);
    } else {
        strCopy(result, protocol);
        strCat(result, domain);
        strCat(result, path);
        strCat(result, relativeUrl);
    }
}

int strCompare(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    return str1[i] - str2[i];
}

const char* strFindSubstring(const char* haystack, const char* needle) {
    int lenHay = strLength(haystack);
    int lenNeedle = strLength(needle);

    if (lenNeedle == 0) return haystack;

    for (int i = 0; i <= lenHay - lenNeedle; ++i) {
        int j = 0;
        while (j < lenNeedle && haystack[i + j] == needle[j]) {
            ++j;
        }
        if (j == lenNeedle) {
            return &haystack[i];
        }
    }

    return nullptr;
}

const char* strFindChar(const char* str, char ch) {
    while (*str) {
        if (*str == ch) return str;
        str++;
    }
    return nullptr;
}
int strCompareIgnoreCase(const char* a, const char* b) {
    int i = 0;
    while (a[i] && b[i]) {
        char ca = toLower(a[i]);
        char cb = toLower(b[i]);
        if (ca != cb) return (ca - cb);
        i++;
    }
    return (a[i] - b[i]);
}
