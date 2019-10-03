//
// Created by Zachary Kurmas on 2019-10-01.
//

#ifndef CIS263_NUMBERTREE_H
#define CIS263_NUMBERTREE_H

#include <string>
#include <map>
// Include more headers as needed 

namespace cis263 {

  using namespace std;

  class NumberTree {

  public:

    // Please implement these methods in a separate .cpp file.

    int size() const;

    int height() const;

    void load(const string &str);

    bool verify(const string& str) const;

    map<string, int> solve() const;

   // Add instance data and private methods here 
  };
}


#endif //CIS263_NUMBERTREE_H
