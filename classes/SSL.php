<?php


class SSL
{
    private $_privateSSLKeyData ;

    public function __construct()
    {
        $this->_privateSSLKeyData = openssl_pkey_new(
            array(
                'digest_alg' => 'md5',
                'private_keys_bits' => 2048,
                'private_key_type' => OPENSSL_KEYTYPE_RSA
            ));
    }

    public function generate_private_rsa_key()
    {
        openssl_pkey_export($this->_privateSSLKeyData, $privateRSAKey);

        return $privateRSAKey;
    }

    public function generate_public_rsa_key()
    {
        return openssl_pkey_get_details($this->_privateSSLKeyData)['key'];
    }

    public function encrypt_rsa($publicKey, $data)
    {
        if(empty($publicKey) || empty($data)) return "";
        $encryptData = "";
        openssl_public_encrypt($data, $encryptData, $publicKey);

        return $encryptData;
    }

    public function decrypt_rsa($privateKey, $data)
    {
        if(empty($privateKey) || empty($data)) return "";

        $decryptData = "";
        openssl_private_decrypt($data, $decryptData, $privateKey);

        return $decryptData;
    }
}