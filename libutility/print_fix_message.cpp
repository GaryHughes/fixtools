#include "print_fix_message.hpp"
#include <algorithm>

namespace utility
{

size_t widest_field_name_in_base(const FIX8::MessageBase& base)
{
    const auto& metadata = base.get_metadata();

    size_t width = 0;

    for (const auto& position : base.get_positions())
    {
        const auto tbe = metadata.find_be(position.second->get_tag());
        width = std::max(width, std::strlen(tbe->_name));
    }

    return width;
}

size_t widest_field_name(const FIX8::Message& message)
{
    size_t width = widest_field_name_in_base(message);

    if (message.Header())
    {
        width = std::max(width, widest_field_name_in_base(*message.Header()));
    }

    if (message.Trailer())
    {
        width = std::max(width, widest_field_name_in_base(*message.Trailer()));
    }

    return width;
}

void print(const FIX8::MessageBase& base, size_t field_column_width, std::ostream& os)
{
    const auto& metadata = base.get_metadata();

    for (const auto& position : base.get_positions())
    {
        const auto tbe = metadata.find_be(position.second->get_tag());

        os << std::setw(field_column_width) << tbe->_name 
           << std::setw(8) << "(" + std::to_string(position.second->get_tag()) + ")"
           << " "
           << *position.second;

        int idx;
		
        if (position.second->get_realm() && (idx = (position.second->get_rlm_idx())) >= 0)
        {
			os << " - " << position.second->get_realm()->_descriptions[idx];
        }

        os << '\n';
    }
}

void print_fix_message(const FIX8::Message& message, std::ostream& os)
{
    const auto& metadata = message.get_metadata();

    auto field_column_width = widest_field_name(message);

    const auto msg_type = metadata._bme.find_ptr(message.get_msgtype().c_str());

    if (msg_type) 
    {
        os << msg_type->_name; 
    }
    else 
    {
        os << message.get_msgtype();
    }

    os << "\n{\n";

    if (message.Header()) 
    {
        print(*message.Header(), field_column_width, os);
    }

    print(message, field_column_width, os);

    if (message.Trailer()) 
    {
        print(*message.Trailer(), field_column_width, os);
    }

    os << "}";
}

}