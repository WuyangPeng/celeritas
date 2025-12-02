#include "sdk_process_type.h"

std::string celeritas::get_sdk_process_type_description(const sdk_process_type sdk_process_type)
{
    switch (sdk_process_type)
    {
        case sdk_process_type::we_chat:
        {
            return "we_chat_";
        }
        default:
        {
            return "";
        }
    }
}