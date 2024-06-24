//
// Created by kiran on 5/20/24.
//

#ifndef KL_OPTIONS_H
#define KL_OPTIONS_H

#include <string>
#include <vector>

using namespace std;

class CommandLineArguments {
public:
  string cwd;
  string file;
  string out;
  
  CommandLineArguments(int argc, const char **argv);
};

#endif //KL_OPTIONS_H
