#ifndef DATAVEC_H
#define DATAVEC_H

#include <vector>
#include <memory>
#include <cstring>

namespace uio{
	namespace serialize{
		typedef struct Mfp{
			char* ptr = nullptr;
			size_t size = 0;
			size_t fp = 0;
			inline bool good() {
				return ptr != nullptr && size > 0
					&& fp >= 0 && fp <= size;
			}
			inline bool writable(size_t data_size) {
				return good() && fp + data_size <= size
					&& fp < size; }
			inline bool readable(size_t data_size) {
				return good() && fp + data_size <= size
					&& fp < size; }
			inline bool seek(int offset) {
				if (offset < 0 && fp < static_cast<size_t>(abs(offset)))
					return true;
				if (offset + fp > size)
					return true;
				fp += offset;
				return false;
			}
			inline bool seekbeg(int offset) {
				if (offset < 0)
					return true;
				fp = 0;
				return seek(offset);
			}
			inline bool seekback(int offset) {
				if (offset < 0)
					return true;
				fp = size;
				return seek(-offset);
			}
			inline bool seekable(int offset) {
				size_t pre_fp = fp;
				if (seek(offset)) return false;
				fp = pre_fp;
				return true;
			}
			template<typename T>
			bool open(T* target_ptr, int target_size) {
				close();
				if (target_ptr == nullptr || target_size == 0)
					return true;
				ptr = dynamic_cast<char*>(target_ptr);
				size = target_size;
				return false;
			}
			inline void close() {
				ptr= nullptr;
				size = fp = 0;
			}
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
			template<typename T>
			T get() {
				T t;
				read<T>(&t);
				return t;
			}
			template<typename T>
			bool set(T t) {
				return write<T>(&t); 
			}
			inline bool write_from_mfp(Mfp mf, size_t data_size) {
				if (data_size == 0) data_size = mf.size;
				if (!good() || !writable(data_size)
					|| !mf.good() || !mf.readable(data_size)
					|| data_size == 0)
					return true;
				memcpy(ptr + fp, mf.ptr + mf.fp, data_size);
				mf.fp += data_size;
				fp += data_size;
				return false;
			}
			inline bool read_to_mfp(Mfp mf, size_t data_size) {
				if (data_size == 0) data_size = mf.size;
				if (!good() || !readable(data_size)
					|| !mf.good() || !mf.writable(data_size)
					|| data_size == 0)
					return true;
				memcpy(mf.ptr + mf.fp, ptr + fp, data_size);
				mf.fp += data_size;
				fp += data_size;
				return false;
			}
		} Mfp;
	}
}

#endif // !DATAVEC_H