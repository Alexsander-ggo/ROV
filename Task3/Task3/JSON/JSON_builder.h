#ifndef JSON_BUILDER_H
#define JSON_BUILDER_H

#include "JSON/JSON.h"

#include <memory>
#include <stack>
#include <string>

namespace json {

    class BuildConstructor;
    class BuildContextFirst;
    class BuildContextSecond;
    class KeyContext;
    class ValueKeyContext;
    class ValueArrayContext;
    class DictContext;
    class ArrayContext;
    class Builder;

    //------------ BuildConstructor ---------------

    class BuildConstructor {
    public:
        // Конструктор
        explicit BuildConstructor(Builder& builder);

    protected:
        Builder& mBuilder;  // Ссылка на билдера JSON
    };

    //------------ BuildContextFirst ---------------

    class BuildContextFirst : public BuildConstructor {
    public:
        // Конструктор
        explicit BuildContextFirst(Builder& builder);

        // Задание начала словаря
        DictContext& startDict();

        // Задание начала массива
        ArrayContext& startArray();
    };

    //------------ BuildContextSecond ---------------

    class BuildContextSecond : public BuildConstructor {
    public:
        // Конструктор
        explicit BuildContextSecond(Builder& builder);

        // Задание ключа
        KeyContext& key(const std::string& key);

        // Задание конца словаря
        Builder& endDict();
    };

    //------------ KeyContext ---------------

    class KeyContext : public BuildContextFirst {
    public:
        // Конструктор
        explicit KeyContext(Builder& builder);

        // Задание значения
        ValueKeyContext& value(const Node::Value& value);
    };

    //------------ ValueKeyContext ---------------

    class ValueKeyContext : public BuildContextSecond {
    public:
        // Конструктор
        explicit ValueKeyContext(Builder& builder);
    };

    //------------ ValueArrayContext ---------------

    class ValueArrayContext : public BuildContextFirst {
    public:
        // Конструктор
        explicit ValueArrayContext(Builder& builder);

        // Задать значение
        ValueArrayContext& value(const Node::Value& value);

        // Задать конец массива
        Builder& endArray();
    };

    //------------ DictContext ---------------

    class DictContext : public BuildContextSecond {
    public:
        // Конструктор
        explicit DictContext(Builder& builder);
    };

    //------------ ArrayContext ---------------

    class ArrayContext : public ValueArrayContext {
    public:
        // Конструктор
        explicit ArrayContext(Builder& builder);
    };

    //------------ Builder ---------------

    class Builder final : virtual public KeyContext, virtual public ValueKeyContext,
        virtual public DictContext, virtual public ArrayContext {
    public:
        // Конструктор
        Builder();

        // Задать ключ
        KeyContext& key(const std::string& key);

        // Задать значение
        Builder& value(const Node::Value& value);

        // Задать начало словаря
        DictContext& startDict();

        // Задать конец словаря
        Builder& endDict();

        // Задать начало массива
        ArrayContext& startArray();

        // Задать конец массива
        Builder& endArray();

        // Построить JSON
        Node build() const;

    private:
        // Добавить невозможно?
        bool unableAdd() const;

        // Объект не пустой?
        bool isMakeObj() const;

        // Использовать ключ невозможно?
        bool unableUseKey() const;

        // Использовать значение невозможно?
        bool unableUseValue() const;

        // Использовать начало словаря невозможно?
        bool unableUseStartDict() const;

        // Использовать конец словаря невозможно?
        bool unableUseEndDict() const;

        // Использовать начало словаря невозможно?
        bool unableUseStartArray() const;

        // Использовать конец словаря невозможно?
        bool unableUseEndArray() const;

        // Построить JSON невозможно?
        bool unableUseBuild() const;

        // Добавление узла в JSON
        Builder& addNode(const Node& node);

        // Добавление узла по значению в JSON
        void pushNode(const Node::Value& value);

    private:
        Node mRoot = nullptr;   // Корневой узел JSON
        std::stack<std::unique_ptr<Node>> Nodes; // Стек узлов JSON
    };

}  // namespace json

#endif // JSON_BUILDER_H
