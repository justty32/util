#include "./Dvec.h"

using namespace std;
namespace uio {
	namespace serialize {
		namespace detail {
			std::vector<Dvec> split_dvec(Dvec dvec) {
				if (dvec.empty()) return {};
				Mfp mf{ dvec.data(), dvec.size() };
				size_t ds_size = mf.get<size_t>();
				std::vector<Dvec> ds(ds_size);
				for (size_t i = 0; i < ds_size; i++)
					ds[i] = mf_get_dvec(mf);
				return ds;
			}
			Dvec mf_get_dvec(Mfp& mf) {
				size_t d_size = mf.get<size_t>();
				Dvec d(d_size);
				mf.read(d.data(), d_size);
				return d;
			}
			Dvec to_dvec_str(std::string& target) {
				if (target.empty()) return {};
				Dvec d(target.size());
				memcpy(d.data(), target.c_str(), target.size());
				return d;
			}
			std::string from_dvec_str(Dvec dvec) { 
				if (dvec.empty()) return {};
				std::string str(dvec.data(), dvec.size());
				str.resize(str.size() + 1, 0);
				return str;
			}
			size_t cp_with_size(char* ptr, Dvec d) {
				// not consider ptr's size, just copy to it
				size_t s = d.size();
				memcpy(ptr, &s, sizeof(size_t));
				memcpy(ptr + sizeof(size_t), d.data(), d.size());
				return s + sizeof(size_t);
			}
		}
	}
}