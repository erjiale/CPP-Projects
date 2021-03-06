#include <cstdio>
#include <iostream>
#include "../VisionStartCode/image.h"

using namespace ComputerVisionProjects;

int main(int argc, char *argv[]) {
  if (argc!=3) {
    printf("Usage: %s input-binary-image output-labeled-image\n", argv[0]);
    return 0;
  }

  const std::string input_file(argv[1]);
  const std::string output_file(argv[2]);

  Image the_image;
  if (!ReadImage(input_file, &the_image)) {
    std::cout <<"Can't open file " << input_file << std::endl;
    return 0;
  }

  Image output_image;
  output_image.AllocateSpaceAndSetSize(the_image.num_rows(), the_image.num_columns());

  // sequentialLabelingAlgorithm(the_image, output_image);
  int label = 1;
  for(size_t r = 0; r < the_image.num_rows(); r++) {
    for(size_t c = 0; c < the_image.num_columns(); c++) {
      if(r==0 && c==0) {
        if(the_image.GetPixel(r,c)==0) output_image.SetPixel(r,c,0);
        else output_image.SetPixel(r,c,label);
        label++;
        continue;
      }
      if(r==0) {
        output_image.SetPixel(r,c,output_image.GetPixel(r, c-1));
        continue;
      }
      else if(c==0) {
        output_image.SetPixel(r,c,output_image.GetPixel(r-1,c));
        continue;
      }
      else {
        int top = the_image.GetPixel(r, c-1), 
            left = the_image.GetPixel(r-1, c), 
            top_left = the_image.GetPixel(r-1, c-1);
            output_top = output_image.GetPixel(r, c-1), 
            output_left = output_image.GetPixel(r-1, c), 
            output_top_left = output_image.GetPixel(r-1, c-1);

        if(top==0 && left==0 && top_left==0) output_image.SetPixel(r, c, 0); // Background
        else if(top==0 && left==0 && top_left!=0) output_image.SetPixel(r, c, output_top_left); // Top_left
        else if(top==0 && left!=0 && top_left==0) output_image.SetPixel(r, c, output_left); // Left
        else if(top!=0 && left==0 && top_left==0) output_image.SetPixel(r, c, output_top); // Top
        // if((top==left || top!=left) && top_left==0) the_image.SetPixel(r, c, left); // 
        else output_image.SetPixel(r, c, label);
      }
    }
  }

  if (!WriteImage(output_file, output_image)) {
    std::cout << "Can't write to file " << output_file << std::endl;
    return 0;
  }
}
