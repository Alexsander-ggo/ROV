#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {

    class Node;
    using integer = uint64_t;
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node final : private std::variant<std::nullptr_t, Array, Dict, bool, integer, double, std::string> {
    public:
        using variant::variant;
        using Value = variant;

        // Является ли узел целым числом?
        bool isInt() const;

        // Вернуть целое значение узла
        integer asInt() const;

        // Является ли узел числом с плавающей точкой?
        bool isPureDouble() const;

        // Является ли узел числом?
        bool isDouble() const;

        // Вернуть значение числа узла
        double asDouble() const;

        // Является ли узел булевой переменной?
        bool isBool() const;

        // Вернуть булевое значение узла
        bool asBool() const;

        // Является ли узел nullptr?
        bool isNull() const;

        // Является ли узел массивом?
        bool isArray() const;

        // Воернуть константную ссылку на массив узла
        const Array& asArray() const;

        // Вернуть ссылку на массив узла
        Array& asArray();

        // Является ли узел строкой?
        bool isString() const;

        // Вернуть константную ссылку на строку узла
        const std::string& asString() const;

        // Является ли узел словарём?
        bool isDict() const;

        // Вернуть константную ссылку на словарь узла
        const Dict& asDict() const;

        // Вернуть сслыку на словарь узла
        Dict& asDict();

        // Перегрузка оператора равенстра узлов
        bool operator==(const Node& rhs) const;

        // Вернуть значение узла
        const Value& getValue() const;
    };

    // Перегрузка оператора неравенства узлов
    inline bool operator!=(const Node& lhs, const Node& rhs);

    class Document {
    public:
        // Конструктор
        explicit Document(Node root);

        // Вернуть констрантную ссылку на узел корня
        const Node& getRoot() const;

    private:
        Node mRoot; // Корневой узел
    };

    // Перегрузка оператора равенства документов JSON
    inline bool operator==(const Document& lhs, const Document& rhs);

    // Перегрузка операторов неравенства документов JSON
    inline bool operator!=(const Document& lhs, const Document& rhs);

    // Загрузка документа JSON из потока
    Document Load(std::istream& input);

    // Вывод документа JSON в поток
    void Print(const Document& doc, std::ostream& output);

}  // namespace json

#endif // JSON_H
