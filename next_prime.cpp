

    #include <stdint.h>
    #include <stdio.h>

    static const int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 

    uint64_t next_prime(uint64_t val) {
      ++val;
      if (val % 2 == 0) val += 1;
      if (val % 3 == 0) val += 1*2;
      if (val % 5 == 0) val += 1*2*3;
      if (val % 7 == 0) val += 1*2*3*5;
      if (val % 11 == 0) val += 1*2*3*5*7;
      return val;
    }

    int main() {
      int j = 1000000;
      for(uint64_t i = j; i != j+100; ++i) {
        printf("%ld %ld\n", i, next_prime(i));
      }
    }

