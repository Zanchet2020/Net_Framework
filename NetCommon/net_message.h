#pragma once

#include <iostream>
#include <net_common.h>

namespace networking {

	template<typename T>
	class message_header {
	public:
		T id{};
		uint32_t size = 0;
	};


	template<typename T>
	class message {
	public:
		message_header<T> header{};
		std::vector<uint8_t> body;

		size_t size()
		{
			return body.size();
		}


		friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
		{
			os << "ID: " << static_cast<int>(msg.header.id) << "\nSize: " << msg.header.size << "\nBody: ";
			return os;
		}



		template<typename DataType>
		friend message<T>& operator<<(message<T>& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to insert into vector! Create custom overload");

			//caching old size to place new data
			size_t old_end = msg.size();

			// resize array to fit new data
			msg.body.resize(old_end + sizeof(DataType));

			// copy new data to the end of the array
			std::memcpy(msg.body.data() + old_end, &data, sizeof(DataType));

			// update the size of the array in the header
			msg.header.size += sizeof(DataType);

			return msg;
		}

		template<typename DataType>
		friend message<T>& operator >> (message<T>& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to insert into vector! Create custom overload");

			// caching new end for the vector
			size_t new_end = msg.size() - sizeof(DataType);

			// save value into output data
			std::memcpy(&data, msg.body.data() + new_end, sizeof(DataType));

			// resize vector
			msg.body.resize(new_end);

			msg.header.size = new_end;

			return msg;
		}

	};

}


