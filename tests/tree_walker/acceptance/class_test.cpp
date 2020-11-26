//
// Created by toebs on 17.05.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(ClassTest, EmptyClass) {
    std::string code = R"(
        class MyClass
        {
        }
        print(MyClass)

        var myclass = MyClass()
        print(myclass)
    )";

    SafeRun(code, "class(MyClass)\nInstance of class(MyClass)");
}

TEST_F(ClassTest, ClassGetterSetter) {
    std::string code = R"(
        class MyClass
        {
        }
        var myclass = MyClass()
        myclass.x = 123
        print(myclass.x);
    )";

    SafeRun(code, "123");
}

TEST_F(ClassTest, ClassMethods) {
    std::string code = R"(
    class Bacon
    {
        def eat()
        {
            print("Crunch crunch crunch!")
        }
    }

    Bacon().eat()
    )";

    SafeRun(code, R"("Crunch crunch crunch!")");
}

TEST_F(ClassTest, BoundMethods) {
    std::string code = R"(
    class Bacon
    {
        def eat()
        {
            return "Mmh "+ self.name
        }
    }
    var bacon = Bacon()
    bacon.name = "Bacon"
    var eat = bacon.eat
    print(eat())
    )";

    SafeRun(code, R"("Mmh Bacon")");
}

TEST_F(ClassTest, Initializer) {
    std::string code = R"(
    class Bacon
    {
        def init()
        {
            self.name = "Bacon"
        }

        def eat()
        {
            return "Mmh "+ self.name
        }
    }
    var bacon = Bacon()
    print(bacon.eat())
    )";

    SafeRun(code, R"("Mmh Bacon")");
}

TEST_F(ClassTest, ProperBindingOfSuper) {
    std::string code = R"(
    class A {
      def method() {
        print("A method");
      }
    }

    class B(A) {
      def method() {
        print("B method");
      }

      def test() {
        super.method();
      }
    }

    class C(B) {}

    C().test();
    )";

    SafeRun(code, R"("A method")");
}

TEST_F(ClassTest, ProperBindingOfSuperWithIntializers) {
    std::string code = R"(
    class A {
      def init()
      {
        self.name = "A";
      }

      def method() {
        print(self.name + " method");
      }
    }

    class B(A) {
      def init()
      {
        super.init();
        self.name = "B";
      }

      def test() {
        super.method();
      }
    }

    B().test();
    )";

    SafeRun(code, R"("B method")");
}