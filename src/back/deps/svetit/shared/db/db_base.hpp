#pragma once

#include "userver/storages/postgres/cluster_types.hpp"
#include <userver/storages/postgres/transaction.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit::db {

struct Base {
	Base(storages::postgres::ClusterPtr pg);
	Base(storages::postgres::ClusterPtr pg, storages::postgres::Transaction&& trx);
	Base(Base&&) = default;
	Base(const Base& o) = default;
	~Base();

	Base WithTrx(const storages::postgres::TransactionOptions& opt = storages::postgres::Transaction::RW);
	void Commit();
	void Rollback();

	template <typename... Args>
	storages::postgres::ResultSet Execute(storages::postgres::ClusterHostType flags, const storages::postgres::Query& query, const Args&... args) {
		assert(!!_db || !!_trx);
		if (_trx)
			return _trx->Execute(query, args...);
		return _db->Execute(flags, query, args...);
	}

	template <typename... Args>
	storages::postgres::ResultSet Execute(const storages::postgres::Query& query, const Args&... args) {
		return Execute(storages::postgres::ClusterHostType::kMaster, query, args...);
	}

private:
	storages::postgres::ClusterPtr _db;
	std::shared_ptr<storages::postgres::Transaction> _trx;
};

} // namespace svetit::db
