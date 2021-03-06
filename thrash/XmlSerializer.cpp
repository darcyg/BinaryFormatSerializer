// XmlSerializer.cpp : Defines the entry point for the console application.
//

#include "xml_formatters/lexical_stringizer.h"
#include "xml_formatters/text_formatter.h"
#include "xml_formatters/attribute_formatter.h"
#include "xml_formatters/element_formatter.h"
#include "xml_formatters/element_counter.h"
#include "xml_formatters/attribute_counter.h"
#include "xml_formatters/assign_name.h"
#include "xml_formatters/assign_text_content.h"
#include "xml_formatters/content_check.h"
#include "xml_formatters/document_formatter.h"
#include "xml_formatters/declaration_formatter.h"

#include "formatters/vector_formatter.h"
#include "formatters/optional_formatter.h"
#include "formatters/map_formatter.h"
#include "formatters/map_kv_formatter.h"
#include "formatters/tuple_formatter.h"

#include "xml_serializers/RapidXmlTree.h"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>

using namespace xml_format;

int main(int argc, char* argv[])
{
    RapidXmlDocument document("utf-8");
    RapidXmlDocument allDocument("utf-8");

    auto vec = std::vector<int> { 1, 2, 3, 4, 5 };
    auto nameToAge = std::map<std::string, int> { {"Ela", 5}, {"Ala", 6}, {"Ola", 7} };
    auto nameToAgeVec = std::vector<std::pair<std::string, int>> { {"Ela", 5}, {"Ala", 6}, {"Ola", 7} };
    boost::optional<std::string> optNone;
    boost::optional<std::string> optStr("A value!");

    auto numberFormatter = create_attribute_formatter("number");
    auto sizeFormatter = attribute_counter("number");
    auto vectorFormatter = binary_format::create_vector_formatter(sizeFormatter, numberFormatter);
    auto vectorElementFormatter = create_element_formatter("numbers", vectorFormatter);

    auto pairFormatter = binary_format::create_tuple_formatter(assign_name<>(), assign_text_content<>());
    auto valueFormatter = create_attribute_formatter(boost::none, pairFormatter);
    auto vectorFormatter2 = binary_format::create_vector_formatter(attribute_counter(), valueFormatter);
    auto vectorElementFormatter2 = create_element_formatter("nameToAgeVec", vectorFormatter2);

    auto kvFormatter = create_attribute_formatter(boost::none, binary_format::create_tuple_formatter(assign_name<>(), assign_text_content<>()));
    auto mapFormatter = binary_format::create_map_kv_formatter(attribute_counter(), kvFormatter);
    auto mapElementFormatter = create_element_formatter("nameToAge", mapFormatter);

    auto k2Formatter = create_attribute_formatter("value");
    auto kvFormatter2 = create_element_formatter("key", binary_format::create_tuple_formatter(k2Formatter, assign_text_content<>()));
    auto mapFormatter2 = binary_format::create_map_kv_formatter(element_counter(), kvFormatter2);
    auto mapElementFormatter2 = create_element_formatter("nameToAge", mapFormatter2);

    auto k3Formatter = create_element_formatter("key");
    auto v3Formatter = create_element_formatter("value");
    auto mapFormatter3 = binary_format::create_map_formatter(element_counter("key"), k3Formatter, v3Formatter);
    auto mapElementFormatter3 = create_element_formatter("nameToAge", mapFormatter3);

    auto optionalFormatter = create_element_formatter("optional", binary_format::create_optional_formatter(content_exists(), assign_text_content<>()));

    //auto doc_format = create_element_formatter< text_formatter<lexical_stringizer> >("myxml");
    //doc_format.save(document.getDocumentElement(), 6);

    declaration_formatter<>().save(document, "utf-8");
    vectorElementFormatter.save(document.getDocumentElement(), vec);
    vectorElementFormatter2.save(document.getDocumentElement(), nameToAgeVec);
    mapElementFormatter.save(document.getDocumentElement(), nameToAge);
    mapElementFormatter2.save(document.getDocumentElement(), nameToAge);
    mapElementFormatter3.save(document.getDocumentElement(), nameToAge);
    optionalFormatter.save(document.getDocumentElement(), optNone);
    optionalFormatter.save(document.getDocumentElement(), optStr);

    auto allFormatter = create_element_formatter("all", binary_format::create_tuple_formatter(vectorElementFormatter, vectorElementFormatter2, mapElementFormatter));

    allFormatter.save(document.getDocumentElement(), std::tie(vec, nameToAgeVec, nameToAge));

    auto docFormatter = create_document_formatter("doc", allFormatter);
    docFormatter.save(allDocument, std::tie(vec, nameToAgeVec, nameToAge));

    std::stringstream str;
    str << document;
    std::string documentText = str.str();
    std::cout << documentText;

    std::stringstream allStr;
    allStr << allDocument;
    std::string allDocumentText = allStr.str();
    std::cout << allDocumentText;

    RapidXmlDocument document2(documentText, "utf-8");
    std::cout << document2;
    RapidXmlDocument allDocument2(allDocumentText, "utf-8");
    std::cout << allDocument2;

    std::string encoding;
    std::vector<int> loadedVec;
    std::vector<std::pair<std::string, int>> loadedNameToAgeVec;
    std::map<std::string, int> loadedNameToAge;
    std::map<std::string, int> loadedNameToAge2;
    std::map<std::string, int> loadedNameToAge3;
    boost::optional<std::string> optNone2;
    boost::optional<std::string> optStr2;

    declaration_formatter<>().load(document2, encoding);
    vectorElementFormatter.load(document2.getDocumentElement(), loadedVec);
    vectorElementFormatter2.load(document2.getDocumentElement(), loadedNameToAgeVec);
    mapElementFormatter.load(document2.getDocumentElement(), loadedNameToAge);
    mapElementFormatter2.load(document2.getDocumentElement(), loadedNameToAge2);
    mapElementFormatter3.load(document2.getDocumentElement(), loadedNameToAge3);
    optionalFormatter.load(document2.getDocumentElement(), optNone2);
    optionalFormatter.load(document2.getDocumentElement(), optStr2);

    std::vector<int> loadedVec2;
    std::vector<std::pair<std::string, int>> loadedNameToAgeVec2;
    std::map<std::string, int> loadedNameToAge4;

    allFormatter.load(document2.getDocumentElement(), std::tie(loadedVec2, loadedNameToAgeVec2, loadedNameToAge4));

    std::vector<int> loadedVec5;
    std::vector<std::pair<std::string, int>> loadedNameToAgeVec5;
    std::map<std::string, int> loadedNameToAge5;

    docFormatter.load(allDocument2, std::tie(loadedVec5, loadedNameToAgeVec5, loadedNameToAge5));

    std::cout << "Eaten Xml: '"<< document2 << "'";

    return 0;
}
