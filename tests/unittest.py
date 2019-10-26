import sys
from eosfactory.eosf import *
import eosfactory.core.setup as setup
verbosity([Verbosity.INFO, Verbosity.OUT, Verbosity.DEBUG])

CONTRACT_WORKSPACE = sys.path[0] + "/../"

# Actors of the test:
MASTER = MasterAccount()
HOST = Account()
ALICE = Account()
BOB = Account()
CAROL = Account()
ISSUER = Account()

def test():
    SCENARIO('''
    Initialize the token and run a couple of transfers between different accounts.
    ''')
    reset()
    create_master_account("MASTER")

    COMMENT('''
    Build & deploy the contract:
    ''')
    create_account("HOST", MASTER)
    smart = Contract(HOST, CONTRACT_WORKSPACE)
    smart.build(force=False)
    smart.deploy()

    COMMENT('''
    Create test accounts:
    ''')
    create_account("ALICE", MASTER)
    create_account("BOB", MASTER)
    create_account("CAROL", MASTER)
    create_account("ISSUER", MASTER)

    COMMENT('''
    MASTER.set_account_permission()
    ''')
    HOST.set_account_permission(
        Permission.ACTIVE, {
            "threshold":
                1,
            "keys": [],
            "accounts": [{
                "permission": {
                    "actor": str(HOST),
                    "permission": "active"
                },
                "weight": 1
            }, {
                "permission": {
                    "actor": str(HOST),
                    "permission": "eosio.code"
                },
                "weight": 1
            }]
        }, Permission.OWNER, (HOST, Permission.OWNER))

    COMMENT('''
    Register/update issuer information and create badge:
    ''')    

    HOST.push_action(
        "regissuer",
        {
            "issuer": ISSUER,
            "data": "74e202709e5e36075b72f23e6b7c253d998685014571ba10e362cc634ffdac18"
        },
        permission=(ISSUER, Permission.ACTIVE))

    HOST.push_action(
        "updateissuer",
        {
            "issuer": ISSUER,
            "data": "d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907"
        },
        permission=(ISSUER, Permission.ACTIVE))

    HOST.push_action(
        "createbadge",
        {
            "issuer": ISSUER,
            "owner": ISSUER,
            "idata": "525843a99dfe43dc3e0da059e9d400490d6bfc322593bb95926a71ec5b1feadb",
            "mdata": "d15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907"
        },
        permission=(ISSUER, Permission.ACTIVE))
    table_issuer = HOST.table("issuers", HOST)

    table_badge = HOST.table("cbadges", ISSUER)
    badge_id = table_badge.json["rows"][0]["badgeid"]

    COMMENT('''
    create Certification and issue to user or user should claim it
    ''')

    HOST.push_action(
        "createcert",
        {
            "issuer": ISSUER, "owner": ALICE,
            "badgeid": badge_id, 
            "idata":"8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691",
            "requireclaim": 0
        },
        permission=(ISSUER, Permission.ACTIVE))

    HOST.push_action(
        "createcert",
        {
            "issuer": ISSUER, "owner": CAROL,
            "badgeid": badge_id, 
            "idata":"8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691",
            "requireclaim": 1
        },
        permission=(ISSUER, Permission.ACTIVE))

    table_cert = HOST.table("ccerts", ISSUER)
    cert_id = table_cert.json["rows"][0]["id"]

    HOST.push_action(
        "claimcert",
        {
            "claimer": CAROL, "certids": [cert_id], 
        },
        permission=(CAROL, Permission.ACTIVE))


    COMMENT('''
    attach detail certification
    ''')
    cert_data = "{\"issuedOn\":\"01-01-2011\",\"evidences\":\"By 123\",\"narratives\":\"Can foundation\",\"expire\":\"01-01-2021\",\"description\":\"Blockchain Company\"}"
    HOST.push_action(
        "attach",
        {
            "owner": CAROL,
            "certid": cert_id, 
            "data":cert_data
        },
        permission=(CAROL, Permission.ACTIVE))

    table_cert_detail = HOST.table("ccertinfos", CAROL)
    cert_detail = table_cert_detail.json["rows"][0]["cert_id"]

    COMMENT('''
    Verify the outcome:
    ''')

    assert(table_issuer.json["rows"][0]["data"] == 'd15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907')

    assert(table_badge.json["rows"][0]["mdata"] == 'd15abff66e1d0c0705249a621dac4ab6897b54369c7cd6157ac8800973115907')

    assert(table_cert.json["rows"][0]["idata"] == '8032553af2ff539027eb9fb4eaaa731a5574811badf2dc01d5db4bee64976691')

    assert(table_cert_detail.json["rows"][0]["cert_content"] == cert_data)

    stop()


if __name__ == "__main__":
    test()
