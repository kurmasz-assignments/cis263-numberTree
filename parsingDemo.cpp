//
// Created by Zachary Kurmas on 2019-10-03.
//

#include <iostream>
#include <sstream>

namespace cis263 {

  using namespace std;

  void parse(const string &str) {

    // A stringstream allows you to read from a string as if it were input and/or a file.
    // Thus, you can grab words using the extraction operator (i.e., ">>")
    istringstream ss(str);

    // Here we grab "lines" of input.  But, rather than using "\n" to separate lines, we use ";"
    // (Any newlines will be left in the lines; but, will be ignored when parsing the individual
    // lines.
    std::string line;
    while (std::getline(ss, line, ';')) {

      // create another stringstream for parsing the individual line.
      istringstream iss(line);

      string name;

      // read the first word, then figure out if it is an integer.
      iss >> name;

      cout << "Processing =>" << name << "<=" << endl;

      // an integer will begin with either a digit.  (Hence, the rule that node names must begin
      // with a letter)
      if (std::isdigit(name[0])) {

        // Try to parse the string as an integer.
        // pos will contain the number of characters used.
        size_t pos;
        int value = std::stoi(name, &pos);
        if (pos != name.size()) {
          cout << "\t=>" << name << "<= starts with a digit but is not an integer" << endl;
        }
        cout << "\t Value: " << value << std::endl;
      }

      string child;
      while (iss >> child) {
        cout << "\t=>" << child << "<=" << endl;
      }
    }
  }
} // end namespace

int main(int argc, char *argv[]) {

  if (argc == 1) {
    cis263::parse("a; node2; 15; node3 a node2 15; 17 a 15;");
    cis263::parse("4ourth");
  } else {
    for (int i = 1; i < argc; ++i) {
      cis263::parse(argv[i]);
    }
  }
}