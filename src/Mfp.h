#ifndef DATAVEC_H
#define DATAVEC_H

#include <vector>
#include <memory>

namespace uio{
	namespace serialize{
		typedef struct Mfp{
			char* ptr = nullptr;
			size_t size = 0;
			size_t fp = 0;
			bool good();
			bool writable(size_t data_size);
			bool readable(size_t data_size);
			bool seek(int offset);
			bool seekbeg(int offset = 0);
			bool seekback(int offset = 0);
			bool seekable(int offset);
			template<typename T>
			bool open(T* target_ptr, int target_size) {
				close();
				if (target_ptr == nullptr || target_size == 0)
					return true;
				ptr = dynamic_cast<char*>(target_ptr);
				size = target_size;
				return false;
			}
			void close();
			template<typename T>
			bool write(T* data, size_t data_size = 0) {
				if (data_size == 0) data_size = sizeof(T);
				if (!good() || !writable(data_size)
					|| data == nullptr || data_size == 0)
					return true;
				memcpy(ptr + fp, data, data_size);
				fp += data_size;
				return false;
			}
			template<typename T>
			bool read(T* target, size_t data_size = 0, size_t target_size = 0) {
				if (data_size == 0 && target_size == 0) data_size = target_size = sizeof(T);
				else if (target_size == 0) target_size = data_size;
				else if (data_size == 0) data_size = target_size;
				if (!good() || !readable(data_size)
					|| data_size == 0 || target_size == 0
					|| target == nullptr || target_size < data_size)
					return true;
				memcpy(target, ptr + fp, data_size);
				fp += data_size;
				return false;
			}
			bool write_from_mfp(Mfp mf, size_t data_size = 0);
			bool read_to_mfp(Mfp mf, size_t data_size = 0);
			template<typename T>
			T get() {
				T t;
				read<T>(&t);
				return t;
			}
			template<typename T>
			bool set(T t) { return write<T>(&t); }
		} Mfp;
	}
}

#endif // !DATAVEC_H