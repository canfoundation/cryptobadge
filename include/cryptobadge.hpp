
#include <eosio/singleton.hpp>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/time.hpp>
#include <eosio/symbol.hpp>
#include <eosio/transaction.hpp>
#include <eosio/crypto.hpp>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace eosio;
using namespace std;
using std::string;

class[[eosio::contract]] cryptobadge : public contract
{

public:
	//using contract::contract;
	cryptobadge(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),_global(_self, _self.value) {}

	/*
		* New issuer registration. issuer is required to create new badge. Markets *may* choose to use information here
		* to display info about the issuer
		*
		* issuer	- issuer account who will create badges;
		* data		- sha256 string. Recommendations to include: name, url, telephone, desc, image, email, address;
		*/
	ACTION regissuer(name issuer, checksum256 & data);

	/*
		* issuers info update. Used to update issuer information.
		*
		* issuer	- issuers account who will create badges;
		* data		- sha256 string. Recommendations to include: name, url, telephone, desc, image, email, address;
		*/
	ACTION updateissuer(name issuer, checksum256 & data);

	/*
		* Create a new badge.
		*
		* issuer		- badge's issuer, who will able to updated badge's mdata;
		* badge_id		- unique id;		
		* name			- badge name;			
		* image_url		- image url of badge;
		* path			- path of badge;
		* description	- description of badge;
		* criteria		- criteria of badge;
		*/
	ACTION createbadge(name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria);

	/*
		* Update badges data. Action is available only for issuers.
		*
		* issuer			- issuers account;
		* badge_id			- badge id to update;
		* name				- name of badge;
		* image_url		- image url of badge;
		* path				- path of badge;
		* description	- description of badge;
		* criteria		- criteria of badge;
		*/
	ACTION updatebadge(name issuer, uint64_t badge_id, string name, string image_url, string path, string description, string criteria);

	/*
		* Create a new certification.
		*
		* issuer			- badge owner;
		* owner				- certification owner;
		* badge_id			- The badge id which was issued to user;
		* badge_revision	- The badge revision;		
		* cert_id			- The certification id;
		* encrypted_data	- The encrypted certifications data;
		* encrypted_data	- expired time;
		* require_claim		- true or false. If disabled, upon creation, the certification will be transfered to owner (but
		*		  but issuer'S memory will be used until the certification is transferred again).  If enabled,
		*		  issuer will remain the owner, but an offer will be created for the account specified in
		*		  the owner field to claim the certification using the account's RAM.
		*/
	ACTION issuebadge( name issuer, name owner, uint64_t badge_id, uint64_t badge_revision, uint64_t cert_id, string& encrypted_data, uint64_t expire_at, bool require_claim);

	/*
		* Update certification state to expired.
		*
		* cert_id	- certification id;
		* owner		- the owner of certification;
		*/
	ACTION expirecert(name updater, uint64_t cert_id, name owner);

	/*
		* Revoke certification
		*
		* issuer	- issuer of certification;
		* cert_id	- certification id;
		* owner		- the owner of certification;
		*/
	ACTION revokecert(name issuer, uint64_t cert_id, name owner, string reason);

	/*
		* Claim the specified certification (assuming it was offered to claimer by the certification owner).
		*
		* claimer	- account claiming the badge;
		* cert_ids	- array of cert_id's to claim;
		*/
	ACTION claimbadge(name claimer, name issuer, uint64_t cert_ids);

	/*
		* Empty action. Used by create action to log cert_id so that third party explorers can
		* easily get new certification ids and other information.
		*/
	ACTION issuelog(name issuer, name owner, uint64_t cert_id, const checksum256 & issued_tx_id);
	

	/*
		* Set global config for badge contract
		* 
		*/
	ACTION setconfig(name ram_payer_account, name governance_design);

private:
	enum CertificationState
	{
		CERTIFIED = 0,
		EXPIRED,
		REVOKED	
	};

	checksum256 gettrxid();

	/*
		* issuers table. Can be used by badge markets, badge explorers, or wallets for correct badge
		* data presentation.
		* Scope: self
		*/
	TABLE v1_issuer
	{
		name issuer;
		checksum256 data;

		auto primary_key() const
		{
			return issuer.value;
		}
	};
	typedef multi_index<"v1.issuers"_n, v1_issuer> v1_issuers;

	/*
	* Badges table which stores information about simple badges.
	* Scope: badge owner
	*/
	TABLE v1_badge
	{
		uint64_t badge_id;
		name issuer;
		uint64_t version = 1;

		auto primary_key() const { return badge_id; }
		uint64_t by_issuer() const { return issuer.value; }
	};

	typedef eosio::multi_index<"v1.badges"_n, v1_badge,
							   eosio::indexed_by<"issuer"_n, eosio::const_mem_fun<v1_badge, uint64_t, &v1_badge::by_issuer>>>
		v1_badges;

	/*
		* Certification table which stores information about  certifications.
		* Scope: certification owner
		*/
	TABLE v1_cert
	{
		uint64_t id;
		uint64_t badge_id;
		uint64_t badge_revision;
		name owner;
		uint64_t state;
		uint64_t expire_at;

		auto primary_key() const
		{
			return id;
		}
		uint64_t by_badge_id() const
		{
			return badge_id;
		}
		uint64_t by_owner() const
		{
			return owner.value;
		}
	};

	typedef eosio::multi_index<"v1.certs"_n, v1_cert,
							   eosio::indexed_by<"badgeid"_n, eosio::const_mem_fun<v1_cert, uint64_t, &v1_cert::by_badge_id>>,
							   eosio::indexed_by<"owner"_n, eosio::const_mem_fun<v1_cert, uint64_t, &v1_cert::by_owner>>>
		v1_certs;

	/*
		* Certification table which stores information about  certifications.
		* Scope: certification owner
		*/
	TABLE v1_issuing_cert
	{
		uint64_t id;
		uint64_t badge_id;
		uint64_t badge_revision;
		name owner;
		uint64_t state;
		uint64_t expire_at;
		checksum256 issued_tx_id;

		auto primary_key() const
		{
			return id;
		}
		uint64_t by_badge_id() const
		{
			return badge_id;
		}
		uint64_t by_owner() const
		{
			return owner.value;
		}
	};

	typedef eosio::multi_index<"v1.issuing"_n, v1_issuing_cert,
							   eosio::indexed_by<"badgeid"_n, eosio::const_mem_fun<v1_issuing_cert, uint64_t, &v1_issuing_cert::by_badge_id>>,
							   eosio::indexed_by<"owner"_n, eosio::const_mem_fun<v1_issuing_cert, uint64_t, &v1_issuing_cert::by_owner>>>
		v1_issuing_certs;
		
	/*
		* Certification table which stores information about  certifications.
		* Scope: certification owner
		*/
	TABLE v1_certinfo
	{
		uint64_t cert_id;
		string cert_content;

		auto primary_key() const
		{
			return cert_id;
		}
	};

	typedef eosio::multi_index<"v1.certinfos"_n, v1_certinfo> v1_certinfos;

	/*
		* global singelton table, used for cert_id building
		* Scope: self
		*/
	TABLE v1_global
	{

		v1_global() {}
		name ram_payer_account = "ram.can"_n;
		name governance_design = "governance"_n;

		EOSLIB_SERIALIZE(v1_global, (ram_payer_account)(governance_design))
	};

	typedef eosio::singleton<"v1.global"_n, v1_global> v1_global_table;
	typedef eosio::multi_index<"v1.global"_n, v1_global> fv1_global_table;
	// v1_global _cstate;

	v1_global_table _global;
	

	//refer govenance design
	TABLE communityf
	{
		name community_account;
		name creator;
		string community_name;
		vector<uint64_t> member_badge;
		string community_url;
		string description;

		uint64_t primary_key() const { return community_account.value; }
	};

	typedef eosio::multi_index<"community"_n, communityf> community_table;
};