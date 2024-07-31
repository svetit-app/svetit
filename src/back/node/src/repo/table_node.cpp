#include "table_node.hpp"
#include <memory>
#include <shared/errors.hpp>
#include <shared/paging.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid_io.hpp>

namespace svetit::node::table {

namespace pg = storages::postgres;
using pg::ClusterHostType;

Node::Node(std::shared_ptr<db::Base> dbPtr)
	: _db{std::move(dbPtr)}
{}

const pg::Query kSelect{
	"SELECT id, name, description, latitude, longitude, created_at FROM node.node WHERE id = $1",
	pg::Query::Name{"nodes.select"},
};

model::Node Node::Select(const boost::uuids::uuid& id) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelect, id);
	if (res.IsEmpty())
		throw errors::NotFound404{};

	return res.AsSingleRow<model::Node>(pg::kRowTag);
}

const pg::Query kInsert{
	"INSERT INTO node.node (name, description, latitude, longitude) "
	"VALUES ($1, $2, $3, $4) RETURNING id",
	pg::Query::Name{"node.insert"},
};

boost::uuids::uuid Node::Create(const model::Node& item) {
	const auto res = _db->Execute(ClusterHostType::kMaster, kInsert, item.name, item.description, item.latitude, item.longitude);
	return res.AsSingleRow<boost::uuids::uuid>();
}

const pg::Query kDelete {
	"DELETE FROM node.node WHERE id = $1",
	pg::Query::Name{"node.delete"},
};

void Node::Delete(const boost::uuids::uuid& id) {
	auto res = _db->Execute(ClusterHostType::kMaster, kDelete, id);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kUpdate {
	"UPDATE node.node SET name = $2, description = $3, latitude = $4, longitude = $5 "
	"WHERE id = $1",
	pg::Query::Name{"node.update"},
};

void Node::Update(const model::Node& item) {
	auto res = _db->Execute(ClusterHostType::kMaster, kUpdate, item.id, item.name, item.description, item.latitude, item.longitude);
	if (!res.RowsAffected())
		throw errors::NotFound404();
}

const pg::Query kSelectList{
	"SELECT id, name, description, latitude, longitude, created_at, COUNT(*) OVER() FROM node.node OFFSET $1 LIMIT $2",
	pg::Query::Name{"node.select_list"},
};

PagingResult<model::Node> Node::SelectList(int32_t start, int32_t limit) {
	auto res = _db->Execute(ClusterHostType::kSlave, kSelectList, start, limit);
	LOG_WARNING() << "123";
	PagingResult<model::Node> data;
	LOG_WARNING() << "234";
	data = res.AsContainer<decltype(data)::RawContainer>(pg::kRowTag);
	return data;
}

} // namespace svetit::space::table
