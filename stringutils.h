#pragma once

template <typename T>
inline int strLength(const T* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

template <typename T>
inline void strCopy(T* dest, const T* src) {
    int i = 0;
    while ((dest[i] = src[i]) != '\0') i++;
}

template <typename T>
inline void strCat(T* dest, const T* src) {
    int len = strLength(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[len + i] = src[i];
        i++;
    }
    dest[len + i] = '\0';
}

template <typename T>
inline bool strStartsWith(const T* str, const T* prefix) {
    while (*prefix) {
        if (*str != *prefix) return false;
        str++;
        prefix++;
    }
    return true;
}

template <typename T>
inline bool isAlpha(T ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

template <typename T>
inline T toLower(T ch) {
    return (ch >= 'A' && ch <= 'Z') ? ch + 32 : ch;
}

bool strToInt(const char* str, int& result);
void intToStr(long long value, char* buffer);
void unescapeHtmlEntities(char* str);
void normalizeUrl(const char* baseUrl, const char* relativeUrl, char* result);
int strCompare(const char* str1, const char* str2);
const char* strFindSubstring(const char* haystack, const char* needle);
const char* strFindChar(const char* str, char ch);

int strCompareIgnoreCase(const char* a, const char* b);
