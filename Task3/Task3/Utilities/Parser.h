#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using Args = std::vector<std::string>;

class Parser
{
public:
    Parser() = default;

    Args split(const std::string& text);

private:
    void addWord(const std::string& word);

private:
    Args mWords;
};

#endif // PARSER_H
