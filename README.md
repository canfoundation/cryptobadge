Crypto-Badge
----------

Version 2.0.0

This contract provides multiple functionalities:
- Users can register to become a badge issuer,
- Issuer can create/update badge and assign to badge owner
- Badge owner can create new certification based on the created badge
- Badge owner can create new certification and issue to users 
- Badge owner can create new certification and reserved it for non CAN account
- Users who just have CAN account can claim badge which belong to them under verification from crypto-badge
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

## Example to run contract with cleos:

* [Example](https://github.com/canfoundation/cryptobadge/tree/master/example)

## Actions:

#### badge::regissuer
   - Indicates that a particular account wishes to become a issuer
   - **issuer** account registering to be a badge issuer who has permission to create badge
   - **data** sha256 of issuer info

#### badge::updateissuer
   - **issuer** account owner of issuer
   - **data** sha256 updated issuer info

#### badge::createbadge
   - Create a new badge
   - **issuer** the account registering as a issuer
   - **badge_id** the id of badge, should be unique in scope of issuer
   - **name** the name of badge
   - **image_url** the url of badge
   - **path** the path of badge
   - **description** the description of badge
   - **criteria** the criteria of badge

#### badge::updatebadge
   - Update badge
   - **issuer** account has created badge
   - **badge_id** id of badge
   - **name** the name of badge
   - **image_url** the url of badge
   - **path** the path of badge
   - **description** the description of badge
   - **criteria** the criteria of badge

#### badge::issuebadge
   - Create a new certification
   - **issuer** account whose onwer of badge_id
   - **owner** account whose certification owner
   - **badge_id** badge id for certification
   - **badge_revision** the version of badge use for create certificate
   - **cert_id** certification id for certification, should be unique in scope of owner
   - **encrypted_data** ceritification encrypted data
   - **expire_at** expire time of certification as epoch second
   - **require_claim** true or false. If disabled, upon creation, the certification will be transfered to owner but issuer'S memory will be used until the certification is transferred again).  If enabled issuer will remain the owner, but an offer will be created for the account specified in the owner field to claim the certification using the account's RAM.

#### badge::claimbadge
   - **claimer** account whose onwer of certification
   - **issuer** issuer of certification
   - **cert_id** certification to claim. The action will be failed if cert_id does not exist
   - Note : claimer must pay RAM for certification if they do this action

#### badge::expirecert
   - update certification state to EXPIRED
   - **updater** current certification issuer account
   - **cert_id** certification to expire
   - Note: Can not update if certification's expire_at > now

#### badge::revokecert
   - update certification state to REVOKE
   - **issuer** issuer of certification
   - **cert_id** certification to expire
   - **encrypted_data** updated encrypted data of certification with revocation reason
   - Note: Can not update if certification's expire_at > now




