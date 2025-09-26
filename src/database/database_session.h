#pragma once


namespace celeritas
{
    class database_session
    {
    public:
        using class_type = database_session;

        database_session() noexcept = default;

        virtual ~database_session() noexcept = default;

        database_session(const database_session& rhs) noexcept = default;

        database_session& operator=(const database_session& rhs) noexcept = default;

        database_session(database_session&& rhs) noexcept = default;

        database_session& operator=(database_session&& rhs) noexcept = default;
    };
}
