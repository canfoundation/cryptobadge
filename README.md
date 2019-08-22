Crypto-Badge
----------

Version 1.0.0

This contract provides multiple functionalities:
- Users can register to become a badge issuer,
- Issuer can create/update badge and assign to badge owner
- Badge owner can create new certification based on the created badge
- Badge owner can create new certification and issue to users (issuer pay for RAM) directly or offer it to users (Users must pay RAM if they accept the offer)
- Badge owner can cancel the offered certification.
- User can accept the offer to receive certification from Badge owner
- User can choose to public certification detail

Actions:

## badge::regissuer issuer data
   - Indicates that a particular account wishes to become a issuer
   - **issuer** account registering to be a badge issuer who has permission to create badge
   - **data** issuer info

## badge::updateissuer issuer data
   - **issuer** account owner of issuer
   - **data** updated issuer info

## badge::createbadge issuer owner idata mdata
   - Create a new badge
   - **issuer** the account registering as a issuer
   - **owner** the badge owner account
   - **idata** immutable badges data
   - **mdata** mutable badges data

## badge::updatebadge issuer owner badgeid mdata 
   - **issuer** account has created badge
   - **owner** the badge owner account
   - **badgeid** id of badge
   - **mdata** mutable badges data

## badge::createcert issuer owner badgeid idata requireclaim
   - Create a new certification
   - **issuer** account whose onwer of badgeid
   - **owner** account whose certification owner
   - **badgeid** the badge on certification
   - **idata** ceritification detail data
   - **requireclaim** true or false. If disabled, upon creation, the certification will be transfered to owner but issuer'S memory will be used until the certification is transferred again).  If enabled ssuer will remain the owner, but an offer will be created for the account specified in the owner field to claim the certification using the account's RAM.

## badge::claimcert claimer certids
   - **claimer** account whose onwer of certification
   - **certids** list certifications to claim. The action will be failed if one certids does not work
   - Note : claimer must pay RAM for certification if they do this action

## badge::canceloffer issuer certids
   - **issuer** current certification issuer account
   - **certids** list certifications to cancel offer
   - Issuer only cancel offer when the certification still not issue it to owner
   
## badge::removecert owner certids memo
   - Deposits tokens to user REX fund
   - **owner** current certification owner account
   - **certids** array of certid's to revoke
   - **memo** memo for revoke action
   - This action is only available for the certification owner. After executing, the certification will disappear forever, and RAM used for certification will be released.

## badge::attach owner certid  data
   - public certification detail. Action is not mandatory
   - **owner** certification owner account
   - **certid** id of certificastion when to public on blockchain
   - **data** json stringify data of certification
   - Note: once the certification is public, there are no way to remove data on blockchain

## badge::detach owner certids
   - Detach certification detail. Action is not mandatory.
   - **owner** certification owner account
   - **certids** list of id certification
   - The Ram will be release 

## badge::createlog issuer owner idata certid requireclaim
   - Empty action. Used by create action to log certid so that third party explorers can easily get new certification ids and other information.


Example session using cleos with jungle testnet
-------
#### Prerequisites


* Please create new account to deploy smart contract:  cryptobadge1
 

* Endpoint network:

https://jungle2.cryptolions.io:443 


* Contract name: cryptobadge1


##### Set smart contract to cryptobadge1 account
````bash

eosio-cpp -abigen -I include -contract cryptobage -o cryptobage.wasm src/cryptobadge.cpp

$ cleos -u https://jungle2.cryptolions.io:443 set contract cryptobadge1 ./cbadge/ -p cryptobadge1
````

##### Check smart contract ram
````bash
cleos -u https://jungle2.cryptolions.io:443 get account cryptobadge1
...
memory: 
    quota:     1.866 MiB    used:     577.1 KiB
````




##### pre-process information data
````bash

Issuer Profile:
{
  "name": "Canfoudation",
  "url": "canfoudation.com",
  "description": "Canfoudation is the company behind CAN network",
  "email": "canfoudation@gmail.com",
  "address": "canfoudation Street"
}

minified text:

{"name":"Canfoudation","url":"canfoudation.com","description":"Canfoudation is the company behind CAN network","email":"canfoudation@gmail.com","address":"canfoudation Street"}

stringified text: 

"{\"name\":\"Canfoudation\",\"url\":\"canfoudation.com\",\"description\":\"Canfoudation is the company behind CAN network\",\"email\":\"canfoudation@gmail.com\",\"address\":\"canfoudation Street\"}"

hash:

74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18

Badge:

{
  "name": "Cryptobadge",
  "url": "cryptobadge.app",
  "description": "cryptobadge application",
  "framework": "Blockchain",
  "code": "123"
}

Badge minify:

{"name":"Cryptobadge","url":"cryptobadge.app","description":"cryptobadge application","framework":"Blockchain","code":"123"}

Badge stringified:

"{\"name\":\"Cryptobadge\",\"url\":\"cryptobadge.app\",\"description\":\"cryptobadge application\",\"framework\":\"Blockchain\",\"code\":\"123\"}"

hash:

d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907


Certification:

{
  "issuedOn": "01-01-2011",
  "evidences": "By 123",
  "narratives": "Can foundation",
  "expire": "01-01-2021",
  "description": "Blockchain Company"
}

json minify: 

{"issuedOn":"01-01-2011","evidences":"By 123","narratives":"Can foundation","expire":"01-01-2021","description":"Blockchain Company"}

json stringify:

"{\"issuedOn\":\"01-01-2011\",\"evidences\":\"By 123\",\"narratives\":\"Can foundation\",\"expire\":\"01-01-2021\",\"description\":\"Blockchain Company\"}"

hash:

8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691

Tool:

Json minified: https://codebeautify.org/jsonminifier

Json stringify: https://onlinetexttools.com/json-stringify-text

Json hash256: https://emn178.github.io/online-tools/sha256.html
````


##### register issuer

````bash

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 regissuer '[badgeissuer1, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p badgeissuer1

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 updateissuer '[badgeissuer1, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p badgeissuer1

$ cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 cryptobadge1 issuers
{
  "rows": [{
      "issuer": "badgeissuer1",
      "data": "74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18"
    }
  ],
  "more": false
}

````


##### create badge

````bash

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 createbadge '[badgeissuer2, badgeissuer1, 525843a99dfe43dc3e0da059e9d400490d6bfc322593bb95926a71ec5b1feadb, d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907]' -p badgeissuer2

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 updateissuer '[badgeissuer1, d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907]' -p badgeissuer1

$ cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 badgeissuer1 cbadges
{
  "rows": [{
      "badgeid": 1,
      "issuer": "badgeissuer1",
      "owner": "badgeissuer1",
      "idata": "d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907",
      "mdata": "d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907"
    }
  ],
  "more": false
}
````

##### create Certification and issue directly to owner

````bash

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 createcert '[badgeissuer1, iwantmybadge, 1,  8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691, false]' -p badgeissuer1


$ cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 iwantmybadge ccerts
{
  "rows": [{
      "id": 1000001,
      "owner": "iwantmybadge",
      "issuer": "badgeissuer1",
      "badgeid": 1,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    }
  ],
  "more": false
}

````


##### create Certification and allow user can claim it

````bash

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 createcert '[badgeissuer1, iwantmybadge, 1,  8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691, true]' -p badgeissuer1

$ cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 cryptobadge1 offers
{
  "rows": [{
      "certid": 1000002,
      "owner": "badgeissuer1",
      "offeredto": "iwantmybadge",
      "cdate": 1560248740
    }
  ],
  "more": false
}

$ cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 badgeissuer1 ccerts
{
  "rows": [{
      "id": 1000002,
      "owner": "badgeissuer1",
      "issuer": "badgeissuer1",
      "badgeid": 1,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    }
  ],
  "more": false
}

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 claimcert '[iwantmybadge, [1000002] ]' -p iwantmybadge

$ cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 cryptobadge1 offers
{
  "rows": [],
  "more": false
}

$ cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 iwantmybadge ccerts
{
  "rows": [{
      "id": 1000001,
      "owner": "iwantmybadge",
      "issuer": "badgeissuer1",
      "badgeid": 1,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    },{
      "id": 1000002,
      "owner": "iwantmybadge",
      "issuer": "badgeissuer1",
      "badgeid": 0,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    }
  ],
  "more": false
}

````

##### attach detail certification

````bash

$ cleos -u https://jungle2.cryptolions.io:443 push action cryptobadge1 attach '[iwantmybadge, 1000001,  "{\"issuedOn\":\"01-01-2011\",\"evidences\":\"By 123\",\"narratives\":\"Can foundation\",\"expire\":\"01-01-2021\",\"description\":\"Blockchain Company\"}"]' -p iwantmybadge

cleos -u https://jungle2.cryptolions.io:443 get table cryptobadge1 iwantmybadge ccertinfos
````

