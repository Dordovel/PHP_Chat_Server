<?php

	require_once "Handler.php";

    class Authorization extends RequestHandler
    {
        const USER_LOGIN = "auth_user_login";
        const USER_AUTH = "auth_user_stat";

        private $_db;
        private $_ssl;

        private $Login = 'Login';
        private $Password = 'Password';
        private $AuthStatus = 'Status';

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
                                $request->{$field});

            return $data;
        }

        private function try_auth_user($login, $password)
        {
            $q = "SELECT
                        CASE
                            WHEN COUNT(*) <= 0
                                THEN 0
                            ELSE 1
                        END auth
                    FROM users
                    WHERE users.login = ?
                    AND users.password = ?
                    AND users.deleted <> 1";

            $connection = $this->_db->connection();
            $stmt = $connection->prepare($q);
            $stmt->bindParam(1, $login, PDO::PARAM_STR);
            $stmt->bindParam(2, $password, PDO::PARAM_STR);

            $stmt->execute();

            $user = $stmt->fetch(PDO::FETCH_ASSOC);

            return $user['auth'];
        }

        private function validate($client, $request)
        {
            if(!isset($client[Type::HANDSHAKE]))
                return false;

            if(empty($request))
                return false;

            if(!isset($request->{$this->Login}))
                return false;

            if(!isset($request->{$this->Password}))
                return false;

            return true;
        }

        private function exec(&$client, $request)
        {
            $response = array("Type" => Type::AUTORIZATION,
                $this->AuthStatus => ResponceStatus::FAILD);

            if($this->validate($client, $request))
            {
                $sslKey = $client[Type::HANDSHAKE][Handshake::PRIVATE_KEY];

                $login = $this->get_decode_data($this->Login, $sslKey, $request);
                $pass = $this->get_decode_data($this->Password, $sslKey, $request);

                $client[Type::AUTORIZATION][Authorization::USER_AUTH] = false;

                if ($this->try_auth_user($login, $pass))
                {
                    $client[Type::AUTORIZATION][Authorization::USER_AUTH] = true;
                    $client[Type::AUTORIZATION][Authorization::USER_LOGIN] = $login;

                    $response[$this->AuthStatus] = ResponceStatus::OK;
                }
            }

            return Data::encode($response);
        }

        public function execute(&$client, $request)
        {
            if($request->Type == Type::AUTORIZATION)
                return $this->exec($client, $request);

            return parent::execute($client, $request);
        }
    }
?>
