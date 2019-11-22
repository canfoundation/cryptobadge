# How to Work with Canchain

## Configuration

### Default Settings

Private key: `your_private_key`
Public key: `your_public_key_start_with_EOS`
Default contract: `badge.can`
Base account with lots of money: `cancreator12`
Endpoint: `http://can-testnet.com`

```bash
# Test chain status
curl http://can-testnet.com/v1/chain/get_info | jq
```

### Wallet

An unlocked wallet is required to use `cleos`

```bash
# Create a new wallet if necessary
cleos wallet list
cleos wallet create -n tien1 --to-console
# Output:
# Creating wallet: tien1
# Save password to use in the future to unlock this wallet.
# Without password imported keys will not be retrievable.
# "generated_password"
cleos wallet unlock -n tien1 --password generated_password

# Default wallet:
# Creating wallet: default
# Save password to use in the future to unlock this wallet.
# Without password imported keys will not be retrievable.
# "generated_password"
# cleos wallet unlock --password generated_password
cleos wallet import --private-key your_private_key
```

### Online Tools

[JSON Minified](https://codebeautify.org/jsonminifier) [JSON Stringify](https://onlinetexttools.com/json-stringify-text) [Hashing](https://emn178.github.io/online-tools/sha256.html)

## CLEOS Commands

### Create a new issuer

Sample issuer profile:

```json
{
  "name": "TienIssuer11",
  "url": "tienissuer11.com",
  "description": "TienIssuer11 is the first test issuer of Tien",
  "email": "tiendq@gmail.com",
  "address": "LEGO City"
}
```

Minified and then stringified, then hashed SHA256:

`{"name":"TienIssuer11","url":"tienissuer11.com","description":"TienIssuer11 is the first test issuer of Tien","email":"tiendq@gmail.com","address":"LEGO City"}`

`"{\"name\":\"TienIssuer11\",\"url\":\"tienissuer11.com\",\"description\":\"TienIssuer11 is the first test issuer of Tien\",\"email\":\"tiendq@gmail.com\",\"address\":\"LEGO City\"}"`

`d6c7a7503df39d2b7f6cc4d2d5a2fbe09997ff8ef9c0a4df238a99da8fadfbd9`

```bash
# Create a new account named tienissuer11 (MUST BE 12-character long)
cleos -u http://can-testnet.com system newaccount cancreator12 tienissuer11 your_public_key_start_with_EOS your_public_key_start_with_EOS --stake-net "10.0000 CAT" --stake-cpu "10.0000 CAT" --buy-ram-kbytes 10 -p cancreator12
# Output:
# executed transaction: a2c1b90474575e637bd9d27e6eab115edf359c5aa89a2ce273f7c2c6bfc38889
# warning: transaction executed locally, but may not be confirmed by the network yet

# Register account as an issuer, using hashed issuer profile data above
cleos -u http://can-testnet.com push action badge.can regissuer '[tienissuer11, d6c7a7503df39d2b7f6cc4d2d5a2fbe09997ff8ef9c0a4df238a99da8fadfbd9]' -p tienissuer11
# Output:
# executed transaction: edd4099064cc2b8c0c2102a7e36adfc93dbf9c0653dad80aa17081396762c8d6
# badge.can <= badge.can::regissuer         {"issuer":"tienissuer11" "data":"d6c7a7503df39d2b7f6cc4d2d5a2fbe09997ff8ef9c0a4df238a99da8fadfbd9"} # tienissuer11 <= badge.can::regissuer         {"issuer":"tienissuer11","data":"d6c7a7503df39d2b7f6cc4d2d5a2fbe09997ff8ef9c0a4df238a99da8fadfbd9"}
```

Check for detail information of created issuer at [this page](https://explorer-test.canfoundation.io/account/tienissuer11).

Go to [Hyperion History API](https://history-testnet.canfoundation.io/v2/docs/index.html) to check transaction information.

Or run this command:

```bash
curl -X GET "https://history-testnet.canfoundation.io/v2/history/get_transaction?id=edd4099064cc2b8c0c2102a7e36adfc93dbf9c0653dad80aa17081396762c8d6"
```

Output:

```json
"actions": [
    {
      "action_ordinal": 1,
      "creator_action_ordinal": 0,
      "act": {
        "account": "badge.can",
        "name": "regissuer",
        "authorization": [
          {
            "actor": "tienissuer11",
            "permission": "active"
          }
        ],
        "data": {
          "issuer": "tienissuer11",
          "data": "D6C7A7503DF39D2B7F6CC4D2D5A2FBE09997FF8EF9C0A4DF238A99DA8FADFBD9"
        }
      }
    }
]
```

### Create a new badge

```bash
# Create a new badge
cleos -u http://can-testnet.com push action badge.can createbadge '[9228496215211002, tienissuer11, tienissuer11, "{\"name\":\"Badge 01\",\"url\":\"cryptobadge.app\",\"description\":\"Test badge of tienissuer11\",\"framework\":\"Blockchain\",\"code\":\"101\"}"]' -p tienissuer11
# executed transaction: efeebb37bf9fad2c771024c5e6833b1a757a1cee367f34a5e7b7f5692bcb5c7e

# Check created badge info
cleos -u http://can-testnet.com get table badge.can tienissuer11 cbadges -L 9228496215211002 -U 9228496215211002
```

### Create a new certification

Sample certification:

```json
{
  "issuedOn": "09-09-2019",
  "evidences": "By Tien Do",
  "narratives": "NodeJS Foundation",
  "expire": "09-09-2029",
  "description": "Test cert. of tienissuer11"
}
```

Minified and then stringified, then hashed SHA256:

`{"issuedOn":"09-09-2019","evidences":"By Tien Do","narratives":"NodeJS Foundation","expire":"09-09-2029","description":"Test cert. of tienissuer11"}`

`"{\"issuedOn\":\"09-09-2019\",\"evidences\":\"By Tien Do\",\"narratives\":\"NodeJS Foundation\",\"expire\":\"09-09-2029\",\"description\":\"Test cert. of tienissuer11\"}"`

`069294c4fe9ca31a88209f8b9b3ca63efc2d2d06edba763e265ddf4976ef13c4`

```bash
# Create a certification and allow user to claim it
cleos -u http://can-testnet.com push action badge.can createcert '[100, tienissuer11, issuer.can, 9228496215211002, 069294c4fe9ca31a88209f8b9b3ca63efc2d2d06edba763e265ddf4976ef13c4, true]' -p tienissuer11

# Output:
# executed transaction: ac609645c0f127a5dc3553abeb388047c423c4a0687091d4f575bcccaccbf8fe  208 bytes  2293 us
#     badge.can <= badge.can::createcert        {"certid":100,"issuer":"tienissuer11","owner":"issuer.can","badgeid":"9228496215211002","idata":"06929...
```

```bash
# Check created cert. info
cleos -u http://can-testnet.com get table badge.can badge.can offers
```
```json
{
  "rows": [{
      "certid": 100,
      "owner": "tienissuer11",
      "offeredto": "issuer.can",
      "cdate": 1574410254
    }
  ],
  "more": false
}
```

`cleos -u http://can-testnet.com get table badge.can tienissuer11 ccerts`
```json
{
  "rows": [{
      "id": 100,
      "owner": "tienissuer11",
      "issuer": "tienissuer11",
      "badgeid": "9228496215211002",
      "idata": "069294c4fe9ca31a88209f8b9b3ca63efc2d2d06edba763e265ddf4976ef13c4"
    }
  ],
  "more": false
}
```
