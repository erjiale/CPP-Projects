#include <iostream>
#include <thread>
#include <fstream>
#include <string>

void createFile(int thread_num, std::string text) {
  std::ofstream outfile("file" + std::to_string(thread_num) + ".txt");
  outfile << text << "\n";
  outfile.close();
}

bool isNumber(std::string str) {
  for(char c : str) {
    if(!isdigit(c)) return false;
  }
  return true;
}

bool checkInputParameters(int argc, char *argv[]) {
  if(argc!=3) {
    std::cout << "Wrong format\nUsage: a.exe number_of_files(integer) text(string)\n";
    return false;
  }

  std::string arg1 = argv[1];
  if(!isNumber(arg1)) {
    std::cout << "Wrong format\nUsage: a.exe number_of_files(integer) text(string)\nSecond Argument needs to be an int\n";
    return false;
  }
  int number_of_files_ = std::stoi(arg1);

  return true;
}

void runNThreads(int n_files, std::string text) {
  std::thread t[n_files];
  for(int i=0; i<n_files; ++i) {
    t[i] = std::thread(createFile, i, text);
  }

  for(int i=0; i<n_files; ++i) {
    t[i].join();
    std::cout << "[Working on file " << i << "]\n";
  }
}

int main(int argc, char *argv[]) {
  if(!checkInputParameters(argc, argv)) return false;
  
  // Input Params
  std::string n_files_ = argv[1];
  int number_of_files_ = std::stoi(n_files_);
  std::string text_ = argv[2];

  auto t1 = std::chrono::high_resolution_clock::now();
  // runNThreads(number_of_files_, text_);
  for(int i=0; i<number_of_files_; i++) {
    createFile(i, text_);
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  std::cout << "SEQUENTIALLY took "
                << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                << " milliseconds\n";

  auto t3 = std::chrono::high_resolution_clock::now();
  runNThreads(number_of_files_, text_);
  auto t4 = std::chrono::high_resolution_clock::now();
  std::cout << "CONCURRENTLY took "
                << std::chrono::duration_cast<std::chrono::milliseconds>(t4-t3).count()
                << " milliseconds\n";
  return 0;
}