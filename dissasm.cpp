#include "DISSAM.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cctype>

static const char *optString = "I:O:";

int main(int argc, char **argv) {

  int opt = 0;
  if (argc != 3) {
    assert("wrong number of parameters");
  }
  const char *InputFile = "";
  const char *OutputFile = "";

  while ((opt = getopt (argc, argv, optString)) != -1) {
    switch (opt) {
      case 'I':
        InputFile = optarg;
        break;
      case 'O':
        OutputFile = optarg;
        break;
      case '?':
        assert("smth uncommon");
      default:
        abort();
    }
  }
  DissasmFile(InputFile, OutputFile);
  return 0;
}
