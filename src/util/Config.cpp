#include "Config.h"

#include <fstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wswitch-default"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#pragma GCC diagnostic pop

#include "Tools.h"

std::vector<Config::Dict*> Config::data;

// Read config data from file
void Config::readFile(const std::string& path) {
  // Clear old config
  Config::data.clear();

  // Load XML from file
  std::ifstream f(path.c_str());

  // Ensure file exists
  if (!f.good()) {
    tools::log_message("Warning: Could not find file " + path);
    return;
  }

  // Make buffer
  std::vector<char> xml_buffer((std::istreambuf_iterator<char>(f)),
                               std::istreambuf_iterator<char>());

  // Push EOF
  xml_buffer.push_back('\0');

  // Create XML doc
  rapidxml::xml_document<> doc;

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  rapidxml::xml_node<>* root_node = doc.first_node("data");

  // Iterate over the nodes
  for (rapidxml::xml_node<>* entry = root_node->first_node("entry"); entry;
       entry = entry->next_sibling()) {
    addKey(entry->first_attribute()->name(), entry->first_attribute()->value());
  }

  // Clean up
  f.close();
  doc.clear();
}

// Write config data to file
void Config::writeFile(const std::string& path) {
  // Create XML doc
  rapidxml::xml_document<> doc;

  // Push encoding and version data
  rapidxml::xml_node<>* decl = doc.allocate_node(rapidxml::node_declaration);
  decl->append_attribute(doc.allocate_attribute("version", "1.0"));
  decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
  doc.append_node(decl);

  // Create root node
  rapidxml::xml_node<>* root_node =
      doc.allocate_node(rapidxml::node_element, "data");
  doc.append_node(root_node);

  // Parse key val pairs
  const char* entry = doc.allocate_string("entry");
  for (unsigned int i = 0; i < Config::data.size(); i++) {
    rapidxml::xml_node<>* object_node =
        doc.allocate_node(rapidxml::node_element, entry);
    object_node->append_attribute(doc.allocate_attribute(
        doc.allocate_string(Config::data.at(i)->getKey().c_str()),
        doc.allocate_string(Config::data.at(i)->getValue().c_str())));
    root_node->append_node(object_node);
  }

  // Save to file
  std::ofstream f(path);

  // Ensure file exists
  if (!f.good()) {
    tools::log_message("Warning: Could not find file " + path);
    return;
  }

  // Output
  f << doc;

  // Clean up
  f.close();
  doc.clear();
}

// Get string value from key
std::string Config::getValue(const std::string& key) {
  // Find key
  Dict* element = findKey(key);

  // Return value
  if (element != nullptr) {
    return element->getValue();
  }

  // Not found
  return "";
}

// Get int value from key
int Config::getIntValue(const std::string& key) {
  return tools::stringToInt(getValue(key));
}

// Get boolean value from key
bool Config::getBooleanValue(const std::string& key) {
  return (bool)tools::stringToInt(getValue(key));
}

// Set string value
void Config::setValue(const std::string& key, std::string value) {
  // Search for key
  Dict* element = findKey(key);

  // Update value
  if (element != nullptr) {
    element->setValue(value);
    return;
  }

  // Create key value if not found
  addKey(key, value);
}

// Set int const char
void Config::setValue(const std::string& key, const char* value) {
  setValue(key, tools::toString(value));
}

// Set int value
void Config::setValue(const std::string& key, const int value) {
  setValue(key, tools::toString(value));
}

// Set boolean value
void Config::setValue(const std::string& key, const bool value) {
  setValue(key, tools::toString(value));
}

// Find values
Config::Dict* Config::findKey(const std::string& key) {
  // Search by key
  for (unsigned int i = 0; i < Config::data.size(); i++) {
    if (Config::data.at(i)->getKey() == key) {
      return Config::data.at(i);
    }
  }

  // Not found
  return nullptr;
}

// Add key
void Config::addKey(const std::string& key, std::string value) {
  Config::data.push_back(new Dict(key, value));
}
