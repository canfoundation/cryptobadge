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
   - **badgeid** unique badge id
   - **issuer** the account registering as a issuer
   - **owner** the badge owner account
   - **badgedata** stringified json with mutable badges data

## badge::updatebadge issuer owner badgeid mdata 
   - **issuer** account has created badge
   - **owner** the badge owner account
   - **badgeid** id of badge
   - **badgedata** mutable badges data

## badge::createcert issuer owner badgeid idata requireclaim
   - Create a new certification
   - **certid** unique cert id
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


* Please create new account to deploy smart contract:  badge.can
 

* CAN Testnet endpoint network:

http://3.10.0.98:8888 


* Contract name: badge.can


##### Set smart contract to badge.can account
````bash

eosio-cpp -abigen -I include -contract cryptobadge -o cryptobadge.wasm src/cryptobadge.cpp

cleos -u http://3.10.0.98:8888 set contract badge.can . cryptobadge.wasm cryptobadge.abi -p badge.can

````

##### Check smart contract ram
````bash
cleos -u http://3.10.0.98:8888 get account badge.can
...
memory: 
    quota:     8.001 MiB    used:     632.2 KiB 
````

##### set server permission in case using server permission
````bash

$ cleos -u http://3.10.0.98:8888  set account permission badge.can server '{"threshold": 1,"keys": [{"key": "EOS5W9TbfQwz5TM5ap3mpHJHpJH842HBVGH3F7pqtoQgj4B4bYF8k","weight": 1}]}' "active" -p badge.can

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can regissuer server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can updateissuer server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can createbadge server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can updatebadge server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can createcert server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can createlog server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can removecert server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can canceloffer server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can claimcert server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can attach server

$ cleos -u http://3.10.0.98:8888  set action permission badge.can badge.can detach server
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

for example issuer.can as first issuer

$ cleos -u http://3.10.0.98:8888 push action badge.can regissuer '[issuer.can, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p issuer.can

$ cleos -u http://3.10.0.98:8888 push action badge.can updateissuer '[issuer.can, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p issuer.can

$ cleos -u http://3.10.0.98:8888 get table badge.can badge.can issuers
{
  "rows": [{
      "issuer": "issuer.can",
      "data": "74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18"
    }
  ],
  "more": false
}

````


##### create badge

````bash

$ cleos -u http://3.10.0.98:8888 push action badge.can createbadge '[99999, issuer.can, issuer.can, "{\"name\":\"Cryptobadge\",\"url\":\"cryptobadge.app\",\"description\":\"cryptobadge application\",\"framework\":\"Blockchain\",\"code\":\"123\"}"]' -p issuer.can

$ cleos -u http://3.10.0.98:8888 push action badge.can updateissuer '[issuer.can, d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907]' -p issuer.can

$ cleos -u http://3.10.0.98:8888 get table badge.can issuer.can cbadges
{
  "rows": [{
      "badgeid": 99999,
      "issuer": "issuer.can",
      "owner": "issuer.can",
      "badgedata": "{\"name\":\"Cryptobadge\",\"url\":\"cryptobadge.app\",\"description\":\"cryptobadge application\",\"framework\":\"Blockchain\",\"code\":\"123\"}"
    }
  ],
  "more": false
}
````

##### create Certification and issue directly to owner

````bash

$ cleos -u http://3.10.0.98:8888 push action badge.can createcert '[0, issuer.can, iwantmybadge, 1,  8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691, false]' -p issuer.can


$ cleos -u http://3.10.0.98:8888 get table badge.can iwantmybadge ccerts
{
  "rows": [{
      "id": 1000001,
      "owner": "iwantmybadge",
      "issuer": "issuer.can",
      "badgeid": 1,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    }
  ],
  "more": false
}

````


##### create Certification and allow user can claim it

````bash

$ cleos -u http://3.10.0.98:8888 push action badge.can createcert '[1, issuer.can, iwantmybadge, 1,  8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691, true]' -p issuer.can

$ cleos -u http://3.10.0.98:8888 get table badge.can badge.can offers
{
  "rows": [{
      "certid": 1000002,
      "owner": "issuer.can",
      "offeredto": "iwantmybadge",
      "cdate": 1560248740
    }
  ],
  "more": false
}

$ cleos -u http://3.10.0.98:8888 get table badge.can issuer.can ccerts
{
  "rows": [{
      "id": 1000002,
      "owner": "issuer.can",
      "issuer": "issuer.can",
      "badgeid": 1,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    }
  ],
  "more": false
}

$ cleos -u http://3.10.0.98:8888 push action badge.can claimcert '[iwantmybadge, [1000002] ]' -p iwantmybadge

$ cleos -u http://3.10.0.98:8888 get table badge.can badge.can offers
{
  "rows": [],
  "more": false
}

$ cleos -u http://3.10.0.98:8888 get table badge.can iwantmybadge ccerts
{
  "rows": [{
      "id": 1000001,
      "owner": "iwantmybadge",
      "issuer": "issuer.can",
      "badgeid": 1,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    },{
      "id": 1000002,
      "owner": "iwantmybadge",
      "issuer": "issuer.can",
      "badgeid": 0,
      "idata": "8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691"
    }
  ],
  "more": false
}

````

##### attach detail certification

````bash

$ cleos -u http://3.10.0.98:8888 push action badge.can attach '[iwantmybadge, 1000001,  "{\"issuedOn\":\"01-01-2011\",\"evidences\":\"By 123\",\"narratives\":\"Can foundation\",\"expire\":\"01-01-2021\",\"description\":\"Blockchain Company\"}"]' -p iwantmybadge

cleos -u http://3.10.0.98:8888 get table badge.can iwantmybadge ccertinfos
````

