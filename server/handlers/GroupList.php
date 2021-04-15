<?php

    class GroupList extends RequestHandler
    {
        private $_db;

        private $GroupList = 'Groups';

        public function __construct(Connection $db)
        {
            $this->_db = $db;
        }

        private function get_user_group_list($login)
        {
            $q = "SELECT
                        groups.name,
                        groups.info,
                        (SELECT users.login
                            WHERE users.id = groups.creater_id
                        ) AS creator
                    FROM users
                    LEFT JOIN user_group AS ug ON ug.user_id = users.id
                    LEFT JOIN groups ON groups.id = ug.group_id
                    WHERE users.login = ?
                    AND users.deleted <> 1
                    AND groups.deleted <> 1
                    AND ug.deleted <> 1";

            $connection = $this->_db->connection();
            $stmt = $connection->prepare($q);
            $stmt->bindParam(1, $login, PDO::PARAM_STR);
            $stmt->execute();

            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        }

        private function validate($client)
        {
            if(!isset($client[Type::AUTORIZATION]))
                return false;

            if(!$client[Type::AUTORIZATION][Authorization::USER_AUTH])
                return false;

            return true;
        }

        private function exec(&$client)
        {
            $response = array("Type"=>Type::GROUP_LIST,
                $this->GroupList => array());

            if($this->validate($client))
            {
                $login = $client[Type::AUTORIZATION][Authorization::USER_LOGIN];
                $groupList = $this->get_user_group_list($login);

                $response[$this->GroupList] = $groupList;
            }

            return Data::encode($response);
        }

        public function execute(&$client, $request)
        {
            if($request->Type == Type::GROUP_LIST)
                return $this->exec($client);

            return parent::execute($client, $request);
        }
    }
?>