<?php

    require_once "../classes/Data.php";

    class Handshake extends RequestHandler
    {
        const PUBLIC_KEY = "handshake_pub_key";
        const PRIVATE_KEY = "handshake_priv_key";
        const RsaPublicKey = 'Key';

        private $_ssl;

        public function __construct(SSL $ssl)
        {
            $this->_ssl = $ssl;
        }

        private function exec(&$client, $request)
        {
            $privateServerRSAkey = $this->_ssl->generate_private_rsa_key();
            $publicServerRSAKey = $this->_ssl->generate_public_rsa_key();

            $publicUserRSAKey = $request->{Handshake::RsaPublicKey};

            $client[Type::HANDSHAKE][Handshake::PUBLIC_KEY] = $publicUserRSAKey;
            $client[Type::HANDSHAKE][Handshake::PRIVATE_KEY] = $privateServerRSAkey;

            $responce = array("Type" => Type::HANDSHAKE,
                                Handshake::RsaPublicKey => $publicServerRSAKey);

            return Data::encode($responce);
        }

        public function execute(&$client, $request)
        {
            if($request->Type == Type::HANDSHAKE)
                return $this->exec($client, $request);

            return parent::execute($client, $request);
        }
    }
?>