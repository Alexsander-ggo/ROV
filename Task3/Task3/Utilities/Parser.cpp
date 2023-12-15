#include "Utilities/Parser.h"

Args Parser::split(const std::string& text)
{
    mWords.clear();
    std::string word;
    for (char c : text) {
         if (c != ' ') {
             word.push_back(c);
         } else {
             addWord(word);
             word.clear();
         }
    }
    addWord(word);
    return mWords;
}

void Parser::addWord(const std::string& word)
{
    if (!word.empty()) {
        mWords.push_back(word);
    }
}
