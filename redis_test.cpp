#include <sw/redis++/redis++.h>
#include <iostream>

using namespace sw::redis;
using namespace std;

int main (int argc, char *argv[]) {
    auto redis = Redis("tcp://127.0.0.1:6379");
    cout << redis.ping() << endl;

    return 0;
}