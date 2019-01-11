#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <time.h>

#include "sa_to_lcp.h"
#include "sais_lcp.h"
#include "sais_util.h"

using namespace std;

bool allow_printing = true;

string LoadFromFile(const string& filename) {
  ifstream input_stream(filename);

  stringstream ss;
  ss << input_stream.rdbuf();

  return ss.str();
}

vector<int> EncodeAlphabetically(const string& text) {
  int n = text.length();
  vector<int> encoded(n);

  int min_char = *min_element(begin(text), end(text));

  for (int i = 0; i < n; i++) {
    encoded[i] = text[i] - min_char;
  }

  return encoded;
}

int main(int argc, char* argv[]) {
  bool enable_printing = true;
  bool run_original = false;
  int alphabet_size = 256;
  // Check the number of parameters
  if (argc < 2) {
    // Tell the user how to run the program
    cerr << "Usage: " << argv[0] << " DATA_PATH ENABLE_PRINTING ALPHABET_SIZE" << endl
         << "\tDATA_PATH - path to a .txt file containing a single line of text"
         << endl << "\tENABLE_PRINTING - if steps should be printed"
         << "\tALPHABET_SIZE - (optional) size of the alphabet";
    /* "Usage messages" are a conventional way of telling the user
     * how to run a program if they enter the command incorrectly.
     */
    return 1;
  } else if (argc == 3) {
    if (stoi(argv[2]) == 0) {
      enable_printing = false;
    }
  } else if (argc == 4) {
    if (stoi(argv[2]) == 0) {
      enable_printing = false;
    }
    if (stoi(argv[3]) == 1) {
      run_original = true;
    }
  } else if (argc == 5) {
    alphabet_size = stoi(argv[3]);
  }

  string filename = argv[1];
  string text = LoadFromFile(filename);
  int n = text.size() + 1;

  vector<int> encoded = EncodeAlphabetically(text);

  if (!enable_printing) {
    allow_printing = false;
    cout.setstate(ios_base::failbit);
  }

  vector<int> suffix_array(n, -1);
  vector<LCP_ARRAY_TYPE> lcp_array(n, -1);

  clock_t start, finish;
  double measuredTime;

  start = clock();
  BuildSuffixArray(encoded, alphabet_size, &suffix_array, &lcp_array);
  finish = clock();
  measuredTime = (double)(finish - start) / (double)CLOCKS_PER_SEC;
  cerr << "Our time: " << measuredTime << '\n';

  if (enable_printing) {
    allow_printing = true;
    cout.clear();
  }

  if (!run_original) {
    return 0;
  }

  int cell_size = NumDigits(text.size()) + 1;
  vector<char> text_chars(n);
  for (int i = 0; i < n; i++) {
    if (i < n - 1) {
      text_chars[i] = text[i];
    }
  }

  vector<int> correct_sa(n + 1);
  vector<int> correct_lcp(n + 1);

  // // start sluzbeno novija
  start = clock();
  SAIS_SA_LCP(text_chars, &correct_sa, &correct_lcp);
  finish = clock();
  measuredTime = (double)(finish - start) / (double)CLOCKS_PER_SEC;
  cerr << "Original time: " << measuredTime << '\n';
  // // end sluzbeno novija

  correct_sa.resize(n);
  correct_lcp.resize(n);
  PrintVector(suffix_array, "Suffix array: ", cell_size);
  PrintVector(correct_sa, "SA should be: ", cell_size);
  PrintVector(lcp_array, "LCP array: ", cell_size);
  PrintVector(correct_lcp, "LCP should be: ", cell_size);

  cout.clear();

  if (suffix_array == correct_sa) {
    cout << "SA is OK" << endl;
  } else {
    cout << "SA is WRONG!" << endl;
  }

  vector<int> lcp_arr_test(lcp_array.size());
  for (int i = 0, nn = lcp_array.size(); i < nn; i++) {
    lcp_arr_test[i] = lcp_array[i];
  }

  if (lcp_arr_test == correct_lcp) {
    cout << "LCP is OK" << endl;
  } else {
    cout << "LCP is WRONG!" << endl;
  }
}
