Kickpass
========

Kickpass is a stupid simple password safe. It keep each password in a specific
safe, protected with modern cryptography. Its main user interface is command
line.

Full documentation is available throught manual page ``kickpass(1)``:
```bash
man 1 kickpass
```

Technical overview
------------------

Kickpass is built around a shared library named libkickpass.

libkickpass leverage [https://github.com/jedisct1/libsodium/](libsodium) to
create safes.

Safes are created using authenticated encryption with associated data. As of
now libkickpass use chacha20 along with poly1305 to encrypt and authenticate
the safe.