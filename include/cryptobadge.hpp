
#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>
#include <eosio/time.hpp>
#include <eosio/symbol.hpp>
#include <eosio/transaction.hpp>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/system.hpp>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace eosio;
using std::string;

class[[eosio::contract]] cryptobadge : public contract{

	public:
		//using contract::contract;
		cryptobadge(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds){}

		/*
		* New issuer registration. issuer is required to create new badge. Markets *may* choose to use information here
		* to display info about the issuer
		*
		* issuer	- issuer account who will create badges;
		* data		- sha256 string. Recommendations to include: name, url, telephone, desc, image, email, address;
		*/
		ACTION regissuer( name issuer, checksum256& data);


		/*
		* issuers info update. Used to update issuer information.
		*
		* issuer	- issuers account who will create badges;
		* data		- sha256 string. Recommendations to include: name, url, telephone, desc, image, email, address;
		*/
		ACTION updateissuer( name issuer, checksum256& data);

		/*
		* Create a new badge.
		*
		* issuer	- badge's issuer, who will able to updated badge's mdata;
		* category	- badges category;
		* owner		- badges owner;
		* badgedata	- stringified json with mutable badges data;
		*/
		ACTION createbadge( uint64_t badgeid, name issuer, name owner, string& badgedata);

		/*
		* Update badges mutable data (mdata) field. Action is available only for issuers.
		*
		* issuer	- issuers account;
		* owner		- current badges owner;
		* badgeid	- badgeid to update;
		* badgedata		- stringified json with mutable badges data. All mdata will be replaced;
		*/
		ACTION updatebadge( name issuer, name owner, uint64_t badgeid, string& badgedata);

		/*
		* Create a new certification.
		*
		* issuer	- badge owner;
		* owner		- certification owner;
		* idata		- sha256 string with immutable certifications data;
		* requireclaim	- true or false. If disabled, upon creation, the certification will be transfered to owner (but
		*		  but issuer'S memory will be used until the certification is transferred again).  If enabled,
		*		  issuer will remain the owner, but an offer will be created for the account specified in
		*		  the owner field to claim the certification using the account's RAM.
		*/
		ACTION createcert( uint64_t certid, name issuer, name owner, uint64_t badgeid, checksum256& idata, bool requireclaim);

		/*
		* Claim the specified certification (assuming it was offered to claimer by the certification owner).
		*
		* claimer	- account claiming the badge;
		* certids	- array of certid's to claim;
		*/
		ACTION claimcert( name claimer, std::vector<uint64_t>& certids);

		/*
		* Cancel and remove offer. Available for the certification owner.
		*
		* issuer	- current certification issuer account;
		* certids	- array of certid's to cancel from offer;
		*/
		ACTION canceloffer( name issuer, std::vector<uint64_t>& certids);


		/*
		* Remove certification {{certid}}. This action is only available for the certification owner. After executing, the
		* certification will disappear forever, and RAM used for certification will be released.
		*
		* owner		- current certification owner account;
		* certids	- array of certid's to remove;
		* memo		- memo for remove action;
		*/
		ACTION removecert( name owner, std::vector<uint64_t>& certids, string memo );

  
		/*
		* Attach certification detail. Action is not mandatory.  owner *may* choose to public information here
		* to display info
		*
		* owner	- account who is certification owner;
		* certid - certid's to attach
		* data		- stringified json. Recommendations to include: issuedOn, evidences, narratives, expire, description;
		*/
		ACTION attach( name owner, uint64_t certid, string data);
	
	
		/*
		* detach certification detail. Action is not mandatory.  owner *may* choose to remove their information
		*
		* owner	- account who is certification owner;
		* certids - array of certid's to detach
		* data		- stringified json. Recommendations to include: issuedOn, evidences, narratives, expire, description;
		*/	
		ACTION detach( name owner, std::vector<uint64_t>& certids);


		/*
		* Empty action. Used by create action to log certid so that third party explorers can
		* easily get new certification ids and other information.
		*/
		ACTION createlog( name issuer, name owner, checksum256& idata, uint64_t certid, bool requireclaim);


	private:

		uint64_t getid(uint64_t gindex);	

	bool compareHash(const checksum256& current_hash, const checksum256&  input_hash);

		template<typename... Args>
		void sendEvent(name issuer, name rampayer, name seaction, const std::tuple<Args...> &tup);

		/*
		* issuers table. Can be used by badge markets, badge explorers, or wallets for correct badge
		* data presentation.
		* Scope: self
		*/
    TABLE cissuer
    {
			name						issuer;
			checksum256			data;

			auto primary_key() const {
				return issuer.value;
			}
    };
    typedef multi_index<"issuers"_n, cissuer> issuers;


		/*
		* Badges table which stores information about simple badges.
		* Scope: badge owner
		*/
	   TABLE cbadge
    {
			uint64_t                badgeid;
			name                    issuer;
			name                    owner;
			string        			badgedata;

			auto primary_key() const {
				return badgeid;
			}
			uint64_t by_issuer() const {
				return issuer.value;
			}
    };
  	typedef eosio::multi_index< "cbadges"_n, cbadge,
			eosio::indexed_by< "issuer"_n, eosio::const_mem_fun<cbadge, uint64_t, &cbadge::by_issuer> >
			> cbadges;
			

		/*
		* Certification table which stores information about  certifications.
		* Scope: certification owner
		*/
		TABLE ccert
		{
			uint64_t                id;
			name                    owner;
			name                    issuer;
			uint64_t                badgeid;
			checksum256        idata;

			auto primary_key() const {
				return id;
			}
			uint64_t by_issuer() const {
				return issuer.value;
			}

		};

		typedef eosio::multi_index< "ccerts"_n, ccert,
			eosio::indexed_by< "issuer"_n, eosio::const_mem_fun<ccert, uint64_t, &ccert::by_issuer> >
			> ccerts;

		/*
		* Certification table which stores information about  certifications.
		* Scope: certification owner
		*/
		TABLE ccertinfo
		{
			uint64_t      cert_id;
			string        cert_content;

			auto primary_key() const {
				return cert_id;
			}

		};

		typedef eosio::multi_index< "ccertinfos"_n, ccertinfo> ccertinfos;

		/*
		* Offers table keeps records of open offers of certifications (ie. certifications waiting to be claimed by their
		* intendend recipients.
		*
		* Scope: self
		*/
		TABLE coffer
		{
			uint64_t		certid;
			name			owner;
			name			offeredto;
			uint64_t		cdate;

			auto primary_key() const {
				return certid;
			}
			uint64_t by_owner() const {
				return owner.value;
			}
			uint64_t by_offeredto() const {
				return offeredto.value;
			}
		};

		typedef eosio::multi_index< "offers"_n, coffer,
			eosio::indexed_by< "owner"_n, eosio::const_mem_fun<coffer, uint64_t, &coffer::by_owner> >,
			eosio::indexed_by< "offeredto"_n, eosio::const_mem_fun<coffer, uint64_t, &coffer::by_offeredto> >
			> offers;



		/*
		* global singelton table, used for certid building
		* Scope: self
		*/
		TABLE global{
		  
			global(){}
			uint64_t deferid	= 100000000000000;
			uint64_t certid		= 1000000;
			uint64_t badgeid	= 0000000;

			EOSLIB_SERIALIZE( global, (deferid)(certid)(badgeid) )
		};

		typedef eosio::singleton< "global"_n, global> conf;
		global _cstate;

		enum gindex : uint8_t
		{
			DEFER = 0,
			CERT = 1,
			BADGE = 2,
		};


	
};