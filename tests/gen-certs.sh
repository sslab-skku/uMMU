#!/bin/bash
cd "$(dirname "$0")"
mkdir -p data
cd data

CN=localhost

# Generate Root CA
if [ ! -f ca.key ];then
    openssl genrsa -out ca.key 2048
    openssl req -new -x509 -days 10000 -key ca.key -out ca.crt
fi
# chmod 600 ca.key ca.crt

# Generate CSR
openssl genrsa -out host.key 2048
openssl req -new -sha256 -key host.key -subj "/C=US/ST=CA/O=MyOrg, Inc./CN=${CN}" -out host.csr

openssl x509 -req -in host.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out host.crt -days 10000 -sha256
