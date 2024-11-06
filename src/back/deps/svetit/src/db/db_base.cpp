#include <svetit/db/db_base.hpp>
#include <memory>

namespace svetit::db {

Base::Base(storages::postgres::ClusterPtr pg)
	: _db{std::move(pg)} {}

Base::Base(storages::postgres::ClusterPtr pg, storages::postgres::Transaction&& trx)
	: _db{std::move(pg)}
	, _trx{std::make_shared<storages::postgres::Transaction>(std::move(trx))} {}

Base::~Base()
{
	if (_trx)
		_trx->Rollback();
}

Base Base::WithTrx(const storages::postgres::TransactionOptions& opt)
{
	assert(!!_db);
	return Base{_db, _db->Begin(opt)};
}

void Base::Commit()
{
	if (!_trx)
	{
		LOG_ERROR() << "attempt to commit finished transaction";
		return;
	}

	_trx->Commit();
	_trx = nullptr;
}

void Base::Rollback()
{
	if (!_trx)
	{
		LOG_ERROR() << "attempt to rollback finished transaction";
		return;
	}

	_trx->Rollback();
	_trx = nullptr;
}

} // namespace svetit::db
