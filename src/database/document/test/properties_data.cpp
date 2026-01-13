#include "properties_data.h"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"

#include <boost/numeric/conversion/cast.hpp>

celeritas::properties_data::properties_data()
    : expire_time_{ 0 }
      , int32_val_{ 0 }
      , int32_count_val_{ 0 }
      , int64_count_val_{ 0 }
      , double_val_{ 0.0 }
      , bool_val_{ false }
{
}

celeritas::properties_data::properties_data(const int64_t expire_time)
    : expire_time_{ expire_time }
      , int32_val_{ 0 }
      , int32_count_val_{ 0 }
      , int64_count_val_{ 0 }
      , double_val_{ 0.0 }
      , bool_val_{ false }
{
}

int64_t celeritas::properties_data::get_expire_time() const
{
    return expire_time_;
}

void celeritas::properties_data::set_expire_time(const int64_t expire_time)
{
    expire_time_ = expire_time;
}

const std::string& celeritas::properties_data::get_string_val() const
{
    return string_val_;
}

void celeritas::properties_data::set_string_val(std::string string_val)
{
    string_val_ = std::move(string_val);
}

const std::vector<std::string>& celeritas::properties_data::get_string_array_val() const
{
    return string_array_val_;
}

void celeritas::properties_data::set_string_array_val(std::vector<std::string> string_array_val)
{
    string_array_val_ = std::move(string_array_val);
}

int32_t celeritas::properties_data::get_int32_val() const
{
    return int32_val_;
}

void celeritas::properties_data::set_int32_val(const int32_t int32_val)
{
    int32_val_ = int32_val;
}

int32_t celeritas::properties_data::get_int32_count_val() const
{
    return int32_count_val_;
}

void celeritas::properties_data::set_int32_count_val(const int32_t int32_count_val)
{
    int32_count_val_ = int32_count_val;
}

const std::vector<int32_t>& celeritas::properties_data::get_int32_array_val() const
{
    return int32_array_val_;
}

void celeritas::properties_data::set_int32_array_val(std::vector<int32_t> int32_array_val)
{
    int32_array_val_ = std::move(int32_array_val);
}

int64_t celeritas::properties_data::get_int64_count_val() const
{
    return int64_count_val_;
}

void celeritas::properties_data::set_int64_count_val(const int64_t int64_count_val)
{
    int64_count_val_ = int64_count_val;
}

const std::vector<int64_t>& celeritas::properties_data::get_int64_array_val() const
{
    return int64_array_val_;
}

void celeritas::properties_data::set_int64_array_val(std::vector<int64_t> int64_array_val)
{
    int64_array_val_ = std::move(int64_array_val);
}

double celeritas::properties_data::get_double_val() const
{
    return double_val_;
}

void celeritas::properties_data::set_double_val(const double double_val)
{
    double_val_ = double_val;
}

const std::vector<double>& celeritas::properties_data::get_double_array_val() const
{
    return double_array_val_;
}

void celeritas::properties_data::set_double_array_val(std::vector<double> double_array_val)
{
    double_array_val_ = std::move(double_array_val);
}

bool celeritas::properties_data::get_bool_val() const
{
    return bool_val_;
}

void celeritas::properties_data::set_bool_val(const bool bool_val)
{
    bool_val_ = bool_val;
}

const celeritas::properties_data::document_type& celeritas::properties_data::get_document_val() const
{
    return document_val_;
}

void celeritas::properties_data::set_document_val(document_type document_val)
{
    document_val_ = std::move(document_val);
}

const celeritas::traits::document_array_type& celeritas::properties_data::get_document_array_val() const
{
    return document_array_val_;
}

void celeritas::properties_data::set_document_array_val(traits::document_array_type document_array_val)
{
    document_array_val_ = std::move(document_array_val);
}

celeritas::properties_data::document_type celeritas::properties_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(expire_time_description, expire_time_);
    document.emplace_back(string_val_description, string_val_);
    document.emplace_back(string_array_val_description, string_array_val_);
    document.emplace_back(int32_val_description, int32_val_);
    document.emplace_back(int32_count_val_description, int32_count_val_);
    document.emplace_back(int32_array_val_description, int32_array_val_);
    document.emplace_back(int64_count_val_description, int64_count_val_);
    document.emplace_back(int64_array_val_description, int64_array_val_);
    document.emplace_back(double_val_description, double_val_);
    document.emplace_back(double_array_val_description, double_array_val_);
    document.emplace_back(bool_val_description, bool_val_);
    document.emplace_back(document_val_description, document_val_);
    document.emplace_back(document_array_val_description, document_array_val_);

    return document;
}

celeritas::properties_data celeritas::properties_data::from_document(const document_type& document)
{
    properties_data properties_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == expire_time_description)
        {
            properties_data.set_expire_time(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == string_val_description)
        {
            properties_data.set_string_val(element.get_value<database_data_type::string_type>());
        }
        else if (element.get_field_name() == string_array_val_description)
        {
            properties_data.set_string_array_val(element.get_value<database_data_type::string_array_type>());
        }
        else if (element.get_field_name() == int32_val_description)
        {
            if (element.has_value<database_data_type::int32_type>())
            {
                properties_data.set_int32_val(element.get_value<database_data_type::int32_type>());
            }
            else if (element.has_value<database_data_type::int64_type>())
            {
                properties_data.set_int32_val(boost::numeric_cast<int32_t>(element.get_value<database_data_type::int64_type>()));
            }
        }
        else if (element.get_field_name() == int32_count_val_description)
        {
            if (element.has_value<database_data_type::int32_type>())
            {
                properties_data.set_int32_count_val(element.get_value<database_data_type::int32_count_type>());
            }
            else if (element.has_value<database_data_type::int64_type>())
            {
                properties_data.set_int32_count_val(boost::numeric_cast<int32_t>(element.get_value<database_data_type::int64_count_type>()));
            }
        }
        else if (element.get_field_name() == int32_array_val_description)
        {
            if (element.has_value<database_data_type::int32_array_type>())
            {
                properties_data.set_int32_array_val(element.get_value<database_data_type::int32_array_type>());
            }
            else if (element.has_value<database_data_type::int64_array_type>())
            {
                const auto& array = element.get_value<database_data_type::int64_array_type>();
                std::vector<int32_t> int32_array{};

                for (const auto& int64_value : array)
                {
                    int32_array.emplace_back(boost::numeric_cast<int32_t>(int64_value));
                }
                properties_data.set_int32_array_val(int32_array);
            }
        }
        else if (element.get_field_name() == int64_count_val_description)
        {
            properties_data.set_int64_count_val(element.get_value<database_data_type::int64_count_type>());
        }
        else if (element.get_field_name() == int64_array_val_description)
        {
            properties_data.set_int64_array_val(element.get_value<database_data_type::int64_array_type>());
        }
        else if (element.get_field_name() == double_val_description)
        {
            properties_data.set_double_val(element.get_value<database_data_type::double_type>());
        }
        else if (element.get_field_name() == double_array_val_description)
        {
            properties_data.set_double_array_val(element.get_value<database_data_type::double_array_type>());
        }
        else if (element.get_field_name() == bool_val_description)
        {
            properties_data.set_bool_val(element.get_value<database_data_type::bool_type>());
        }
        else if (element.get_field_name() == document_val_description)
        {
            properties_data.set_document_val(element.get_value<database_data_type::document_type>());
        }
        else if (element.get_field_name() == document_array_val_description)
        {
            properties_data.set_document_array_val(element.get_value<database_data_type::document_array_type>());
        }
    }

    return properties_data;
}
