#include "JSON/JSON.h"

#include <iterator>
#include <utility>

namespace json {

    using namespace std;
    using namespace std::literals;

    namespace {

        Node LoadNode(std::istream& input);
        Node LoadString(std::istream& input);

        string LoadLiteral(istream& input)
        {
            string s;
            while (isalpha(input.peek())) {
                s.push_back(static_cast<char>(input.get()));
            }
            return s;
        }

        Node LoadArray(istream& input)
        {
            vector<Node> result;

            for (char c; input >> c && c != ']';) {
                if (c != ',') {
                    input.putback(c);
                }
                result.push_back(LoadNode(input));
            }
            if (!input) {
                throw ParsingError("Array parsing error"s);
            }
            return Node(move(result));
        }

        Node LoadDict(istream& input)
        {
            Dict dict;

            for (char c; input >> c && c != '}';) {
                if (c == '"') {
                    std::string key = LoadString(input).asString();
                    if (input >> c && c == ':') {
                        if (dict.find(key) != dict.end()) {
                            throw ParsingError("Duplicate key '"s + key + "' have been found");
                        }
                        dict.emplace(move(key), LoadNode(input));
                    }
                    else {
                        throw ParsingError(": is expected but '"s + c + "' has been found"s);
                    }
                }
                else if (c != ',') {
                    throw ParsingError(R"(',' is expected but ')"s + c + "' has been found"s);
                }
            }
            if (!input) {
                throw ParsingError("Dictionary parsing error"s);
            }
            return Node(move(dict));
        }

        Node LoadString(istream& input)
        {
            auto it = istreambuf_iterator<char>(input);
            auto end = istreambuf_iterator<char>();
            string s;
            while (true) {
                if (it == end) {
                    throw ParsingError("String parsing error");
                }
                const char ch = *it;
                if (ch == '"') {
                    ++it;
                    break;
                }
                else if (ch == '\\') {
                    ++it;
                    if (it == end) {
                        throw ParsingError("String parsing error");
                    }
                    const char escaped_char = *(it);
                    switch (escaped_char) {
                    case 'n':
                        s.push_back('\n');
                        break;
                    case 't':
                        s.push_back('\t');
                        break;
                    case 'r':
                        s.push_back('\r');
                        break;
                    case '"':
                        s.push_back('"');
                        break;
                    case '\\':
                        s.push_back('\\');
                        break;
                    default:
                        throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
                    }
                }
                else if (ch == '\n' || ch == '\r') {
                    throw ParsingError("Unexpected end of line"s);
                }
                else {
                    s.push_back(ch);
                }
                ++it;
            }
            return Node(move(s));
        }

        Node LoadBool(istream& input)
        {
            const auto s = LoadLiteral(input);
            if (s == "true"sv) {
                return Node{ true };
            }
            else if (s == "false"sv) {
                return Node{ false };
            }
            else {
                throw ParsingError("Failed to parse '"s + s + "' as bool"s);
            }
        }

        Node LoadNull(istream& input)
        {
            if (auto literal = LoadLiteral(input); literal == "null"sv) {
                return Node{ nullptr };
            }
            else {
                throw ParsingError("Failed to parse '"s + literal + "' as null"s);
            }
        }

        Node LoadNumber(istream& input)
        {
            string parsed_num;

            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("Failed to read number from stream"s);
                }
            };

            auto read_digits = [&input, read_char] {
                if (!isdigit(input.peek())) {
                    throw ParsingError("A digit is expected"s);
                }
                while (isdigit(input.peek())) {
                    read_char();
                }
            };

            if (input.peek() == '-') {
                read_char();
            }

            if (input.peek() == '0') {
                read_char();
            }
            else {
                read_digits();
            }

            bool is_int = true;
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {
                    try {
                        return static_cast<integer>(stoi(parsed_num));
                    }
                    catch (...) {}
                }
                return stod(parsed_num);
            }
            catch (...) {
                throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
            }
        }

        Node LoadNode(istream& input)
        {
            char c;
            if (!(input >> c)) {
                //throw ParsingError("Unexpected EOF"s);
                return Node(nullptr);
            }
            switch (c) {
            case '[':
                return LoadArray(input);
            case '{':
                return LoadDict(input);
            case '"':
                return LoadString(input);
            case 't':
                [[fallthrough]];
            case 'f':
                input.putback(c);
                return LoadBool(input);
            case 'n':
                input.putback(c);
                return LoadNull(input);
            default:
                input.putback(c);
                return LoadNumber(input);
            }
        }

        struct PrintContext
        {
            std::ostream& out;
            int indent_step = 4;
            int indent = 0;

            void PrintIndent() const {
                for (int i = 0; i < indent; ++i) {
                    out.put(' ');
                }
            }

            PrintContext Indented() const {
                return { out, indent_step, indent_step + indent };
            }
        };

        void PrintNode(const Node& value, const PrintContext& ctx);

        template <typename Value>
        void PrintValue(const Value& value, const PrintContext& ctx)
        {
            ctx.out << value;
        }

        void PrintString(const string& value, ostream& out)
        {
            out.put('"');
            for (const char c : value) {
                switch (c) {
                case '\r':
                    out << "\\r"sv;
                    break;
                case '\n':
                    out << "\\n"sv;
                    break;
                case '"':
                    [[fallthrough]];
                case '\\':
                    out.put('\\');
                    [[fallthrough]];
                default:
                    out.put(c);
                    break;
                }
            }
            out.put('"');
        }

        template <>
        void PrintValue<string>(const string& value, const PrintContext& ctx)
        {
            PrintString(value, ctx.out);
        }

        template <>
        void PrintValue<nullptr_t>(const nullptr_t&, const PrintContext& ctx)
        {
            ctx.out << "null"sv;
        }

        template <>
        void PrintValue<bool>(const bool& value, const PrintContext& ctx)
        {
            ctx.out << (value ? "true"sv : "false"sv);
        }

        template <>
        void PrintValue<Array>(const Array& nodes, const PrintContext& ctx)
        {
            ostream& out = ctx.out;
            out << "[\n"sv;
            bool first = true;
            auto inner_ctx = ctx.Indented();
            for (const Node& node : nodes) {
                if (first) {
                    first = false;
                }
                else {
                    out << ",\n"sv;
                }
                inner_ctx.PrintIndent();
                PrintNode(node, inner_ctx);
            }
            out.put('\n');
            ctx.PrintIndent();
            out.put(']');
        }

        template <>
        void PrintValue<Dict>(const Dict& nodes, const PrintContext& ctx)
        {
            ostream& out = ctx.out;
            out << "{\n"sv;
            bool first = true;
            auto inner_ctx = ctx.Indented();
            for (const auto& [key, node] : nodes) {
                if (first) {
                    first = false;
                } else {
                    out << ",\n"sv;
                }
                inner_ctx.PrintIndent();
                PrintString(key, ctx.out);
                out << ": "sv;
                PrintNode(node, inner_ctx);
            }
            out.put('\n');
            ctx.PrintIndent();
            out.put('}');
        }

        void PrintNode(const Node& node, const PrintContext& ctx)
        {
            visit(
                [&ctx](const auto& value) {
                    PrintValue(value, ctx);
                },
                node.getValue());
        }

    }  // namespace

        //------------ Node ---------------

    bool Node::isInt() const
    {
        return holds_alternative<integer>(*this);
    }

    integer Node::asInt() const
    {
        if (!isInt()) {
            throw logic_error("Not an int"s);
        }
        return get<integer>(*this);
    }

    bool Node::isPureDouble() const
    {
        return holds_alternative<double>(*this);
    }

    bool Node::isDouble() const
    {
        return isInt() || isPureDouble();
    }

    double Node::asDouble() const
    {
        if (!isDouble()) {
            throw logic_error("Not a double"s);
        }
        return isPureDouble() ? get<double>(*this) : asInt();
    }

    bool Node::isBool() const
    {
        return holds_alternative<bool>(*this);
    }

    bool Node::asBool() const
    {
        if (!isBool()) {
            throw logic_error("Not a bool"s);
        }
        return get<bool>(*this);
    }

    bool Node::isNull() const {
        return holds_alternative<nullptr_t>(*this);
    }

    bool Node::isArray() const
    {
        return holds_alternative<Array>(*this);
    }

    const Array& Node::asArray() const
    {
        if (!isArray()) {
            throw logic_error("Not an array"s);
        }
        return get<Array>(*this);
    }

    Array&Node::asArray()
    {
        if (!isArray()) {
            throw logic_error("Not an array"s);
        }
        return get<Array>(*this);
    }

    bool Node::isString() const
    {
        return holds_alternative<string>(*this);
    }

    const string& Node::asString() const
    {
        if (!isString()) {
            throw logic_error("Not a string"s);
        }
        return get<string>(*this);
    }

    bool Node::isDict() const
    {
        return holds_alternative<Dict>(*this);
    }

    const Dict& Node::asDict() const
    {
        if (!isDict()) {
            throw logic_error("Not a dict"s);
        }
        return get<Dict>(*this);
    }

    Dict& Node::asDict() {
        if (!isDict()) {
            throw logic_error("Not a dict"s);
        }
        return get<Dict>(*this);
    }

    bool Node::operator==(const Node& rhs) const
    {
        return getValue() == rhs.getValue();
    }

    const Node::Value& Node::getValue() const
    {
        return *this;
    }

    bool operator!=(const Node& lhs, const Node& rhs)
    {
        return !(lhs == rhs);
    }

    //------------ Document ---------------

    Document::Document(Node root)
        : mRoot(move(root)) {}

    const Node& Document::getRoot() const
    {
        return mRoot;
    }

    bool operator==(const Document& lhs, const Document& rhs)
    {
        return lhs.getRoot() == rhs.getRoot();
    }

    bool operator!=(const Document& lhs, const Document& rhs)
    {
        return !(lhs == rhs);
    }

    Document Load(istream& input)
    {
        return Document{ LoadNode(input) };
    }

    void Print(const Document& doc, ostream& output)
    {
        PrintNode(doc.getRoot(), PrintContext{ output });
    }

}  // namespace json
