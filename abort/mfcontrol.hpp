#pragma once
#ifndef MFCONTROL_H
#define MFCONTROL_H

#include <memory>

struct Mfp {
	char* ptr = nullptr;
	int size = 0;
	int fp = 0;
	bool isGood() {
		return ptr != nullptr
			&& size > 0
			&& fp >= 0;
	}
	bool isWritable(int data_size = 0) {
		if (data_size < 0) return false;
		return isGood() && fp + data_size <= size
			&& fp < size; }
	bool isReadable(int data_size = 0) {
		if (data_size < 0) return false;
		return isGood() && fp + data_size <= size
			&& fp < size; }
	bool seek(int offset) {
		fp += offset;
		if (fp < 0 || fp > size) {
			fp -= offset;
			return true;
		}
		return false;
	}
	void seekbeg(int offset = 0) {
		if (offset < 0)
			return;
		int origin = fp;
		fp = offset;
		if (fp < 0 || fp > size)
			fp = origin;
	}
	void seekend(int offset = 0) {
		if (offset < 0)
			return;
		int origin = fp;
		fp = size - offset;
		if (fp < 0 || fp > size)
			fp = origin;
	}
	bool open(void* target_ptr, int target_size) {
		close();
		if (target_ptr == nullptr || target_size <= 0)
			return true;
		ptr = (char*)target_ptr;
		size = target_size;
		return false;
	}
	void close() {
		ptr= nullptr;
		size = fp = 0;
	}
	bool writevoid(void* data, int data_size) {
		if (!isGood() || !isWritable(data_size)
			|| data == nullptr || data_size <= 0)
			return true;
		memcpy(ptr + fp, (char*)data, data_size);
		fp += data_size;
		return false;
	}
	bool readvoid(void* target, int data_size) {
		if (!isGood() || !isReadable(data_size)
			|| data_size <= 0)
			return true;
		memcpy((char*)target, ptr + fp, data_size);
		fp += data_size;
		return false;
	}
	template<typename T>
	bool write(T* data, int data_num = 1) {
		return writevoid((void*)data, data_num * sizeof(T));
	}
	template<typename T>
	bool read(T* data, int data_num = 1) {
		return readvoid((void*)data, data_num * sizeof(T));
	}
	template<typename T>
	bool open(T* target_ptr, int target_size){
		return open((void*)target_ptr, target_size);
	}
};

#endif // !MFCONTROL_H

