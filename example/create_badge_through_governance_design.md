

Example session using cleos with can test net
-------
#### Prerequisites


* Please create new account to deploy crypto badge smart contract:  cryptobadge5
* Please create new account to deploy smart contract:  governance23

* Endpoint network:

Use one of the following:
http://network-endpoint:8888

* Contract name: cryptobadge2


##### Set smart contract to badge account

navigate to contracts/crypto.badge and run those commands to complie and deploy contract:

*NOTE* make sure the `GOVERNANCE_DESIGN` variable in the top of `cryptobadge.cpp` file is set to the name of your governance contract account.

*NOTE* make sure the `cryptobadge_contract` variable in the top of `community.cpp` file of governance smart contract is set to the name of your crypto badge contract account.

````bash
eosio-cpp -abigen -I include -contract cryptobadge -R ricardian -o cryptobadge.wasm src/cryptobadge.cpp

cleos -u http://network-endpoint:8888 set contract cryptobadge5 . cryptobadge.wasm cryptobadge.abi -p cryptobadge5
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


1. Register using issuer by using crypto badge smart contract:

````bash

$ cleos -u http://network-endpoint:8888 push action cryptobadge5 regissuer '[badgeissuer1, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p badgeissuer1

$ cleos -u http://network-endpoint:8888 push action cryptobadge5 updateissuer '[badgeissuer1, 74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18]' -p badgeissuer1

$ cleos -u http://network-endpoint:8888 get table cryptobadge5 cryptobadge5 issuers
{
  "rows": [{
      "issuer": "badgeissuer1",
      "data": "74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18"
    }
  ],
  "more": false
}

````

2. Register issuer by create new community in governance designer contract:

```bash
// transfer token to create new community account, memo is the name of community account
cleos -u http://network-endpoint:8888 transfer creator.can governance23 "20.0000 CAT" "community145" -p creator.can

// update community infomation
cleos -u http://network-endpoint:8888 push action governance23 create '[creator.can, community145, "Test", [], "test-v.com", "Testing vo", 1]' -p creator.can
```


##### create badge

- create badge require two signature of cryptobadge smart contract account and issuer account.
=> we use `eosio.msig` contract to propose and sign transaction to create badge


1. If issuer is registered by governance contract (create community), we need to create `createbadge` and `issuebadge` code and set right holder for those codes first:

```bash
cleos -u http://18.182.95.163:8888 convert pack_action_data governance23 createcode '["community145", "badge", "governance23", ["createbadge", "issuebadge"], 0 ]'
5048f0d94d2d25450000000000c59239308442d3ccab36650200144be6a86cd445008062c91ca5317600

cleos -u http://18.182.95.163:8888 push action governance23 execcode '[community145, creator.can, co.amend, createcode, "5048f0d94d2d25450000000000c59239308442d3ccab36650200144be6a86cd445008062c91ca5317600"]' -p creator.can

cleos -u http://18.182.95.163:8888 convert pack_action_data governance23 setrightcode '["community145", "badge", ["creator.can"], [] ]'
5048f0d94d2d25450000000000c592390100a641e0d26cd44500

cleos -u http://18.182.95.163:8888 push action governance23 execcode '[community145, creator.can, co.amend, setrightcode, "5048f0d94d2d25450000000000c592390100a641e0d26cd44500"]' -p creator.can

cleos -u http://18.182.95.163:8888 get table governance23 community145 codes                                                                                            130 ↵
{
  "rows": [{
      "code_id": "badge",
      "contract_name": "governance23",
      "code_actions": [
        "createbadge",
        "issuebadge"
      ],
      "right_holder": {
        "required_badges": [],
        "required_positions": [],
        "required_tokens": [],
        "required_exp": 0,
        "accounts": [
          "creator.can"
        ]
      },
      "exec_type": 0
    }]....
```

2. create proposal for transaction create badge use eosio.msig contract:
```bash
cleos -u http://network-endpoint:8888 multisig propose newbadge1111  '[{"actor": "cryptobadge5", "permission": "active"},{"actor": "community145", "permission": "active"}]' '[{"actor": "cryptobadge5", "permission": "active"},{"actor": "community145", "permission": "active"}]' cryptobadge5 createbadge '{"issuer":"community145", "badge_id":1, "version":100, "name":"CryptoBadge", "image_url":"http://cryptobadge.app/image.img", "path":"cryptobadge", "description":"cryptobadge application", "criteria":"cryptobadge criteria"}' cryptobadge5 12

cleos -u http://network-endpoint:8888 multisig review cryptobadge5 newbadge1111
{
  "proposer": "badge",
  "proposal_name": "newbadge1111",
  "transaction_id": "db5fb6cb6e46acb9b04c5c25b12e0fc68db0a0ff189430308da6d9181e87f52d",
  "packed_transaction": "5936065e000000000000000000000150144be6d05cfd4500144be6a86cd4450250144be6d05cfd4500000000a8ed32325048f0d94d2d254500000000a8ed32327e5048f0d94d2d2545010000000000000064000000000000000b43727970746f426164676520687474703a2f2f63727970746f62616467652e6170702f696d6167652e696d670b63727970746f62616467651763727970746f6261646765206170706c69636174696f6e1463727970746f626164676520637269746572696100",
  "transaction": {
    "expiration": "2019-12-27T16:50:33",
    "ref_block_num": 0,
    "ref_block_prefix": 0,
    "max_net_usage_words": 0,
    "max_cpu_usage_ms": 0,
    "delay_sec": 0,
    "context_free_actions": [],
    "actions": [{
        "account": "cryptobadge5",
        "name": "createbadge",
        "authorization": [{
            "actor": "cryptobadge5",
            "permission": "active"
          },{
            "actor": "community145",
            "permission": "active"
          }
        ],
        "data": {
          "issuer": "community145",
          "badge_id": 1,
          "version": 100,
          "name": "CryptoBadge",
          "image_url": "http://cryptobadge.app/image.img",
          "path": "cryptobadge",
          "description": "cryptobadge application",
          "criteria": "cryptobadge criteria"
        },
        "hex_data": "5048f0d94d2d2545010000000000000064000000000000000b43727970746f426164676520687474703a2f2f63727970746f62616467652e6170702f696d6167652e696d670b63727970746f62616467651763727970746f6261646765206170706c69636174696f6e1463727970746f6261646765206372697465726961"
      }
    ],
    "transaction_extensions": []
  }
}
```

3. use cryptobadge smart contract account to sign this proposal:

```bash
cleos -u http://network-endpoint:8888 get table eosio.msig cryptobadge5 approvals2 -U newbadge1111 -L newbadge1111
{
  "rows": [{
      "version": 1,
      "proposal_name": "newbadge1111",
      "requested_approvals": [{
          "level": {
            "actor": "cryptobadge5",
            "permission": "active"
          },
          "time": "1970-01-01T00:00:00.000"
        },{
          "level": {
            "actor": "community145",
            "permission": "active"
          },
          "time": "1970-01-01T00:00:00.000"
        }
      ],
      "provided_approvals": []
    }
  ],
  "more": false
}

cleos -u http://network-endpoint:8888 multisig approve cryptobadge5 newbadge1111 '{"actor": "cryptobadge5", "permission": "active"}' -p cryptobadge5

cleos -u http://network-endpoint:8888 get table eosio.msig cryptobadge5 approvals2 -U newbadge1111 -L newbadge1111
{
  "rows": [{
      "version": 1,
      "proposal_name": "newbadge1111",
      "requested_approvals": [{
          "level": {
            "actor": "community145",
            "permission": "active"
          },
          "time": "1970-01-01T00:00:00.000"
        }
      ],
      "provided_approvals": [{
          "level": {
            "actor": "cryptobadge5",
            "permission": "active"
          },
          "time": "2019-12-27T06:29:37.000"
        }
      ]
    }
  ],
  "more": false
}

```

4. execute code in community to sign and execute create badge proposal:

```bash
cleos -u http://18.182.95.163:8888 convert pack_action_data governance23 createbadge '["community145", newbadge1111 ]'
5048f0d94d2d25451042088a2573b89a

cleos -u http://18.182.95.163:8888 push action governance23 execcode '[community145, creator.can, badge, createbadge, "5048f0d94d2d25451042088a2573b89a"]' -p creator.can

// verify in state data
cleos -u http://network-endpoint:8888 get table cryptobadge5 community145 cbadges
{
  "rows": [{
      "badge_id": 1,
      "issuer": "community145"
    }
  ],
  "more": false
}

```

##### create Certification

Do the same flow with create badge

1. create proposal for transaction create certification (issue badge) use eosio.msig contract:
```bash
cleos -u http://network-endpoint:8888 multisig propose newcert1111  '[{"actor": "cryptobadge5", "permission": "active"},{"actor": "community145", "permission": "active"}]' '[{"actor": "cryptobadge5", "permission": "active"},{"actor": "community145", "permission": "active"}]' cryptobadge5 issuebadge '{"issuer":"community145", "owner":"community145", "badge_id":1, "badge_revision":100, "cert_id": 1, "encrypted_data":"abc123abcqweqweqr", "require_claim":0}' cryptobadge5 12

cleos -u http://network-endpoint:8888 multisig review cryptobadge5 newcert1111
{
  "proposer": "cryptobadge5",
  "proposal_name": "newcert1111",
  "transaction_id": "e8949b77fe766162821c790312164b6d140e382eaf9438c83886234fea4142f1",
  "packed_transaction": "f352065e000000000000000000000150144be6d05cfd45008062c91ca531760250144be6d05cfd4500000000a8ed32325048f0d94d2d254500000000a8ed32323b5048f0d94d2d25455048f0d94d2d25450100000000000000640000000000000001000000000000001161626331323361626371776571776571720100",
  "transaction": {
    "expiration": "2019-12-27T18:52:35",
    "ref_block_num": 0,
    "ref_block_prefix": 0,
    "max_net_usage_words": 0,
    "max_cpu_usage_ms": 0,
    "delay_sec": 0,
    "context_free_actions": [],
    "actions": [{
        "account": "cryptobadge5",
        "name": "issuebadge",
        "authorization": [{
            "actor": "cryptobadge5",
            "permission": "active"
          },{
            "actor": "community145",
            "permission": "active"
          }
        ],
        "data": {
          "issuer": "community145",
          "owner": "community145",
          "badge_id": 1,
          "badge_revision": 100,
          "cert_id": 1,
          "encrypted_data": "abc123abcqweqweqr",
          "require_claim": 1
        },
        "hex_data": "5048f0d94d2d25455048f0d94d2d254501000000000000006400000000000000010000000000000011616263313233616263717765717765717201"
      }
    ],
    "transaction_extensions": []
  }
}
```

3. use cryptobadge smart contract account to sign this proposal:

```bash
cleos -u http://network-endpoint:8888 get table eosio.msig cryptobadge5 approvals2 -U newcert1111 -L newcert1111
{
  "rows": [{
      "version": 1,
      "proposal_name": "newcert1111",
      "requested_approvals": [{
          "level": {
            "actor": "cryptobadge5",
            "permission": "active"
          },
          "time": "1970-01-01T00:00:00.000"
        },{
          "level": {
            "actor": "community145",
            "permission": "active"
          },
          "time": "1970-01-01T00:00:00.000"
        }
      ],
      "provided_approvals": []
    }
  ],
  "more": false
}

cleos -u http://network-endpoint:8888 multisig approve cryptobadge5 newcert1111 '{"actor": "cryptobadge5", "permission": "active"}' -p cryptobadge5

cleos -u http://network-endpoint:8888 get table eosio.msig cryptobadge5 approvals2 -U newcert1111 -L newcert1111
{
  "rows": [{
      "version": 1,
      "proposal_name": "newcert1111",
      "requested_approvals": [{
          "level": {
            "actor": "community145",
            "permission": "active"
          },
          "time": "1970-01-01T00:00:00.000"
        }
      ],
      "provided_approvals": [{
          "level": {
            "actor": "cryptobadge5",
            "permission": "active"
          },
          "time": "2019-12-27T06:55:27.000"
        }
      ]
    }
  ],
  "more": false
}

```

4. execute code in community to sign and execute create certification proposal:

```bash
cleos -u http://18.182.95.163:8888 convert pack_action_data governance23 issuebadge '["community145", newcert1111 ]'
5048f0d94d2d2545004208215f85b89a

cleos -u http://18.182.95.163:8888 push action governance23 execcode '[community145, creator.can, badge, issuebadge, "5048f0d94d2d2545004208215f85b89a"]' -p creator.can

// verify in state data
cleos -u http://network-endpoint:8888 get table cryptobadge5 community145 ccerts
{
  "rows": [{
      "badge_id": 1,
      "issuer": "community145"
    }
  ],
  "more": false
}

```

base on `require_claim` parameter use corresponding command to verify certification in state data

```bash
// require_claim is false
cleos -u http://network-endpoint:8888 get table cryptobadge5 community145 ccerts
{
  "rows": [{
      "id": 1,
      "badge_id": 1,
      "badge_revision": 100,
      "owner": "community145",
      "encripted_data": "abc123abcqweqweqr"
    }
  ],
  "more": false
}

// require_claim is true
cleos -u http://network-endpoint:8888 get table cryptobadge5 cryptobadge5 offers
```

