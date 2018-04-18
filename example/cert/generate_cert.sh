#/bin/bash

set -x

rm -f RootCA.key
rm -f RootCA.cer
openssl genrsa -out RootCA.key 4096
openssl req -new -x509 -days 3650 -key RootCA.key -out RootCA.cer

rm -f IntermediateCA.key
rm -f IntermediateCA.csr
rm -f IntermediateCA.cer
openssl genrsa -out IntermediateCA.key 4096
openssl req -new -key IntermediateCA.key -out IntermediateCA.csr
openssl x509 -req -days 1000 -in IntermediateCA.csr -CA RootCA.cer -CAkey RootCA.key -CAcreateserial -out IntermediateCA.cer

rm -f Server.key
rm -f Server.csr
rm -f Server.cer
openssl genrsa -out Server.key 2048
openssl req -new -key Server.key -out Server.csr
openssl x509 -req -days 1000 -in Server.csr -CA IntermediateCA.cer -CAkey IntermediateCA.key -CAcreateserial -serial -out Server.cer -sha1

rm -f Client.key
rm -f Client.csr
rm -f Client.cer
openssl genrsa -out Client.key 2048
openssl req -new -key Client.key -out Client.csr
openssl x509 -req -days 1000 -in Client.csr -CA IntermediateCA.cer -CAkey IntermediateCA.key -CAcreateserial -serial -out Client.cer -sha1

set +x
