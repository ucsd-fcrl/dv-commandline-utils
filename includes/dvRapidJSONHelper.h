#ifndef dv_RapidJSONHelper_h
#define dv_RapidJSONHelper_h

#include <rapidjson/document.h>

namespace dv {

void
check_and_set_int(const rapidjson::Document& doc, int& a, const std::string& s);

void
check_and_set_uint(const rapidjson::Document& doc,
                   unsigned int& a,
                   const std::string& s);

void
check_and_set_bool(const rapidjson::Document& doc,
                   bool& a,
                   const std::string& s);

void
check_and_set_double(const rapidjson::Document& doc,
                     double& a,
                     const std::string& s);

void
check_and_set_double_array(const rapidjson::Document& doc,
                           double a[3],
                           std::string s);

}

#endif
