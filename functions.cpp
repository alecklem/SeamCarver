#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

#define INFO(X) cout << "[INFO] ("<<__FUNCTION__<<":"<<__LINE__<<") " << #X << " = " << X << endl;

using std::cout;
using std::endl;
using std::string;
using std::istringstream;
using std::ifstream;
using std::ofstream;


Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) {
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) {
      fail = true;
    }
  }
  
  if (fail) {
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i< width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  // image = nullptr; // commented this out, we will see what happens
}

int* createSeam(int length) {
    // TODO(student): create a seam
    INFO(length);

    int *seamArray = new int[length];
    for (int i = 0; i < length; i++) {
      seamArray[i] = 0;
    }

  return seamArray;
}

void deleteSeam(int* seam) {
    // TODO(student): delete a seam
    INFO(seam);
    delete[] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
    // TODO(student): load an image
    cout << "Start loadImage..." << endl;

    INFO(filename);
    INFO(image);
    INFO(width);
    INFO(height);

    ifstream inImage;
    inImage.open(filename.c_str());

    if(!inImage.is_open()) {
        cout << "Error: failed to open input file - " << filename << endl;
        return false;
    }

    char type[3];
    inImage >> type;
    if (inImage.eof()) {
      cout << "Error: not enough color values" << endl;
      return false;
    } else if (inImage.fail()) {
      cout << "Error: read non-integer value" << endl;
      return false;
    }
    if((toupper(type[0]) != 'P') || (type[1] != '3')) {
        cout << "Error: type is " << type << " instead of P3" << endl;
        return false;
    }

    int w = 0, h = 0;
    inImage >> w;
    if (inImage.eof()) {
      cout << "Error: not enough color values" << endl;
      return false;
    } else if (inImage.fail()) {
      cout << "Error: read non-integer value" << endl;
      return false;
    }
    if (w != width) {
        cout << "Error: input width (" << width << ") does not match value in file (" << w << ")" << endl;
        return false;
    }
    inImage >> h;
    if (inImage.eof()) {
      cout << "Error: not enough color values" << endl;
      return false;
    } else if (inImage.fail()) {
      cout << "Error: read non-integer value" << endl;
      return false;
    }
    if (h != height) {
        cout << "Error: input height (" << height << ") does not match value in file (" << h << ")" << endl;
        return false;
    }

    int colorMax = 0;
    inImage >> colorMax;
    if (inImage.eof()) {
      cout << "Error: not enough color values" << endl;
      return false;
    } else if (inImage.fail()) {
      cout << "Error: read non-integer value" << endl;
      return false;
    }
    if (colorMax > 255 || colorMax < 0) {
        cout << "Error: invalid color value" << colorMax << endl;
        return false;
    }

    Pixel *colorRGB = new Pixel();

    for (int i  = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        inImage >> colorRGB->r;
          if (inImage.eof()) {
            cout << "Error: not enough color values" << endl;
            return false;
          } else if (inImage.fail()) {
            cout << "Error: read non-integer value" << endl;
            return false;
          }
        inImage >> colorRGB->g;
          if (inImage.eof()) {
            cout << "Error: not enough color values" << endl;
            return false;
          } else if (inImage.fail()) {
            cout << "Error: read non-integer value" << endl;
            return false;
          }
        inImage >> colorRGB->b;
        if (i != height-1 || (i == height-1 && j != width-1)) {
          if (inImage.eof()) {
            cout << "Error: not enough color values" << endl;
            return false;
          }
        } else if (inImage.fail()) {
          cout << "Error: read non-integer value" << endl;
          return false;
        }

        image[j][i] = { colorRGB->r, colorRGB->g, colorRGB->b };
        if (image[j][i].r < 0 || image[j][i].r > 255) {
          cout << "Error: invalid color value " << image[j][i].r << endl;
          return false;
        } else if (image[j][i].g < 0 || image[j][i].g > 255) {
          cout << "Error: invalid color value " << image[j][i].g << endl;
          return false;
        } else if (image[j][i].b < 0 || image[j][i].b > 255) {
          cout << "Error: invalid color value " << image[j][i].b << endl;
          return false;
        } else if (!(i == height-1) && !(j == width-1)) {
          if (inImage.eof()) {
            cout << "Error: not enough color values" << endl;
            return false;
          }
        }
      }
    }
    inImage >> w;
    if (!inImage.fail()) {
      cout << "Error: too many color values" << endl;
      return false;
    }
    delete colorRGB;

    inImage.close();
    cout << "End loadImage..." << endl;
  return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
    // TODO(student): output an image
    cout << "Start outputImage..." << endl;
    string file_name = filename;
    ofstream outFS;
    outFS.open(file_name);

    if (!outFS.is_open()) {
      cout << "Error: failed to open output file - " << filename << endl;
      return false;
    }
    outFS << "P3" << endl;
    outFS << width << " " << height << endl;
    outFS << "255" << endl;

    for (int i  = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        outFS << image[j][i].r << " " << image[j][i].g << " " << image[j][i].b << " ";
      }
      if (!(i == height - 1)) {
          outFS << endl;
        }
    }


    INFO(filename);
    INFO(image);
    INFO(width);
    INFO(height);

    outFS.close();
  cout << "End outputImage..." << endl;
  return true;
}

int energy(Pixel** image, int column, int row, int width, int height) { 
    // TODO(student): compute the energy of a pixel
    // INFO(image);
    // INFO(column);
    // INFO(row);
    // INFO(width);
    // INFO(height);

    int energyX;
    int energyY;
    int energyTotal;
    int energyX_R;
    int energyX_G;
    int energyX_B;
    int energyY_R;
    int energyY_G;
    int energyY_B;
    
    if ((column != 0 && column != width-1) && (row != 0 && row != height-1)) { // regular case
      energyX_R = image[column-1][row].r - image[column+1][row].r;
      energyX_G = image[column-1][row].g - image[column+1][row].g;
      energyX_B = image[column-1][row].b - image[column+1][row].b;
      energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

      energyY_R = image[column][row-1].r - image[column][row+1].r;
      energyY_G = image[column][row-1].g - image[column][row+1].g;
      energyY_B = image[column][row-1].b - image[column][row+1].b;
      energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
      energyTotal = energyX + energyY;
    } else if (column == 0 && (row != 0 && row != height-1)) { // If it is in the left column, but not top or bottom row
      if (width < 3 && height < 3) {
         energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;

        energyY_R = image[column][row-1].r - image[column][row+1].r;
        energyY_G = image[column][row-1].g - image[column][row+1].g;
        energyY_B = image[column][row-1].b - image[column][row+1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[width-1][row].r - image[1][row].r;
        energyX_G = image[width-1][row].g - image[1][row].g;
        energyX_B = image[width-1][row].b - image[1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[width-1][row].r - image[1][row].r;
        energyX_G = image[width-1][row].g - image[1][row].g;
        energyX_B = image[width-1][row].b - image[1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[column][row-1].r - image[column][row+1].r;
        energyY_G = image[column][row-1].g - image[column][row+1].g;
        energyY_B = image[column][row-1].b - image[column][row+1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    } else if (column == width-1 && (row != 0 && row != height-1)) { // If it is in the right column, but not the top or bottom row
      if (width < 3 && height < 3) {
         energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;

        energyY_R = image[column][row-1].r - image[column][row+1].r;
        energyY_G = image[column][row-1].g - image[column][row+1].g;
        energyY_B = image[column][row-1].b - image[column][row+1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[column-1][row].r - image[0][row].r;
        energyX_G = image[column-1][row].g - image[0][row].g;
        energyX_B = image[column-1][row].b - image[0][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[column-1][row].r - image[0][row].r;
        energyX_G = image[column-1][row].g - image[0][row].g;
        energyX_B = image[column-1][row].b - image[0][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[column][row-1].r - image[column][row+1].r;
        energyY_G = image[column][row-1].g - image[column][row+1].g;
        energyY_B = image[column][row-1].b - image[column][row+1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    } else if (row == 0 && (column != 0 && column != width-1)) { // If it is the top row, but not the left or right column
      if (width < 3 && height < 3) {
         energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;

        energyY_R = image[column][height-1].r - image[column][1].r; 
        energyY_G = image[column][height-1].g - image[column][1].g; 
        energyY_B = image[column][height-1].b - image[column][1].b; 
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[column-1][row].r - image[column+1][row].r;
        energyX_G = image[column-1][row].g - image[column+1][row].g;
        energyX_B = image[column-1][row].b - image[column+1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[column-1][row].r - image[column+1][row].r;
        energyX_G = image[column-1][row].g - image[column+1][row].g;
        energyX_B = image[column-1][row].b - image[column+1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[column][height-1].r - image[column][1].r; 
        energyY_G = image[column][height-1].g - image[column][1].g; 
        energyY_B = image[column][height-1].b - image[column][1].b; 
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    } else if (row == height-1 && (column != 0 && column != width-1)) { // If it is the bottom row, but not the left or right column
      if (width < 3 && height < 3) {
         energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;

        energyY_R = image[column][0].r - image[column][row-1].r;
        energyY_G = image[column][0].g - image[column][row-1].g;
        energyY_B = image[column][0].b - image[column][row-1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[column-1][row].r - image[column+1][row].r;
        energyX_G = image[column-1][row].g - image[column+1][row].g;
        energyX_B = image[column-1][row].b - image[column+1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[column-1][row].r - image[column+1][row].r;
        energyX_G = image[column-1][row].g - image[column+1][row].g;
        energyX_B = image[column-1][row].b - image[column+1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[column][0].r - image[column][row-1].r;
        energyY_G = image[column][0].g - image[column][row-1].g;
        energyY_B = image[column][0].b - image[column][row-1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    } else if (column == 0 && row == 0) { // If it is the top left corner
      if (width < 3 && height < 3) {
         energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;

        energyY_R = image[0][height-1].r - image[0][1].r; 
        energyY_G = image[0][height-1].g - image[0][1].g; 
        energyY_B = image[0][height-1].b - image[0][1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[width-1][0].r - image[1][0].r;
        energyX_G = image[width-1][0].g - image[1][0].g;
        energyX_B = image[width-1][0].b - image[1][0].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[width-1][0].r - image[1][0].r;
        energyX_G = image[width-1][0].g - image[1][0].g;
        energyX_B = image[width-1][0].b - image[1][0].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[0][height-1].r - image[0][1].r; 
        energyY_G = image[0][height-1].g - image[0][1].g; 
        energyY_B = image[0][height-1].b - image[0][1].b; 
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    } else if (column == 0 && row == height-1) { // If it is the bottom left corner
      if (width < 3 && height < 3) {
        energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;

        energyY_R = image[column][0].r - image[column][row-1].r;
        energyY_G = image[column][0].g - image[column][row-1].g;
        energyY_B = image[column][0].b - image[column][row-1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[column+1][row].r - image[width-1][row].r;
        energyX_G = image[column+1][row].g - image[width-1][row].g;
        energyX_B = image[column+1][row].b - image[width-1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);
        
        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[column+1][row].r - image[width-1][row].r;
        energyX_G = image[column+1][row].g - image[width-1][row].g;
        energyX_B = image[column+1][row].b - image[width-1][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[column][0].r - image[column][row-1].r;
        energyY_G = image[column][0].g - image[column][row-1].g;
        energyY_B = image[column][0].b - image[column][row-1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    } else if (column == width-1 && row == 0) { // If it is the top right corner
      if (width < 3 && height < 3) {
        energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;
        
        energyY_R = image[column][height-1].r - image[column][row+1].r; 
        energyY_G = image[column][height-1].g - image[column][row+1].g; 
        energyY_B = image[column][height-1].b - image[column][row+1].b; 
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[column-1][row].r - image[0][row].r;
        energyX_G = image[column-1][row].g - image[0][row].g;
        energyX_B = image[column-1][row].b - image[0][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);
        
        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[column-1][row].r - image[0][row].r;
        energyX_G = image[column-1][row].g - image[0][row].g;
        energyX_B = image[column-1][row].b - image[0][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[column][height-1].r - image[column][row+1].r; 
        energyY_G = image[column][height-1].g - image[column][row+1].g; 
        energyY_B = image[column][height-1].b - image[column][row+1].b; 
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    } else if (column == width-1 && row == height-1) { // If it is the bottom right corner
    if (width < 3 && height < 3) {
        energyTotal = 0;
      } else if (width < 3) {
        energyX = 0;
        
        energyY_R = image[column][0].r - image[column][row-1].r;
        energyY_G = image[column][0].g - image[column][row-1].g;
        energyY_B = image[column][0].b - image[column][row-1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      } else if (height < 3) {
        energyX_R = image[column-1][row].r - image[0][row].r;
        energyX_G = image[column-1][row].g - image[0][row].g;
        energyX_B = image[column-1][row].b - image[0][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);
        
        energyY = 0;
        energyTotal = energyX + energyY;
      } else {
        energyX_R = image[column-1][row].r - image[0][row].r;
        energyX_G = image[column-1][row].g - image[0][row].g;
        energyX_B = image[column-1][row].b - image[0][row].b;
        energyX = pow(energyX_R, 2) + pow(energyX_G, 2) + pow(energyX_B, 2);

        energyY_R = image[column][0].r - image[column][row-1].r;
        energyY_G = image[column][0].g - image[column][row-1].g;
        energyY_B = image[column][0].b - image[column][row-1].b;
        energyY = pow(energyY_R, 2) + pow(energyY_G, 2) + pow(energyY_B, 2);
        energyTotal = energyX + energyY;
      }
    }
    cout << "Returning energyTotal: " << energyTotal << endl;
  return energyTotal;
}

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
    INFO(start_col);
    
    seam[0] = start_col;
    int totalSeamEnergy = energy(image, start_col, 0, width, height);
    int leftEnergy;
    int forwardEnergy;
    int rightEnergy;
    for (int i = 1; i < height; i++) {
      if (start_col != 0 && start_col != width-1) { // Make sure seam calculations aren't crossing over border
        forwardEnergy = energy(image, start_col, i, width, height);
        rightEnergy = energy(image, start_col-1, i, width, height);
        leftEnergy = energy(image, start_col+1, i, width, height);
        // cout << "Forward Energy i = " << i << ": " << forwardEnergy << endl;
        // cout << "Left Energy i = " << i << ": " << leftEnergy << endl;
        // cout << "Right Energy i = " << i << ": " << rightEnergy << endl;
        if ((forwardEnergy < rightEnergy) && (forwardEnergy < leftEnergy)) {
          // If straight down is less energy than diagonal right and left
          totalSeamEnergy += forwardEnergy;
          seam[i] = start_col;
        } else if ((leftEnergy < forwardEnergy) && (leftEnergy < rightEnergy)) {
          // If diagonal left is less than straight down and diagonal right
          totalSeamEnergy += leftEnergy;
          start_col = start_col+1; // changed
          seam[i] = start_col;
        } else if ((rightEnergy < forwardEnergy) && (rightEnergy < leftEnergy)) {
          // If diagonal right is less than straight down and diagonal left
          totalSeamEnergy += rightEnergy;
          start_col = start_col-1; // changed
          seam[i] = start_col;
        } else { // If one is equal to the other
          if (forwardEnergy == rightEnergy) { // If down and right are equal
            totalSeamEnergy += forwardEnergy;
            seam[i] = start_col;
          } else if (leftEnergy == rightEnergy) {
            // If left and right are equal, go to the left
            totalSeamEnergy += leftEnergy;
            start_col = start_col+1; // changed
            seam[i] = start_col;
          } else if (forwardEnergy == leftEnergy) {
            // If left and forward are equal, go forward
            totalSeamEnergy += forwardEnergy;
            seam[i] = start_col;
          } else { 
            cout << "Error, IDK how you ended up in this hoe" << endl;
          }
        }
      } else if (start_col == 0) { // Only calculate down and to the right
        forwardEnergy = energy(image, start_col, i, width, height);
        leftEnergy = energy(image, start_col+1, i, width, height);
        if (forwardEnergy < leftEnergy) { // if straight down is less energy
          totalSeamEnergy += forwardEnergy;
          seam[i] = start_col;
        } else if (forwardEnergy > leftEnergy) { // if the right diagonal is less energy
          totalSeamEnergy += leftEnergy;
          start_col = start_col+1;
          seam[i] = start_col;
        } else { // If one is equal to the other go straight down
          if (forwardEnergy == leftEnergy) {
            totalSeamEnergy += forwardEnergy;
            seam[i] = start_col;
          }
        }
      } else if (start_col == width-1) { // Only calculate down and to the left
        forwardEnergy = energy(image, start_col, i, width, height);
        rightEnergy = energy(image, start_col-1, i, width, height);
        if (forwardEnergy < rightEnergy) { // if straight down is less energy
          totalSeamEnergy += forwardEnergy;
          seam[i] = start_col;
        } else if (forwardEnergy > rightEnergy) { // if to the left diagonal is less energy
          totalSeamEnergy += rightEnergy;
          start_col = start_col-1;
          seam[i] = start_col;
        } else { // If one is equal to the other, go straight down
          if (forwardEnergy == rightEnergy) {
            totalSeamEnergy += forwardEnergy;
            seam[i] = start_col;
          }
        }
      }
    }

  return totalSeamEnergy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
    int* seam1 = createSeam(height);

    int lowestEnergySeamIndex = 0; // initialize lowest energy as first index
    int minimalEnergy;

    minimalEnergy = loadVerticalSeam(image, lowestEnergySeamIndex, width, height, seam1);

    for (int i = 1; i < width; i++) { // creating the seam 2d array with all the info
      if (minimalEnergy > loadVerticalSeam(image, i, width, height, seam1)) {
        minimalEnergy = loadVerticalSeam(image, i, width, height, seam1);
        lowestEnergySeamIndex = i;
      } else if (minimalEnergy == loadVerticalSeam(image, i, width, height, seam1)) {
        if (lowestEnergySeamIndex > i) {
          lowestEnergySeamIndex = i;
        }
      }
    }
    minimalEnergy = loadVerticalSeam(image, lowestEnergySeamIndex, width, height, seam1);

  return seam1; // return array pointer of lowest energy index
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
    // TODO(student): remove a vertical seam
    INFO(image);
    INFO(width);
    INFO(height);
    INFO(verticalSeam);

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width-1; j++) { // stop one early so we can remove the last width
        if (j >= verticalSeam[i]) {
          image[j][i] = image[j+1][i];
        }
      }
    }
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
    // TODO(student): load a horizontal seam
    INFO(image);
    INFO(start_row);
    INFO(width);
    INFO(height);
    INFO(seam);
  return 0;
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
    // TODO(student): remove a horizontal seam
    INFO(image);
    INFO(width);
    INFO(height);
    INFO(horizontalSeam);
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
    // TODO(student): find min horizontal seam
    INFO(image);
    INFO(width);
    INFO(height);
  return nullptr;
}