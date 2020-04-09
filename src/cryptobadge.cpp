
#include "../include/cryptobadge.hpp"

const name governance_design = "governance"_n;
const name ghost_account = "ghost.can"_n;
const name ram_payer_system = "ram.can"_n;

ACTION cryptobadge::regissuer( name issuer, checksum256& data) {

	require_auth( issuer );

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	issuers _issuer(_self, _self.value);
	auto itr = _issuer.find( issuer.value );

	check (itr == _issuer.end(), "issuer already registered");

	_issuer.emplace( ram_payer, [&]( auto& s ) {     
		s.issuer = issuer;
		s.data = data;	
	});
}

ACTION cryptobadge::updateissuer( name issuer, checksum256& data) {
	require_auth( issuer );

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	issuers _issuer(_self, _self.value);
	auto itr = _issuer.find( issuer.value );

	check ( itr != _issuer.end(), "issuer not registered" );
	
	_issuer.modify( itr, ram_payer, [&]( auto& s ) {
		s.data = data;	
	});
}

ACTION cryptobadge::createbadge(name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria) {
	require_auth( issuer );
	require_auth( _self );

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;
	
	// TODO check if issuer is community account
	issuers _issuer(_self, _self.value);
	community_table _community(governance_design, governance_design.value);
	auto issuer_itr = _issuer.find( issuer.value );
	if(issuer_itr == _issuer.end()){
		auto community_itr = _community.find( issuer.value );
		check ( community_itr != _community.end(), "issuer does not exist" );
	}

	cbadges _badges( _self, issuer.value );
	auto badge_itr = _badges.find(badge_id);
	check ( badge_itr == _badges.end(), "badge_id already exist" );

	_badges.emplace( ram_payer, [&]( auto& s ) {
		s.badge_id = badge_id; 
		s.issuer = issuer;
	});
}

ACTION cryptobadge::updatebadge( name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria ) {

	require_auth( issuer );
	require_auth( _self );

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	cbadges _badges( _self, issuer.value );
	auto itr = _badges.find( badge_id );

	check (itr != _badges.end(), "badge does not exist");
	check (itr->issuer == issuer, "badge does not belong to issuer");

	_badges.modify( itr, ram_payer, [&]( auto& s ) {
		s.version = itr->version + 1;
	});
}

ACTION cryptobadge::issuebadge( name issuer, name owner, uint64_t badge_id, uint64_t badge_revision, uint64_t cert_id, string& encripted_data, uint64_t expire_at, bool require_claim) {

	require_auth( issuer );
	require_auth( _self );

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	check( is_account( owner ), "owner account does not exist");

	issuers _issuer(_self, _self.value);
	community_table _community(governance_design, governance_design.value);
	auto issuer_itr = _issuer.find( issuer.value );
	if(issuer_itr == _issuer.end()){
		auto community_itr = _community.find( issuer.value );
		check ( community_itr != _community.end(), "issuer does not exist" );
	}

	require_recipient( owner );
	
	cbadges _badges( _self, issuer.value );
	auto badge_itr = _badges.find( badge_id );

	check (badge_itr != _badges.end(), "badge does not exist");
	
	
	check (!(owner == ghost_account && require_claim == 1), "require_claim only owner == ghost account.");
	
	ccerts _certs(_self, owner.value);
	auto cert_itr = _certs.find(cert_id);
	check (cert_itr == _certs.end(), "cert already exist");

	_certs.emplace( ram_payer, [&]( auto& s ) {     
		s.id = cert_id;
		s.badge_id = badge_id;
		s.badge_revision = badge_revision;
		s.owner = owner;
		s.state = CertificationState::CERTIFIED;
		s.expire_at = expire_at;
	});
	
}

ACTION cryptobadge::expirecert( name updater, uint64_t cert_id, name owner ) {
	require_auth(updater);

	auto ram_payer = updater;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	ccerts _certs(_self, owner.value);
	auto cert_itr = _certs.find(cert_id);

	check (cert_itr != _certs.end(), "cert does not exist");

  	check (cert_itr->expire_at, "Certificate can not be expired");
	check (cert_itr->expire_at < current_time_point().sec_since_epoch(), "Certificate has not been expired");
	check (cert_itr->state == CertificationState::CERTIFIED, "Certificate has been revoked or expired");

	_certs.modify( cert_itr, ram_payer, [&]( auto& s ) {
		s.state = CertificationState::EXPIRED;
	});
}

ACTION cryptobadge::revokecert( name issuer, uint64_t cert_id, name owner, string reason ) {
	require_auth(issuer);
	require_auth( _self );

	ccerts _certs(_self, owner.value);
	auto cert_itr = _certs.find(cert_id);

	check (cert_itr != _certs.end(), "cert does not exist");

	cbadges _badges( _self, issuer.value );
	auto itr = _badges.find( cert_itr->badge_id );
	check (itr != _badges.end(), "certificate's issuer is invalid");
	check (itr->issuer == issuer, "badge does not belong to issuer");

	_certs.modify( cert_itr, issuer, [&]( auto& s ) {
		s.state = CertificationState::REVOKED;
	});
}


ACTION cryptobadge::createlog( name issuer, name owner, checksum256& idata, uint64_t cert_id, bool require_claim) {
	require_auth(get_self());
}


ACTION cryptobadge::claimcert( name claimer, name issuer, uint64_t cert_id) {
	require_auth( _self );
	require_auth( claimer );

 	ccerts certs_ghost(_self, ghost_account.value);
	auto itrc = certs_ghost.find( cert_id );
	check(itrc != certs_ghost.end(), "Cannot find the certs you're attempting to claim.");

	cbadges _badges( _self, issuer.value );
	auto badge_itr = _badges.find( itrc->badge_id );

	check (badge_itr != _badges.end(), "badge of certification index does not exist");

	auto ram_payer = claimer;
	if(has_auth(issuer)) ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	ccerts certs_claimer(_self, claimer.value);

	auto itr = certs_claimer.find( cert_id );
	check(itr == certs_claimer.end(), "the certs you're attempting to claim already exist.");
	certs_claimer.emplace( ram_payer, [&]( auto& row ) {     
		row.id = itrc->id;
		row.badge_id = itrc->badge_id;
		row.badge_revision = itrc->badge_revision;
		row.owner = itrc->owner;
		row.state = itrc->state;
		row.expire_at = itrc->expire_at;
	});
	certs_ghost.erase(itrc);
}

/*
* Increment, save and return id for a new certification.
*/
uint64_t cryptobadge::getid(uint64_t gindex){

	conf config(_self, _self.value);
	_cstate = config.exists() ? config.get() : global{};


	uint64_t resid;
	if (gindex == DEFER) {
		_cstate.defer_id++;
		resid = _cstate.defer_id;
	} else if(gindex == CERT) {
		_cstate.cert_id++;
		resid = _cstate.cert_id;
	} else if(gindex == BADGE){
		_cstate.badge_id++;
		resid = _cstate.badge_id;
	}

	config.set(_cstate, _self);
	return resid;
}


template<typename... Args>
void cryptobadge::sendEvent(name issuer, name rampayer, name seaction, const std::tuple<Args...> &adata) {

	transaction sevent{};
	sevent.actions.emplace_back( permission_level{_self, "active"_n}, issuer, seaction, adata);
	sevent.delay_sec = 0;
	sevent.send(getid(DEFER), rampayer);
}

#define EOSIO_ABI_CUSTOM(TYPE, MEMBERS)                                                       \
    extern "C"                                                                                \
    {                                                                                         \
        void apply(uint64_t receiver, uint64_t code, uint64_t action)                         \
        {                                                                                     \
            auto self = receiver;                                                             \
            if (code == self || code == "eosio.token"_n.value || action == "onerror"_n.value) \
            {                                                                                 \
                if (action == "transfer"_n.value)                                             \
                {                                                                             \
                    check(code == "eosio.token"_n.value, "Must transfer Token");              \
                }                                                                             \
                switch (action)                                                               \
                {                                                                             \
                    EOSIO_DISPATCH_HELPER(TYPE, MEMBERS)                                      \
                }                                                                             \
                /* does not allow destructor of thiscontract to run: eosio_exit(0); */        \
            }                                                                                 \
        }                                                                                     \
    }

EOSIO_ABI_CUSTOM( cryptobadge, (regissuer)(updateissuer)(createbadge)(updatebadge)(issuebadge)(createlog)(claimcert)(revokecert)(expirecert))
