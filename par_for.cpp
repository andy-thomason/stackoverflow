// https://stackoverflow.com/questions/55083764/why-does-threading-floating-point-computations-on-the-cpu-make-them-take-signifi

    #include <atomic>
    #include <future>
    #include <thread>
    #include <vector>
    #include <stdio.h>

    template<typename Func>
    void par_for(int start, int end, int step, int chunks_per_thread, Func func) {
      using namespace std;
      using namespace chrono;

      atomic<int> work_item{start};
      vector<future<void>> futures(std::thread::hardware_concurrency());

      for (auto &fut : futures) {
        fut = async(std::launch::async, [&work_item, end, step, chunks_per_thread, &func]() {
          for(;;) {
            int wi = work_item.fetch_add(step * chunks_per_thread);
            if (wi > end) break;
            int wi_max = std::min(end, wi+step * chunks_per_thread);
            while (wi < wi_max) {
              func(wi);
              wi += step;
            }
          }
        });
      }

      for (auto &fut : futures) {
        fut.wait();
      }
    }

    int main() {
      using namespace std;
      using namespace chrono;
      for (int k = 0; k != 2; ++k) {
        auto t0 = high_resolution_clock::now();
        constexpr int loops = 100000000;
        if (k == 0) {
          for (int i = 0; i != loops; ++i ) {
            if (i % 10000000 == 0) printf("%d\n", i);
          }
        } else {
          par_for(0, loops, 1, 100000, [](int i) {
            if (i % 10000000 == 0) printf("%d\n", i);
          });
        }
        auto t1 = high_resolution_clock::now();
        duration<double, milli> ns = t1 - t0;
        printf("k=%d %fms total\n", k, ns.count());
      }
    }

