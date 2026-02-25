#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

class DivaConverter {
   private:
    std::string diva_lut_filename;
    // this can be used in a bidirectional way,
    // i.e., diva -> name and name->diva
    // should be fine.
    std::unordered_map<std::string, std::string> diva_cache;

    // these functions have the side effect of adding the pair
    // to the cache for faster retrieval the next time they're
    // queried.

    // This function always returns (DIVA, name)!!
    std::pair<std::string, std::string> fetch_from_file_by_name(
        const std::string&);

    // This function always returns (DIVA, name)!!
    std::pair<std::string, std::string> fetch_from_file_by_diva(
        const std::string&);

   public:
    DivaConverter(std::string);

    std::string get_diva_by_name(const std::string&);
    std::string get_name_by_diva(const std::string&);
    std::string get_name_by_diva(const uint32_t);
};
