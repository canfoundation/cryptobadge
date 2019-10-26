
Example session using cleos with jungle testnet
-------
#### Prerequisites


* Please create new account to deploy smart contract:  badge.can
 

* CAN Testnet endpoint network:

http://can-testnet.com 


* Contract name: badge.can


##### Set smart contract to badge.can account
````bash

eosio-cpp -abigen -I include -contract cryptobadge -o cryptobadge.wasm src/cryptobadge.cpp

cleos -u http://can-testnet.com set contract badge.can . cryptobadge.wasm cryptobadge.abi -p badge.can

````

##### Check smart contract ram
````bash
cleos -u http://can-testnet.com get account badge.can
...
memory: 
    quota:     8.001 MiB    used:     632.2 KiB 
````

##### set server permission in case using server permission
````bash

$ cleos -u http://can-testnet.com  set account permission badge.can server '{"threshold": 1,"keys": [{"key": "EOS5W9TbfQwz5TM5ap3mpHJHpJH842HBVGH3F7pqtoQgj4B4bYF8k","weight": 1}]}' "active" -p badge.can

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can regissuer server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can updateissuer server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can createbadge server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can updatebadge server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can createcert server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can createlog server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can removecert server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can canceloffer server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can claimcert server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can attach server

$ cleos -u http://can-testnet.com  set action permission badge.can badge.can detach server
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

$ cleos -u http://can-testnet.com push action badge.can regissuer '[issuer.can, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p issuer.can

$ cleos -u http://can-testnet.com push action badge.can updateissuer '[issuer.can, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p issuer.can

$ cleos -u http://can-testnet.com get table badge.can badge.can issuers
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

$ cleos -u http://can-testnet.com push action badge.can createbadge '[99999, issuer.can, issuer.can, "{\"name\":\"Cryptobadge\",\"url\":\"cryptobadge.app\",\"description\":\"cryptobadge application\",\"framework\":\"Blockchain\",\"code\":\"123\"}"]' -p issuer.can

$ cleos -u http://can-testnet.com push action badge.can updateissuer '[issuer.can, d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907]' -p issuer.can

$ cleos -u http://can-testnet.com get table badge.can issuer.can cbadges
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

$ cleos -u http://can-testnet.com push action badge.can createcert '[0, issuer.can, iwantmybadge, 1,  8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691, false]' -p issuer.can


$ cleos -u http://can-testnet.com get table badge.can iwantmybadge ccerts
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

$ cleos -u http://can-testnet.com push action badge.can createcert '[1, issuer.can, iwantmybadge, 1,  8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691, true]' -p issuer.can

$ cleos -u http://can-testnet.com get table badge.can badge.can offers
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

$ cleos -u http://can-testnet.com get table badge.can issuer.can ccerts
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

$ cleos -u http://can-testnet.com push action badge.can claimcert '[iwantmybadge, [1000002] ]' -p iwantmybadge

$ cleos -u http://can-testnet.com get table badge.can badge.can offers
{
  "rows": [],
  "more": false
}

$ cleos -u http://can-testnet.com get table badge.can iwantmybadge ccerts
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

$ cleos -u http://can-testnet.com push action badge.can attach '[iwantmybadge, 1000001,  "{\"issuedOn\":\"01-01-2011\",\"evidences\":\"By 123\",\"narratives\":\"Can foundation\",\"expire\":\"01-01-2021\",\"description\":\"Blockchain Company\"}"]' -p iwantmybadge

cleos -u http://can-testnet.com get table badge.can iwantmybadge ccertinfos
````

