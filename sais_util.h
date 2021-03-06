#ifndef SAIS_UTIL_H_
#define SAIS_UTIL_H_

#include <iostream>
#include <string>
#include <vector>

#include "sais_lcp.h"

template <typename T>
int NumDigits(T number) {
  if (std::is_same<T, char>::value) {
    return 1;
  }
  int digits = 0;
  if (number <= 0) digits = 1;
  while (number) {
    number /= 10;
    digits++;
  }
  return digits;
}

extern bool allow_printing;

template <typename T>
void PrintVector(const std::vector<T> &v, const std::string &name = "",
                 int cell_size = 0, int depth = 0, std::ostream& out = std::cout) {
  // if (!allow_printing) {
  //   return;
  // }
  for (int i = 0; i < depth; i++) {
    out << "   ";
  }
  out << name;
  int padding = 20 - name.length();
  for (int i = 0; i < padding; i++) {
    out << ' ';
  }
  for (auto i = v.begin(); i != v.end(); ++i) {
    int number_padding = cell_size - NumDigits(*i);
    for (int i = 0; i < number_padding; i++) {
      out << ' ';
    }
    out << *i << "  ";
  }
  out << std::endl;
}

std::vector<int> TypeCount(std::vector<char> &typemap);

void FindBucketHeads(const std::vector<int> &bucket_sizes,
                     std::vector<int> *bucket_heads);
void FindBucketTails(const std::vector<int> &bucket_sizes,
                     std::vector<int> *bucket_tails);

int FindMinInRange(const std::vector<LCP_ARRAY_TYPE> &array, int start_index,
                   int end_index);

void EncodeSeam(const std::vector<int> &bucket_heads,
                const std::vector<int> &bucket_tails,
                const std::vector<int> &ls_seam, std::vector<char> *typemap);

void PrintPerBucket(const std::vector<int> &values,
                    const std::vector<char> &typemap,
                    const std::vector<int> &bucket_heads,
                    const std::vector<int> &bucket_tails, int cell_size,
                    int debug_depth);

bool CheckUnique(std::vector<int> vec);

#endif
