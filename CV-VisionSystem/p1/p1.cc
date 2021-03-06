#include <cstdio>
#include <iostream>

#include "../VisionStartCode/image.h"

using namespace ComputerVisionProjects;

int main(int argc, char *argv[]) {
  if (argc!=4) {
    printf("Usage: %s gray-level-image gray-level-threshold output-binary-image\n", argv[0]);
    return 0;
  }

  const std::string input_file(argv[1]);
  const std::string output_file(argv[3]);

  Image the_image;
  if (!ReadImage(input_file, &the_image)) {
    std::cout <<"Can't open file " << input_file << std::endl;
    return 0;
  }

  std::string threshold_str(argv[2]);
  const int threshold = std::stoi(threshold_str);

  for(size_t r = 0; r < the_image.num_rows(); r++) {
    for(size_t c = 0; c < the_image.num_columns(); c++) {
      the_image.GetPixel(r, c) <= threshold ? 
                                        the_image.SetPixel(r, c, 0) :
                                        the_image.SetPixel(r, c, 255);
    }
  }


  if (!WriteImage(output_file, the_image)){
    std::cout << "Can't write to file " << output_file << std::endl;
    return 0;
  }




}
