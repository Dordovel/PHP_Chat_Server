<?php
    function encrypt_rsa($publicKey, $data)
    {
        if(empty($publicKey) || empty($data)) return "";
        $encryptData = "";
        openssl_public_encrypt($data, $encryptData, $publicKey);

        return $encryptData;
    }

    function decrypt_rsa($privateKey, $data)
    {
        if(empty($privateKey) || empty($data)) return "";

        $decryptData = "";
        openssl_private_decrypt($data, $decryptData, $privateKey);

        return $decryptData;
    }


	if(in_array("--decode", $argv))
		echo "DAta";
?>
