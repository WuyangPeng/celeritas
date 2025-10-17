#include "http_session.h"
#include "common/logger.h"
#include "detail/http_session_run.h"
#include "detail/http_session_write.h"

celeritas::http_session::http_session(socket_type socket,
                                      int64_t session_id,
                                      std::string game_server_id,
                                      session_callback session_callback)
    : base_type{ session_id, std::move(session_callback) },
      socket_{ std::move(socket) },
      http_run_{ std::make_shared<http_session_run>(socket_, session_id, get_session_callback()) },
      http_write_{ std::make_shared<http_session_write>(socket_) },
      game_server_id_{ std::move(game_server_id) }
{
}

void celeritas::http_session::start()
{
    http_run_->start(shared_from_this());
}

bool celeritas::http_session::is_open() const
{
    return socket_.is_open();
}

void celeritas::http_session::write(buffer_guard data)
{
    http_write_->write(std::move(data));
}
