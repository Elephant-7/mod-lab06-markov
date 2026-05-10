// Copyright 2021 GHA Test Team
#include <cassert>

#include <deque>
#include <iostream>
#include <string>
#include <vector>

#include "../include/textgen.h"

using Prefix = std::deque<std::string>;

void test_prefix_creation() {
    std::cout << "Test 1: Prefix creation\n";
    TextGenerator gen(2, 100);

    Prefix prefix;
    prefix.push_back("hello");
    prefix.push_back("world");

    gen.addSuffix(prefix, "test");

    assert(gen.hasPrefix(prefix));
    std::cout << "  PASSED\n";
}

void test_prefix_suffix_record() {
    std::cout << "Test 2: Prefix-suffix record creation\n";
    TextGenerator gen(2, 100);

    Prefix prefix;
    prefix.push_back("hello");
    prefix.push_back("world");

    gen.addSuffix(prefix, "test");
    auto suffixes = gen.getSuffixes(prefix);

    assert(suffixes.size() == 1);
    assert(suffixes[0] == "test");
    std::cout << "  PASSED\n";
}

void test_single_suffix_selection() {
    std::cout << "Test 3: Single suffix selection (using RNG)\n";
    TextGenerator gen(2, 100);

    Prefix prefix;
    prefix.push_back("cat");
    prefix.push_back("sat");

    gen.addSuffix(prefix, "on");
    auto suffixes = gen.getSuffixes(prefix);

    assert(suffixes.size() == 1);
    assert(suffixes[0] == "on");
    std::cout << "  PASSED\n";
}

void test_multiple_suffix_selection() {
    std::cout << "Test 4: Multiple suffix selection (using RNG)\n";
    TextGenerator gen(2, 100);

    Prefix prefix;
    prefix.push_back("the");
    prefix.push_back("dog");

    gen.addSuffix(prefix, "ran");
    gen.addSuffix(prefix, "jumped");
    gen.addSuffix(prefix, "walked");

    auto suffixes = gen.getSuffixes(prefix);
    assert(suffixes.size() == 3);

    bool found1 = false, found2 = false, found3 = false;
    for (int i = 0; i < 50; ++i) {
        auto result = gen.generateFromPrefix(prefix);
        if (result.find("ran") != std::string::npos) found1 = true;
        if (result.find("jumped") != std::string::npos) found2 = true;
        if (result.find("walked") != std::string::npos) found3 = true;
        if (found1 && found2 && found3) break;
    }

    assert(found1 && found2 && found3);
    std::cout << "  PASSED\n";
}

void test_text_generation_fixed_length() {
    std::cout << "Test 5: Text generation with fixed length (manual table)\n";
    TextGenerator gen(2, 5);

    Prefix p1, p2, p3;
    p1.push_back("I"); p1.push_back("am");
    p2.push_back("am"); p2.push_back("very");
    p3.push_back("very"); p3.push_back("happy");

    gen.addSuffix(p1, "very");
    gen.addSuffix(p2, "happy");
    gen.addSuffix(p3, "today");

    std::string result = gen.generateFromPrefix(p1);

    assert(!result.empty());
    std::cout << "  Generated: " << result << "\n";
    std::cout << "  PASSED\n";
}

void test_different_prefix_sizes() {
    std::cout << "Test 6: Different prefix sizes\n";
    TextGenerator gen3(3, 100);

    Prefix prefix;
    prefix.push_back("one");
    prefix.push_back("two");
    prefix.push_back("three");

    gen3.addSuffix(prefix, "four");

    assert(gen3.hasPrefix(prefix));
    std::cout << "  PASSED\n";
}

void test_empty_table() {
    std::cout << "Test 7: Empty table handling\n";
    TextGenerator gen(2, 100);

    bool exceptionCaught = false;
    try {
        gen.generate();
    }
    catch (const std::runtime_error&) {
        exceptionCaught = true;
    }

    assert(exceptionCaught);
    std::cout << "  PASSED\n";
}

void test_clear_table() {
    std::cout << "Test 8: Clear table functionality\n";
    TextGenerator gen(2, 100);

    Prefix prefix;
    prefix.push_back("test");
    prefix.push_back("word");

    gen.addSuffix(prefix, "suffix");
    assert(gen.hasPrefix(prefix));

    gen.clearTable();
    assert(!gen.hasPrefix(prefix));
    std::cout << "  PASSED\n";
}

void test_multiple_suffixes_same_prefix() {
    std::cout << "Test 9: Multiple suffixes for same prefix\n";
    TextGenerator gen(2, 100);

    Prefix prefix;
    prefix.push_back("to");
    prefix.push_back("be");

    gen.addSuffix(prefix, "or");
    gen.addSuffix(prefix, "and");
    gen.addSuffix(prefix, "but");

    auto suffixes = gen.getSuffixes(prefix);
    assert(suffixes.size() == 3);
    assert(suffixes[0] == "or");
    assert(suffixes[1] == "and");
    assert(suffixes[2] == "but");
    std::cout << "  PASSED\n";
}

void test_generation_limit() {
    std::cout << "Test 10: Generation respects MAXGEN limit\n";
    TextGenerator gen(2, 3);

    Prefix p1, p2, p3, p4;
    p1.push_back("a"); p1.push_back("b");
    p2.push_back("b"); p2.push_back("c");
    p3.push_back("c"); p3.push_back("d");
    p4.push_back("d"); p4.push_back("e");

    gen.addSuffix(p1, "c");
    gen.addSuffix(p2, "d");
    gen.addSuffix(p3, "e");
    gen.addSuffix(p4, "f");

    std::string result = gen.generateFromPrefix(p1);

   
    std::stringstream ss(result);
    std::string word;
    int wordCount = 0;
    while (ss >> word) wordCount++;
    assert(wordCount <= 5);
    std::cout << "  PASSED\n";
}

int main() {
    std::cout << "\n=== Running Markov Chain Text Generator Tests ===\n\n";
    test_prefix_creation();
    test_prefix_suffix_record();
    test_single_suffix_selection();
    test_multiple_suffix_selection();
    test_text_generation_fixed_length();
    test_different_prefix_sizes();
    test_empty_table();
    test_clear_table();
    test_multiple_suffixes_same_prefix();
    test_generation_limit();

    std::cout << "\n=== All tests passed! ===\n\n";

    return 0;
}
