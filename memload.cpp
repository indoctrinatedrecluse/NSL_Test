#include <iostream>
#include "happly.h"

using namespace std;

int main (int argc, char *argv[]) {
  // Construct the data object by reading from file
  happly::PLYData plyIn("kinect_captures/1.ply");

  // Get mesh-style data from the object
  std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
  std::vector<std::array<unsigned char, 3>> vCol = plyIn.getVertexColors();

  int count = 10;
  for (auto x: vPos) {
    cout << x[0] << " " << x[1] << " " << x[2] << endl;
    count --;
    if (count==0) {
      break;
    }
  }
}
