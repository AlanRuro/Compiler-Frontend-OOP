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
    reserve(Word::Any);
    reserve(Word::If);
    reserve(Word::Elif);
    reserve(Word::Else);
    reserve(Word::For);
    reserve(Word::While);
    reserve(Word::Class);
    reserve(Word::Def);
    reserve(Word::Init);
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

    // OOP decorators
    reserve(Word::Property);
    reserve(Word::StaticMethod);
    reserve(Word::ClassMethod);
    reserve(Word::AbstractMethod);
    reserve(Word::Cls);

    // Types
    reserve(Word("int", static_cast<int>(Tag::TYPE)));
    reserve(Word("float", static_cast<int>(Tag::TYPE)));
    reserve(Word("str", static_cast<int>(Tag::TYPE)));
    reserve(Word("list", static_cast<int>(Tag::TYPE)));
    reserve(Word("dict", static_cast<int>(Tag::TYPE)));
    reserve(Word("tuple", static_cast<int>(Tag::TYPE)));
    reserve(Word("set", static_cast<int>(Tag::TYPE)));
    reserve(Word("bool", static_cast<int>(Tag::TYPE)));

}

Lexer::~Lexer() {
    if (this->source.is_open()) {
        this->source.close();
    }
    
    // Clean up dynamically allocated Word objects
    for (auto& pair : this->words) {
        delete pair.second;
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


void Lexer::skipWhitespace(bool at_line_start) {
    // Skip whitespace except for newlines and indentation at the start of a line
    this->spaces = 0;
    while (true) {
        if (this->peek == ' ') {
            if (at_line_start) {
                this->spaces++;
            }
            readch();
        } else if (this->peek == '\t') {
            if (at_line_start) {
                this->spaces += 4; // Consider tab as 4 spaces in Python
            }
            readch();
        } else if (this->peek == '\r') { // Carriage return
            readch();
        } else {
            break;
        }
    }
}

Token* Lexer::handlePendingDedents() {
    Token* t = this->dedent_queue.front();
    this->dedent_queue.pop();
    return t;
}

Token* Lexer::handleDedents() {
    // Dedent (possibly multiple levels)
    this->indent_stack.pop();
    
    // Create a DEDENT token
    Token* dedent = new Token(static_cast<int>(Tag::DEDENT));
    
    // Check if we need multiple dedents
    while (!this->indent_stack.empty() && this->spaces < this->indent_stack.top()) {
        this->indent_stack.pop();
        this->dedent_queue.push(new Token(static_cast<int>(Tag::DEDENT)));
    }
    
    // Verify indentation level is valid
    if (this->indent_stack.empty() || this->spaces != this->indent_stack.top()) {
        throw std::runtime_error("Invalid indentation at this->line " + std::to_string(this->line));
    }
    
    return dedent;
}

Token* Lexer::handleIdent() {
    // Indent
    this->indent_stack.push(this->spaces);
    return new Token(static_cast<int>(Tag::INDENT));
}

Token* Lexer::handleNewLines() {
    this->line++;
    this->column = 0;
    this->line_start = true;  // Next token will be at the start of a line
    readch();
    return new Token(static_cast<int>(Tag::NEWLINE));
}

void Lexer::handleComments() {
    // Skip the entire comment line
    while (this->peek != '\n' && this->peek != EOF) {
        readch();
    }
}

Token* Lexer::handleEOF() {
    // Si hay tokens en la cola de DEDENTs, los procesamos primero
    if (!this->dedent_queue.empty()) {
        return handlePendingDedents();
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
    
    return nullptr;
}

Word* Lexer::findKeyword(std::string word) {
    auto it = this->words.find(word);
    if (it != this->words.end()) {
        return it->second; // Return the keyword token
    }
    return nullptr;
}

Token* Lexer::handleVariables(int tag) {
    std::string buffer;
    do {
        buffer += this->peek;
        readch();
    } while (std::isalnum(this->peek) || this->peek == '_');
    
    // Check if identifier is a keyword
    if (Word* word = findKeyword(buffer)) {
        return word;
    }

    // Not a keyword, so create a new identifier token
    Word* w = new Word(buffer, tag);
    this->words[buffer] = w;
    return w;
}

Token* Lexer::handleNumbers() {
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
            
            // In a full implementation, you'd want to create a Real or Float token
            return new Num(std::stoi(float_str));
        }
    }
    
    return new Num(value);
}

Token* Lexer::handleStrings() {
    char quote = this->peek;
    std::string str;
    std::string quoteBuffer;
    
    // Read opening quotes into buffer
    while (this->peek == quote && quoteBuffer.length() < 3) {
        quoteBuffer += this->peek;
        readch();
    }
    
    bool isDocString = (quoteBuffer.length() == 3);
    
    // If it's not a docstring, add the quotes to the string
    if (!isDocString) {
        str = quoteBuffer;
    }
    
    while (true) {
        if (isDocString) {
            // For docstrings, check for triple quotes to end
            if (this->peek == quote) {
                quoteBuffer.clear();
                while (this->peek == quote && quoteBuffer.length() < 3) {
                    quoteBuffer += this->peek;
                    readch();
                }
                
                if (quoteBuffer.length() == 3) {
                    break;
                }
                
                // Not enough quotes, add them to the string
                str += quoteBuffer;
                continue;
            }
        } else if (this->peek == quote) {
            readch(); // Consume the closing quote for normal strings
            break;
        }
        
        if (this->peek == EOF || (!isDocString && this->peek == '\n')) {
            throw std::runtime_error("Unterminated " + std::string(isDocString ? "docstring" : "string") + 
                                   " at line " + std::to_string(this->line));
        }
        
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
    
    Word* w = new Word(str, static_cast<int>(isDocString ? Tag::DOCSTRING : Tag::STRING));
    return w;
}

Token* Lexer::handleOperators() {
    Token* t = nullptr;
    switch (this->peek) {
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
        return nullptr;
    }
    
    readch();
    return t;
}

Token* Lexer::handlePunctuation() {
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
    default:
        return nullptr;
    }
    readch();
    return t;
}


Token* Lexer::scan() {
    // Handle any pending dedents
    if (!this->dedent_queue.empty()) {
        return handlePendingDedents();
    }
    
    bool at_line_start = this->line_start;
    skipWhitespace(at_line_start);
    // Reset line_start flag now that we've processed any indentation
    this->line_start = false;
    
    // Handle indentation
    if (at_line_start && this->peek != '\n' && this->peek != '#' && this->peek != EOF) {
        int current_indent = this->indent_stack.top();
    
        if (this->spaces > current_indent) {
            return handleIdent();
        } else if (this->spaces < current_indent) {
            return handleDedents();
        }
        // If spaces == current_indent, no indentation token needed
    }
    
    // Handle comments
    if (this->peek == '#') {
        handleComments();
        return scan();
    }

    // Handle newlines
    if (this->peek == '\n') {
        return handleNewLines();
    }
    
    // Handle EOF
    if (this->peek == EOF) {
        return handleEOF();
    }
    
    // Handle decorators (@ symbol)
    if (this->peek == '@') {
        return handleVariables(static_cast<int>(Tag::DECORATOR));
    }
    
    // Handle identifiers and keywords
    if (std::isalpha(this->peek) || this->peek == '_') {
        return handleVariables(static_cast<int>(Tag::VARIABLE));
    }
    
    // Handle numbers
    if (std::isdigit(this->peek)) {
        return handleNumbers();
    }
    
    // Handle strings
    if (this->peek == '"' || this->peek == '\'') {
        return handleStrings();
    }
    
    Token* t = nullptr;
    t = handlePunctuation();
    if (t) return t;
    else t = handleOperators();
    if (t) return t;
    else throw std::runtime_error("Unexpected character: " + std::string(1, this->peek) + 
                                    " at line " + std::to_string(this->line) + 
                                    ", column " + std::to_string(this->column));
    
}