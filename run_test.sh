g++ -std=c++17 -o redis memload.cpp /usr/local/lib/libredis++.a /usr/local/lib/libhiredis.a -pthread
./redis
