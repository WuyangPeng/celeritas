#include "chat_server.h"
#include "common/logger.h"
#include "../initializer/initializer_fwd.h"

#include <exception>

celeritas::chat_server::chat_server()
    : base_type{chat_type} {
}

int main(const int argc, char **argv) {
    try {
        const celeritas::chat_server server{};

        server.run(argc, argv);
    } catch (const std::exception &error) {
        LOG_CHANNEL(celeritas::default_channel, error) << "error:" << error.what();
        return 1;
    } catch (...) {
        LOG_CHANNEL(celeritas::default_channel, fatal) << "unknown error.";
        return 1;
    }

    return 0;
}