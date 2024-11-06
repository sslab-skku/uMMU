# Setup
Add `$HOME/.local/musl/bin` to the `PATH` variable.

```
./setup.sh
```

# Build CMI pass & libs
```
cd llvm-pass
./build.sh
cd -
cd cmi/
./build.sh
cd -
```

# Build CMI mbedtls app
```
cd apps/mbedtls
./build.sh
cd -
```

# Test
## mbedtls ssl_server2
```
tests/test-ssl_server2.sh zmm
```
