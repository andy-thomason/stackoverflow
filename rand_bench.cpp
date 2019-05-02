#include <random>
#include <iostream>
#include <chrono>
#include <stdlib.h>

struct rot_xor {
  int32_t seed = 0x95abcfad;
  inline uint32_t operator() () {
    return seed = (seed << 1) ^ ((seed >> 31) & 0xa53a9be9);
  }
};

struct crand {
  int32_t seed = 0x95abcfad;
  inline uint32_t operator() () {
    return rand();
  }
};

template <class Generator>
void benchmark(std::vector<int> &histo) {
  Generator r;
  int mask = histo.size() - 1;
  for (int i = 0; i != 10000000; ++i) {
    uint32_t val = (uint32_t)r();
    histo[(val>>23) & mask]++;
  }
}

int main() {
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::microseconds;

  for (int i = 0; i != 6; ++i) {
    std::vector<int> histo(0x100);
    auto t0 = high_resolution_clock::now();
    switch (i) {
      case 0: benchmark<std::minstd_rand0>(histo); break;
      case 1: benchmark<std::minstd_rand>(histo); break;
      case 2: benchmark<std::mt19937>(histo); break;
      case 3: benchmark<std::mt19937_64>(histo); break;
      case 4: benchmark<crand>(histo); break;
      case 5: benchmark<rot_xor>(histo); break;
    }
    auto t1 = high_resolution_clock::now();
    
    int min_histo = histo[0];
    int max_histo = histo[0];
    for (auto h : histo) {
      min_histo = std::min(min_histo, h);
      max_histo = std::max(max_histo, h);
    }
    std::cout << "test " << i << " took " << duration_cast<microseconds>(t1-t0).count() << "us\n";
    std::cout << " smoke test = " << min_histo << " .. " << max_histo << "\n";
  }
}

