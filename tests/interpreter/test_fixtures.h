//
// Created by toebs on 30.03.2020.
//

#ifndef ANGREAL_TESTS_INTERPRETER_TEST_FIXTURES_H_
#define ANGREAL_TESTS_INTERPRETER_TEST_FIXTURES_H_

#include <gtest/gtest.h>

#include "analysis/semantic/semantic_analyzer.h"
#include "context.h"
#include "parser/ast.h"

using namespace angreal;
using namespace angreal::parser;
using namespace angreal::interpreter;
using namespace angreal::interpreter::environment;

class BaseTest : public ::testing::Test {
   public:
   protected:
    Context context_;
};

class DeclarationTest : public BaseTest {
   public:
    type_t GetResultType() {
        auto obj = context_.interpreter->Stack().top();
        return obj->GetType();
    }

    type_t GetResultType(const std::string& name) {
        auto obj = context_.global->Get(name);
        return obj->GetType();
    }

    void DeclareString(const std::string& name, const std::string& value) {
        auto decl = std::make_shared<AST::Declaration>(
            name, std::make_shared<StringLiteral>(value));
        context_.interpreter->visit(decl);
    }

    void DeclareInt(const std::string& name, int value) {
        auto decl = std::make_shared<AST::Declaration>(
            name, std::make_shared<IntLiteral>(value));
        context_.interpreter->visit(decl);
    }

    void DeclareBool(const std::string& name, bool value) {
        auto decl = std::make_shared<AST::Declaration>(
            name, std::make_shared<BoolLiteral>(value));
        context_.interpreter->visit(decl);
    }

    void DeclareFloat(const std::string& name, float value) {
        auto decl = std::make_shared<AST::Declaration>(
            name, std::make_shared<FloatLiteral>(value));
        context_.interpreter->visit(decl);
    }
};

class UnaryOpTest : public DeclarationTest {
   protected:
    void DeclareUnaryOp(const std::string& op, const std::string& lit) {
        auto unary_op = std::make_shared<UnaryOperation>(
            op, std::make_shared<IdentifierLiteral>(lit));
        context_.interpreter->visit(unary_op);
    }
};

class BinaryOpTest : public DeclarationTest {
   protected:
    void DeclareBinaryOp(const std::string op, const std::string lhs,
                         const std::string rhs) {
        auto bin_op = std::make_shared<BinaryOperation>(
            op, std::make_shared<IdentifierLiteral>(lhs),
            std::make_shared<IdentifierLiteral>(rhs));

        context_.interpreter->visit(bin_op);
    }
};

class FloatBinaryOpTest : public BinaryOpTest {
   protected:
    FloatBinaryOpTest() {
        DeclareFloat("a", 10.0);
        DeclareFloat("b", -5.0);
    }
};

class IntBinaryOpTest : public BinaryOpTest {
   protected:
    IntBinaryOpTest() {
        DeclareInt("a", 10);
        DeclareInt("b", -5);
    }
};

class BooleanBinaryOpTest : public BinaryOpTest {
   protected:
    BooleanBinaryOpTest() {
        DeclareBool("a", false);
        DeclareBool("b", true);
    }
};

class StringBinaryOpTest : public BinaryOpTest {
   protected:
    StringBinaryOpTest() {
        DeclareString("a", "a");
        DeclareString("b", "b");
    }
};

class FunctionTest : public DeclarationTest {
    void DeclareFunction(const std::string& name,
                         const statements_t& statements,
                         const formal_parameters& parameters) {
        auto declaration =
            std::make_shared<FunctionDeclaration>(name, parameters, statements);
        last_function_ = std::make_shared<IdentifierLiteral>(name);

        auto analyzer = std::make_shared<analysis::SemanticAnalyzer>(
            *context_.interpreter.get());

        analyzer->Resolve(declaration);
        context_.interpreter->visit(declaration);
    }

   protected:
    expression_t last_function_;

    const expressions_t kNoArgs {};

    void DeclareArityZeroFunction(const std::string& name) {
        auto inner_decl = std::make_shared<AST::Declaration>(
            "text", std::make_shared<StringLiteral>("World"));

        auto ident = std::make_shared<AST::IdentifierLiteral>("text");
        context_.interpreter->ResolveLocal(ident, 0);
        statements_t statements {inner_decl,
                                 std::make_shared<AST::Return>(ident)};

        formal_parameters parameters = {};
        DeclareFunction(name, statements, parameters);
    }

    void DeclareArityOneIntFunction(const std::string& name, int init_value) {
        auto ident = std::make_shared<AST::IdentifierLiteral>("test");
        context_.interpreter->ResolveLocal(ident, 0);

        statements_t statements {
            std::make_shared<AST::Declaration>(
                "test", std::make_shared<BinaryOperation>(
                            "+", std::make_shared<IntLiteral>(init_value),
                            std::make_shared<IdentifierLiteral>("arg1"))),

            std::make_shared<AST::Return>(ident)};

        formal_parameters parameters = {
            std::make_shared<FormalParameter>("arg1")};

        DeclareFunction(name, statements, parameters);
    }

    void DeclareArityOneIntFunctionWithTwoReturns(const std::string& name,
                                                  int init_value) {
        auto ident = std::make_shared<AST::IdentifierLiteral>("test");
        context_.interpreter->ResolveLocal(ident, 0);

        statements_t statements {
            std::make_shared<AST::Declaration>(
                "test", std::make_shared<BinaryOperation>(
                            "+", std::make_shared<IntLiteral>(init_value),
                            std::make_shared<IdentifierLiteral>("arg1"))),
            std::make_shared<AST::Return>(
                std::make_shared<IntLiteral>(init_value)),
            std::make_shared<AST::Return>(ident)};

        formal_parameters parameters = {
            std::make_shared<FormalParameter>("arg1")};
        DeclareFunction(name, statements, parameters);
    }
};

class ClassTest : public FunctionTest {
   protected:
    expression_t last_class_;
    const functions_t kNoFunctions {};

    void DeclareClass(const std::string& name, const functions_t& methods,
                      const std::string& base_class = "") {
        std::optional<identifier_t> superclass {std::nullopt};
        if (!base_class.empty()) {
            superclass = std::make_shared<IdentifierLiteral>(base_class);
        }
        auto declaration =
            std::make_shared<ClassDeclaration>(name, methods, superclass);

        last_class_ = std::make_shared<IdentifierLiteral>(name);
        auto analyzer = std::make_shared<analysis::SemanticAnalyzer>(
            *context_.interpreter.get());
        analyzer->Resolve(declaration);
        context_.interpreter->visit(declaration);
    }

    void DeclareEmptyClass(const std::string& name) {
        functions_t methods = {};
        DeclareClass(name, methods);
    }

    statement_t AssignMember(const std::string& member_name,
                             const expression_t& value) {
        return std::make_shared<ExpressionStatement>(std::make_shared<Set>(
            std::make_shared<Self>(), member_name, value));
    }

    void DeclareSingleFunctionClass(const std::string& name,
                                    const std::string& method_name,
                                    int return_value = 42) {
        functions_t methods = {std::make_shared<FunctionDeclaration>(
            method_name, formal_parameters {},
            statements_t {std::make_shared<Return>(
                std::make_shared<IntLiteral>(return_value))})};
        DeclareClass(name, methods);
    }

    void DeclareClassWithInitializerNoArgs(const std::string& name,
                                           const std::string& member_name,
                                           int member_value = 42) {
        functions_t methods = {std::make_shared<FunctionDeclaration>(
            "init", formal_parameters {},
            statements_t {
                AssignMember(member_name,
                             std::make_shared<IntLiteral>(member_value)),
            })};
        DeclareClass(name, methods);
    }

    void DeclareClassWithInitializerSingleArg(const std::string& name,
                                              const std::string& member_name,
                                              const std::string& arg_name) {
        functions_t methods = {std::make_shared<FunctionDeclaration>(
            "init",
            formal_parameters {std::make_shared<FormalParameter>(arg_name)},
            statements_t {
                AssignMember(member_name,
                             std::make_shared<IdentifierLiteral>(arg_name)),
            })};
        DeclareClass(name, methods);
    }

    void DeclareClassWithReturnInitializer(const std::string& name) {
        functions_t methods = {std::make_shared<FunctionDeclaration>(
            "init", formal_parameters {},
            statements_t {
                std::make_shared<Return>(expression_t()),
            })};
        DeclareClass(name, methods);
    }

    void DeclareClassWithMethodReturningSelf(const std::string& name,
                                             const std::string& method_name) {
        functions_t methods = {std::make_shared<FunctionDeclaration>(
            method_name, formal_parameters {},
            statements_t {std::make_shared<Return>(std::make_shared<Self>())})};
        DeclareClass(name, methods);
    }

    void DeclareClassWithInheritance(const std::string& name,
                                     const std::string& base_class,
                                     const std::string& method_name) {
        functions_t methods = {std::make_shared<FunctionDeclaration>(
            method_name, formal_parameters {},
            statements_t {std::make_shared<Return>(std::make_shared<Self>())})};

        DeclareClass(base_class, methods);
        DeclareClass(name, kNoFunctions, base_class);
    }
};
#endif  // ANGREAL_TESTS_INTERPRETER_TEST_FIXTURES_H_
