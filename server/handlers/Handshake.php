<?php

    require_once "../classes/Data.php";

    class Handshake extends RequestHandler
    {
        const PUBLIC_KEY = "handshake_pub_key";
        const PRIVATE_KEY = "handshake_priv_key";
        private $RsaPublicKey = 'Key';

        private $_ssl;

        public function __construct(SSL $ssl)
        {
            $this->_ssl = $ssl;
        }

        private function validate($request)
        {
            if(!isset($request->{$this->RsaPublicKey}))
            {
                return false;
            }

            return true;
        }

        private function exec(&$client, $request)
        {
            $response = array("Type" => Type::HANDSHAKE,
                $this->RsaPublicKey => ResponceStatus::FAILD);

            if($this->validate($request))
            {
                $privateServerRSAKey = $this->_ssl->generate_private_rsa_key();
                $publicServerRSAKey = $this->_ssl->generate_public_rsa_key();

                $publicUserRSAKey = $request->{$this->RsaPublicKey};

                $client[Type::HANDSHAKE][Handshake::PUBLIC_KEY] = $publicUserRSAKey;
                $client[Type::HANDSHAKE][Handshake::PRIVATE_KEY] = $privateServerRSAKey;

                $response = array("Type" => Type::HANDSHAKE,
                    $this->RsaPublicKey => $publicServerRSAKey);
            }

            return Data::encode($response);
        }

        public function execute(&$client, $request)
        {
            if($request->Type == Type::HANDSHAKE)
                return $this->exec($client, $request);

            return parent::execute($client, $request);
        }
    }
?>