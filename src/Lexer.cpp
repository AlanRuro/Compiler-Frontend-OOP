#include "Lexer.h"
#include <cctype>
#include <stack>

Lexer::Lexer(std::string filename) {
    this->source.open(filename);
    if (!this->source.is_open()) {
        throw std::runtime_error("Cannot open source file: " + filename);
    }
    
    // Initialize indentation stack with 0
    this->indent_stack.push(0);
    readch();
    
    // Reserve keywords
    reserve(Word::And);
    reserve(Word::Or);
    reserve(Word::Not);
    reserve(Word::True);
    reserve(Word::False);
    reserve(Word::None);
    reserve(Word::If);
    reserve(Word::Elif);
    reserve(Word::Else);
    reserve(Word::For);
    reserve(Word::While);
    reserve(Word::Class);
    reserve(Word::Def);
    reserve(Word::Self);
    reserve(Word::Super);
    reserve(Word::Return);
    reserve(Word::Pass);
    reserve(Word::In);
    reserve(Word::Is);
    reserve(Word::Break);
    reserve(Word::Continue);
    reserve(Word::From);
    reserve(Word::Import);

    // Types
    reserve(Word("int", static_cast<int>(Tag::TYPE)));
    reserve(Word("float", static_cast<int>(Tag::TYPE)));
    reserve(Word("str", static_cast<int>(Tag::TYPE)));

    // OOP decorators
    reserve(Word("@property", static_cast<int>(Tag::POO_DECORATOR)));
    reserve(Word("@staticmethod", static_cast<int>(Tag::POO_DECORATOR)));
    reserve(Word("@classmethod", static_cast<int>(Tag::POO_DECORATOR)));
    reserve(Word("@abstractmethod", static_cast<int>(Tag::POO_DECORATOR)));
}

Lexer::~Lexer() {
    if (this->source.is_open()) {
        this->source.close();
    }
    
    // Clean up dynamically allocated Word objects
    for (auto& pair : this->words) {
        // Don't delete static keyword instances
        if (pair.second != &Word::And && 
            pair.second != &Word::Or &&
            pair.second != &Word::Not &&
            pair.second != &Word::True &&
            pair.second != &Word::False &&
            pair.second != &Word::None &&
            pair.second != &Word::If &&
            pair.second != &Word::Elif &&
            pair.second != &Word::Else &&
            pair.second != &Word::For &&
            pair.second != &Word::While &&
            pair.second != &Word::Class &&
            pair.second != &Word::Def &&
            pair.second != &Word::Self &&
            pair.second != &Word::Super &&
            pair.second != &Word::Return &&
            pair.second != &Word::Pass &&
            pair.second != &Word::In &&
            pair.second != &Word::Is &&
            pair.second != &Word::Break &&
            pair.second != &Word::Continue) {
            delete pair.second;
        }
    }
}

void Lexer::reserve(Word w) {
    this->words[w.lexeme] = new Word(w.lexeme, w.tag);
}

void Lexer::readch() {
    this->peek = this->source.get();
    if (this->peek != EOF) {
        this->column++;
    }
}

bool Lexer::readch(char c) {
    readch();
    if (this->peek != c) return false;
    this->peek = ' ';
    return true;
}

Token* Lexer::scan() {
    // Handle any pending dedents
    if (!this->dedent_queue.empty()) {
        Token* t = this->dedent_queue.front();
        this->dedent_queue.pop();
        return t;
    }
    
    // Skip whitespace except for newlines and indentation at the start of a line
    bool at_line_start = this->line_start;
    int spaces = 0;
    
    while (true) {
        if (this->peek == ' ') {
            if (at_line_start) {
                spaces++;
            }
            readch();
        } else if (this->peek == '\t') {
            if (at_line_start) {
                spaces += 4; // Consider tab as 4 spaces in Python
            }
            readch();
        } else if (this->peek == '\r') { // Carriage return
            readch();
        } else {
            break;
        }
    }
    
    // Reset line_start flag now that we've processed any indentation
    this->line_start = false;
    
    // Handle indentation
    if (at_line_start && this->peek != '\n' && this->peek != '#' && this->peek != EOF) {
        int current_indent = this->indent_stack.top();
        
        if (spaces > current_indent) {
            // Indent
            this->indent_stack.push(spaces);
            return new Token(static_cast<int>(Tag::INDENT));
        } else if (spaces < current_indent) {
            // Dedent (possibly multiple levels)
            this->indent_stack.pop();
            
            // Create a DEDENT token
            Token* dedent = new Token(static_cast<int>(Tag::DEDENT));
            
            // Check if we need multiple dedents
            while (!this->indent_stack.empty() && spaces < this->indent_stack.top()) {
                this->indent_stack.pop();
                this->dedent_queue.push(new Token(static_cast<int>(Tag::DEDENT)));
            }
            
            // Verify indentation level is valid
            if (this->indent_stack.empty() || spaces != this->indent_stack.top()) {
                throw std::runtime_error("Invalid indentation at this->line " + std::to_string(this->line));
            }
            
            return dedent;
        }
        // If spaces == current_indent, no indentation token needed
    }
    
    // Handle newlines
    if (this->peek == '\n') {
        this->line++;
        this->column = 0;
        this->line_start = true;  // Next token will be at the start of a line
        readch();
        return new Token(static_cast<int>(Tag::NEWLINE));
    }
    
    // Handle comments
    if (this->peek == '#') {
        // Skip the entire comment line
        while (this->peek != '\n' && this->peek != EOF) {
            readch();
        }
        if (this->peek == '\n') {
            this->line++;
            this->column = 0;
            this->line_start = true;
            readch();
            return new Token(static_cast<int>(Tag::NEWLINE));
        }
    }
    
    // Handle EOF
    if (this->peek == EOF) {
        // Siempre generamos los DEDENTs pendientes al encontrar EOF
        // Si hay tokens en la cola de DEDENTs, los procesamos primero
        if (!this->dedent_queue.empty()) {
            Token* t = this->dedent_queue.front();
            this->dedent_queue.pop();
            return t;
        }
        
        // Si el stack tiene más de un nivel, generamos un DEDENT
        if (this->indent_stack.size() > 1) {
            this->indent_stack.pop();
            Token* dedent = new Token(static_cast<int>(Tag::DEDENT));
            
            // Guardamos los DEDENTs adicionales en la cola
            while (this->indent_stack.size() > 1) {
                this->indent_stack.pop();
                this->dedent_queue.push(new Token(static_cast<int>(Tag::DEDENT)));
            }
            
            return dedent;
        }
        
        // Si no hay DEDENTs pendientes, retornamos nullptr
        return nullptr;
    }
    
    // Handle decorators (@ symbol)
    if (this->peek == '@') {
        std::string buffer;
        do {
            buffer += this->peek;
            readch();
        } while (std::isalnum(this->peek) || this->peek == '_' || this->peek == '.');

        // Return OOP decorator if applies
        auto it = this->words.find(buffer);
        if (it != this->words.end()) {
            return it->second;  
        }

        Word* w = new Word(buffer, static_cast<int>(Tag::DECORATOR));
        this->words[buffer] = w;
        return w;
    }
    
    // Handle identifiers and keywords
    if (std::isalpha(this->peek) || this->peek == '_') {
        std::string buffer;
        do {
            buffer += this->peek;
            readch();
        } while (std::isalnum(this->peek) || this->peek == '_');
        
        // Check for special method names (starting with __)
        if (buffer.size() >= 4 && buffer.substr(0, 2) == "__" && buffer.substr(buffer.size() - 2) == "__") {
            Word* w = new Word(buffer, static_cast<int>(Tag::SPECIAL_NAME));
            return w;
        }
        
        // Check if identifier is a keyword
        auto it = this->words.find(buffer);
        if (it != this->words.end()) {
            return it->second; // Return the keyword token
        }

        
        // Not a keyword, so create a new identifier token
        Word* w = new Word(buffer, static_cast<int>(Tag::VARIABLE));
        this->words[buffer] = w;
        return w;
    }
    
    // Handle numbers
    if (std::isdigit(this->peek)) {
        int value = 0;
        do {
            value = 10 * value + (this->peek - '0');
            readch();
        } while (std::isdigit(this->peek));
        
        // Check for float
        if (this->peek == '.') {
            std::string float_str = std::to_string(value) + ".";
            readch();
            
            // Parse decimal part
            if (std::isdigit(this->peek)) {
                do {
                    float_str += this->peek;
                    readch();
                } while (std::isdigit(this->peek));
                
                // For simplicity, we'll still return a Num token
                // In a full implementation, you'd want to create a Real or Float token
                return new Num(std::stoi(float_str));
            }
        }
        
        return new Num(value);
    }
    
    // Handle strings
    if (this->peek == '"' || this->peek == '\'') {
        char quote = this->peek;
        std::string str;
        readch(); // Consume the opening quote
        
        while (this->peek != quote && this->peek != EOF && this->peek != '\n') {
            if (this->peek == '\\') {
                readch(); // Skip escape character
                switch (this->peek) {
                    case 'n': str += '\n'; break;
                    case 't': str += '\t'; break;
                    case 'r': str += '\r'; break;
                    case '\\': str += '\\'; break;
                    case '\'': str += '\''; break;
                    case '\"': str += '\"'; break;
                    default: str += this->peek;
                }
            } else {
                str += this->peek;
            }
            readch();
        }
        
        if (this->peek == quote) {
            readch(); // Consume the closing quote
            Word* w = new Word(str, static_cast<int>(Tag::STRING));
            return w;
        } else {
            throw std::runtime_error("Unterminated string at this->line " + std::to_string(this->line));
        }
    }
    
    // Handle operators and punctuation
    Token* t = nullptr;
    switch (this->peek) {
        case ':':
            t = new Token(static_cast<int>(Tag::COLON));
            break;
        case ',':
            t = new Token(static_cast<int>(Tag::COMMA));
            break;
        case '.':
            t = new Token(static_cast<int>(Tag::DOT));
            break;
        case '(':
            t = new Token(static_cast<int>(Tag::OPEN_PARENTHESIS));
            break;
        case ')':
            t = new Token(static_cast<int>(Tag::CLOSE_PARENTHESIS));
            break;
        case '[':
            t = new Token(static_cast<int>(Tag::OPEN_BRACKET));
            break;
        case ']':
            t = new Token(static_cast<int>(Tag::CLOSE_BRACKET));
            break;
        case '{':
            t = new Token(static_cast<int>(Tag::OPEN_BRACE));
            break;
        case '}':
            t = new Token(static_cast<int>(Tag::CLOSE_BRACE));
            break;
        case '+':
            t = new Token(static_cast<int>(Tag::PLUS));
            break;
        case '-':
            readch();
            if (this->peek == '>') {
                readch();
                t = new Token(static_cast<int>(Tag::ARROW));
            } else {
                t = new Token(static_cast<int>(Tag::MINUS));
                // Don't readch() again as we already did above
                return t;
            }
            break;
        case '*':
            t = new Token(static_cast<int>(Tag::MULT));
            break;
        case '/':
            t = new Token(static_cast<int>(Tag::DIV));
            break;
        case '%':
            t = new Token(static_cast<int>(Tag::MOD));
            break;
        case '=':
            readch();
            if (this->peek == '=') {
                readch();
                t = new Word("==", static_cast<int>(Tag::LOGIC_OP));
            } else {
                t = new Token(static_cast<int>(Tag::ASSIGN));
                // Don't readch() again as we already did above
                return t;
            }
            break;
        case '!':
            readch();
            if (this->peek == '=') {
                readch();
                t = new Word("!=", static_cast<int>(Tag::LOGIC_OP));
            } else {
                throw std::runtime_error("Unexpected character: ! at line " + std::to_string(this->line));
            }
            break;
        case '<':
            readch();
            if (this->peek == '=') {
                readch();
                t = new Word("<=", static_cast<int>(Tag::LOGIC_OP));
            } else {
                t = new Word("<", static_cast<int>(Tag::LOGIC_OP));
                // Don't readch() again as we already did above
                return t;
            }
            break;
        case '>':
            readch();
            if (this->peek == '=') {
                readch();
                t = new Word(">=", static_cast<int>(Tag::LOGIC_OP));
            } else {
                t = new Word(">", static_cast<int>(Tag::LOGIC_OP));
                // Don't readch() again as we already did above
                return t;
            }
            break;
        default:
            throw std::runtime_error("Unexpected character: " + std::string(1, this->peek) + 
                                     " at line " + std::to_string(this->line) + 
                                     ", column " + std::to_string(this->column));
    }
    
    readch();
    return t;
}