import { loadConfig, Blockchain } from "@klevoya/hydra";

const config = loadConfig("hydra.yml");

describe("test cryptobadge contract", () => {
  let blockchain = new Blockchain(config);
  let badgeContract = blockchain.createAccount(`badge`);
  let badgeIssuer1 = blockchain.createAccount(`issuer12.c`);
  let badgeIssuer2 = blockchain.createAccount(`issuer14.c`);
  let certificateReceiver = blockchain.createAccount(`receiver11`);

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

  describe("test issue badge follow", () => {
    it("should not allowed to issue new badge successfully", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: false,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      const badges = badgeContract.getTableRowsScoped(`v1.certs`)[certificateReceiver.accountName];

      expect(badges.length).toBe(1);
      expect(Number(badges[0].badge_id)).toBe(issueBadgeData.badge_id);
      expect(Number(badges[0].badge_revision)).toBe(issueBadgeData.badge_revision);
      expect(Number(badges[0].id)).toBe(issueBadgeData.cert_id);
      expect(Number(badges[0].expire_at)).toBe(issueBadgeData.expire_at);

    }, 20000);

    it("should not allowed to issue new badge if missing badge signature", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: false,
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

      await expect(badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow('missing authority of badge');
    }, 20000);

    it("should not allowed to issue new badge if missing badge signature", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: '',
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: true,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      const badges = badgeContract.getTableRowsScoped(`v1.issuing`)[badgeIssuer1.accountName];

      expect(badges.length).toBe(1);
      expect(Number(badges[0].badge_id)).toBe(issueBadgeData.badge_id);
      expect(Number(badges[0].badge_revision)).toBe(issueBadgeData.badge_revision);
      expect(Number(badges[0].id)).toBe(issueBadgeData.cert_id);
      expect(Number(badges[0].expire_at)).toBe(issueBadgeData.expire_at);
    }, 20000);

    it("should owner claim badge", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: '',
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: true,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      await badgeContract.contract.claimbadge({
        claimer: certificateReceiver.accountName,
        issuer: badgeIssuer1.accountName,
        cert_id: 1010,
      }, [{
          actor: certificateReceiver.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }]);

      const badges = badgeContract.getTableRowsScoped(`v1.certs`)[certificateReceiver.accountName];

      expect(badges.length).toBe(1);
      expect(Number(badges[0].badge_id)).toBe(issueBadgeData.badge_id);
      expect(Number(badges[0].badge_revision)).toBe(issueBadgeData.badge_revision);
      expect(Number(badges[0].id)).toBe(issueBadgeData.cert_id);
      expect(Number(badges[0].expire_at)).toBe(issueBadgeData.expire_at);
    }, 20000);

    it("should not allowed to issue new badge if missing badge signature", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: false,
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

      await expect(badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow('missing authority of badge');
    }, 20000);

    it("should not allowed to issue new badge if missing issuer signature", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: false,
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

      await expect(badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeContract.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow(`missing authority of ${badgeIssuer1.accountName}`);
    }, 20000);

    it("should not allowed to issue new badge if owner is can account and require claim is true", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: true,
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

      await expect(badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeContract.accountName,
          permission: 'active',
        },
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow(`require_claim only for non CAN account`);
    }, 20000);

    it("should not allowed to issue new badge if issuer does not exist", async () => {
      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: 999,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: false,
      };

      await expect(badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeContract.accountName,
          permission: 'active',
        },
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow(`issuer does not exist`);
    });

    it("should not allowed to issue new badge if badge is not exist", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: 999,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: false,
      };

      await expect(badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeContract.accountName,
          permission: 'active',
        },
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow(`badge does not exist`);
    }, 20000);

    it("should not allowed to issue new badge if cert id's already existed", async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1744863519,
        require_claim: false,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeContract.accountName,
          permission: 'active',
        },
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ]);

      await expect(badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeContract.accountName,
          permission: 'active',
        },
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
      ])).rejects.toThrow(`cert already exist`);
    }, 20000);

    it('should expire certification', async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 946684000, // Friday, 31 December 1999 23:46:40
        require_claim: false,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      await badgeContract.contract.expirecert(
        {
          updater: badgeIssuer1.accountName,
          cert_id: issueBadgeData.cert_id,
          owner: certificateReceiver.accountName,
        },[{
          actor: badgeIssuer1.accountName,
          permission: 'active',
        }]
      );

      const badges = badgeContract.getTableRowsScoped(`v1.certs`)[certificateReceiver.accountName];

      expect(badges.length).toBe(1);
      expect(Number(badges[0].badge_id)).toBe(issueBadgeData.badge_id);
      expect(Number(badges[0].badge_revision)).toBe(issueBadgeData.badge_revision);
      expect(Number(badges[0].state)).toBe(1);
      expect(Number(badges[0].expire_at)).toBe(issueBadgeData.expire_at);
    }, 20000);

    it('should not expire certification if expire_at greater than now', async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1902640333, // Wednesday, 17 April 2030 07:12:13
        require_claim: false,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      await expect(badgeContract.contract.expirecert(
        {
          updater: badgeIssuer1.accountName,
          cert_id: issueBadgeData.cert_id,
          owner: certificateReceiver.accountName,
        },[{
          actor: badgeIssuer1.accountName,
          permission: 'active',
        }]
      )).rejects.toThrowError('Certificate has not been expired');
    }, 20000);

    it('should revoke certification', async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1902640333, // Wednesday, 17 April 2030 07:12:13
        require_claim: false,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      await badgeContract.contract.revokecert(
        {
          issuer: badgeIssuer1.accountName,
          cert_id: issueBadgeData.cert_id,
          owner: certificateReceiver.accountName,
          reason: 'I like it',
        },[
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

      const badges = badgeContract.getTableRowsScoped(`v1.certs`)[certificateReceiver.accountName];

      expect(badges.length).toBe(1);
      expect(Number(badges[0].badge_id)).toBe(issueBadgeData.badge_id);
      expect(Number(badges[0].badge_revision)).toBe(issueBadgeData.badge_revision);
      expect(Number(badges[0].state)).toBe(2);
    }, 20000);

    it('should not revoke cert if revoker is not the issuer', async () => {
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

      const issueBadgeData = {
        issuer: badgeIssuer1.accountName,
        owner: certificateReceiver.accountName,
        badge_id: badgeData.badge_id,
        badge_revision: 1,
        cert_id: 1010,
        encrypted_data: 'af65fbb2cc8b04b6d9bfd26bfc4d3ed842fb18016a3b3937a25d20afcfbe9f9c',
        expire_at: 1902640333, // Wednesday, 17 April 2030 07:12:13
        require_claim: false,
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

      await badgeContract.contract.issuebadge(issueBadgeData, [
        {
          actor: badgeIssuer1.accountName,
          permission: 'active',
        },
        {
          actor: badgeContract.accountName,
          permission: 'active',
        }
      ]);

      await expect(badgeContract.contract.revokecert(
        {
          issuer: badgeIssuer2.accountName,
          cert_id: issueBadgeData.cert_id,
          owner: certificateReceiver.accountName,
          reason: 'I like it',
        },[
          {
            actor: badgeIssuer2.accountName,
            permission: 'active',
          },
          {
            actor: badgeContract.accountName,
            permission: 'active',
          }
        ]
      )).rejects.toThrowError('certificate\'s issuer is invalid');
    }, 20000);
  });
});


