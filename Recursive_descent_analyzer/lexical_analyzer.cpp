/**
 * Implement "lexical_analyzer.h"
 */

#include "lexical_analyzer.h"


void LEX_ANA::read_file() {
    __text.clear();
    std::ifstream inp_file(inp_file_name.c_str(), std::ios::in);
    str line = "";
    int row_ord = 0;
    while (std::getline(inp_file, line)) {
        __text.push_back(line);
        token_table.token_line.push_back(Token_Line(row_ord, line));
        row_ord++;
    }
    inp_file.close();
}

void LEX_ANA::output_result() {
    std::ofstream otp_file(otp_file_name.c_str(), std::ios::out);
    otp_file.close();
    otp_file.open(otp_file_name.c_str(), std::ios::app);

    otp_file << token_table.ret_token_table();

    otp_file.close();
}

void LEX_ANA::analyze() {
    bool __EOF = false;
    int state = 1;
    char ch = '\0';

    while (!__EOF) {
        switch (state) {
        case 1:
            clear_buffer();
            ch = nextChar();
            buffer += ch;
            if (letter_not(ch, 'e') && letter_not(ch, 'r') && letter_not(ch, 'w')
                && letter_not(ch, 'v') && letter_not(ch, 'i') && isalpha(ch)) {
                state = 25;
            }
            else if (is_illegal(ch)) {
                state = 27;
            }
            else if (is_delim(ch)) {
                state = 29;
            }
            else if (isdigit(ch)) {
                state = 31;
            }
            else {
                switch (ch) {
                case 'e': state = 2; break;
                case 'r': state = 7; break;
                case 'w': state = 13; break;
                case 'v': state = 18; break;
                case 'i': state = 22; break;
                case '/': state = 33; break;
                case '<': state = 37; break;
                case '>': state = 40; break;
                case '=': state = 41; break;
                case '!': state = 42; break;
                case '(': state = 51; break;
                case ')': state = 50; break;
                case ',': state = 43; break;
                case ';': state = 44; break;
                case '[': state = 45; break;
                case ']': state = 46; break;
                case '+': state = 47; break;
                case '-': state = 48; break;
                case '*': state = 49; break;
                case '{': state = 58; break;
                case '}': state = 59; break;
                case '\0': state = 52; break;
                default:
                    assert(false);
                    break;
                }
            }
            break;
        case 2:
            state = ret_state_rw(ch, 'l', 3);
            break;
        case 3:
            state = ret_state_rw(ch, 's', 4);
            break;
        case 4:
            state = ret_state_rw(ch, 'e', 5);
            break;
        case 5:
            state = ret_state_ls(ch);
            break;
        case 6:
            state = get_token(Type::RW);
            break;
        case 7:
            state = ret_state_rw(ch, 'e', 8);
            break;
        case 8:
            state = ret_state_rw(ch, 't', 9);
            break;
        case 9:
            state = ret_state_rw(ch, 'u', 10);
            break;
        case 10:
            state = ret_state_rw(ch, 'r', 11);
            break;
        case 11:
            state = ret_state_rw(ch, 'n', 12);
            break;
        case 12:
            state = ret_state_ls(ch);
            break;
        case 13:
            state = ret_state_rw(ch, 'h', 14);
            break;
        case 14:
            state = ret_state_rw(ch, 'i', 15);
            break;
        case 15:
            state = ret_state_rw(ch, 'l', 16);
            break;
        case 16:
            state = ret_state_rw(ch, 'e', 17);
            break;
        case 17:
            state = ret_state_ls(ch);
            break;
        case 18:
            state = ret_state_rw(ch, 'o', 19);
            break;
        case 19:
            state = ret_state_rw(ch, 'i', 20);
            break;
        case 20:
            state = ret_state_rw(ch, 'd', 21);
            break;
        case 21:
            state = ret_state_ls(ch);
            break;
        case 22:
            ch = nextChar();
            buffer += ch;
            if (ch == 'n') state = 23;
            else if (ch == 'f') state = 60;
            else if (letter_not(ch, 'n') && letter_not(ch, 'f'))
                state = 25;
            else if (ch == '\0') state = 53;
            else if (is_split(ch)) state = 26;
            else state = 27;
            break;
        case 23:
            state = ret_state_rw(ch, 't', 24);
            break;
        case 24:
            state = ret_state_ls(ch);
            break;
        case 25:
            ch = nextChar();
            buffer += ch;
            if (isalpha(ch)) state = 25;
            else if (ch == '\0') state = 53;
            else if (is_split(ch)) state = 26;
            else state = 27;
            break;
        case 26:
            state = get_token(Type::ID);
            break;
        case 27:
            ch = nextChar();
            buffer += ch;
            if (is_split(ch)) state = 28;
            else if (ch == '\0') state = 56;
            else state = 27;
            break;
        case 28:
            state = get_token(Type::ERROR);
            break;
        case 29:
            ch = nextChar();
            buffer += ch;
            if (is_delim(ch)) state = 29;
            else state = 30;
            break;
        case 30:
            retract();
            state = 1;
            break;
        case 31:
            ch = nextChar();
            buffer += ch;
            if (isdigit(ch)) state = 31;
            else if (ch == '\0') state = 54;
            else if (is_split(ch)) state = 32;
            else state = 27;
            break;
        case 32:
            state = get_token(Type::NUM);
            break;
        case 33:
            ch = nextChar();
            buffer += ch;
            if (ch == '*') state = 34;
            else if (ch == '\0') state = 56;
            else state = 39;
            break;
        case 34:
            ch = nextChar();
            buffer += ch;
            if (ch == '*') state = 35;
            else if (ch == '\0') state = 56;
            else state = 34;
            break;
        case 35:
            ch = nextChar();
            buffer += ch;
            if (ch == '*') state = 35;
            else if (ch == '/') state = 36;
            else if (ch == '\0') state = 56;
            else state = 34;
            break;
        case 36:
            state = get_token(Type::COMMENT, false, false);
            break;
        case 37:
            ch = nextChar();
            buffer += ch;
            if (ch == '=') state = 38;
            else if (ch == '\0') state = 57;
            else state = 39;
            break;
        case 38:
            state = get_token(Type::OP, false, false);
            break;
        case 39:
            state = get_token(Type::OP);
            break;
        case 40:
            ch = nextChar();
            buffer += ch;
            if (ch == '=') state = 38;
            else if (ch == '\0') state = 57;
            else state = 39;
            break;
        case 41:
            ch = nextChar();
            buffer += ch;
            if (ch == '=') state = 38;
            else if (ch == '\0') state = 57;
            else state = 39;
            break;
        case 42:
            ch = nextChar();
            buffer += ch;
            if (ch == '=') state = 38;
            else if (ch == '\0') state = 56;
            else if (is_split(ch)) state = 28;
            else state = 27;
            break;
        case 43: case 44: case 45: case 46: case 47:
        case 48: case 49: case 50: case 51: case 58: case 59:
            state = get_token(Type::OP, false, false);
            break;
        case 52:
            __EOF = true;
            break;
        case 53:
            state = get_token(Type::ID, false);
            __EOF = true;
            break;
        case 54:
            state = get_token(Type::NUM, false);
            __EOF = true;
            break;
        case 55:
            state = get_token(Type::RW, false);
            __EOF = true;
            break;
        case 56: //< ERROR
            state = get_token(Type::ERROR, false);
            __EOF = true;
            break;
        case 57:
            state = get_token(Type::OP, false);
            __EOF = true;
            break;
        case 60:
            state = ret_state_ls(ch);
            break;
        default:
            //< error state
            assert(false);
            break;
        }
    }
}

