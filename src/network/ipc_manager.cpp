#include "ipc_manager.h"

celeritas::ipc_manager::ipc_manager(boost::asio::io_context& io_context, const std::string& socket_path, message_handler_type handler)
    : io_context_{ io_context }, acceptor_{ io_context, { socket_path } }, message_handler_{ std::move(handler) }
{
}

boost::asio::awaitable<void> celeritas::ipc_manager::accept_connections()
{
    try
    {
        while (true)
        {
            boost::asio::local::stream_protocol::socket socket = co_await acceptor_.async_accept(boost::asio::use_awaitable);
            // std::cout << "Accepted new IPC connection." << std::endl;
            // TODO: 为新连接创建一个 IpcSession，并启动
        }
    }
    catch (const boost::system::system_error& e)
    {
        // std::cerr << "IPC Acceptor error: " << e.what() << std::endl;
    }
}

boost::asio::awaitable<std::shared_ptr<celeritas::ipc_session>> celeritas::ipc_manager::connect_to(const std::string& remote_path)
{
    try
    {
        boost::asio::local::stream_protocol::socket socket(io_context_);
        co_await socket.async_connect({ remote_path }, boost::asio::use_awaitable);
        // std::cout << "Connected to IPC server: " << remote_path << std::endl;
        co_return std::make_shared<ipc_session>(std::move(socket), message_handler_);
    }
    catch (const boost::system::system_error& e)
    {
        // std::cerr << "IPC Connection error: " << e.what() << std::endl;
        co_return nullptr;
    }
}