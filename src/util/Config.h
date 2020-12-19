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
  static void readFile(std::string path);

  // Write keyvals to file
  static void writeFile(std::string path);

  // Get string value from key
  static std::string getValue(std::string key);

  // Get int value from key
  static int getIntValue(std::string key);

  // Get boolean value from key
  static bool getBooleanValue(std::string key);

  // Set string value
  static void setValue(std::string key, std::string value);

  // String literal (so int is not called)
  static void setValue(std::string key, const char* value);

  // Set int value
  static void setValue(std::string key, const int value);

  // Set boolean value
  static void setValue(std::string key, const bool value);

 private:
  // Key value pairs
  class Dict {
   public:
    Dict(std::string key, std::string value) {
      this->key = key;
      this->value = value;
    }

    std::string getKey() { return key; }
    std::string getValue() { return value; }
    void setValue(std::string value) { this->value = value; }

   private:
    // Key
    std::string key;

    // Value
    std::string value;
  };

  // Find key val pair
  static Dict* findKey(std::string key);

  // Add ket val pair
  static void addKey(std::string key, std::string value);

  // Values
  static std::vector<Dict*> data;
};

#endif  // CONFIG_H
