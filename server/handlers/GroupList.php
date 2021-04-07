<?php

    class GroupList extends RequestHandler
    {
        private $_db;

        const GroupList = 'Groups';

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
                    WHERE users.login = '$login'
                    AND users.deleted <> 1
                    AND groups.deleted <> 1
                    AND ug.deleted <> 1";

            return $this->_db->query($q)->fetchAll(PDO::FETCH_ASSOC);
        }

        private function exec(&$client)
        {
            if($client[Type::AUTOINTIFICATION][Authorization::USER_AUTH])
            {
                $login = $client[Type::AUTOINTIFICATION][Authorization::USER_LOGIN];
                $groupList = $this->get_user_group_list($login);

                $responce = array("Type"=>Type::GROUP_LIST,
                                    GroupList::GroupList => $groupList);

                return Data::encode($responce);
            }

            return Data::encode(array());
        }

        public function execute(&$client, $request)
        {
            if($request->Type == Type::GROUP_LIST)
                return $this->exec($client);

            return parent::execute($client, $request);
        }
    }
?>