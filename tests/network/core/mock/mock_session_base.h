#pragma once

#include "network/core/session_base.h"

namespace celeritas
{
    class mock_session_base : public session_base
    {
    public:
        using class_type = mock_session_base;
        using base_type = session_base;
        using any_io_executor = boost::asio::any_io_executor;

        mock_session_base(any_io_executor executor, server_network_type server_network_type, int64_t session_id, session_callback session_callback);

        void start() override;

        [[nodiscard]] void_awaitable_type start_awaitable() override;

        [[nodiscard]] bool is_open() const override;

        [[nodiscard]] bool is_full() const override;

        void stop() override;

        [[nodiscard]] any_io_executor get_any_io_executor() override;

        [[nodiscard]] bool was_do_write_called() const;

        [[nodiscard]] bool was_do_write_immediately_called() const;

        [[nodiscard]] size_t get_last_write_size() const;

    private:
        void do_write(buffer_guard data) override;

        [[nodiscard]] void_awaitable_type do_write_immediately(buffer_guard data) override;

        boost::asio::any_io_executor executor_;
        bool do_write_called_ = false;
        bool do_write_immediately_called_ = false;
        size_t last_write_size_ = 0;
    };
}
