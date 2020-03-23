
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
	cryptobadge(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

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
	ACTION issuebadge( name issuer, name owner, uint64_t badge_id, uint64_t badge_revision, uint64_t cert_id, string& encripted_data, uint64_t expire_at, bool require_claim);

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
	ACTION claimcert(name claimer, std::vector<uint64_t> & cert_ids);

	/*
		* Cancel and remove offer. Available for the certification owner.
		*
		* issuer	- current certification issuer account;
		* cert_ids	- array of cert_id's to cancel from offer;
		*/
	ACTION canceloffer(name issuer, std::vector<uint64_t> & cert_ids);

	/*
		* Remove certification {{cert_id}}. This action is only available for the certification owner. After executing, the
		* certification will disappear forever, and RAM used for certification will be released.
		*
		* owner		- current certification owner account;
		* cert_ids	- array of cert_id's to remove;
		* memo		- memo for remove action;
		*/
	ACTION removecert(name owner, std::vector<uint64_t> & cert_ids, string memo);

	/*
		* Empty action. Used by create action to log cert_id so that third party explorers can
		* easily get new certification ids and other information.
		*/
	ACTION createlog(name issuer, name owner, checksum256 & idata, uint64_t cert_id, bool require_claim);

private:
	enum CertificationState
	{
		CERTIFIED = 0,
		EXPIRED,
		REVOKED
	};

	uint64_t getid(uint64_t gindex);

	template <typename... Args>
	void sendEvent(name issuer, name rampayer, name seaction, const std::tuple<Args...> &tup);

	/*
		* issuers table. Can be used by badge markets, badge explorers, or wallets for correct badge
		* data presentation.
		* Scope: self
		*/
	TABLE cissuer
	{
		name issuer;
		checksum256 data;

		auto primary_key() const
		{
			return issuer.value;
		}
	};
<<<<<<< HEAD
	typedef multi_index<"v1.issuers"_n, cissuer> issuers;
=======
	typedef multi_index<"issuers"_n, cissuer> issuers;
>>>>>>> beada7b1f4ac75b4d6c438450605e1f12f8cd910

	/*
	* Badges table which stores information about simple badges.
	* Scope: badge owner
	*/
	TABLE cbadge
	{
		uint64_t badge_id;
		name issuer;
		uint64_t version = 1;

		auto primary_key() const { return badge_id; }
		uint64_t by_issuer() const { return issuer.value; }
	};

<<<<<<< HEAD
	typedef eosio::multi_index<"v1.badges"_n, cbadge,
=======
	typedef eosio::multi_index<"cbadges"_n, cbadge,
>>>>>>> beada7b1f4ac75b4d6c438450605e1f12f8cd910
							   eosio::indexed_by<"issuer"_n, eosio::const_mem_fun<cbadge, uint64_t, &cbadge::by_issuer>>>
		cbadges;

	/*
		* Certification table which stores information about  certifications.
		* Scope: certification owner
		*/
	TABLE ccert
	{
		uint64_t id;
		uint64_t badge_id;
		uint64_t badge_revision;
		name owner;
		uint64_t state;
		uint64_t expire_at;
		string encripted_data;

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

<<<<<<< HEAD
	typedef eosio::multi_index<"v1.certs"_n, ccert,
=======
	typedef eosio::multi_index<"ccerts"_n, ccert,
>>>>>>> beada7b1f4ac75b4d6c438450605e1f12f8cd910
							   eosio::indexed_by<"badgeid"_n, eosio::const_mem_fun<ccert, uint64_t, &ccert::by_badge_id>>,
							   eosio::indexed_by<"owner"_n, eosio::const_mem_fun<ccert, uint64_t, &ccert::by_owner>>>
		ccerts;

	/*
		* Certification table which stores information about  certifications.
		* Scope: certification owner
		*/
	TABLE ccertinfo
	{
		uint64_t cert_id;
		string cert_content;

		auto primary_key() const
		{
			return cert_id;
		}
	};

<<<<<<< HEAD
	typedef eosio::multi_index<"v1.certinfo"_n, ccertinfo> ccertinfos;
=======
	typedef eosio::multi_index<"ccertinfos"_n, ccertinfo> ccertinfos;
>>>>>>> beada7b1f4ac75b4d6c438450605e1f12f8cd910

	/*
		* Offers table keeps records of open offers of certifications (ie. certifications waiting to be claimed by their
		* intendend recipients.
		*
		* Scope: self
		*/
	TABLE coffer
	{
		uint64_t cert_id;
		name owner;
		name offered_to;
		time_point date;

		auto primary_key() const
		{
			return cert_id;
		}
		uint64_t by_owner() const
		{
			return owner.value;
		}
		uint64_t by_offered_to() const
		{
			return offered_to.value;
		}
	};

<<<<<<< HEAD
	typedef eosio::multi_index<"v1.offers"_n, coffer,
=======
	typedef eosio::multi_index<"offers"_n, coffer,
>>>>>>> beada7b1f4ac75b4d6c438450605e1f12f8cd910
							   eosio::indexed_by<"owner"_n, eosio::const_mem_fun<coffer, uint64_t, &coffer::by_owner>>,
							   eosio::indexed_by<"offeredto"_n, eosio::const_mem_fun<coffer, uint64_t, &coffer::by_offered_to>>>
		offers;

	/*
		* global singelton table, used for cert_id building
		* Scope: self
		*/
	TABLE global
	{

		global() {}
		uint64_t defer_id = 100000000000000;
		uint64_t cert_id = 1000000;
		uint64_t badge_id = 0000000;

		EOSLIB_SERIALIZE(global, (defer_id)(cert_id)(badge_id))
	};

<<<<<<< HEAD
	typedef eosio::singleton<"v1.global"_n, global> conf;
=======
	typedef eosio::singleton<"global"_n, global> conf;
>>>>>>> beada7b1f4ac75b4d6c438450605e1f12f8cd910
	global _cstate;

	enum gindex : uint8_t
	{
		DEFER = 0,
		CERT = 1,
		BADGE = 2,
	};

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