/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include <cryptobadge.hpp>


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

ACTION cryptobadge::createbadge( name issuer, name owner, checksum256& idata, checksum256& mdata) {
	require_auth( issuer );
	
	issuers _issuer(_self, _self.value);
	auto itr = _issuer.find( issuer.value );
	
	check ( itr != _issuer.end(), "issuer does not exist" );
	
	check( is_account( owner ), "owner account does not exist");

	require_recipient( owner );
	
	cbadges badges_( _self, owner.value );

	uint64_t newID = getid(BADGE);
	badges_.emplace( issuer, [&]( auto& s ) { 
		s.badgeid = newID; 
		s.owner = owner;   
		s.issuer = issuer;
		s.idata = idata;
		s.mdata = mdata;	
	});
}

ACTION cryptobadge::updatebadge( name issuer, name owner, uint64_t badgeid, checksum256& mdata ) {

	require_auth( issuer );
	check( is_account( owner ), "owner account does not exist");

	require_recipient( owner );
	
	cbadges _badges( _self, owner.value );
	auto itr = _badges.find( badgeid );

	check (itr != _badges.end(), "badge does not exist");
	
	check (itr->issuer == issuer, "badge does not belong to issuer");

	_badges.modify( itr, issuer, [&]( auto& s ) {
		s.mdata = mdata;
	});
}

ACTION cryptobadge::createcert( name issuer, name owner, uint64_t badgeid, checksum256& idata, bool requireclaim) {

	require_auth( issuer );
	check( is_account( owner ), "owner account does not exist");

	require_recipient( owner );
	
	cbadges _badges( _self, issuer.value );
	auto itr = _badges.find( badgeid );

	check (itr != _badges.end(), "badge does not exist");
	
	check (itr->owner == issuer, "badge does not belong to issuer");
	
	uint64_t newID = getid(CERT);
	
	name certOwner = owner;
	
	check (!(issuer.value == owner.value && requireclaim == 1), "requireclaim only issuer == owner.");
	
	if (requireclaim){
		certOwner = issuer;
		//add info to offers table
		offers offert(_self, _self.value);
		offert.emplace( issuer, [&]( auto& s ) {     
			s.certid = newID;
			s.offeredto = owner;
			s.owner = issuer;
			s.cdate = current_time_point();
		});
	}
	
	ccerts certs(_self, certOwner.value);
	certs.emplace( issuer, [&]( auto& s ) {     
		s.id = newID;
		s.owner = certOwner;
		s.issuer = issuer;
		s.badgeid = badgeid;
		s.idata = idata; // immutable data
	});
	
	//Events
	sendEvent(issuer, issuer, "cbcreate"_n, std::make_tuple(owner, newID));
	SEND_INLINE_ACTION( *this, createlog, { {_self, "active"_n} },  { issuer, owner, idata, newID, requireclaim} );
}


ACTION cryptobadge::createlog( name issuer, name owner, checksum256& idata, uint64_t certid, bool requireclaim) {
	require_auth(get_self());
}


ACTION cryptobadge::claimcert( name claimer, std::vector<uint64_t>& certids) {
	require_auth( claimer );
	require_recipient( claimer );
	
	offers offert(_self, _self.value);
	ccerts certs_t(_self, claimer.value);
	
	std::map< name, std::map< uint64_t, name > > uniqissuer;
	for( size_t i = 0; i < certids.size(); ++i ) {

		auto itrc = offert.find( certids[i] );

		check(itrc != offert.end(), "Cannot find at least one of the certs you're attempting to claim.");
		check(claimer == itrc->offeredto, "At least one of the certs has not been offerred to you.");

		ccerts certs_f( _self, itrc->owner.value );
		auto itr = certs_f.find( certids[i] );
		check(itr != certs_f.end(), "Cannot find at least one of the certs you're attempting to claim.");

		check(itrc->owner.value == itr->owner.value, "Owner was changed for at least one of the items!?");   

		certs_t.emplace( claimer, [&]( auto& s ) {     
			s.id = itr->id;
			s.owner = claimer;
			s.issuer = itr->issuer;
			s.idata = itr->idata; 		// immutable data
		});

		certs_f.erase(itr);
		offert.erase(itrc);

		//Events
		uniqissuer[itr->issuer][certids[i]] = itrc->owner;
	}

	//Send Event as deferred	
	auto uniqissuerIt = uniqissuer.begin(); 
	while(uniqissuerIt != uniqissuer.end() ) {
		name keyissuer = (*uniqissuerIt).first; 
		sendEvent(keyissuer, claimer, "cbclaim"_n, std::make_tuple(claimer, uniqissuer[keyissuer]));
		uniqissuerIt++;
	}
}


ACTION cryptobadge::canceloffer( name issuer, std::vector<uint64_t>& certids){

	require_auth( issuer );
	
	offers offert(_self, _self.value);

	for( size_t i = 0; i < certids.size(); ++i ) {
		auto itr = offert.find( certids[i] );

		check ( itr != offert.end(), "The offer for at least one of the certs was not found." );
		check (issuer.value == itr->owner.value, "You're not the owner of at least one of the certs whose offers you're attempting to cancel.");

		offert.erase(itr);
	}
}


ACTION cryptobadge::removecert( name owner, std::vector<uint64_t>& certids, string memo ) {

	require_auth( owner );	

	ccerts certs_f( _self, owner.value );
	offers offert(_self, _self.value);
		
	std::map< name, std::vector<uint64_t> > uniqissuer;
	
	for( size_t i = 0; i < certids.size(); ++i ) {
		
		auto itr = certs_f.find( certids[i] );
		check(itr != certs_f.end(), "At least one of the certs was not found.");

		check(owner.value == itr->owner.value, "At least one of the certs you're attempting to burn is not yours.");

		auto itrc = offert.find( certids[i] );
		check ( itrc == offert.end(), "At least one of the certs has an open offer and cannot be burned." );
		
		certs_f.erase(itr);
		
		//Events
		uniqissuer[itr->issuer].push_back(certids[i]);
	}
	
	//Send Event as deferred
	auto uniqissuerIt = uniqissuer.begin(); 
	while(uniqissuerIt != uniqissuer.end() ) {
		name keyissuer = (*uniqissuerIt).first; 
		sendEvent(keyissuer, owner, "cbrevoke"_n, std::make_tuple(owner, uniqissuer[keyissuer], memo));
		uniqissuerIt++;
	}
}

ACTION cryptobadge::attach( name owner, uint64_t certid, string data) {
  

	require_auth( owner );
	
	ccerts certs_f( _self, owner.value );

	auto itr = certs_f.find( certid );
	check(itr != certs_f.end(), "certid cannot be found");

	check(owner.value == itr->owner.value, "the certs does not belong to owner"); 

	checksum256 hashed;
	hashed = sha256((char *)&data, data.length());
	//Todo: check attached data match with certification data
	//check(compareHash(itr->idata, hashed), "the hashed data not the same with current one"); 

	ccertinfos _ccertinfos(_self, owner.value);
	auto ci_itr = _ccertinfos.find( certid );

	check (ci_itr == _ccertinfos.end(), "certid already attach");

	_ccertinfos.emplace( owner, [&]( auto& s ) {  
	  s.cert_id = itr->id;
		s.cert_content = data;
	});
	
}


ACTION cryptobadge::detach( name owner, std::vector<uint64_t>& certids) {

	require_auth( owner );	

	ccerts certs_f( _self, owner.value );
	ccertinfos _ccertinfos(_self, owner.value);
		
	std::map< name, std::vector<uint64_t> > uniqissuer;
	
	for( size_t i = 0; i < certids.size(); ++i ) {
		
		auto itr = certs_f.find( certids[i] );
		check(itr != certs_f.end(), "At least one of the certs was not found.");

		check(owner.value == itr->owner.value, "At least one of the certs you're attempting to burn is not yours.");

		auto itrc = _ccertinfos.find( certids[i] );
		check ( itrc == _ccertinfos.end(), "At least one of the certs has no attached info" );
		
		_ccertinfos.erase(itrc);
		
		//Events
		uniqissuer[itr->issuer].push_back(certids[i]);
	}
	
	//Send Event as deferred
	auto uniqissuerIt = uniqissuer.begin(); 
	while(uniqissuerIt != uniqissuer.end() ) {
		name keyissuer = (*uniqissuerIt).first; 
		sendEvent(keyissuer, owner, "cbdettach"_n, std::make_tuple(owner, uniqissuer[keyissuer]));
		uniqissuerIt++;
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
		_cstate.deferid++;
		resid = _cstate.deferid;
	} else if(gindex == CERT) {
		_cstate.certid++;
		resid = _cstate.certid;
	} else if(gindex == BADGE){
		_cstate.badgeid++;
		resid = _cstate.badgeid;
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

bool cryptobadge::compareHash(const checksum256& current_hash, const checksum256&  input_hash) {

        const uint64_t *current64 = reinterpret_cast<const uint64_t *>(&current_hash);
		const uint64_t *input64 = reinterpret_cast<const uint64_t *>(&input_hash);

		for(int i = 0 ; i < 4; ++i){
            if(current64[i] != input64[i]) return false;
        }
        return true;
}

EOSIO_DISPATCH( cryptobadge, (regissuer)(updateissuer)(createbadge)(updatebadge)(createcert)(createlog)(removecert)(canceloffer)(claimcert)(attach)(detach))
