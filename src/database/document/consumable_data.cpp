#include "consumable_data.h"
#include "common/celeritas_error.h"

#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

celeritas::consumable_data::consumable_data()
    : expire_time_{ 0 }
{
}

celeritas::consumable_data::consumable_data(const document_view_type& document_view)
    : expire_time_{ 0 }
{
    set_document(document_view);
}

celeritas::consumable_data::consumable_data(const int64_t expire_time)
    : expire_time_{ expire_time }
{
}

int64_t celeritas::consumable_data::get_expire_time() const
{
    return expire_time_;
}

void celeritas::consumable_data::set_expire_time(const int64_t expire_time)
{
    expire_time_ = expire_time;
}

celeritas::consumable_data::document_type celeritas::consumable_data::to_document_type() const
{
    document_type builder{};
    builder.append(bsoncxx::builder::basic::kvp(std::string{ expire_time_description }, expire_time_));
    return builder;
}

void celeritas::consumable_data::set_document(const document_view_type& document_view)
{
    if (const auto expire_element = document_view[expire_time_description])
    {
        switch (expire_element.type())
        {
            case bsoncxx::type::k_int64:
            {
                expire_time_ = expire_element.get_int64().value;
                break;
            }
            case bsoncxx::type::k_int32:
            {
                expire_time_ = expire_element.get_int32().value;
                break;
            }
            default:
            {
                throw celeritas_error{ "expire time is not a valid type,type = {}", static_cast<int>(expire_element.type()) };
            }
        }
    }
}
