#pragma once
#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER

#include <fstream>
#include <string>
#include <cctype>
#include <vector>
#include <assert.h>

class Record;
class Token_Line;
class Token_Table;
class LEX_ANA;

typedef std::string str;
typedef std::vector<str> vec_str;
typedef std::vector<Record> vec_rec;
typedef std::vector<Token_Line> vec_line;

enum class Type { RW, OP, ID, NUM, ERROR, COMMENT };

class Record {
public:
    Record(Type _type = Type::RW, str _rec = "") :
        type(_type), rec(_rec) { };
    ~Record() { };

    void set_rec(Type _type, str _rec) {
        type = _type, rec = _rec;
    }


    Type ret_type() const { return type; }
    str ret_rec() const { return rec; }
private:
    Type type;
    str rec;
};

class Token_Line {
public:
    Token_Line(int _ord = 0, str _line = "") :
        line_ord(_ord), line_inf(_line) {
        token_rec.clear();
    }
    ~Token_Line() { }

    void set_line(int _ord, str _line) {
        line_ord = _ord;
        line_inf = _line;
    }

    void push_token_rec(Record _rec) {
        token_rec.push_back(_rec);
    }

    void token_rec_clear() { token_rec.clear(); }

    str ret_line() const { return line_inf; }
    int ret_size() const { return token_rec.size(); }
    Record ret_token(int idx) const {
        assert(idx >= 0 && idx < token_rec.size());
        return token_rec[idx];
    }

    str ret_show_inf() const {
        str line_ord_str = std::to_string(line_ord + 1);
        str all_inf = line_ord_str + ": " + line_inf + "\n";
        for (auto& iter : token_rec) {
            if (iter.ret_type() != Type::COMMENT)
                all_inf += "   " + line_ord_str + ": ";
            switch (iter.ret_type()) {
            case Type::RW:
                all_inf += "reserved word: " + iter.ret_rec();
                break;
            case Type::ID:
                all_inf += "ID, name= " + iter.ret_rec();
                break;
            case Type::NUM:
                all_inf += "NUM, val= " + iter.ret_rec();
                break;
            case Type::OP:
                all_inf += iter.ret_rec();
                break;
            case Type::ERROR:
                all_inf += "Error, invalid string: " + iter.ret_rec();
                break;
            default:
                break;
            }
            if (iter.ret_type() != Type::COMMENT)
                all_inf += "\n";
        }
        return all_inf;
    }

private:
    int line_ord;
    str line_inf;
    vec_rec token_rec;
};

class Token_Table {
public:
    Token_Table() { };
    ~Token_Table() { };

    str ret_token_table() {
        str lex_res = "";
        for (auto& iter : token_line) {
            lex_res += iter.ret_show_inf();
        }
        return lex_res;
    }

    vec_line token_line;
};

class LEX_ANA {
public:
    LEX_ANA(std::string inp_file = "", std::string otp_file = "") :
        inp_file_name(inp_file), otp_file_name(otp_file) {
        curr_row = 0, curr_idx = 0;
    }
    ~LEX_ANA() { }

    void read_file();
    void analyze();
    void output_result();

private:
    std::string inp_file_name;
    std::string otp_file_name;
    Token_Table token_table;
    vec_str __text;
    str buffer;
    int curr_row;
    int curr_idx;
    str __ope = "+-*/<>=,;!()[]{}";
    str __delim = " \t\n";

    /**
     * Only used in the state where judging reserved word
     */
    inline int ret_state_rw(char& ch, char tar, int formal_state) {
        ch = nextChar();
        buffer += ch;
        if (ch == tar) return formal_state;
        else if (ch == '\0') return 53;
        else if (letter_not(ch, tar)) return 25;
        else if (is_split(ch)) return 26;
        else return 27;
    }

    inline int ret_state_ls(char& ch, int formal_state = 6) {
        ch = nextChar();
        buffer += ch;
        if (isalpha(ch)) return 25;
        else if (ch == '\0') return 55;
        else if (is_split(ch)) return formal_state;
        else return 27;
    }

    inline int get_token(Type type, bool eof_flg = true, bool if_retract = true) {
        int row_ord = 0;
        Record curr_rec;
        if (if_retract)
            retract();
        buffer.erase(buffer.length() - eof_flg);
        row_ord = ret_row_ord();
        curr_rec.set_rec(type, buffer);
        token_table.token_line[row_ord].push_token_rec(curr_rec);
        return 1;
    }

    inline int ret_row_ord() const {
        return curr_row - !curr_idx;
    }

    inline bool is_delim(char ch) const {
        return __delim.find(ch) != str::npos;
    }

    inline bool is_ope(char ch) const {
        return __ope.find(ch) != str::npos;
    }

    inline bool is_illegal(char ch) const {
        return (!(is_delim(ch) || is_ope(ch) || isalpha(ch) || isdigit(ch))) && ch != '\0';
    }

    inline bool is_split(char ch) const {
        return is_delim(ch) || is_ope(ch);
    }

    inline bool letter_not(char ch, char not_ch) const {
        return isalpha(ch) && ch != not_ch;
    }

    inline void clear_buffer() {
        buffer.clear();
    }

    inline char nextChar() {
        if (curr_row == __text.size()) return '\0';
        char curr_ch = __text[curr_row][curr_idx];
        if (__text[curr_row].length())
            curr_idx = (curr_idx + 1) % __text[curr_row].length();
        else {
            curr_idx = 0;
            curr_ch = ' ';
        }
        curr_row = !curr_idx ? curr_row + 1 : curr_row;
        return curr_ch;
    }

    inline void retract() {
        assert(!(!curr_idx && !curr_row));
        curr_row = curr_idx ? curr_row : curr_row - 1;
        curr_idx = curr_idx ? curr_idx - 1 : __text[curr_row].length() - 1;
    }
};

#endif // !LEXICAL_ANALYZER

