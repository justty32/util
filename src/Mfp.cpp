#include "./Mfp.h"

using namespace std;

namespace uio{
	namespace serialize{
		bool Mfp::good() {
			return ptr != nullptr && size > 0
				&& fp >= 0 && fp <= size;
		}
		bool Mfp::writable(size_t data_size) {
			return good() && fp + data_size <= size
				&& fp < size; }
		bool Mfp::readable(size_t data_size) {
			return good() && fp + data_size <= size
				&& fp < size; }
		bool Mfp::seek(int offset) {
			if (offset < 0 && fp < static_cast<size_t>(abs(offset)))
				return true;
			if (offset + fp > size)
				return true;
			fp += offset;
			return false;
		}
		bool Mfp::seekbeg(int offset) {
			if (offset < 0)
				return true;
			fp = 0;
			return seek(offset);
		}
		bool Mfp::seekback(int offset) {
			if (offset < 0)
				return true;
			fp = size;
			return seek(-offset);
		}
		bool Mfp::seekable(int offset) {
			size_t pre_fp = fp;
			if (seek(offset)) return false;
			fp = pre_fp;
			return true;
		}
		void Mfp::close() {
			ptr= nullptr;
			size = fp = 0;
		}
		bool Mfp::write_from_mfp(Mfp mf, size_t data_size) {
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
		bool Mfp::read_to_mfp(Mfp mf, size_t data_size) {
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
	}
}