#include "uuid.hpp"

#include <userver/utest/utest.hpp>
#include <userver/utest/using_namespace_userver.hpp>
#include <userver/formats/json/value_builder.hpp>

UTEST(UuidInJson, Basic) {
	const auto id = utils::generators::GenerateBoostUuid();
	const auto idJson = formats::json::ValueBuilder{id}.ExtractValue();
	const auto id2 = idJson.As<boost::uuids::uuid>();

	EXPECT_EQ(id, id2);
	EXPECT_EQ(utils::ToString(id), utils::ToString(id2));
}
