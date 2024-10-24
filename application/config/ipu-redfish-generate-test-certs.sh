#!/bin/bash

echo "This script generates an insecure (self-signed) certificate for the IPU Redfish Server."
echo "Its purpose is to allow users to try out the IPU Redfish Server without creating and provisioning proper certificates."
echo "Do you wish to proceed? (Y/N)"
read -r user_input

if [[ "$user_input" != "Y" && "$user_input" != "y" ]]; then
    echo "Operation cancelled by user."
    exit 0
fi

CERT_DIR="/work/redfish/certs"
KEY_FILE="$CERT_DIR/server.key"
CRT_FILE="$CERT_DIR/server.crt"

if [ -f "$KEY_FILE" ] || [ -f "$CRT_FILE" ]; then
    echo "$CERT_DIR is not empty. If you want to proceed, delete the existing certificate or key and try again."
    exit 0
fi

mkdir -p $CERT_DIR
if [ ! -d "$CERT_DIR" ]; then
    echo "Failed to create directory: $CERT_DIR"
    exit 1
fi

chmod 0700 $CERT_DIR

# Generate a Private Authentication Key with RSA 4096-bit modulus / 65537 exponent
openssl genpkey -algorithm RSA -out "$KEY_FILE" -pkeyopt rsa_keygen_bits:4096 -pkeyopt rsa_keygen_pubexp:65537
echo "Private authentication key generated: $KEY_FILE"

# Generate a Self-Signed Certificate from the private key
# -days 25555: 70 years - MEV-TS boots in 1970
openssl req -new -x509 -key "$KEY_FILE" -out "$CRT_FILE" -days 25555 -sha384 -subj "/C=PL/ST=Pomorskie/L=Gdansk/O=Test/OU=Do Not Use In Production/CN=localhost"
echo "Self-signed certificate generated: $CRT_FILE"
