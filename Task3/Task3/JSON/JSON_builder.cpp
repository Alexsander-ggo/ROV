#include "JSON/JSON_builder.h"

#include <stdexcept>
#include <utility>
#include <variant>

namespace json {

    using namespace std;
    using namespace std::literals;

    //------------ BuildConstructor ---------------

    BuildConstructor::BuildConstructor(Builder& builder)
        : mBuilder(builder) {}

    //------------ BuildContextFirst ---------------

    BuildContextFirst::BuildContextFirst(Builder& builder)
        : BuildConstructor(builder) {}

    DictContext& BuildContextFirst::startDict()
    {
        return mBuilder.startDict();
    }

    ArrayContext& BuildContextFirst::startArray()
    {
        return mBuilder.startArray();
    }

    //------------ BuildContextSecond ---------------

    BuildContextSecond::BuildContextSecond(Builder& builder)
        : BuildConstructor(builder) {}

    KeyContext& BuildContextSecond::key(const std::string& key)
    {
        return mBuilder.key(key);
    }

    Builder& BuildContextSecond::endDict()
    {
        return mBuilder.endDict();
    }

    //------------ KeyContext ---------------

    KeyContext::KeyContext(Builder& builder)
        : BuildContextFirst(builder) {}

    ValueKeyContext& KeyContext::value(const Node::Value& value)
    {
        return mBuilder.value(value);
    }

    //------------ ValueKeyContext ---------------

    ValueKeyContext::ValueKeyContext(Builder& builder)
        : BuildContextSecond(builder) {}

    //------------ ValueArrayContext ---------------

    ValueArrayContext::ValueArrayContext(Builder& builder)
        : BuildContextFirst(builder) {}

    ValueArrayContext& ValueArrayContext::value(const Node::Value& value)
    {
        return mBuilder.value(value);
    }

    Builder& ValueArrayContext::endArray()
    {
        return mBuilder.endArray();
    }

    //------------ DictContext ---------------

    DictContext::DictContext(Builder& builder)
        : BuildContextSecond(builder) {}

    //------------ ArrayContext ---------------

    ArrayContext::ArrayContext(Builder& builder)
        : ValueArrayContext(builder) {}

    //------------ Builder ---------------

    Builder::Builder()
        : KeyContext(*this)
        , ValueKeyContext(*this)
        , DictContext(*this)
        , ArrayContext(*this) {}

    KeyContext& Builder::key(const string& key)
    {
        if (unableUseKey()) {
            throw logic_error("Key сan't be applied"s);
        }
        Nodes.push(make_unique<Node>(key));
        return *this;
    }

    Builder& Builder::value(const Node::Value& value)
    {
        if (unableUseValue()) {
            throw std::logic_error("Value сan't be applied"s);
        }
        pushNode(value);
        return addNode(*Nodes.top().release());
    }

    DictContext& Builder::startDict()
    {
        if (unableUseStartDict()) {
            throw logic_error("StartDict сan't be applied"s);
        }
        Nodes.push(make_unique<Node>(Dict()));
        return *this;
    }

    Builder& Builder::endDict()
    {
        if (unableUseEndDict()) {
            throw logic_error("EndDict сan't be applied"s);
        }
        return addNode(*Nodes.top().release());
    }

    ArrayContext& Builder::startArray()
    {
        if (unableUseStartArray()) {
            throw logic_error("StartArray сan't be applied"s);
        }
        Nodes.push(make_unique<Node>(Array()));
        return *this;
    }

    Builder& Builder::endArray()
    {
        if (unableUseEndArray()) {
            throw logic_error("EndArray сan't be applied"s);
        }
        return addNode(*Nodes.top().release());
    }

    Node Builder::build() const
    {
        if (unableUseBuild()) {
            throw logic_error("Builder сan't be applied"s);
        }
        return mRoot;
    }

    bool Builder::unableAdd() const
    {
        return !(Nodes.empty()
            || Nodes.top()->isArray()
            || Nodes.top()->isString());
    }

    bool Builder::isMakeObj() const
    {
        return !mRoot.isNull();
    }

    bool Builder::unableUseKey() const
    {
        return isMakeObj()
            || Nodes.empty()
            || !Nodes.top()->isDict();
    }

    bool Builder::unableUseValue() const
    {
        return isMakeObj()
            || unableAdd();
    }

    bool Builder::unableUseStartDict() const
    {
        return unableUseValue();
    }

    bool Builder::unableUseEndDict() const
    {
        return isMakeObj()
            || Nodes.empty()
            || !Nodes.top()->isDict();
    }

    bool Builder::unableUseStartArray() const
    {
        return unableUseValue();
    }

    bool Builder::unableUseEndArray() const
    {
        return isMakeObj()
            || Nodes.empty()
            || !Nodes.top()->isArray();
    }

    bool Builder::unableUseBuild() const
    {
        return !isMakeObj();
    }

    Builder& Builder::addNode(const Node& node)
    {
        Nodes.pop();
        if (Nodes.empty()) {
            mRoot = node;
        }
        else if (Nodes.top()->isArray()) {
            Nodes.top()->asArray().push_back(node);
        }
        else {
            const Node& key = *Nodes.top().release();
            Nodes.pop();
            Nodes.top()->asDict().emplace(key.asString(), node);
        }
        return *this;
    }

    void Builder::pushNode(const Node::Value& value)
    {
        visit([this](auto&& val) {
            Nodes.push(make_unique<Node>(val));
        }, value);
    }

}
