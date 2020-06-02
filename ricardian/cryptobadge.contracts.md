<h1 class="contract">regissuer</h1>

---
spec_version: "2.0.0"
title: Register Issuer
summary: 'Register {{nowrap issuer}} as a issuer of cryptobadge'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} agrees to be the issuer of crypto badge and save the hash data {{data}} to the blockchain.

<h1 class="contract">updateissuer</h1>

---
spec_version: "2.0.0"
title: Update Issuer
summary: 'Update infomation of issuer {{nowrap issuer}}'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} agrees to update their data with new hash {{data}} to the blockchain.

<h1 class="contract">createpropos</h1>

---
spec_version: "2.0.0"
title: Create proposal
summary: 'Crypto badge aggree to create new create/update/issue proposal'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

Crypto badge agrees to create proposal to execute action {{action}} with packed parameters {{packed_params}} and name {{proposal_name}}

<h1 class="contract">approvepropo</h1>

---
spec_version: "2.0.0"
title: Approve proposal
summary: 'Approver approve create/update/issue proposal'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{approver}} approve the proposal of {{issuer}} with name {{proposal_name}}

<h1 class="contract">executepropo</h1>

---
spec_version: "2.0.0"
title: Execute proposal
summary: 'Issuer execute proposal'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} approve to execute proposal with name {{proposal_name}}

<h1 class="contract">createbadge</h1>

---
spec_version: "2.0.0"
title: Create Badge
summary: 'Issuer {{nowrap issuer}} create new badge'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} agrees to create new badge with the following data: 1. badge name: {{name}}, 2. badge version {{version}}, 3. image_url {{image_url}}, 4. path {{path}}, 5. description {{description}}, 6. criteria {{criteria}}


<h1 class="contract">updatebadge</h1>

---
spec_version: "2.0.0"
title: Update Badge
summary: 'Update badge with id {{badge_id}} of issuer {{nowrap issuer}}'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} agrees to update badge with id {{badge_id}} with the following data: 1. version {{version}}, 2. name {{name}}, 3. image_url {{image_url}}, 4. path {{path}}, 5. description {{description}}, 6. criteria {{criteria}}

<h1 class="contract">issuebadge</h1>

---
spec_version: "2.0.0"
title: Create certification
summary: 'Create new certification'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} agrees to create new certification with badge id {{badge_id}} for owner {{owner}} with the enrypted data {{idata}} and require claim option {{require_claim}}.

<h1 class="contract">claimbadge</h1>

---
spec_version: "2.0.0"
title: Claim Certifications
summary: 'User claim certifications with specified ids'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{claimer}} agrees to claim certifications with id {{cert_ids}}.

<h1 class="contract">canceloffer</h1>

---
spec_version: "2.0.0"
title: Cancel and remove offer
summary: 'Cancel and remove offer'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{claimer}} agrees to cancel offer for receiving certifications with id {{cert_ids}}.

<h1 class="contract">removecert</h1>

---
spec_version: "2.0.0"
title: Remove certifications
summary: 'Remove certifications'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{owner}} agrees to remove the certications with id {{cert_ids}} because the reason {{memo}}.

<h1 class="contract">attach</h1>

---
spec_version: "2.0.0"
title: Attach Certification Detail
summary: 'Attach certification detail'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{owner}} agrees to attach the detail {{data}} of certication with id {{cert_id}}.

<h1 class="contract">detach</h1>

---
spec_version: "2.0.0"
title: Detach Certification Detail
summary: 'Detach certification detail'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{owner}} agrees to detach the detail of certication with id {{cert_ids}}.

<h1 class="contract">createlog</h1>

---
spec_version: "2.0.0"
title: Create log
summary: 'Create log'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} agrees to create log.

<h1 class="contract">revokecert</h1>

---
spec_version: "2.0.0"
title: Revoke Certification
summary: 'Revoke Certification'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} agrees to revoke certification with ID {{cert_id}} of {{owner}}.

<h1 class="contract">expirecert</h1>

---
spec_version: "2.0.0"
title: Update Certification state to expire
summary: 'Update Certification state to expire'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{contract onwer}} config system envirement.

<h1 class="contract">setconfig</h1>

---
spec_version: "2.0.0"
title: Config system envirement
summary: 'Set/update system envirement'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---

{{issuer}} log event about issued transaction_id to {{owner}}.

<h1 class="contract">issuelog</h1>

---
spec_version: "2.0.0"
title: Log event when owner claim the badge
summary: 'Log event when owner claim the badge'
icon: https://d1i5zj4k49u5j1.cloudfront.net/ed8c2744ba1d0c21c8b3f3542229fbb6.svg
---


