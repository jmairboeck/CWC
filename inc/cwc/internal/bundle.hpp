
//          Copyright Michael Florian Hava 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <map>
#include "../cwc.hpp"
#include "error_handling.hpp"

static
::cwc::intrusive_ptr<::cwc::context> cwc_context;

namespace cwc {
	namespace internal {
		auto registered_factories() noexcept -> std::map<string_ref, intrusive_ptr<component>> & {
			static std::map<string_ref, intrusive_ptr<component>> factories;
			return factories;
		}
	}

	auto this_context() -> intrusive_ptr<context> { return cwc_context; }
}

extern "C" CWC_EXPORT void CWC_CALL cwc_init(::cwc::intrusive_ptr<::cwc::context> context) {
	assert(context);
	assert(!cwc_context);
	cwc_context = context;
}

extern "C" CWC_EXPORT auto CWC_CALL cwc_factory(const ::cwc::string_ref * fqn, cwc::intrusive_ptr<cwc::component> * result) -> const ::cwc::internal::error * {
	assert(fqn);
	assert(result);
	assert(cwc_context);
	return ::cwc::internal::call_and_return_error([&] { *result = cwc::internal::registered_factories().at(*fqn); });
}

extern "C" CWC_EXPORT void CWC_CALL cwc_reflect(::cwc::string_ref * definition) {
	assert(definition);
	*definition = cwc_definition;
}

extern "C" CWC_EXPORT void CWC_CALL cwc_exports(::cwc::array_ref<const ::cwc::string_ref> * exports) {
	assert(exports);
	static const auto data{[] {
		std::vector<cwc::string_ref> result;
		for(const auto & factory : cwc::internal::registered_factories()) result.push_back(factory.first);
		return result;
	}()};
	*exports = data;
}
