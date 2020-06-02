
#include "../include/cryptobadge.hpp"

const name non_can_account = ""_n;

ACTION cryptobadge::regissuer( name issuer, checksum256& data) {

	require_auth( issuer );
	require_auth( _self );

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	auto ram_payer_system = g_state.ram_payer_account;

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	v1_issuers _issuer(_self, _self.value);
	auto itr = _issuer.find( issuer.value );

	check (itr == _issuer.end(), "issuer already registered");

	_issuer.emplace( ram_payer, [&]( auto& s ) {     
		s.issuer = issuer;
		s.data = data;	
	});
}

ACTION cryptobadge::updateissuer( name issuer, checksum256& data) {
	require_auth( issuer );
	require_auth( _self );

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	auto ram_payer_system = g_state.ram_payer_account;
	
	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	v1_issuers _issuer(_self, _self.value);
	auto itr = _issuer.find( issuer.value );

	check ( itr != _issuer.end(), "issuer not registered" );
	
	_issuer.modify( itr, ram_payer, [&]( auto& s ) {
		s.data = data;	
	});
}

ACTION cryptobadge::createpropos(name action, vector<char> packed_params, name proposal_name, time_point expire_at) {
	require_auth(_self);

	check(action == "createbadge"_n || action == "updatebadge"_n || action == "issuebadge"_n, "action is invalid");
	check(expire_at.sec_since_epoch() > current_time_point().sec_since_epoch(), "expire at must be greater than current time point");
	
	datastream packed_params_datastream(&packed_params[0], packed_params.size());
	name issuer;
	packed_params_datastream >> issuer;

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	v1_issuers _issuer(_self, _self.value);
	v1_community_table _community(g_state.governance_design, g_state.governance_design.value);
	
	auto issuer_itr = _issuer.find( issuer.value );
	if(issuer_itr == _issuer.end()) {
		auto community_itr = _community.find( issuer.value );
		check ( community_itr != _community.end(), "issuer does not exist" );
	}

	v1_proposals _proposal( _self, issuer.value );
	auto proposal_itr = _proposal.find(proposal_name.value);
	check ( proposal_itr == _proposal.end(), "proposal name already existed");

	_proposal.emplace( _self, [&]( auto& p ) {
		p.proposal_name = proposal_name;
		p.action = action;
		p.packed_params = packed_params;
		p.expire_at = expire_at;
	});
}

ACTION cryptobadge::approvepropo(name issuer, name approver, name proposal_name) {
	require_auth(approver);

	v1_proposals _proposal( _self, issuer.value );
	auto proposal_itr = _proposal.find(proposal_name.value);
	check ( proposal_itr != _proposal.end(), "proposal is not exist");

	check(proposal_itr->expire_at.sec_since_epoch() > current_time_point().sec_since_epoch(), "proposal has been expired");

	vector<name> new_approvers = proposal_itr->approvers;
	new_approvers.push_back(approver);

	_proposal.modify( proposal_itr, approver, [&]( auto& s ) {
		s.approvers = new_approvers;
	});
}

ACTION cryptobadge::executepropo(name issuer, name proposal_name) {
	require_auth(issuer);

	auto g_state = _global.exists() ? _global.get() : v1_global{};

	v1_proposals _proposal( _self, issuer.value );
	auto proposal_itr = _proposal.find(proposal_name.value);
	check ( proposal_itr != _proposal.end(), "proposal is not exist");

	check(proposal_itr->expire_at.sec_since_epoch() > current_time_point().sec_since_epoch(), "proposal has been expired");

	action sending_action;
  sending_action.authorization.push_back(permission_level{_self, "active"_n});
	if (find(proposal_itr->approvers.begin(), proposal_itr->approvers.end(), g_state.ram_payer_account) != proposal_itr->approvers.end()) {
		sending_action.authorization.push_back(permission_level{g_state.ram_payer_account, "active"_n});
	}
	sending_action.account = _self;
	sending_action.name = proposal_itr->action;
	sending_action.data = proposal_itr->packed_params;
	sending_action.send();

	_proposal.erase(proposal_itr);
}

ACTION cryptobadge::cancelpropos(name issuer, name proposal_name) {
	require_auth(_self);

	v1_proposals _proposal( _self, issuer.value );
	auto proposal_itr = _proposal.find(proposal_name.value);
	check ( proposal_itr != _proposal.end(), "proposal is not exist");

	check(proposal_itr->expire_at.sec_since_epoch() < current_time_point().sec_since_epoch(), "cannot cancel until expiration");

	_proposal.erase(proposal_itr);
};

ACTION cryptobadge::createbadge(name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria) {
	// require_auth( issuer );
	require_auth( _self );

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	auto ram_payer_system = g_state.ram_payer_account;

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	v1_issuers _issuer(_self, _self.value);
	v1_community_table _community(g_state.governance_design, g_state.governance_design.value);
	
	auto issuer_itr = _issuer.find( issuer.value );
	if(issuer_itr == _issuer.end()){
		auto community_itr = _community.find( issuer.value );
		check ( community_itr != _community.end(), "issuer does not exist" );
	}

	// TODO check if issuer is community account

	v1_badges _badges( _self, issuer.value );
	auto badge_itr = _badges.find(badge_id);
	check ( badge_itr == _badges.end(), "badge_id already exist" );

	_badges.emplace( ram_payer, [&]( auto& s ) {
		s.badge_id = badge_id; 
		s.issuer = issuer;
	});
}

ACTION cryptobadge::updatebadge( name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria ) {
	// require_auth( issuer );
	require_auth( _self );

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	auto ram_payer_system = g_state.ram_payer_account;

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	v1_badges _badges( _self, issuer.value );
	auto itr = _badges.find( badge_id );

	check (itr != _badges.end(), "badge does not exist");
	check (itr->issuer == issuer, "badge does not belong to issuer");

	_badges.modify( itr, ram_payer, [&]( auto& s ) {
		s.version = itr->version + 1;
	});
}

ACTION cryptobadge::issuebadge( name issuer, name owner, uint64_t badge_id, uint64_t badge_revision, uint64_t cert_id, string& encrypted_data, uint64_t expire_at, bool require_claim) {
	// require_auth( issuer );
	require_auth( _self );

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	auto ram_payer_system = g_state.ram_payer_account;

	auto ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	v1_issuers _issuer(_self, _self.value);
	v1_community_table _community(g_state.governance_design, g_state.governance_design.value);

	auto issuer_itr = _issuer.find( issuer.value );
	if(issuer_itr == _issuer.end()) {
		auto community_itr = _community.find( issuer.value );
		check ( community_itr != _community.end(), "issuer does not exist" );
	}
	
	v1_badges _badges( _self, issuer.value );
	auto badge_itr = _badges.find( badge_id );

	check (badge_itr != _badges.end(), "badge does not exist");
	
	check ((is_account( owner ) && require_claim == 0) || (owner == non_can_account && require_claim == 1), "require_claim only for non CAN account.");
	
	if(require_claim){
		// Non CAN account
		v1_issuing_certs _issuing_certs(_self, issuer.value);
		auto issuing_cert_itr = _issuing_certs.find(cert_id);
		check (issuing_cert_itr == _issuing_certs.end(), "cert already exist");

		_issuing_certs.emplace( ram_payer, [&]( auto& row ) {     
			row.id = cert_id;
			row.badge_id = badge_id;
			row.badge_revision = badge_revision;
			row.owner = owner;
			row.state = CertificationState::CERTIFIED;
			row.expire_at = expire_at;
			row.issued_tx_id = gettrxid();

		});		
	}else{
		// CAN account
		require_recipient( owner );
	
		v1_certs _certs(_self, owner.value);
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
	
}

ACTION cryptobadge::expirecert( name updater, uint64_t cert_id, name owner ) {
	require_auth(updater);

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	auto ram_payer_system = g_state.ram_payer_account;

	auto ram_payer = updater;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	v1_certs _certs(_self, owner.value);
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

	v1_certs _certs(_self, owner.value);
	auto cert_itr = _certs.find(cert_id);

	check (cert_itr != _certs.end(), "cert does not exist");

	v1_badges _badges( _self, issuer.value );
	auto itr = _badges.find( cert_itr->badge_id );
	check (itr != _badges.end(), "certificate's issuer is invalid");
	check (itr->issuer == issuer, "badge does not belong to issuer");

	_certs.modify( cert_itr, issuer, [&]( auto& s ) {
		s.state = CertificationState::REVOKED;
	});
}

ACTION cryptobadge::issuelog( name issuer, name owner, uint64_t cert_id, const checksum256 & issued_tx_id) {
	require_auth(get_self());
	require_recipient( owner );
}


ACTION cryptobadge::claimbadge( name claimer, name issuer, uint64_t cert_id) {
	require_auth( _self );
	require_auth( claimer );

	auto g_state = _global.exists() ? _global.get() : v1_global{};
	auto ram_payer_system = g_state.ram_payer_account;

 	v1_issuing_certs _issuing_certs(_self, issuer.value);
	auto issuing_cert_itr = _issuing_certs.find( cert_id );
	check(issuing_cert_itr != _issuing_certs.end(), "Cannot find the certs you're attempting to claim.");

	v1_badges _badges( _self, issuer.value );
	auto badge_itr = _badges.find( issuing_cert_itr->badge_id );

	check (badge_itr != _badges.end(), "badge of certification index does not exist");

	auto ram_payer = claimer;
	if(has_auth(issuer)) ram_payer = issuer;
	if(has_auth(ram_payer_system)) ram_payer = ram_payer_system;

	v1_certs _certs(_self, claimer.value);

	auto itr = _certs.find( cert_id );
	check(itr == _certs.end(), "the certs you're attempting to claim already exist.");
	_certs.emplace( ram_payer, [&]( auto& row ) {     
		row.id = issuing_cert_itr->id;
		row.badge_id = issuing_cert_itr->badge_id;
		row.badge_revision = issuing_cert_itr->badge_revision;
		row.owner = issuing_cert_itr->owner;
		row.state = issuing_cert_itr->state;
		row.expire_at = issuing_cert_itr->expire_at;
	});
	eosio::action(
        permission_level{_self, "active"_n},
        get_self(), "issuelog"_n,
        std::make_tuple(issuer, claimer, cert_id, issuing_cert_itr->issued_tx_id))
    .send();

	_issuing_certs.erase(issuing_cert_itr);
}

ACTION cryptobadge::setconfig(name ram_payer_account, name governance_design) {
	require_auth( _self );

  if (!_global.exists())
  {
    v1_global default_config;
    default_config.ram_payer_account = "ram.can"_n;
    default_config.governance_design = "governance"_n;
    _global.get_or_create(_self, default_config);
  }
  auto g_config = _global.get();
  g_config.ram_payer_account = ram_payer_account;
  g_config.governance_design = governance_design;
  _global.set(g_config, _self);

}


checksum256 cryptobadge::gettrxid() {
    size_t size = transaction_size();
    char buf[size];
    size_t read = read_transaction( buf, size );
    check( size == read, "read_transaction failed");
    return sha256( buf, read );
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

EOSIO_ABI_CUSTOM(
	cryptobadge, 
	(setconfig)
	(regissuer)
	(updateissuer)
	(createpropos)
	(approvepropo)
	(executepropo)
	(cancelpropos)
	(createbadge)
	(updatebadge)
	(issuebadge)
	(issuelog)
	(claimbadge)
	(revokecert)
	(expirecert)
)
