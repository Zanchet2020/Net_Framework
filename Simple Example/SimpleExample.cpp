#include <iostream>

#define ASIO_STANDALONE

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

using namespace boost;

int main() {
	system::error_code errc;

	// armazenate variables of the current context
	asio::io_context io_context;

	// final system running an application
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", errc), 80);

	// one side of the connection
	asio::ip::tcp::socket socket(io_context);

	socket.connect(endpoint, errc);

	if (!errc) std::cout << "Connected!" << std::endl;
	else std::cout << "Failed to connect: " << errc.message() << std::endl;

	if (socket.is_open()) {
		std::string request = "GET /index.html HTTP/1.1\r\n" "Host: example.com\r\n" "Connection: close\r\n\r\n";
		
		socket.write_some(asio::buffer(request.data(), request.size()), errc);

		socket.wait(socket.wait_read);

		size_t bytes = socket.available();

		std::cout << "Bytes available: " << bytes << std::endl;

		if (bytes > 0) {
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), errc);
		
			for (char c : vBuffer) {
				std::cout << c;
			}
			
			std::cout << std::endl;
		}

		
	}

	return 0;
}
