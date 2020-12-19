/**
 * Config
 * Allan Legemaate
 * Global configuration. Responsible for
 *   reading config data and updating on close.
 * 22/11/2018
 **/

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

// Static config class
class Config {
 public:
  // Read keyvals from file
  static void readFile(const std::string& path);

  // Write keyvals to file
  static void writeFile(const std::string& path);

  // Get string value from key
  static std::string getValue(const std::string& key);

  // Get int value from key
  static int getIntValue(const std::string& key);

  // Get boolean value from key
  static bool getBooleanValue(const std::string& key);

  // Set string value
  static void setValue(const std::string& key, std::string value);

  // String literal (so int is not called)
  static void setValue(const std::string& key, const char* value);

  // Set int value
  static void setValue(const std::string& key, const int value);

  // Set boolean value
  static void setValue(const std::string& key, const bool value);

 private:
  // Key value pairs
  class Dict {
   public:
    Dict(const std::string& key, std::string value) {
      this->key = key;
      this->value = value;
    }

    std::string getKey() { return key; }
    std::string getValue() { return value; }
    void setValue(const std::string& value) { this->value = value; }

   private:
    // Key
    std::string key;

    // Value
    std::string value;
  };

  // Find key val pair
  static Dict* findKey(const std::string& key);

  // Add ket val pair
  static void addKey(const std::string& key, std::string value);

  // Values
  static std::vector<Dict*> data;
};

#endif  // CONFIG_H
