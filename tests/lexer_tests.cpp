#include <gtest/gtest.h>
#include "Lexer.h"
#include <fstream>
#include <sstream>
#include <memory>

// Fixture para las pruebas del Lexer
class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Crear un archivo temporal para las pruebas
        tempFile = "temp_test.py";
    }

    void TearDown() override {
        // Limpiar el archivo temporal después de las pruebas
        std::remove(tempFile.c_str());
    }

    void writeToTempFile(const std::string& content) {
        std::ofstream file(tempFile);
        file << content;
        file.close();
    }

    std::string tempFile;
};

// Test básico para verificar que el Lexer puede abrir un archivo
TEST_F(LexerTest, CanOpenFile) {
    writeToTempFile("print('Hello')");
    EXPECT_NO_THROW({
        Lexer lexer(tempFile);
    });
}

// Test para verificar que el Lexer lanza una excepción con un archivo inexistente
TEST_F(LexerTest, ThrowsOnNonExistentFile) {
    EXPECT_THROW({
        Lexer lexer("nonexistent_file.py");
    }, std::runtime_error);
}

// Test para verificar el reconocimiento de palabras clave
TEST_F(LexerTest, RecognizesKeywords) {
    writeToTempFile("@property def class if else");
    Lexer lexer(tempFile);
    auto stream = lexer.generateStream();
    
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::PROPERTY));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::DEF));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::CLASS));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::IF));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::ELSE));
    
}

// Test para verificar el reconocimiento de identificadores
TEST_F(LexerTest, RecognizesIdentifiers) {
    writeToTempFile("myVariable _private_var");
    Lexer lexer(tempFile);
    auto stream = lexer.generateStream();
    
    Token* token = stream->next();
    EXPECT_EQ(token->tag, static_cast<int>(Tag::VARIABLE));
    EXPECT_EQ(dynamic_cast<Word*>(token)->lexeme, "myVariable");
    
    token = stream->next();
    EXPECT_EQ(token->tag, static_cast<int>(Tag::VARIABLE));
    EXPECT_EQ(dynamic_cast<Word*>(token)->lexeme, "_private_var");
}

// Test para verificar el reconocimiento de números
TEST_F(LexerTest, RecognizesNumbers) {
    writeToTempFile("123 45.67");
    Lexer lexer(tempFile);
    auto stream = lexer.generateStream();
    
    Token* token = stream->next();
    EXPECT_EQ(token->tag, static_cast<int>(Tag::NUM));
    EXPECT_EQ(dynamic_cast<Num*>(token)->value, 123);
        
}

// Test para verificar el manejo de indentación
TEST_F(LexerTest, HandlesIndentation) {
    writeToTempFile("def test():\n    x = 1\n    y = 2");
    Lexer lexer(tempFile);
    auto stream = lexer.generateStream();
    
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::DEF));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::VARIABLE));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::OPEN_PARENTHESIS));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::CLOSE_PARENTHESIS));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::COLON));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::NEWLINE));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::INDENT));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::VARIABLE));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::ASSIGN));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::NUM));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::NEWLINE));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::VARIABLE));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::ASSIGN));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::NUM));
    EXPECT_EQ(stream->next()->tag, static_cast<int>(Tag::DEDENT));
        
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 