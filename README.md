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

Dependencies:
* [can v1.0.x](https://github.com/canfoundation/CAN/releases/tag/can-v1.0.0)
* [eosio.cdt v1.7.x](https://github.com/EOSIO/eosio.cdt/releases/tag/v1.7.0)

## Build the contracts:
```
cd cryptobadge

eosio-cpp -abigen -I include -contract cryptobadge -o cryptobadge.wasm src/cryptobadge.cpp
```

## How to run unit test

1. Install eosio testing framework [hydra](https://docs.klevoya.com/hydra/about/getting-started) and login into it:

```bash
npm i -g @klevoya/hydra

hydra login
```

2. Install yarn packages:

```bash
yarn install
```
cd cryptobadge

3. Run test:

Run all unit test

```bash
yarn test
```

Run test file:

```bash
yarn test cryptobadge.test.ts
```

***WARING*** Because test need to send many request to hydra server, test timeout error may sometime happend. Make sure your network connection is stable to run this test.


## Actions:

#### badge::regissuer issuer data
   - Indicates that a particular account wishes to become a issuer
   - **issuer** account registering to be a badge issuer who has permission to create badge
   - **data** sha256 of issuer info

#### badge::updateissuer issuer data
   - **issuer** account owner of issuer
   - **data** sha256 updated issuer info

#### badge::createbadge issuer version name image_url path description criteria
   - Create a new badge
   - **issuer** the account registering as a issuer
   - **badge_id** the id of badge, should be unique in scope of issuer
   - **name** the name of badge
   - **image_url** the url of badge
   - **path** the path of badge
   - **description** the description of badge
   - **criteria** the criteria of badge

#### badge::updatebadge issuer badge_id version name image_url path description criteria
   - **issuer** account has created badge
   - **badge_id** id of badge
   - **name** the name of badge
   - **image_url** the url of badge
   - **path** the path of badge
   - **description** the description of badge
   - **criteria** the criteria of badge

#### badge::issuebadge issuer owner idata require_claim
   - Create a new certification
   - **issuer** account whose onwer of badge_id
   - **owner** account whose certification owner
   - **badge_id** badge id for certification
   - **badge_revision** the version of badge use for create certificate
   - **cert_id** certification id for certification, should be unique in scope of owner
   - **encrypted_data** ceritification encrypted data
   - **require_claim** true or false. If disabled, upon creation, the certification will be transfered to owner but issuer'S memory will be used until the certification is transferred again).  If enabled issuer will remain the owner, but an offer will be created for the account specified in the owner field to claim the certification using the account's RAM.

#### badge::claimcert claimer cert_ids
   - **claimer** account whose onwer of certification
   - **cert_ids** list certifications to claim. The action will be failed if one cert_ids does not work
   - Note : claimer must pay RAM for certification if they do this action

#### badge::canceloffer issuer cert_ids
   - **issuer** current certification issuer account
   - **cert_ids** list certifications to cancel offer
   - Issuer only cancel offer when the certification still not issue it to owner
   
#### badge::removecert owner cert_ids memo
   - Deposits tokens to user REX fund
   - **owner** current certification owner account
   - **cert_ids** array of cert_id's to revoke
   - **memo** memo for revoke action
   - This action is only available for the certification owner. After executing, the certification will disappear forever, and RAM used for certification will be released.

#### badge::attach owner cert_id  data
   - public certification detail. Action is not mandatory
   - **owner** certification owner account
   - **cert_id** id of certificastion when to public on blockchain
   - **data** json stringify data of certification
   - Note: once the certification is public, there are no way to remove data on blockchain

#### badge::detach owner cert_ids
   - Detach certification detail. Action is not mandatory.
   - **owner** certification owner account
   - **cert_ids** list of id certification
   - The Ram will be release 

#### badge::createlog issuer owner idata cert_id require_claim
   - Empty action. Used by create action to log cert_id so that third party explorers can easily get new certification ids and other information.




