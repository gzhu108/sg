#/bin/bash

openssl genrsa -out RootCA.key 4096
openssl req -new -x509 -days 3650 -key RootCA.key -out RootCA.crt

openssl genrsa -out IntermediateCA.key 4096
openssl req -new -key IntermediateCA.key -out IntermediateCA.csr
openssl x509 -req -days 1000 -in IntermediateCA.csr -CA RootCA.crt -CAkey RootCA.key -CAcreateserial -out IntermediateCA.crt

openssl genrsa -out Server.key 2048
openssl req -new -key Server.key -out Server.csr
openssl x509 -req -days 1000 -in Server.csr -CA IntermediateCA.crt -CAkey IntermediateCA.key -set_serial 0101 -out Server.crt -sha1

openssl genrsa -out Client.key 2048
openssl req -new -key Client.key -out Client.csr
openssl x509 -req -days 1000 -in Client.csr -CA IntermediateCA.crt -CAkey IntermediateCA.key -set_serial 0101 -out Client.crt -sha1
