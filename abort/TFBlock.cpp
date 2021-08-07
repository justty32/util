#include "TFBlock.h"

template<typename T>
std::optional<std::tuple<char*, int, bool>> zz::ToBlock(T& target) {
	int size = sizeof(T) + sizeof(int);
	char* ptr = new char[size];
	Mfp ff{ ptr, size };
	ff.write(&size); ff.write(&target);
	return std::make_tuple(ptr, size, true);
}
template<typename T>
bool zz::FromBlock(T& refTarget, int& refSize, char* pp, int s, bool r) {
	ZZ_TRT(pp == nullptr || s < sizeof(int));
	Mfp ff{ pp, s };
	ZZ_TRT(ff.read(&refSize));
	ZZ_TRT(refSize != sizeof(T));
	ZZ_TRT(ff.read(&refTarget));
	return false;
}

std::optional<std::tuple<char*, int, bool>> zz::CatBlocks(
	std::initializer_list<std::tuple<char*, int, bool>> bl
) {
	int size = sizeof(int);
	for (auto& t : bl)
		size += std::get<int>(t) + sizeof(int);
	char* ptr = new char[size];
	Mfp ff{ ptr, size };
	ZZ_TRONFUN(ff.write(&size), delete[] ptr);
	char* p = nullptr; int s = 0; bool r = false;
	for (auto& t : bl) {
		std::tie(p, s, r) = t;
		ff.write(&s);
		ff.write(p, s);
		if (r) delete[] p;
	}
	return std::make_tuple(ptr, size, true);
}
std::vector<std::tuple<char*, int, bool>> zz::UnCatBlocks(
	char* ptr, int size, bool r) {
	std::vector<std::tuple<char*, int, bool>> vec;
	if (ptr == nullptr || size <= sizeof(int)) return vec;
	Mfp ff{ ptr, size };
	int vs = 0; ff.read(&vs);
	vec.resize(vs);
	char* p = nullptr; int s = 0;
	for (int i = 0; i < vs; i++) {
		ff.read(&s); ff.read(p, s);
		if (p != nullptr) 
			vec.push_back(std::make_tuple(p, s, true));
	}
	return vec;
}

template<typename T>
std::optional<std::tuple<char*, int, bool>> zz::ToBlockParr(T* arr, int arrsize) {
	ZZ_TRON(arr == nullptr || arrsize < 0);
	int size = sizeof(T) * arrsize + sizeof(int);
	char* ptr = new char[size];
	Mfp fff{ ptr, size };
	ZZ_TRONFUN(fff.write(&size), delete[] ptr;)
	if(arrsize > 0)
		ZZ_TRONFUN(fff.write(arr, arrsize), delete[] ptr);
	return std::make_tuple(ptr, size, true);
}
//FromBlockParr(&arr, arrsize, pp, size);
template<typename T>
bool zz::FromBlockParr(T** arrPtr, int& refArrSize, char* pp, int size, bool r) {
	ZZ_TRT(pp == nullptr || size < sizeof(int));
	Mfp fff{ pp, size };
	ZZ_TRT(fff.read(&refArrSize));
	if ((*arrPtr) == nullptr) (*arrPtr) = new T[refArrSize];
	ZZ_TRT(fff.read(*arrPtr, refArrSize));
	return false;
}

template<typename Tk, typename Tv>
std::optional<std::tuple<char*, int, bool>> zz::ToBlockUmap( std::unordered_map<Tk, Tv>& umap){
	ZZ_TRON(umap.size() == 0);
	int size = umap.size() * (sizeof(Tk) + sizeof(Tv)) + sizeof(int);
	char* ptr = new char[size];
	Mfp ff;
	ZZ_TRONFUN(ff.open(ptr, size), delete [] ptr);
	int usize = umap.size();
	ZZ_TRONFUN(ff.write(&usize), delete [] ptr);
	for (auto& kv : umap) {
		ff.write(&(kv.first));
		ff.write(&(kv.second));
	}
	return std::make_tuple(ptr, size, true);
}
template<typename Tk, typename Tv>
bool zz::FromBlockUmap(std::unordered_map<Tk, Tv>& umap, char* pp, int size, bool r){
	ZZ_TRT(pp == nullptr || size < sizeof(int));
	umap.clear();
	Mfp fff{ pp, size };
	int arrSize = 0;
	ZZ_TRT(fff.read(&arrSize));
	Tk k; Tv v;
	for (int i = 0; i < arrSize; i++) {
		fff.read(&k); fff.read(&v);
		umap.emplace(k, v);
	}
	return false;
}

template<typename T>
std::optional<std::tuple<char*, int, bool>> zz::ToBlockVec( std::vector<T>& ve){
	int size = ve.size() * sizeof(T) + sizeof(int);
	char* ptr = new char[size];
	Mfp ff;
	ZZ_TRONFUN(ff.open(ptr, size), delete[] ptr);
	int usize = ve.size();
	ZZ_TRONFUN(ff.write(&usize), delete[] ptr);
	if(usize > 0)
		ZZ_TRONFUN(ff.write(&(ve[0]), usize), delete[] ptr);
	return std::make_tuple(ptr, size, true);
}
template<typename T>
bool zz::FromBlockVec(std::vector<T>& ve, char* pp, int size, bool r){
	ZZ_TRT(pp == nullptr || size < sizeof(int));
	ve.clear();
	Mfp fff{ pp, size };
	int arrSize = 0;
	ZZ_TRT(fff.read(&arrSize));
	ve.resize(arrSize);
	fff.read(&(ve[0]), arrSize);
	return false;
}

std::optional<std::tuple<char*, int, bool>> zz::ToBlockStr( std::string& str){
	int size = str.length() * sizeof(char) + sizeof(int);
	char* ptr = new char[size];
	Mfp ff;
	ZZ_TRONFUN(ff.open(ptr, size), delete[] ptr);
	int usize = str.length();
	ZZ_TRONFUN(ff.write(&usize), delete [] ptr);
	if(usize > 0)
		ZZ_TRONFUN(ff.write(str.c_str(), usize), delete [] ptr);
	return std::make_tuple(ptr, size, true);
}
bool zz::FromBlockStr(std::string& str, char* pp, int size, bool r){
	ZZ_TRT(pp == nullptr || size < sizeof(int));
	str.clear();
	Mfp fff{ pp, size };
	int arrSize = 0;
	ZZ_TRT(fff.read(&arrSize));
	str.resize(arrSize);
	if(arrSize > 0)
		ZZ_TRT(fff.read(str.c_str(), arrSize));
	return false;
}