import { loadConfig, Blockchain } from "@klevoya/hydra";

const config = loadConfig("hydra.yml");

describe("test cryptobadge contract", () => {
  let blockchain = new Blockchain(config);
  let badgeContract = blockchain.createAccount(`badge`);
  let badgeIssuer1 = blockchain.createAccount(`issuer12.c`);
  let badgeIssuer2 = blockchain.createAccount(`issuer14.c`);

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

  describe("test create badge follow", () => {
    it("should create new badge", async () => {
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

      const badgeData = {
        issuer: badgeIssuer1.accountName,
        badge_id: 999,
        name: 'Badge to test',
        image_url: 'http://google.com/image',
        path: 'abcbadge',
        description: 'Description to test',
        criteria: 'Win the badge test',
      };

      await badgeContract.contract.createbadge(badgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      const badges = badgeContract.getTableRowsScoped(`v1.badges`)[badgeIssuer1.accountName];

      expect(badges.length).toBe(1);
      expect(Number(badges[0].badge_id)).toBe(badgeData.badge_id);
      expect(badges[0].issuer).toBe(badgeData.issuer);
      expect(Number(badges[0].version)).toBe(1);
    }, 20000);

    it("should not allow to create new badge if missing badge signature", async () => {
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

      const badgeData = {
        issuer: badgeIssuer1.accountName,
        badge_id: 999,
        name: 'Badge to test',
        image_url: 'http://google.com/image',
        path: 'abcbadge',
        description: 'Description to test',
        criteria: 'Win the badge test',
      };

      await expect(badgeContract.contract.createbadge(badgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow('missing authority of badge');
    }, 20000);

    it("should update badge", async () => {
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

      const oldBadgeData = {
        issuer: badgeIssuer1.accountName,
        badge_id: 999,
        name: 'Badge to test',
        image_url: 'http://google.com/image',
        path: 'abcbadge',
        description: 'Description to test',
        criteria: 'Win the badge test',
      };

      const updateBadgeData = {
        issuer: badgeIssuer1.accountName,
        badge_id: 999,
        name: 'Update Badge to test',
        image_url: 'http://google.com/update-----image',
        path: 'update-update-path-12345',
        description: 'updated descp to test----',
        criteria: 'updated criteria to test',
      };

      await badgeContract.contract.createbadge(oldBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      await badgeContract.contract.updatebadge(updateBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      const badges = badgeContract.getTableRowsScoped(`v1.badges`)[badgeIssuer1.accountName];

      expect(badges.length).toBe(1);
      expect(Number(badges[0].badge_id)).toBe(updateBadgeData.badge_id);
      expect(badges[0].issuer).toBe(updateBadgeData.issuer);
      expect(Number(badges[0].version)).toBe(2);
    }, 20000);

    it("should throw error if update badge with id is not exist", async () => {
      const issuerData = '8F1908A5EFF2A0ADC88F23F1A15FF0E6AF823129EDFA7C55DCE9D0C6D0ED6B0D';
      await badgeContract.contract.regissuer({
        issuer: badgeIssuer2.accountName,
        data: issuerData,
      }, [
          {
            actor: badgeIssuer2.accountName,
            permission: 'active',
          },
          {
            actor: badgeContract.accountName,
            permission: 'active',
          }
        ]
      );

      const updateBadgeData = {
        issuer: badgeIssuer2.accountName,
        badge_id: 999,
        name: 'Update Badge to test',
        image_url: 'http://google.com/update-----image',
        path: 'update-update-path-12345',
        description: 'updated descp to test----',
        criteria: 'updated criteria to test',
      };

      await expect(badgeContract.contract.updatebadge(updateBadgeData, [
        {
          actor: badgeIssuer2.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ])).rejects.toThrow('badge does not exist');
    }, 20000);
  });
});


