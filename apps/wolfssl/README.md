# wolfSSL Embedded SSL/TLS Library

The [wolfSSL embedded SSL library](https://www.wolfssl.com/products/wolfssl/) 
(formerly CyaSSL) is a lightweight SSL/TLS library written in ANSI C and
targeted for embedded, RTOS, and resource-constrained environments - primarily
because of its small size, speed, and feature set.  It is commonly used in
standard operating environments as well because of its royalty-free pricing
and excellent cross platform support. wolfSSL supports industry standards up
to the current [TLS 1.3](https://www.wolfssl.com/tls13) and DTLS 1.3, is up to
20 times smaller than OpenSSL, and offers progressive ciphers such as ChaCha20,
Curve25519, Blake2b and Post-Quantum TLS 1.3 groups. User benchmarking and
feedback reports dramatically better performance when using wolfSSL over
OpenSSL.

wolfSSL is powered by the wolfCrypt cryptography library. Two versions of
wolfCrypt have been FIPS 140-2 validated (Certificate #2425 and
certificate #3389). FIPS 140-3 validation is in progress. For additional
information, visit the [wolfCrypt FIPS FAQ](https://www.wolfssl.com/license/fips/)
or contact fips@wolfssl.com.

## Why Choose wolfSSL?

There are many reasons to choose wolfSSL as your embedded, desktop, mobile, or
enterprise SSL/TLS solution. Some of the top reasons include size (typical
footprint sizes range from 20-100 kB), support for the newest standards
(SSL 3.0, TLS 1.0, TLS 1.1, TLS 1.2, TLS 1.3, DTLS 1.0, DTLS 1.2, and DTLS 1.3),
current and progressive cipher support (including stream ciphers), multi-platform,
royalty free, and an OpenSSL compatibility API to ease porting into existing
applications which have previously used the OpenSSL package. For a complete
feature list, see [Chapter 4](https://www.wolfssl.com/docs/wolfssl-manual/ch4/)
of the wolfSSL manual.

## Notes, Please Read

### Note 1
wolfSSL as of 3.6.6 no longer enables SSLv3 by default.  wolfSSL also no longer
supports static key cipher suites with PSK, RSA, or ECDH. This means if you
plan to use TLS cipher suites you must enable DH (DH is on by default), or
enable ECC (ECC is on by default), or you must enable static key cipher suites
with one or more of the following defines:

```
WOLFSSL_STATIC_DH
WOLFSSL_STATIC_RSA
WOLFSSL_STATIC_PSK
```
Though static key cipher suites are deprecated and will be removed from future
versions of TLS.  They also lower your security by removing PFS.

When compiling `ssl.c`, wolfSSL will now issue a compiler error if no cipher
suites are available. You can remove this error by defining
`WOLFSSL_ALLOW_NO_SUITES` in the event that you desire that, i.e., you're
not using TLS cipher suites.

### Note 2
wolfSSL takes a different approach to certificate verification than OpenSSL
does. The default policy for the client is to verify the server, this means
that if you don't load CAs to verify the server you'll get a connect error,
no signer error to confirm failure (-188).

If you want to mimic OpenSSL behavior of having `SSL_connect` succeed even if
verifying the server fails and reducing security you can do this by calling:

```c
wolfSSL_CTX_set_verify(ctx, WOLFSSL_VERIFY_NONE, NULL);
```

before calling `wolfSSL_new();`. Though it's not recommended.

### Note 3
The enum values SHA, SHA256, SHA384, SHA512 are no longer available when
wolfSSL is built with `--enable-opensslextra` (`OPENSSL_EXTRA`) or with the
macro `NO_OLD_SHA_NAMES`. These names get mapped to the OpenSSL API for a
single call hash function. Instead the name `WC_SHA`, `WC_SHA256`, `WC_SHA384` and
`WC_SHA512` should be used for the enum name.


# wolfSSL Release 5.6.6 (Dec 19, 2023)

Release 5.6.6 has been developed according to wolfSSL's development and QA
process (see link below) and successfully passed the quality criteria.
https://www.wolfssl.com/about/wolfssl-software-development-process-quality-assurance

NOTE: * --enable-heapmath is being deprecated and will be removed by 2024

REMINDER: When working with AES Block Cipher algorithms, `wc_AesInit()` should
always be called first to initialize the `Aes` structure, before calling other
Aes API functions. Recently we found several places in our documentation,
comments, and codebase where this pattern was not observed. We have since
fixed this omission in several PRs for this release.

## Vulnerabilities

* [Medium] CVE-2023-6935: After review of the previous RSA timing fix in wolfSSL 5.6.4, additional changes were found to be required. A complete resistant change is delivered in this release. This fix is for the Marvin attack, leading to being able to decrypt a saved TLS connection and potentially forge a signature after probing with a very large number of trial connections. This issue is around RSA decryption and affects the optional static RSA cipher suites on the server side, which are considered weak, not recommended to be used and are off by default in wolfSSL (even with `--enable-all`). Static RSA cipher suites were also removed from the TLS 1.3 protocol and are only present in TLS 1.2 and lower. All padding versions of RSA decrypt are affected since the code under review is outside of the padding processing. Information about the private keys is NOT compromised in affected code. It is recommended to disable static RSA cipher suites and update the version of wolfSSL used if using RSA private decryption alone outside of TLS. Thanks to Hubert Kario for the report. The fix for this issue is located in the following GitHub Pull Request: https://github.com/wolfSSL/wolfssl/pull/6955.

* [Low] CVE-2023-6936: A potential heap overflow read is possible in servers connecting over TLS 1.3 when the optional `WOLFSSL_CALLBACKS` has been defined. The out of bounds read can occur when a server receives a malicious malformed ClientHello. Users should either discontinue use of `WOLFSSL_CALLBACKS` on the server side or update versions of wolfSSL to 5.6.6. Thanks to the tlspuffin fuzzer team for the report which was designed and developed by; Lucca Hirschi (Inria, LORIA), Steve Kremer (Inria, LORIA), and Max Ammann (Trail of Bits). The fix for this issue is located in the following GitHub Pull Request: https://github.com/wolfSSL/wolfssl/pull/6949.

* [Low] A side channel vulnerability with AES T-Tables is possible in a very controlled environment where precision sub-cache-line inspection can happen, such as inside an Intel SGX enclave. This can lead to recovery of the AES key. To prevent this type of attack, wolfSSL added an AES bitsliced implementation which can be enabled with the “`--enable-aes-bitsliced`” configure option. Thanks to Florian Sieck, Zhiyuan Zhang, Sebastian Berndt, Chitchanok Chuengsatiansup, Thomas Eisenbarth, and Yuval Yarom for the report (Universities of Lübeck, Melbourne, Adelaide and Bochum). The fix for this issue is located in the following GitHub Pull Request: https://github.com/wolfSSL/wolfssl/pull/6854.

* [Low] CVE-2023-6937: wolfSSL prior to 5.6.6 did not check that messages in a single (D)TLS record do not span key boundaries. As a result, it was possible to combine (D)TLS messages using different keys into one (D)TLS record. The most extreme edge case is that, in (D)TLS 1.3, it was possible that an unencrypted (D)TLS 1.3 record from the server containing first a ServerHello message and then the rest of the first server flight would be accepted by a wolfSSL client. In (D)TLS 1.3 the handshake is encrypted after the ServerHello but a wolfSSL client would accept an unencrypted flight from the server. This does not compromise key negotiation and authentication so it is assigned a low severity rating. Thanks to Johannes Wilson for the report (Sectra Communications and Linköping University). The fix for this issue is located in the following GitHub Pull Request: https://github.com/wolfSSL/wolfssl/pull/7029.

## New Feature Additions

* Build option for disabling CRL date checks (`WOLFSSL_NO_CRL_DATE_CHECK`) (PR 6927)
* Support for STM32WL55 and improvements to PKA ECC support (PR 6937)
* Add option to skip cookie exchange on DTLS 1.3 session resumption (PR 6929)
* Add implementation of SRTP KDF and SRTCP KDF (`--enable-srtp-kdf`) (PR 6888)
* Add `wolfSSL_EXTENDED_KEY_USAGE_free()` (PR 6916)
* Add AES bitsliced implementation that is cache attack safe (`--enable-aes-bitsliced`) (PR 6854)
* Add memcached support and automated testing (PR 6430, 7022)
* Add Hardware Encryption Acceleration for ESP32-C3, ESP32-C6, and ESP32-S2 (PR 6990)
* Add (D)TLS 1.3 support for 0.5-RTT data (PR 7010)

## Enhancements and Optimizations

* Better built in testing of “`--sys-ca-certs`” configure option (PR 6910)
* Updated CMakeLists.txt for Espressif wolfSSL component usage (PR 6877)
* Disable TLS 1.1 by default (unless SSL 3.0 or TLS 1.0 is enabled) (PR 6946)
* Add “`--enable-quic`” to “`--enable-all`” configure option (PR 6957)
* Add support to SP C implementation for RSA exponent up to 64-bits (PR 6959)
* Add result of “`HAVE___UINT128_T`” to options.h for CMake builds (PR 6965)
* Add optimized assembly for AES-GCM on ARM64 using hardware crypto instructions (PR 6967)
* Add built-in cipher suite tests for DTLS 1.3 PQC (PR 6952)
* Add wolfCrypt test and unit test to ctest (PR 6977)
* Move OpenSSL compatibility crypto APIs into `ssl_crypto.c` file (PR 6935)
* Validate time generated from XGMTIME() (PR 6958)
* Allow wolfCrypt benchmark to run with microsecond accuracy (PR 6868)
* Add GitHub Actions testing with nginx 1.24.0 (PR 6982)
* Allow encoding of CA:FALSE BasicConstraint during cert generation (PR 6953)
* Add CMake option to enable DTLS-SRTP (PR 6991)
* Add CMake options for enabling QUIC and cURL (PR 7049)
* Improve RSA blinding to make code more constant time (PR 6955)
* Refactor AES-NI implementation macros to allow dynamic fallback to C (PR 6981)
* Default to native Windows threading API on MinGW (PR 7015)
* Return better error codes from OCSP response check (PR 7028)
* Updated Espressif ESP32 TLS client and server examples (PR 6844)
* Add/clean up support for ESP-IDF v5.1 for a variety of ESP32 chips (PR 7035, 7037)
* Add API to choose dynamic certs based on client ciphers/sigalgs (PR 6963)
* Improve Arduino IDE 1.5 project file to match recursive style (PR 7007)
* Simplify and improve apple-universal build script (PR 7025)

## Fixes

* Fix for async edge case with Intel QuickAssist/Cavium Nitrox (PR 6931)
* Fix for building PKCS#7 with RSA disabled (PR 6902)
* Fix for advancing output pointer in `wolfSSL_i2d_X509()` (PR 6891)
* Fix for `EVP_EncodeBlock()` appending a newline (PR 6900)
* Fix for `wolfSSL_RSA_verify_PKCS1_PSS()` with `RSA_PSS_SALTLEN_AUTO` (PR 6938)
* Fixes for CODESonar reports around `isalpha()` and `isalnum()` calls (PR 6810)
* Fix for SP ARM64 integer math to avoid compiler optimization issues (PR 6942)
* Fix for SP Thumb2 inline assembly to add IAR build support (PR 6943, 6971)
* Fix for SP Thumb2 to make functions not inlined (PR 6993)
* Fix for SP Cortex-M assembly large build with IAR (PR 6954)
* Fix for SP ARM64 assembly montgomery reduction by 4 (PR 6947)
* Fix for SP ARM64 P-256 for not inlining functions for iOS compatibility (PR 6979)
* Fix for `WOLFSSL_CALLBACKS` and potential memory error (PR 6949)
* Fixes for wolfSSL’s Zephyr OS port (PR 6930)
* Fix for build errors when building for NXP mmCAU (`FREESCALE_MMCAU`) (PR 6970)
* Fix for TLS 1.3 `SendBuffered()` return code in non-blocking mode (PR 7001)
* Fix for TLS `Hmac_UpdateFinal()` when padding byte is invalid (PR 6998)
* Fix for ARMv8 AES-GCM streaming to check size of IV before storing (PR 6996)
* Add missing calls to `wc_AesInit()` before `wc_AesSetKey()` (PR 7011)
* Fix build errors with DTLS 1.3 enabled but TLS 1.2 disabled (PR 6976)
* Fixes for building wolfSSL in Visual Studio (PR 7040)

For additional vulnerability information visit the vulnerability page at:
https://www.wolfssl.com/docs/security-vulnerabilities/

See INSTALL file for build instructions.
More info can be found on-line at: https://wolfssl.com/wolfSSL/Docs.html

# Resources

[wolfSSL Website](https://www.wolfssl.com/)

[wolfSSL Wiki](https://github.com/wolfSSL/wolfssl/wiki)

[FIPS 140-2/140-3 FAQ](https://wolfssl.com/license/fips)

[wolfSSL Documentation](https://wolfssl.com/wolfSSL/Docs.html)

[wolfSSL Manual](https://wolfssl.com/wolfSSL/Docs-wolfssl-manual-toc.html)

[wolfSSL API Reference](https://wolfssl.com/wolfSSL/Docs-wolfssl-manual-17-wolfssl-api-reference.html)

[wolfCrypt API Reference](https://wolfssl.com/wolfSSL/Docs-wolfssl-manual-18-wolfcrypt-api-reference.html)

[TLS 1.3](https://www.wolfssl.com/docs/tls13/)

[wolfSSL Vulnerabilities](https://www.wolfssl.com/docs/security-vulnerabilities/)

[Additional wolfSSL Examples](https://github.com/wolfssl/wolfssl-examples)
