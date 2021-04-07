<?php

	require_once "Handler.php";

    class Authorization extends RequestHandler
    {
        const USER_LOGIN = "auth_user_login";
        const USER_AUTH = "auth_user_stat";

        private $_db;
        private $_ssl;

        const Login = 'Login';
        const Password = 'Password';
        const AuthStatus = 'Status';

        public function __construct(Connection $db, SSL $ssl)
        {
            $this->_db = $db;
            $this->_ssl = $ssl;
        }

        private function get_decode_data($field, $privKey, $request)
        {
            if(!isset($request->{$field}))
                return false;

            if(empty($request->{$field}))
                return false;

            $data = $this->_ssl->decrypt_rsa($privKey,
                                base64_decode($request->{$field}));

            return $data;
        }

        private function try_auth_user($login, $password)
        {
            $q = "SELECT
                        CASE
                            WHEN COUNT(users.id) <= 0
                                THEN 0
                            ELSE 1
                        END auth
                    FROM users
                    WHERE users.login = '$login'
                    AND users.password = '$password'
                    AND users.deleted <> 1";

            $user = $this->_db->query($q)->fetch(PDO::FETCH_ASSOC);

            return $user['auth'];
        }

        private function exec(&$client, $request)
        {
            $status = ResponceStatus::FAILD;

            $sslKey = $client[Type::HANDSHAKE][Handshake::PRIVATE_KEY];

            $login = $this->get_decode_data(Authorization::Login, $sslKey, $request);
            $pass = $this->get_decode_data(Authorization::Password, $sslKey, $request);

            $client[Type::AUTOINTIFICATION][Authorization::USER_AUTH] = false;

            if($this->try_auth_user($login, $pass))
            {
                $status = ResponceStatus::OK;
                $client[Type::AUTOINTIFICATION][Authorization::USER_AUTH] = true;
                $client[Type::AUTOINTIFICATION][Authorization::USER_LOGIN] = $login;
            }

            $responceData = array("Type" => Type::AUTOINTIFICATION,
                                Authorization::AuthStatus => $status);

            return Data::encode($responceData);
        }

        public function execute(&$client, $request)
        {
            if($request->Type == Type::AUTOINTIFICATION)
                return $this->exec($client, $request);

            return parent::execute($client, $request);
        }
    }
?>
