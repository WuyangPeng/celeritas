#include "uniqueness_fixture.h"
#include "common/common_constant.h"
#include "common/core/snowflake_generator.h"

#include <boost/numeric/conversion/cast.hpp>
#include <boost/test/unit_test.hpp>

void celeritas::uniqueness_fixture::init()
{
    for (auto index = 0; index < num_threads; ++index)
    {
        threads_.emplace_back(&class_type::generate_ids_task, this, index);
    }

    for (auto& thread : threads_)
    {
        thread.join();
    }
}

int celeritas::uniqueness_fixture::get_generated_ids_size() const
{
    return boost::numeric_cast<int>(generated_ids_.size());
}

void celeritas::uniqueness_fixture::generate_ids_task(const int index)
{
    for (auto i = 0; i < ids_per_thread; ++i)
    {
        const auto id = snowflake_generator::get_instance().generate(index % max_datacenter_id, index % max_worker_id);

        std::lock_guard lock{ mutex_ };
        BOOST_CHECK(!generated_ids_.contains(id));
        generated_ids_.emplace(id);
    }
}