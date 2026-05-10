// Copyright 2026 Belev Timur
#ifndef INCLUDE_TEXTGEN_H
#define INCLUDE_TEXTGEN_H

#include <deque>
#include <map>
#include <vector>
#include <string>
#include <random>

class TextGenerator {
private: 
    using Prefix = std::deque<std::string>;
    using StateTab = std::map<Prefix, std::vector<std::string>>;

    StateTab statetab;
    int npref;
    int maxgen;
    std::mt19937 rng;

    void buildTable(const std::string& filename);
    Prefix createInitialPrefix(const std::vector<std::string>& words);
    std::string getRandomSuffix(const std::vector<std::string>& suffixes);

public: 
    explicit TextGenerator(int npref = 2, int maxgen = 1000);

    void loadText(const std::string& filename);
    std::string generate();
    std::string generateFromPrefix(const Prefix& startPrefix);

    StateTab& getStateTab() { return statetab; }
    void addSuffix(const Prefix& prefix, const std::string& suffix);
    void clearTable();
    bool hasPrefix(const Prefix& prefix) const;
    std::vector<std::string> getSuffixes(const Prefix& prefix) const;
};

#endif
