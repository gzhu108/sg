#!/bin/bash

# verbose
set -v

# generate a selfsigned certificate valid for two years
openssl req -x509 -sha256 -nodes -days 730 -newkey rsa:2048 -keyout selfsigned.key -out cert.pem

