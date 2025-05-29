#include <gtest/gtest.h>
#include "Parser.h"
#include "Lexer.h"
#include "RecursiveDescendant.h"
#include <fstream>
#include <sstream>
#include <memory>

// Fixture para las pruebas del Parser
class ParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        tempFile = "temp_test.py";
    }

    void TearDown() override {
        std::remove(tempFile.c_str());
    }

    void writeToTempFile(const std::string& content) {
        std::ofstream file(tempFile);
        file << content;
        file.close();
    }

    // Función helper para crear un parser desde un string
    std::unique_ptr<RecursiveDescendant> createParser(const std::string& content) {
        writeToTempFile(content);
        Lexer lexer(tempFile);
        auto stream = lexer.generateStream();
        return std::make_unique<RecursiveDescendant>(stream);
    }

    std::string tempFile;
};

// Test para verificar el parsing de una definición de clase simple
TEST_F(ParserTest, ParsesSimpleClass) {
    std::string code = 
        "class MyClass:\n"
        "    def __init__(self):\n"
        "        pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de una clase con herencia
TEST_F(ParserTest, ParsesClassWithInheritance) {
    std::string code = 
        "class Child(Parent1, Parent2):\n"
        "    def __init__(self):\n"
        "        pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de un método simple
TEST_F(ParserTest, ParsesSimpleMethod) {
    std::string code = 
        "def my_method(self):\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de un método con parámetros
TEST_F(ParserTest, ParsesMethodWithParameters) {
    std::string code = 
        "def my_method(self, param1: int, param2: str = 'default'):\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de un método de clase
TEST_F(ParserTest, ParsesClassMethod) {
    std::string code = 
        "@classmethod\n"
        "def my_class_method(cls, param: int):\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de un método estático
TEST_F(ParserTest, ParsesStaticMethod) {
    std::string code = 
        "@staticmethod\n"
        "def my_static_method(param: int):\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de un método con decorador property
TEST_F(ParserTest, ParsesPropertyMethod) {
    std::string code = 
        "@property\n"
        "def my_property(self):\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de un método con tipo de retorno
TEST_F(ParserTest, ParsesMethodWithReturnType) {
    std::string code = 
        "def my_method(self) -> int:\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de un método con parámetros especiales
TEST_F(ParserTest, ParsesMethodWithSpecialParameters) {
    std::string code = 
        "def my_method(self, *args, **kwargs):\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el parsing de una clase completa
TEST_F(ParserTest, ParsesCompleteClass) {
    std::string code = 
        "class MyClass(Parent):\n"
        "    @property\n"
        "    def my_property(self) -> int:\n"
        "        pass\n"
        "\n"
        "    @classmethod\n"
        "    def my_class_method(cls, param: int) -> str:\n"
        "        pass\n"
        "\n"
        "    @staticmethod\n"
        "    def my_static_method(param: str = 'default'):\n"
        "        pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

// Test para verificar el manejo de errores sintácticos
TEST_F(ParserTest, HandlesSyntaxErrors) {
    // Falta el ':' después de la definición de la clase
    std::string code = "class MyClass\n    pass";
    
    auto parser = createParser(code);
    EXPECT_THROW(parser->parse(), std::runtime_error);
}

// Test para verificar el manejo de errores sintácticos de decoradores
TEST_F(ParserTest, HandlesDecoratorSyntax) {
    // @property va con parametro self
    std::string code = 
        "@property\n"
        "def my_property(cls):\n"
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_THROW(parser->parse(), std::runtime_error);
}

// Test para verificar el manejo de indentación incorrecta
TEST_F(ParserTest, HandlesInvalidIndentation) {
    std::string code = 
        "def my_method():\n"
        "  def __init__(self):\n"  // Indentación incorrecta (2 espacios en lugar de 4)
        "    pass";
    
    auto parser = createParser(code);
    EXPECT_THROW(parser->parse(), std::runtime_error);
}

// Test para verificar el parsing de múltiples definiciones
TEST_F(ParserTest, ParsesMultipleDefinitions) {
    std::string code = 
        "class Class1:\n"
        "    def __init__(self):\n"
        "       pass\n"
        "\n"
        "class Class2:\n"
        "    @property\n"
        "    def prop(self):\n"
        "        pass\n"
        "\n"
        "    def method(self):\n"
        "        pass";
    
    auto parser = createParser(code);
    EXPECT_NO_THROW(parser->parse());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 