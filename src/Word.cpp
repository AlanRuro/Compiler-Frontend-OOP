#include "Word.h"

Word::Word(std::string s, int tag) : Token(tag) {
    this->lexeme = s;
}

std::string Word::toString() {
    return lexeme;
}

// Initialize static constants
const Word Word::And("and", static_cast<int>(Tag::AND));
const Word Word::Or("or", static_cast<int>(Tag::OR));
const Word Word::Not("not", static_cast<int>(Tag::NOT));
const Word Word::True("True", static_cast<int>(Tag::TRUE));
const Word Word::False("False", static_cast<int>(Tag::FALSE));
const Word Word::None("None", static_cast<int>(Tag::NONE));
const Word Word::Any("any", static_cast<int>(Tag::ANY));
const Word Word::If("if", static_cast<int>(Tag::IF));
const Word Word::Elif("elif", static_cast<int>(Tag::ELIF));
const Word Word::Else("else", static_cast<int>(Tag::ELSE));
const Word Word::For("for", static_cast<int>(Tag::FOR));
const Word Word::While("while", static_cast<int>(Tag::WHILE));
const Word Word::Class("class", static_cast<int>(Tag::CLASS));
const Word Word::Def("def", static_cast<int>(Tag::DEF));
const Word Word::Init("__init__", static_cast<int>(Tag::INIT));
const Word Word::Self("self", static_cast<int>(Tag::SELF));
const Word Word::Super("super", static_cast<int>(Tag::SUPER));
const Word Word::Return("return", static_cast<int>(Tag::RETURN));
const Word Word::Pass("pass", static_cast<int>(Tag::PASS));
const Word Word::In("in", static_cast<int>(Tag::IN));
const Word Word::Is("is", static_cast<int>(Tag::IS));
const Word Word::Break("break", static_cast<int>(Tag::BREAK));
const Word Word::Continue("continue", static_cast<int>(Tag::CONTINUE));
const Word Word::From("from", static_cast<int>(Tag::FROM));
const Word Word::Import("import", static_cast<int>(Tag::IMPORT));

const Word Word::Property("@property", static_cast<int>(Tag::PROPERTY));
const Word Word::StaticMethod("@staticmethod", static_cast<int>(Tag::STATICMETHOD));
const Word Word::ClassMethod("@classmethod", static_cast<int>(Tag::CLASSMETHOD));
const Word Word::AbstractMethod("@abstractmethod", static_cast<int>(Tag::ABSTRACTMETHOD));
const Word Word::Cls("cls", static_cast<int>(Tag::CLS));