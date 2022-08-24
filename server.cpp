#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/asio.hpp>
#include "happly.h"

#define COL_BUF_NAME "colbuffer"
#define POS_BUF_NAME "posbuffer"

using namespace std;
using namespace boost::interprocess;
using boost::asio::ip::tcp;

static const string IP_ADDRESS = "127.0.0.1";
static const int PORT = 8888;
static const int TCP_BUFFER_SIZE = 1024;

typedef struct PLYPacket {
    uint32_t size;
    std::vector<std::array<double, 3>> vPos;
    std::vector<std::array<unsigned char, 3>> vCol;
} plyPacket;

class TcpClient {
    boost::asio::io_service& io_service;
    boost::asio::ip::tcp::socket socket;

    public:
    
        TcpClient(boost::asio::io_service& svc, std::string const& host, int const& port) : io_service(svc), socket(io_service) 
        {
            cout << "Trying to connect ..." << std::endl;
            socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port));
        }

        ~TcpClient() 
        {
            cout << "Closing connection ..." << endl;
            socket.close();
        }

        void send(const uint8_t *buf, const int buf_size) {
            send_to_socket(buf, buf_size);
            string message = "Testing!";
            socket.send(boost::asio::buffer(message));
        }

    private:
    
        void send_to_socket(const uint8_t *buf, const int buf_size) {
            uint32_t data_sent_in_bytes = 0;
            try
            {
                const uint32_t rc = boost::asio::write(socket, boost::asio::buffer(buf, buf_size));
                data_sent_in_bytes = rc;
            }
            catch (boost::system::system_error e)
            {
                std::cerr << boost::diagnostic_information(e);
            }
            std::cout << " sent " << data_sent_in_bytes << " bytes" << std::endl;
            return;
        }
}

class MemManager {
    public:

        void destroy_shm(const char* name) {
            shared_memory_object::remove(name);
            cout << "Destroyed shared memory " << name << endl;
        }

        void create_shm(const char* name, size_t size, mapped_region& region) {
            //Create a shared memory object
            shared_memory_object shm_object(open_or_create, name, read_write);

            //Set size
            shm_object.truncate(size);

            //Map the whole shared memory in this process
            region = mapped_region(shm_object, read_write);

            cout << "Created shared memory " << name << " with size " << size << endl;
        }

        plyPacket memload(const char* name) {
            // Construct the data object by reading from file
            happly::PLYData plyIn("kinect_captures/1.ply");

            plyPacket packet;

            // Get mesh-style data from the object
            packet.vPos = plyIn.getVertexPositions();
            packet.vCol = plyIn.getVertexColors();

            if (packet.vPos.size() == packet.vCol.size()) {
                packet.size = packet.vPos.size();
                return packet;
            }

            return null;
        }
}

int main (int argc, char *argv[]) {
    cout << "Hi from the server!";

    MemManager memManager = new MemManager();

    //Remove shared memory on construction and destruction
	struct shm_remove
	{
		shm_remove() {
            memManager.destroy_shm(COL_BUF_NAME);
            memManager.destroy_shm(POS_BUF_NAME);
        }
		~shm_remove() {
            memManager.destroy_shm(COL_BUF_NAME);
            memManager.destroy_shm(POS_BUF_NAME);
        }
	} remover;

    //Create a shared memory object
	mapped_region region;
    uint32_t shm_size = get_image_size(path, true);
	create_shm(SHM_NAME, shm_size, region);
	uint8_t *shm_ptr = static_cast<uint8_t *>(region.get_address());

    return 0;
}