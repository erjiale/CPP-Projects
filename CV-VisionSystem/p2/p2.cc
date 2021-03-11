#include <cstdio>
#include <iostream>
#include "../VisionStartCode/image.h"
#include <unordered_map>
#include <unordered_set>
#include "equivalenceTable.cc"

using namespace ComputerVisionProjects;

void sequentialLabelingAlgorithm(Image &the_image) {
  EquivalenceTable labels_map;

  int label = 0;
  for(size_t r = 0; r < the_image.num_rows(); r++) {
    for(size_t c = 0; c < the_image.num_columns(); c++) {
      if(the_image.GetPixel(r,c)==0) continue; // BLACK

      // WHITE 
      if(r==0 && c==0) {
        the_image.SetPixel(r,c,++label);
        labels_map.createKey(label);
      }
      else if(r==0 && c>0) { // First row
        int left = the_image.GetPixel(r,c-1);
        if(left != 0) the_image.SetPixel(r,c,left);
        else {
          the_image.SetPixel(r,c,++label);
          labels_map.createKey(label);
        }
      }
      else if(c==0 && r>0) { // First Column
        int top = the_image.GetPixel(r-1,c);
        if(top != 0) the_image.SetPixel(r,c,top);
        else {
          the_image.SetPixel(r,c,++label);
          labels_map.createKey(label);
        }
      }
      else { // Not First Row || First Column
        int top = the_image.GetPixel(r-1, c);
        int left = the_image.GetPixel(r, c-1);
        int top_left = the_image.GetPixel(r-1, c-1);

        if(top==0 && left==0 && top_left==0) { // None labeled
          if(r==190 && c==402) std::cout << "label\n";
          the_image.SetPixel(r,c,++label);
          labels_map.createKey(label);
          if(r==190 && c==402) labels_map.printMap();
        }
        else if(top==0 && left==0 && top_left!=0) the_image.SetPixel(r,c,top_left); // Top_left
        else if(top==0 && left!=0 && top_left==0) the_image.SetPixel(r,c,left); // Left
        else if(top!=0 && left==0 && top_left==0) the_image.SetPixel(r,c,top); // Top
        else if(top!=0 && left!=0 && top_left==0) {  // Top && Left
          if(top==left) the_image.SetPixel(r,c,top);
          else {
            if(r==267 && c==211) {
              labels_map.printMap();
              std::cout << top << " " << left << " " << top_left << " \n";
            }
            the_image.SetPixel(r,c, top > left ? left : top);
            labels_map.setupEquivalence(top, left);
          }
        }
        else if(top!=0 && left==0 && top_left!=0) { // Top && Top_left
          if(top==top_left) the_image.SetPixel(r,c,top);
          else {
            the_image.SetPixel(r,c, top > top_left ? top_left : top);
            labels_map.setupEquivalence(top, top_left);
          }
        }
        else if(top==0 && left!=0 && top_left!=0) { // Top_left && Left
          if(left==top_left) the_image.SetPixel(r,c,left);
          else {
            the_image.SetPixel(r,c, left > top_left ? top_left : left);
            labels_map.setupEquivalence(left, top_left);
          }
        }
        else if(top!=0 && top_left!=0 && left!=0) the_image.SetPixel(r,c,top); // ALL three labeled
      }
    }
  }

  labels_map.printMap();
  int label_groups_count = labels_map.getNumOfGroups();
  std::cout << label_groups_count << "\n";

  for(size_t r = 0; r < the_image.num_rows(); r++) {
    for(size_t c = 0; c < the_image.num_columns(); c++) {
      if(the_image.GetPixel(r,c)==0) continue;
      int labelGroup = labels_map.findLabel(the_image.GetPixel(r,c));
      the_image.SetPixel(r,c,labelGroup+10);
    }
  }

}

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

  sequentialLabelingAlgorithm(the_image); // Run Sequential Labeling Algo

  if (!WriteImage(output_file, the_image)) {
    std::cout << "Can't write to file " << output_file << std::endl;
    return 0;
  }
}

