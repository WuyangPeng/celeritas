#include "logs_data.h"
#include "common/core/celeritas_error.h"
#include "database/basic/basis_database.tpp"

#include <boost/numeric/conversion/cast.hpp>

celeritas::logs_data::logs_data()
    : int64_value_{ 0 },
      string_value_{},
      string_array_value_{},
      int32_value_{ 0 },
      int32_count_value_{ 0 },
      int32_array_value_{},
      int64_count_value_{ 0 },
      int64_array_value_{},
      double_value_{ 0.0 },
      double_array_value_{},
      bool_value_{ false },
      document_value_{},
      document_array_value_{}
{
}

int64_t celeritas::logs_data::get_int64_value() const
{
    return int64_value_;
}

void celeritas::logs_data::set_int64_value(const int64_t int64_value)
{
    int64_value_ = int64_value;
}

const std::string& celeritas::logs_data::get_string_value() const
{
    return string_value_;
}

void celeritas::logs_data::set_string_value(std::string string_value)
{
    string_value_ = std::move(string_value);
}

const celeritas::logs_data::string_array_type& celeritas::logs_data::get_string_array_value() const
{
    return string_array_value_;
}

void celeritas::logs_data::set_string_array_value(string_array_type string_array_value)
{
    string_array_value_ = std::move(string_array_value);
}

int32_t celeritas::logs_data::get_int32_value() const
{
    return int32_value_;
}

void celeritas::logs_data::set_int32_value(const int32_t int32_value)
{
    int32_value_ = int32_value;
}

int32_t celeritas::logs_data::get_int32_count_value() const
{
    return int32_count_value_;
}

void celeritas::logs_data::set_int32_count_value(const int32_t int32_count_value)
{
    int32_count_value_ = int32_count_value;
}

const celeritas::logs_data::int32_array_type& celeritas::logs_data::get_int32_array_value() const
{
    return int32_array_value_;
}

void celeritas::logs_data::set_int32_array_value(int32_array_type int32_array_value)
{
    int32_array_value_ = std::move(int32_array_value);
}

int64_t celeritas::logs_data::get_int64_count_value() const
{
    return int64_count_value_;
}

void celeritas::logs_data::set_int64_count_value(const int64_t int64_count_value)
{
    int64_count_value_ = int64_count_value;
}

const celeritas::logs_data::int64_array_type& celeritas::logs_data::get_int64_array_value() const
{
    return int64_array_value_;
}

void celeritas::logs_data::set_int64_array_value(int64_array_type int64_array_value)
{
    int64_array_value_ = std::move(int64_array_value);
}

double celeritas::logs_data::get_double_value() const
{
    return double_value_;
}

void celeritas::logs_data::set_double_value(const double double_value)
{
    double_value_ = double_value;
}

const celeritas::logs_data::double_array_type& celeritas::logs_data::get_double_array_value() const
{
    return double_array_value_;
}

void celeritas::logs_data::set_double_array_value(double_array_type double_array_value)
{
    double_array_value_ = std::move(double_array_value);
}

bool celeritas::logs_data::get_bool_value() const
{
    return bool_value_;
}

void celeritas::logs_data::set_bool_value(const bool bool_value)
{
    bool_value_ = bool_value;
}

const celeritas::logs_data::document_type& celeritas::logs_data::get_document_value() const
{
    return document_value_;
}

void celeritas::logs_data::set_document_value(document_type document_value)
{
    document_value_ = std::move(document_value);
}

const celeritas::logs_data::document_array_type& celeritas::logs_data::get_document_array_value() const
{
    return document_array_value_;
}

void celeritas::logs_data::set_document_array_value(document_array_type document_array_value)
{
    document_array_value_ = std::move(document_array_value);
}

celeritas::logs_data::document_type celeritas::logs_data::to_document_type() const
{
    document_type document{};

    document.emplace_back(int64_value_description, int64_value_);
    document.emplace_back(string_value_description, string_value_);
    document.emplace_back(string_array_value_description, string_array_value_);
    document.emplace_back(int32_value_description, int32_value_);
    document.emplace_back(int32_count_value_description, int32_count_value_);
    document.emplace_back(int32_array_value_description, int32_array_value_);
    document.emplace_back(int64_count_value_description, int64_count_value_);
    document.emplace_back(int64_array_value_description, int64_array_value_);
    document.emplace_back(double_value_description, double_value_);
    document.emplace_back(double_array_value_description, double_array_value_);
    document.emplace_back(bool_value_description, bool_value_);
    document.emplace_back(document_value_description, document_value_);
    document.emplace_back(document_array_value_description, document_array_value_);

    return document;
}

celeritas::logs_data celeritas::logs_data::from_document(const document_type& document)
{
    logs_data logs_data{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == int64_value_description)
        {
            logs_data.set_int64_value(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == string_value_description)
        {
            logs_data.set_string_value(element.get_value<database_data_type::string_type>());
        }
        else if (element.get_field_name() == string_array_value_description)
        {
            logs_data.set_string_array_value(element.get_value<database_data_type::string_array_type>());
        }
        else if (element.get_field_name() == int32_value_description)
        {
            if (element.has_value<database_data_type::int32_type>())
            {
                logs_data.set_int32_value(element.get_value<database_data_type::int32_type>());
            }
            else if (element.has_value<database_data_type::int64_type>())
            {
                logs_data.set_int32_value(boost::numeric_cast<int32_t>(element.get_value<database_data_type::int64_type>()));
            }
        }
        else if (element.get_field_name() == int32_count_value_description)
        {
            if (element.has_value<database_data_type::int32_type>())
            {
                logs_data.set_int32_count_value(element.get_value<database_data_type::int32_count_type>());
            }
            else if (element.has_value<database_data_type::int64_type>())
            {
                logs_data.set_int32_count_value(boost::numeric_cast<int32_t>(element.get_value<database_data_type::int64_count_type>()));
            }
        }
        else if (element.get_field_name() == int32_array_value_description)
        {
            if (element.has_value<database_data_type::int32_array_type>())
            {
                logs_data.set_int32_array_value(element.get_value<database_data_type::int32_array_type>());
            }
            else if (element.has_value<database_data_type::int64_array_type>())
            {
                const auto& array = element.get_value<database_data_type::int64_array_type>();
                int32_array_type int32_array{};

                for (const auto& int64_value : array)
                {
                    int32_array.emplace_back(boost::numeric_cast<int32_t>(int64_value));
                }
                logs_data.set_int32_array_value(int32_array);
            }
        }
        else if (element.get_field_name() == int64_count_value_description)
        {
            logs_data.set_int64_count_value(element.get_value<database_data_type::int64_count_type>());
        }
        else if (element.get_field_name() == int64_array_value_description)
        {
            logs_data.set_int64_array_value(element.get_value<database_data_type::int64_array_type>());
        }
        else if (element.get_field_name() == double_value_description)
        {
            logs_data.set_double_value(element.get_value<database_data_type::double_type>());
        }
        else if (element.get_field_name() == double_array_value_description)
        {
            logs_data.set_double_array_value(element.get_value<database_data_type::double_array_type>());
        }
        else if (element.get_field_name() == bool_value_description)
        {
            logs_data.set_bool_value(element.get_value<database_data_type::bool_type>());
        }
        else if (element.get_field_name() == document_value_description)
        {
            logs_data.set_document_value(element.get_value<database_data_type::document_type>());
        }
        else if (element.get_field_name() == document_array_value_description)
        {
            logs_data.set_document_array_value(element.get_value<database_data_type::document_array_type>());
        }
    }

    return logs_data;
}
