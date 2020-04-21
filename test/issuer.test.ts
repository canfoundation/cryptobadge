import { loadConfig, Blockchain } from "@klevoya/hydra";

const config = loadConfig("hydra.yml");

describe("test cryptobadge contract", () => {
  let blockchain = new Blockchain(config);
  let badgeContract = blockchain.createAccount(`badge`);
  let badgeIssuer1 = blockchain.createAccount(`issuer12.c`);

  beforeAll(async () => {
    badgeContract.setContract(blockchain.contractTemplates[`cryptobadge`]);
    badgeContract.updateAuth(`active`, `owner`, {
      accounts: [
        {
          permission: {
            actor: badgeContract.accountName,
            permission: `eosio.code`
          },
          weight: 1
        }
      ]
    });
  });

  beforeEach(async () => {
    badgeContract.resetTables();
  });

  describe("test issuer follow", () => {
    it("should register issuer", async () => {
      const issuerData = '8F1908A5EFF2A0ADC88F23F1A15FF0E6AF823129EDFA7C55DCE9D0C6D0ED6B0D';
      await badgeContract.contract.regissuer({
        issuer: badgeIssuer1.accountName,
        data: issuerData,
      }, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
        ]
      );

      const issuers = badgeContract.getTableRowsScoped(`v1.issuers`)[`badge`];

      expect(issuers.length).toBe(1);
      expect(issuers[0].issuer).toBe(badgeIssuer1.accountName);
      expect(issuers[0].data).toBe(issuerData);
    });

    it("should update issuer", async () => {
      const oldIssuerData = '8F1908A5EFF2A0ADC88F23F1A15FF0E6AF823129EDFA7C55DCE9D0C6D0ED6B0D';
      const newIssuerData = '0D11D0682B491AE9140BC38F3BCAF16842D4445A83FA6BBCA6103A9DFA459468';
      await badgeContract.contract.regissuer({
        issuer: badgeIssuer1.accountName,
        data: oldIssuerData,
      }, [
          {
            actor: badgeIssuer1.accountName,
            permission: 'active',
          },
          {
            actor: badgeContract.accountName,
            permission: 'active',
          }
        ]
      );

      await badgeContract.contract.updateissuer({
        issuer: badgeIssuer1.accountName,
        data: newIssuerData,
      }, [
          {
            actor: badgeIssuer1.accountName,
            permission: 'active',
          },
          {
            actor: badgeContract.accountName,
            permission: 'active',
          }
        ]
      );

      const issuers = badgeContract.getTableRowsScoped(`v1.issuers`)[`badge`];

      expect(issuers.length).toBe(1);
      expect(issuers[0].issuer).toBe(badgeIssuer1.accountName);
      expect(issuers[0].data).toBe(newIssuerData);
    }, 10000);
  });
});


