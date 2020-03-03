//
// Created by bichlmaier on 06.02.2020.
//

#ifndef TBLANG_SYMBOLS_H
#define TBLANG_SYMBOLS_H

namespace tb_lang
{

    struct Symbols
    {
        static const std::set<char> Letters()
        {
            std::set<char> letters = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
                                      'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
            std::transform(letters.begin(), letters.end(),
                           std::inserter(letters, letters.end()),
                           [](char c) -> char
                           {
                               return std::toupper(c);
                           });

            return letters;
        }

        static std::set<char> Digits()
        { return {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; };

        static std::set<char> Whitespace()
        { return {' ', '\t'}; };

        static std::set<char> NewLine()
        { return {'\n'}; };

        static std::set<char> Period()
        { return {'.'}; };

        static std::set<char> AddSub()
        { return {'+', '-'}; }

        static std::set<char> Slash()
        { return {'/'}; };

        static std::set<char> Asterisk()
        { return {'*'}; };

        static std::set<char> HashTag()
        { return {'#'}; };

        static std::set<char> UnderScore()
        { return {'_'}; };

        static std::set<char> QuotationMark()
        { return {'\"'}; };

        static std::set<char> Equals()
        { return {'='}; };

        static std::set<char> Punctuation()
        { return {'{', '}', '(', ')', ',', ':', ';'}; };

        static std::set<char> Relational()
        { return {'<', '>'}; };

        static std::set<char> Exclamation()
        { return {'!'}; };
    };

}
#endif //TBLANG_SYMBOLS_H
