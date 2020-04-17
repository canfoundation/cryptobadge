import { loadConfig, Blockchain } from "@klevoya/hydra";

const config = loadConfig("hydra.yml");

describe("test cryptobadge contract", () => {
  let blockchain = new Blockchain(config);
  let badgeContract = blockchain.createAccount(`badge`);

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

  it("should set contract configuration", async () => {
    await badgeContract.contract.setconfig({
      ram_payer_account: 'ram.can',
      governance_design: "gov11111111",
    }, [{
      actor: 'badge',
      permission: 'active',
    }]);

    const globalConfig = badgeContract.getTableRowsScoped(`v1.global`)[`badge`];

    expect(globalConfig[0].governance_design).toBe('gov11111111');
    expect(globalConfig[0].ram_payer_account).toBe('ram.can');
  });
});


