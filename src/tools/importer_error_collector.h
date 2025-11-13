#pragma once

#include "google/protobuf/compiler/importer.h"

namespace celeritas
{
    class importer_error_collector final : public google::protobuf::compiler::MultiFileErrorCollector
    {
    public:
        using class_type = importer_error_collector;
        using base_type = MultiFileErrorCollector;

        void RecordError(absl::string_view filename, int line, int column, absl::string_view message) override;

        void RecordWarning(absl::string_view filename, int line, int column, absl::string_view message) override;
    };
}

