#include "../include/textgen.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

TextGenerator::TextGenerator(int npref, int maxgen)
    : npref(npref), maxgen(maxgen), rng(std::random_device{}()) {
}

void TextGenerator::loadText(const std::string& filename) {
    buildTable(filename);
}

void TextGenerator::buildTable(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string text((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    std::vector<std::string> words;
    std::stringstream ss(text);
    std::string word;

    while (ss >> word) {
        // Clean word from punctuation at ends
        while (!word.empty() && std::ispunct(word.back())) {
            word.pop_back();
        }
        while (!word.empty() && std::ispunct(word.front())) {
            word.erase(0, 1);
        }
        if (!word.empty()) {
            words.push_back(word);
        }
    }

    if (words.size() < static_cast<size_t>(npref + 1)) {
        throw std::runtime_error("Text is too short for given prefix size");
    }

    Prefix prefix;
    for (int i = 0; i < npref; ++i) {
        prefix.push_back(words[i]);
    }

    for (size_t i = npref; i < words.size(); ++i) {
        statetab[prefix].push_back(words[i]);
        prefix.pop_front();
        prefix.push_back(words[i]);
    }
}

TextGenerator::Prefix TextGenerator::createInitialPrefix(const std::vector<std::string>& words) {
    Prefix prefix;
    for (int i = 0; i < npref && static_cast<size_t>(i) < words.size(); ++i) {
        prefix.push_back(words[i]);
    }
    return prefix;
}

std::string TextGenerator::getRandomSuffix(const std::vector<std::string>& suffixes) {
    if (suffixes.empty()) {
        return "";
    }
    std::uniform_int_distribution<size_t> dist(0, suffixes.size() - 1);
    return suffixes[dist(rng)];
}

std::string TextGenerator::generate() {
    if (statetab.empty()) {
        throw std::runtime_error("No data loaded. Call loadText() first.");
    }

    auto it = statetab.begin();
    Prefix current = it->first;

    return generateFromPrefix(current);
}

std::string TextGenerator::generateFromPrefix(const Prefix& startPrefix) {
    if (statetab.empty()) {
        throw std::runtime_error("No data loaded. Call loadText() first.");
    }

    Prefix current = startPrefix;

    std::vector<std::string> result;
    for (const auto& word : current) {
        result.push_back(word);
    }

    for (int i = 0; i < maxgen; ++i) {
        auto it = statetab.find(current);
        if (it == statetab.end() || it->second.empty()) {
            break;
        }

        std::string nextWord = getRandomSuffix(it->second);
        if (nextWord.empty()) {
            break;
        }

        result.push_back(nextWord);
        current.pop_front();
        current.push_back(nextWord);
    }

    std::string output;
    for (size_t i = 0; i < result.size(); ++i) {
        if (i > 0) output += " ";
        output += result[i];
    }

    return output;
}

void TextGenerator::addSuffix(const Prefix& prefix, const std::string& suffix) {
    statetab[prefix].push_back(suffix);
}

void TextGenerator::clearTable() {
    statetab.clear();
}

bool TextGenerator::hasPrefix(const Prefix& prefix) const {
    return statetab.find(prefix) != statetab.end();
}

std::vector<std::string> TextGenerator::getSuffixes(const Prefix& prefix) const {
    auto it = statetab.find(prefix);
    if (it != statetab.end()) {
        return it->second;
    }
    return std::vector<std::string>();
}