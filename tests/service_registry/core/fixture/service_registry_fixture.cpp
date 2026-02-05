#include "service_registry_fixture.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "service_registry/core/service_registry.h"

celeritas::service_registry_fixture::~service_registry_fixture()
{
    noexcept_safe_call_and_log([this] {
                                   this->clear_services();
                               },
                               default_channel,
                               "clear services error: ");
}

void celeritas::service_registry_fixture::clear_services()
{
    service_registry::clear_services("test_service");
    service_registry::clear_services("idle_service_test");
    service_registry::clear_services("health_test_service");
}

