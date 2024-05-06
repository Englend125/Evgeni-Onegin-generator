#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>

std::vector<char*> Buffers;

std::pair<char*, std::streampos> ReadFile(std::string file_name) {
    std::ifstream in_file(file_name, std::ios::binary);

    in_file.seekg(0, std::ios::end);
    std::streampos in_file_size = in_file.tellg();
    in_file.seekg(0, std::ios::beg);

    char* buffer = new char[in_file_size];

    in_file.read(buffer, in_file_size);

    in_file.close();
    Buffers.push_back(buffer);

    return {buffer, in_file_size};
}

std::vector<std::pair<char*, char*>> GetStrings (std::pair <char*, std::streampos> file) {
    std::vector<std::pair<char*, char*>> text;
    std::vector<char*> s_begin;
    std::vector<char*> s_end;

    s_begin.push_back(file.first);

    for(int i = 0; i < file.second; ++i) {
        if (file.first[i] == '\n') {
            s_end.push_back(file.first + i);
            s_begin.push_back(file.first + i + 1);
        }
    }

    for(int i = 0; i < s_begin.size(); ++i) {
        text.push_back({s_begin[i], s_end[i]});
    }

    return text;
}

std::string GetString (std::pair<char*, char*> str) {
    std::string new_s;

    for(int i = 0; str.first + i < str.second; ++i) {
        new_s += str.first[i];
    }

    return new_s;
}

int GetRand () {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution rr;
    int a = 0;
    a += rr(gen) % 5262;

    return a;
}

std::pair<std::string, std::string> GetRithm (std::vector<std::pair<char*, char*>> clear_text,
                                              std::vector<std::pair<char*, char*>> text) {
    int a = GetRand();
    std::string a_end = GetString({clear_text[a].second - 6, clear_text[a].second});
    std::string a_string = GetString({clear_text[a].first, clear_text[a].second});

    for (int i = GetRand(); i < clear_text.size(); ++i) {
        std::string i_end = GetString({clear_text[i].second - 6, clear_text[i].second});
        std::string i_string = GetString({clear_text[i].first, clear_text[i].second});

        if(a_string == i_string) {
            continue;
        }
        if (i_end == a_end ) {
            return {GetString(text[a]), GetString(text[i])};
        }
        if(i == clear_text.size() - 1) {
            i = GetRand();
        }
    }
}

int main() {
    std::ofstream out_file("out.txt");
    std::vector<std::pair<char*, char*>> clear_text = GetStrings(ReadFile("clear_text.txt"));
    std::vector<std::pair<char*, char*>> text = GetStrings(ReadFile("Text.txt"));

    for(int i = 0; i < 3; ++i) {
        std::pair<std::string, std::string> rithm_1 = GetRithm(clear_text, text);
        std::pair<std::string, std::string> rithm_2 = GetRithm(clear_text, text);
        if (i == 0) {
            out_file << rithm_1.first << rithm_1.second
                     << rithm_2.first << rithm_2.second << '\n';
        }
        if (i == 1) {
            out_file << rithm_1.first << rithm_2.first
                     << rithm_1.second << rithm_2.second << '\n';
        }
        if (i == 2) {
            out_file << rithm_1.first << rithm_2.first
                     << rithm_2.second << rithm_1.second << '\n';
        }
    }

    std::pair<std::string, std::string> end_rithm = GetRithm(clear_text, text);

    out_file << end_rithm.first << end_rithm.second;

    delete Buffers[0];
    delete Buffers[1];

    return 0;
}
