#pragma once

#include "database/basic/database_data_type_traits.h"

#include <cstdint>
#include <string>

namespace celeritas
{
    class log_header
    {
    public:
        using class_type = log_header;
        using document_type = traits::document_type;

        log_header();

        log_header(int64_t player_id, int64_t timestamp, std::string ip_address);

        [[nodiscard]] int64_t get_player_id() const;

        void set_player_id(int64_t player_id);

        [[nodiscard]] int64_t get_timestamp() const;

        void set_timestamp(int64_t timestamp);

        [[nodiscard]] std::string get_ip_address() const;

        void set_ip_address(const std::string& ip_address);

        [[nodiscard]] document_type to_document_type() const;

        [[nodiscard]] static log_header from_document(const document_type& document);

        static constexpr std::string_view player_id_description = "player_id";
        static constexpr std::string_view timestamp_description = "timestamp";
        static constexpr std::string_view ip_address_description = "ip_address";

    private:
        int64_t player_id_;
        int64_t timestamp_;
        std::string ip_address_;
    };
}
