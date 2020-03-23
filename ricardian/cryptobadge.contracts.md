<h1 class="contract">regissuer</h1>

---
spec_version: "0.1.0"
title: Register Issuer
summary: 'Register {{nowrap issuer}} as a issuer of cryptobadge'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{issuer}} agrees to be the issuer of crypto badge and save the hash data {{data}} to the blockchain.

<h1 class="contract">updateissuer</h1>

---
spec_version: "0.1.0"
title: Update Issuer
summary: 'Update infomation of issuer {{nowrap issuer}}'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{issuer}} agrees to update their data with new hash {{data}} to the blockchain.

<h1 class="contract">createbadge</h1>

---
spec_version: "0.1.0"
title: Create Badge
summary: 'Issuer {{nowrap issuer}} create new badge'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{issuer}} agrees to create new badge with the following data: 1. badge name: {{name}}, 2. badge version {{version}}, 3. image_url {{image_url}}, 4. path {{path}}, 5. description {{description}}, 6. criteria {{criteria}}


<h1 class="contract">updatebadge</h1>

---
spec_version: "0.1.0"
title: Update Badge
summary: 'Update badge with id {{badge_id}} of issuer {{nowrap issuer}}'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{issuer}} agrees to update badge with id {{badge_id}} with the following data: 1. version {{version}}, 2. name {{name}}, 3. image_url {{image_url}}, 4. path {{path}}, 5. description {{description}}, 6. criteria {{criteria}}

<h1 class="contract">issuebadge</h1>

---
spec_version: "0.1.0"
title: Create certification
summary: 'Create new certification'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{issuer}} agrees to create new certification with badge id {{badge_id}} for owner {{owner}} with the enrypted data {{idata}} and require claim option {{require_claim}}.

<h1 class="contract">claimcert</h1>

---
spec_version: "0.1.0"
title: Claim Certifications
summary: 'User claim certifications with specified ids'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{claimer}} agrees to claim certifications with id {{cert_ids}}.

<h1 class="contract">canceloffer</h1>

---
spec_version: "0.1.0"
title: Cancel and remove offer
summary: 'Cancel and remove offer'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{claimer}} agrees to cancel offer for receiving certifications with id {{cert_ids}}.

<h1 class="contract">removecert</h1>

---
spec_version: "0.1.0"
title: Remove certifications
summary: 'Remove certifications'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{owner}} agrees to remove the certications with id {{cert_ids}} because the reason {{memo}}.

<h1 class="contract">attach</h1>

---
spec_version: "0.1.0"
title: Attach Certification Detail
summary: 'Attach certification detail'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{owner}} agrees to attach the detail {{data}} of certication with id {{cert_id}}.

<h1 class="contract">detach</h1>

---
spec_version: "0.1.0"
title: Detach Certification Detail
summary: 'Detach certification detail'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{owner}} agrees to detach the detail of certication with id {{cert_ids}}.

<h1 class="contract">createlog</h1>

---
spec_version: "0.1.0"
title: Create log
summary: 'Create log'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{issuer}} agrees to create log.

<h1 class="contract">revokecert</h1>

---
spec_version: "0.1.0"
title: Revoke Certification
summary: 'Revoke Certification'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{issuer}} agrees to revoke certification with ID {{cert_id}} of {{owner}}.

<h1 class="contract">expirecert</h1>

---
spec_version: "0.1.0"
title: Update Certification state to expire
summary: 'Update Certification state to expire'
icon: https://ddnb1wizcm1i5.cloudfront.net/fd91d4bdbcfbb8fc528b0cc8d7b11fa5.svg#a4148806780c5b0a2ed0e1e44a36b1947dd932f653184964d9e44e5dae084a9f
---

{{updater}} update certification with ID {{cert_id}} of {{owner}} to expired.



