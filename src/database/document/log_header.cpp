#include "log_header.h"
#include "database/basic/basis_database.tpp"

celeritas::log_header::log_header()
    : player_id_{}, timestamp_{}, ip_address_{}
{
}

celeritas::log_header::log_header(const int64_t player_id, const int64_t timestamp, std::string ip_address)
    : player_id_{ player_id }, timestamp_{ timestamp }, ip_address_{ std::move(ip_address) }
{
}

int64_t celeritas::log_header::get_player_id() const
{
    return player_id_;
}

void celeritas::log_header::set_player_id(const int64_t player_id)
{
    player_id_ = player_id;
}

int64_t celeritas::log_header::get_timestamp() const
{
    return timestamp_;
}

void celeritas::log_header::set_timestamp(const int64_t timestamp)
{
    timestamp_ = timestamp;
}

std::string celeritas::log_header::get_ip_address() const
{
    return ip_address_;
}

void celeritas::log_header::set_ip_address(const std::string& ip_address)
{
    ip_address_ = ip_address;
}

celeritas::log_header::document_type celeritas::log_header::to_document_type() const
{
    document_type document{};

    document.emplace_back(player_id_description, player_id_);
    document.emplace_back(timestamp_description, timestamp_);
    document.emplace_back(ip_address_description, ip_address_);

    return document;
}

celeritas::log_header celeritas::log_header::from_document(const document_type& document)
{
    log_header header{};

    for (const auto& element : document)
    {
        if (element.get_field_name() == player_id_description)
        {
            header.set_player_id(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == timestamp_description)
        {
            header.set_timestamp(element.get_value<database_data_type::int64_type>());
        }
        else if (element.get_field_name() == ip_address_description)
        {
            header.set_ip_address(element.get_value<database_data_type::string_type>());
        }
    }

    return header;
}
