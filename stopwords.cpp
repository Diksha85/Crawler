#include "stopwords.h"

const char* stopwords[] = {
  "a", "an", "the", "is", "are", "was", "were", "in", "on", "and", "or", "of",
    "to", "for", "with", "about", "as", "at", "by", "from", "has", "have", "had",
    "be", "been", "being", "but", "if", "into", "no", "not", "such", "that", "their",
    "then", "there", "these", "they", "this", "those", "too", "very", "can", "will",
    "just", "up", "down", "out", "over", "under", "again", "further", "here", "once",
    "do", "does", "did", "doing", "don", "should", "now", "because", "while", "where",
    "when", "how", "what", "which", "who", "whom", "why", "you", "your", "yours",
    "he", "him", "his", "she", "her", "hers", "we", "us", "our", "ours", "i", "me",
    "my", "mine", "it", "its", "they", "them", "their", "theirs", "so", "some",
    "any", "all", "each", "few", "more", "most", "other", "such", "only", "own",
    "same", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should",
    "now", "i'm", "you're", "we're", "they're", "i've", "you've", "we've", "they've",
    "i'll", "you'll", "he'll", "she'll", "we'll", "they'll", "isn't", "aren't",
    "wasn't", "weren't", "hasn't", "haven't", "hadn't", "doesn't", "don't","didn't"

};

const int stopwordCount = sizeof(stopwords) / sizeof(stopwords[0]);

bool StopWords::isStopWord(const char* word) {
    for (int i = 0; i < stopwordCount; i++) {
        const char* sw = stopwords[i];
        int j = 0;
        while (word[j] != '\0' && sw[j] != '\0' && word[j] == sw[j]) {
            j++;
        }
        if (word[j] == '\0' && sw[j] == '\0') {
            return true;
        }
    }
    return false;
}
