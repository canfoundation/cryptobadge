
#include "../include/cryptobadge.hpp"

const uint64_t BADGE_SCHEMA_V1 = 1000;
const name GOVERNANCE_DESIGN = "governance"_n;

ACTION cryptobadge::regissuer( name issuer, checksum256& data) {

	require_auth( issuer );
	require_recipient( issuer );
	
	issuers _issuer(_self, _self.value);
	auto itr = _issuer.find( issuer.value );

	check (itr == _issuer.end(), "issuer already registered");

	_issuer.emplace( issuer, [&]( auto& s ) {     
		s.issuer = issuer;
		s.data = data;	
	});
}

ACTION cryptobadge::updateissuer( name issuer, checksum256& data) {
	require_auth( issuer );
	require_recipient( issuer );

	issuers _issuer(_self, _self.value);
	auto itr = _issuer.find( issuer.value );

	check ( itr != _issuer.end(), "issuer not registered" );
	
	_issuer.modify( itr, issuer, [&]( auto& s ) {
		s.data = data;	
	});
}

ACTION cryptobadge::createbadge(name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria) {
	require_auth( issuer );
	require_auth( _self );
	
	// TODO check if issuer is community account
	issuers _issuer(_self, _self.value);
	community_table _community(GOVERNANCE_DESIGN, GOVERNANCE_DESIGN.value);
	auto issuer_itr = _issuer.find( issuer.value );
	if(issuer_itr == _issuer.end()){
		auto community_itr = _community.find( issuer.value );
		check ( community_itr != _community.end(), "issuer does not exist" );
	}

	cbadges _badges( _self, issuer.value );
	auto badge_itr = _badges.find(badge_id);
	check ( badge_itr == _badges.end(), "badge_id already exist" );

	_badges.emplace( issuer, [&]( auto& s ) {
		s.badge_id = badge_id; 
		s.issuer = issuer;
	});
}

ACTION cryptobadge::updatebadge( name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria ) {

	require_auth( issuer );
	require_auth( _self );

	cbadges _badges( _self, issuer.value );
	auto itr = _badges.find( badge_id );

	check (itr != _badges.end(), "badge does not exist");
	check (itr->issuer == issuer, "badge does not belong to issuer");

	_badges.modify( itr, issuer, [&]( auto& s ) {
		s.version = itr->version + 1;
	});
}

ACTION cryptobadge::issuebadge( name issuer, name owner, uint64_t badge_id, uint64_t badge_revision, uint64_t cert_id, string& encripted_data, uint64_t expire_at, bool require_claim) {

	require_auth( issuer );
	require_auth( _self );

	check( is_account( owner ), "owner account does not exist");

	issuers _issuer(_self, _self.value);
	community_table _community(GOVERNANCE_DESIGN, GOVERNANCE_DESIGN.value);
	auto issuer_itr = _issuer.find( issuer.value );
	if(issuer_itr == _issuer.end()){
		auto community_itr = _community.find( issuer.value );
		check ( community_itr != _community.end(), "issuer does not exist" );
	}

	require_recipient( owner );
	
	cbadges _badges( _self, issuer.value );
	auto badge_itr = _badges.find( badge_id );

	check (badge_itr != _badges.end(), "badge does not exist");
	
	
	name certOwner = owner;
	
	check (!(issuer.value == owner.value && require_claim == 1), "require_claim only issuer == owner.");
	
	if (require_claim){
		certOwner = issuer;
		//add info to offers table
		offers _offert(_self, _self.value);
		auto offer_itr = _offert.find(cert_id);
		check (offer_itr == _offert.end(), "offer already exist");

		_offert.emplace( issuer, [&]( auto& s ) {     
			s.cert_id = cert_id;
			s.offered_to = owner;
			s.owner = issuer;
			s.date = current_time_point();
		});
	}
	
	ccerts _certs(_self, certOwner.value);
	auto cert_itr = _certs.find(cert_id);
	check (cert_itr == _certs.end(), "cert already exist");

	_certs.emplace( issuer, [&]( auto& s ) {     
		s.id = cert_id;
		s.badge_id = badge_id;
		s.badge_revision = badge_revision;
		s.owner = owner;
		s.state = CertificationState::CERTIFIED;
		s.expire_at = expire_at;
		s.encripted_data = encripted_data;
	});
	
}

ACTION cryptobadge::expirecert( name updater, uint64_t cert_id, name owner ) {
	require_auth(updater);

	ccerts _certs(_self, owner.value);
	auto cert_itr = _certs.find(cert_id);

	check (cert_itr != _certs.end(), "cert does not exist");

  check (cert_itr->expire_at, "Certificate can not be expired");
	check (cert_itr->expire_at < current_time_point().sec_since_epoch(), "Certificate has not been expired");
	check (cert_itr->state == CertificationState::CERTIFIED, "Certificate has been revoked or expired");

	_certs.modify( cert_itr, updater, [&]( auto& s ) {
		s.state = CertificationState::EXPIRED;
	});
}

ACTION cryptobadge::revokecert( name issuer, uint64_t cert_id, name owner, string reason ) {
	require_auth(issuer);

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


ACTION cryptobadge::claimcert( name claimer, std::vector<uint64_t>& cert_ids) {
	require_auth( claimer );
	require_recipient( claimer );
	
	offers offert(_self, _self.value);
	ccerts certs_t(_self, claimer.value);
	
	std::map< name, std::map< uint64_t, name > > uniqissuer;
	for( size_t i = 0; i < cert_ids.size(); ++i ) {

		auto itrc = offert.find( cert_ids[i] );

		check(itrc != offert.end(), "Cannot find at least one of the certs you're attempting to claim.");
		check(claimer == itrc->offered_to, "At least one of the certs has not been offerred to you.");

		ccerts certs_f( _self, itrc->owner.value );
		auto itr = certs_f.find( cert_ids[i] );
		check(itr != certs_f.end(), "Cannot find at least one of the certs you're attempting to claim.");

		check(itrc->owner.value == itr->owner.value, "Owner was changed for at least one of the items!?");   

		certs_t.emplace( claimer, [&]( auto& s ) {     
			s.id = itr->id;
			s.owner = claimer;
			s.encripted_data = itr->encripted_data; 		// immutable data
		});

		certs_f.erase(itr);
		offert.erase(itrc);

	}

}


ACTION cryptobadge::canceloffer( name issuer, std::vector<uint64_t>& cert_ids){

	require_auth( issuer );
	
	offers offert(_self, _self.value);

	for( size_t i = 0; i < cert_ids.size(); ++i ) {
		auto itr = offert.find( cert_ids[i] );

		check ( itr != offert.end(), "The offer for at least one of the certs was not found." );
		check (issuer.value == itr->owner.value, "You're not the owner of at least one of the certs whose offers you're attempting to cancel.");

		offert.erase(itr);
	}
}


ACTION cryptobadge::removecert( name owner, std::vector<uint64_t>& cert_ids, string memo ) {

	require_auth( owner );	

	ccerts certs_f( _self, owner.value );
	offers offert(_self, _self.value);
		
	std::map< name, std::vector<uint64_t> > uniqissuer;
	
	for( size_t i = 0; i < cert_ids.size(); ++i ) {
		
		auto itr = certs_f.find( cert_ids[i] );
		check(itr != certs_f.end(), "At least one of the certs was not found.");

		check(owner.value == itr->owner.value, "At least one of the certs you're attempting to burn is not yours.");

		auto itrc = offert.find( cert_ids[i] );
		check ( itrc == offert.end(), "At least one of the certs has an open offer and cannot be burned." );
		
		certs_f.erase(itr);
		
	}
	
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


EOSIO_DISPATCH( cryptobadge, (regissuer)(updateissuer)(createbadge)(updatebadge)(issuebadge)(createlog)(removecert)(canceloffer)(claimcert)(revokecert)(expirecert))
