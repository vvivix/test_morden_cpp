#include <iostream>
#include <memory>
#include "gtest/gtest.h"

class HtmlBuilder;

class HtmlElement {
    std::string mValue;
    std::string mTag;

    std::vector<HtmlElement> mElements;

    HtmlElement(const std::string tag, const std::string value = "");

    friend class HtmlBuilder;

public:

    static std::unique_ptr<HtmlBuilder> build(std::string tag) {
        return std::make_unique<HtmlBuilder>(tag);
    }

    std::string str(size_t indent = 0) const;

    void add(HtmlElement e);
};

class HtmlBuilder {
public:
    HtmlElement mRoot;

    HtmlBuilder(std::string tag);

    HtmlBuilder &add(std::string tag, std::string content);

    operator HtmlElement() const;
};

HtmlElement::HtmlElement(const std::string tag, const std::string value) : mTag(tag), mValue(value) {}

std::string HtmlElement::str(size_t indent) const {
    std::ostringstream oss;

    oss << std::string(indent, '\t') << "<" << mTag << ">" << std::endl;

    if (mElements.empty() && !mValue.empty())
        oss << std::string(indent + 1, '\t') << mValue << std::endl;

    for (const auto& e : mElements) oss << e.str(indent + 1);

    oss << std::string(indent, '\t') << "</" << mTag << ">" << std::endl;
    return oss.str();
}

void HtmlElement::add(HtmlElement e) {
    mElements.emplace_back(e);
}


HtmlBuilder:: operator HtmlElement() const { return mRoot; }

HtmlBuilder::HtmlBuilder(std::string tag) : mRoot(tag) {}

HtmlBuilder& HtmlBuilder::add(std::string tag, std::string content) {
    mRoot.add({tag, content});
    return *this;
}


TEST(HtmlElement, TEST_CASE_1) {
    std::cout << std::endl;
    HtmlElement builder = HtmlElement::build("ul")
            ->add("li", "Hello")
            .add("li", "World!");
    std::cout << builder.str() << std::endl;
}