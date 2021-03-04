#include <iostream>
#include <thread>

void hello(int num) {
    std::cout<<"Hello World "<<num<<"\n";
}

int main() {
    auto time1 = std::chrono::high_resolution_clock::now();
    std::thread t1(hello, 1);
    std::thread t2(hello, 2);
    std::thread t3(hello, 3);
    std::thread t4(hello, 4);

    std::cout << "Main thread\n";

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    auto time2 = std::chrono::high_resolution_clock::now();
    std::cout << "CONCURRENTLY took "
                << std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1).count()
                << " milliseconds\n";
    auto time3 = std::chrono::high_resolution_clock::now();
    for(int i=1; i<=4; i++) {
        hello(i);
    }
    auto time4 = std::chrono::high_resolution_clock::now();
    std::cout << "SEQUENTIALLY took "
                << std::chrono::duration_cast<std::chrono::milliseconds>(time4-time3).count()
                << " milliseconds\n";

}