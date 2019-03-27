#include "NeumannProc.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cctype>

static const char *optString = "I:";


int main(int argc, char **argv) {

  int opt = 0;
  if (argc != 2) {
    assert("wrong number of parameters");
  }
  const char *InputFile = "";

  while ((opt = getopt (argc, argv, optString)) != -1) {
    switch (opt) {
      case 'I':
        InputFile = optarg;
        break;
      case '?':
        assert("smth uncommon");
      default:
        abort();
    }
  }
  Processor a;
  a.ExecuteNewProgramm(InputFile);

  return 0;
}


