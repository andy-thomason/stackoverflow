
// parallel-on-c-isnt-fast-like-c-sharp

#include <stdint.h>
#include <stdio.h>

// This is typical of the random number generators used in professional games.
// It is less "correct" than mersenne twisters, for example, but much faster.
inline uint32_t fast_rand(int32_t &seed, uint32_t limit) {
  // Prevent infinite loops.
  if (limit == 0) return 0;

  // Make a mask that has all 1s in the bottom few bits.
  // This reduces the number of iterations of the loop to ~1
  int leading_zeros = __builtin_clz(limit);
  int mask = 0xffffffff >> leading_zeros;

  // Loop until our result is in range using rotate and xor.
  do {
    seed = (seed << 1) ^ ((seed >> 31) & 0xa53a9be9);
  } while ((seed & mask) >= limit);

  return seed & mask;
}

int main() {
  // I'm using two seeds to prevent coupling.
  // On their own, their quantiles are pretty flat, but
  // in this example they couple, causing conditioning in the results.
  int32_t length_seed = (int32_t)0x95abcfad;
  int32_t swap_seed = (int32_t)0xba7235fab;

  for (int i = 0; i != 10000000; ++i) {
    // Note we don't use a std::string. These are very slow.
    char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    auto num_chars = sizeof(chars) - 1;
    auto length = fast_rand(length_seed, num_chars-1) + 1;

    // Trim the string to the right length.
    chars[length] = 0;

    // Shuffle the characters.
    for (int j = 0; j != length; ++j) {
      int swapper = j + fast_rand(swap_seed, length - j);
      auto tmp = chars[j];
      chars[j] = chars[swapper];
      chars[swapper] = tmp;
    }

    // Print with puts (not iostreams).
    puts(chars);
  }
}

