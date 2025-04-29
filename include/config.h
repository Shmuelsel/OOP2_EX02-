#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Field.h"

namespace Config {
    extern std::vector<std::string> fieldLabelsBase;
    extern std::vector<std::string> fieldLabelsHotel;
    extern std::vector<std::string> fieldLabelsFlight;

	extern std::vector<std::unique_ptr<FieldBase>> fieldsForm;
	extern std::vector<std::unique_ptr<FieldBase>> fieldsHotel;
	extern std::vector<std::unique_ptr<FieldBase>> fieldsFlight;
}
