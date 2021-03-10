#include <cstdio>
#include <iostream>
#include "../VisionStartCode/image.h"
#include <unordered_map>
#include <unordered_set>
using namespace ComputerVisionProjects;

void sequentialLabelingAlgorithm(Image &the_image) {
  std::unordered_map<int, std::unordered_set<int>> labels_map;

  int label = 0;
  for(size_t r = 0; r < the_image.num_rows(); r++) {
    for(size_t c = 0; c < the_image.num_columns(); c++) {
      if(the_image.GetPixel(r,c)==0) continue; // BLACK

      // WHITE 
      if(r==0 && c==0) continue;
      else if(r==0 && c>0) { // First row
        int left = the_image.GetPixel(r,c-1);
        if(left != 0) the_image.SetPixel(r,c,left);
        else {
          the_image.SetPixel(r,c,++label);
          std::unordered_set<int> ( {label} );
        }
      }
      else if(c==0 && r>0) { // First Column
        int top = the_image.GetPixel(r-1,c);
        if(top != 0) the_image.SetPixel(r,c,top);
        else {
          the_image.SetPixel(r,c,++label);
          std::unordered_set<int> ( {label} );
        }
      }
      else { // Not First Row || First Column
        int top = the_image.GetPixel(r, c-1);
        int left = the_image.GetPixel(r-1, c);
        int top_left = the_image.GetPixel(r-1, c-1);

        if(top==0 && left==0 && top_left==0) { // None labeled
          the_image.SetPixel(r,c,++label);
          labels_map[label] = std::unordered_set<int> ( {label} );
        }
        else if(top==0 && left==0 && top_left!=0) the_image.SetPixel(r,c,top_left); // Top_left
        else if(top==0 && left!=0 && top_left==0) the_image.SetPixel(r,c,left); // Left
        else if(top!=0 && left==0 && top_left==0) the_image.SetPixel(r,c,top); // Top
        else if(top!=0 && left!=0 && top_left==0) {  // Top && Left
          if(top==left) the_image.SetPixel(r,c,top);
          else {
            int smaller_label = top>left ? left : top;
            int bigger_label = top>left ? top : left;
            the_image.SetPixel(r,c, smaller_label);
            // Set up equivalences
            if(labels_map.count(smaller_label)) {
              labels_map[smaller_label].insert(bigger_label);
              labels_map.erase(bigger_label);
            }
            else {
              bool canDeleteBigLabel = false;
              for(auto m : labels_map) {
                if(canDeleteBigLabel) break;
                if(m.second.count(smaller_label)) {
                  m.second.insert(bigger_label);
                  canDeleteBigLabel = true;
                }
              }
              labels_map.erase(bigger_label);
            }
          }
        }
        else if(top!=0 && left==0 && top_left!=0) { // Top && Top_left
          if(top==top_left) the_image.SetPixel(r,c,top);
          else {
            int smaller_label = top>top_left ? top_left : top;
            int bigger_label = top>top_left ? top : top_left;
            the_image.SetPixel(r,c, smaller_label);
            // Set up equivalences
            if(labels_map.count(smaller_label)) {
              labels_map[smaller_label].insert(bigger_label);
              labels_map.erase(bigger_label);
            }
            else {
              bool canDeleteBigLabel = false;
              for(auto m : labels_map) {
                if(canDeleteBigLabel) break;
                if(m.second.count(smaller_label)) {
                  m.second.insert(bigger_label);
                  canDeleteBigLabel = true;
                }
              }
              labels_map.erase(bigger_label);
            }
          }
        }
        else if(top==0 && left!=0 && top_left!=0) { // Top_left && Left
          if(left==top_left) the_image.SetPixel(r,c,left);
          else {
            int smaller_label = left>top_left ? top_left : left;
            int bigger_label = left>top_left ? left : top_left;
            the_image.SetPixel(r,c, smaller_label);
            // Set up equivalences
            if(labels_map.count(smaller_label)) {
              labels_map[smaller_label].insert(bigger_label);
              labels_map.erase(bigger_label);
            }
            else {
              bool canDeleteBigLabel = false;
              for(auto m : labels_map) {
                if(canDeleteBigLabel) break;
                if(m.second.count(smaller_label)) {
                  m.second.insert(bigger_label);
                  canDeleteBigLabel = true;
                }
              }
              labels_map.erase(bigger_label);
            }
          }
        }
        else if(top!=0 && top_left!=0 && left!=0) the_image.SetPixel(r,c,top); // ALL three labeled
        else {
          std::cout << "else ";
        }
      }
    }
  }

  for(auto p : labels_map) {
    std::cout << "key: [" << p.first << "]\n";
    std::cout << "val: [";
    for(auto set : p.second) {
      std::cout << set << ", ";
    }
    std::cout << "\n";
  }

  int label_groups_count = labels_map.size();
  std::cout << label_groups_count << "\n";

  for(size_t r=0; r<the_image.num_rows(); r++) {
    for(size_t c=0; c<the_image.num_columns(); c++) {
      if(the_image.GetPixel(r,c)==0) continue; // BLACK

      
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

  // if (!WriteImage(output_file, the_image)) {
  //   std::cout << "Can't write to file " << output_file << std::endl;
  //   return 0;
  // }
}

