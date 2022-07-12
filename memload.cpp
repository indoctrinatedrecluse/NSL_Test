#include <sw/redis++/redis++.h>
#include <iostream>
#include "happly.h"

using namespace sw::redis;
using namespace std;

int main (int argc, char *argv[]) {
  // Open redis interaction object
  auto redis = Redis("tcp://127.0.0.1:6379");

  // Construct the data object by reading from file
  happly::PLYData plyIn("kinect_captures/1.ply");

  // Get mesh-style data from the object
  std::vector<std::array<double, 3>> vPos = plyIn.getVertexPositions();
  std::vector<std::array<unsigned char, 3>> vCol = plyIn.getVertexColors();

  std::vector<std::array<double, 3>> vPosHead;
  int count = 10;
  for (auto x: vPos) {
    cout << x[0] << " " << x[1] << " " << x[2] << endl;
    vPosHead.push_back(x);
    count --;
    if (count==0) {
      break;
    }
  }

  redis.set("positions", vPosHead);
  auto value = redis.get("positions");
  if (value) {
    std::vector<std::array<double, 3>> head = (std::vector<std::array<double, 3>>) (*value);
    cout << head[0][0] << endl;
  }
  return 0;
}
